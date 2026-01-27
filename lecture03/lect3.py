import time

# list operands practicing...

def practice_list_operations():
    print("=== list operations ===")

    # mixed-type list + nested indexing
    L = [3, 4.5, "abc", [1, 2]]

    print("Initial L:", L)
    print("L[0] =", L[0])
    print("L[1] =", L[1])
    print("L[2] =", L[2])
    print("L[3] =", L[3])
    print("L[3][0] =", L[3][0])

    # append / extend / insert ops
    L.append(True)
    L.extend([None, {"x": 1}])
    L.insert(1, "INSERTED")
    print("\nAfter append/extend/insert:", L)

    # copy
    C = L.copy()
    print("copy() -> C:", C)

    # count / index / remove using an element that is guaranteed to exist:
    # choose the current middle element =
    target = L[len(L) // 2]
    print("target chosen from L:", target)

    cnt = L.count(target)
    idx = L.index(target)
    print("count(target) =", cnt)
    print("index(target) =", idx)

    # remove the same target 
    L.remove(target)
    print("After remove(target):", L)

    # pop (last and first)
    last = L.pop()
    first = L.pop(0)
    print("pop() ->", last)
    print("pop(0) ->", first)
    print("After pops:", L)

    # reverse
    L.reverse()
    print("After reverse():", L)

    # sort: must be comparable types, so separately on numeric list
    S = [5, 2, 9, 1, 5, 6]
    print("\nNumeric list S before sort:", S)
    S.sort()
    print("S after sort():", S)

    # clear
    L.clear()
    print("\nAfter clear():", L)
    print("=== end list operations ===\n")


# Deterministic pseudo random generator 
def make_random_int_list(n, seed=42): #change seed if you want to...

    L = []
    x = seed
    a = 1557575757
    c = 12345
    m = 2**31  # 2^31
    for _ in range(n):
        x = (a * x + c) % m
        L.append(x)
    return L


# timing helpers...
def average_time_seconds(fn, repeats):
    total = 0.0
    for _ in range(repeats):
        t0 = time.perf_counter()
        fn()
        t1 = time.perf_counter()
        total += (t1 - t0)
    return total / repeats


# the randomlist benchmark operations (1,2,3 L.pop(), L.pop(0), slicing copy, reverse...)

def benchmark_list_ops(N0, repeats=3, steps=4, pop_fraction=0.5, copy_batch=50):

    print("=== Performance benchmarks ===")
    print(f"Base N0={N0}, repeats={repeats}, steps={steps}, pop_fraction={pop_fraction}, copy_batch={copy_batch}\n")

    header = (
        "N".ljust(9) +
        "pop_many".rjust(12) +
        "pop0_many".rjust(12) +
        "L[:]".rjust(12) +
        "list(L)".rjust(12) +
        "reverse()".rjust(12) +
        "[::-1]".rjust(12) +
        "  " +
        "x2(pop)".rjust(9) +
        "x2(pop0)".rjust(10)
    )
    print("Average time (seconds)")
    print(header)
    print("-" * len(header))

    prev_pop = None
    prev_pop0 = None

    N = N0
    for _ in range(steps):
        base = make_random_int_list(N)
        k = max(1, int(N * pop_fraction))  # do many pops (default: N/2)

        # 1) pop cost
        def pop_many():
            L = base.copy()
            for _ in range(k):
                L.pop()

        def pop0_many():
            L = base.copy()
            for _ in range(k):
                L.pop(0)

        # 2) copy cost (batch to reduce noise)
        def slice_copy_batch():
            for _ in range(copy_batch):
                _ = base[:]

        def list_copy_batch():
            for _ in range(copy_batch):
                _ = list(base)

        # 3) reverse cost
        def reverse_in_place():
            L = base.copy()
            L.reverse()

        def reverse_out_of_place():
            _ = base[::-1]

        t_pop = average_time_seconds(pop_many, repeats)
        t_pop0 = average_time_seconds(pop0_many, repeats)
        t_slice = average_time_seconds(slice_copy_batch, repeats) / copy_batch
        t_list = average_time_seconds(list_copy_batch, repeats) / copy_batch
        t_rev = average_time_seconds(reverse_in_place, repeats)
        t_rev_slice = average_time_seconds(reverse_out_of_place, repeats)

        # Doubling ratios (relative to previous N)
        if prev_pop is None:
            r_pop = "   -   "
            r_pop0 = "   -   "
        else:
            r_pop = f"{t_pop / prev_pop:7.2f}x"
            r_pop0 = f"{t_pop0 / prev_pop0:7.2f}x"

        print(
            f"{str(N).ljust(9)}"
            f"{t_pop:12.6f}"
            f"{t_pop0:12.6f}"
            f"{t_slice:12.6f}"
            f"{t_list:12.6f}"
            f"{t_rev:12.6f}"
            f"{t_rev_slice:12.6f}"
            f"  {r_pop:>9}{r_pop0:>10}"
        )

        prev_pop = t_pop
        prev_pop0 = t_pop0
        N *= 2



# ============================================================
# Main
# ============================================================

if __name__ == "__main__":
    practice_list_operations()

    # Choose N0 (whichh is N).
    # You can change if needed...
    benchmark_list_ops(N0=20000, repeats=3, steps=4, pop_fraction=0.5, copy_batch=50)
