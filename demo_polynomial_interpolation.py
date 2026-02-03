# lab 5 5250 Minoo Hong


import math

# Gaussian Elimination Function: GaussElimination(A,b) for Ax=b using Gaussian elimination (partial pivoting)
def GaussElimination(A, b, tol=1e-14):
 
    n = len(A)
    if n == 0:
        return []

    if len(b) != n:
        raise ValueError("Dimension mismatch: len(b) must match number of rows in A.")
    for i in range(n):
        if len(A[i]) != n:
            raise ValueError("Matrix A must be square (n x n)...")

    # This part do deep copy as floats so original inputs are not modified...
    M = [[float(A[i][j]) for j in range(n)] for i in range(n)]
    rhs = [float(b[i]) for i in range(n)]

    # Forward elimination
    for k in range(n):
        # partial pivot by choosign row with the largest abs entry in column k
        pivot_row = k
        pivot_val = abs(M[k][k])
        for r in range(k + 1, n):
            if abs(M[r][k]) > pivot_val:
                pivot_val = abs(M[r][k])
                pivot_row = r

        if pivot_val < tol:
            raise ValueError("Matrix is singular or nearly singular (tiny pivot).")

        # swap if necessary
        if pivot_row != k:
            M[k], M[pivot_row] = M[pivot_row], M[k]
            rhs[k], rhs[pivot_row] = rhs[pivot_row], rhs[k]

        pivot = M[k][k]
        # eliminate below
        for i in range(k + 1, n):
            factor = M[i][k] / pivot
            # inner loop..
            for j in range(k, n):
                M[i][j] -= factor * M[k][j]
            rhs[i] -= factor * rhs[k]


    # back substitution...
    x = [0.0] * n
    for i in range(n - 1, -1, -1):
        s = rhs[i]

        # inner loop
        for j in range(i + 1, n):
            s -= M[i][j] * x[j]
        if abs(M[i][i]) < tol:
            raise ValueError("Matrix is singular or nearly singular (zero diagonal).")
        x[i] = s / M[i][i]

    return x


# Cubic interpolation of cos(x) at x = -0.1, -0.02, 0.02, 0.1

# setup linear system for computing abcd...
def build_system(xs, f):

    A = []
    b = []
    for x in xs:
        A.append([x**3, x**2, x, 1.0])
        b.append(f(x))
    return A, b

# p(x) = a x^3 + b x^2 + c x + d,
def p_of_x(x, coeffs):
    a, b, c, d = coeffs
    return a*x**3 + b*x**2 + c*x + d


def main():
    # the numbers at x = -0.1, -0.02, 0.02, 0.1
    xs = [-0.1, -0.02, 0.02, 0.1]
    f = math.cos

    # Set up and solve system for (a,b,c,d)
    A, rhs = build_system(xs, f)

    #Gaussian Elimination
    coeffs = GaussElimination(A, rhs)

    # measuring errors at the 4 interpolation points....
    errs = []
    for x in xs:
        err = abs(f(x) - p_of_x(x, coeffs))
        errs.append(err)

    max_err = max(errs)

    # print results
    a, b2, c, d = coeffs
    print("Cubic interpolant p(x) = a x^3 + b x^2 + c x + d")
    print(f"a = {a:.16e}")
    print(f"b = {b2:.16e}")
    print(f"c = {c:.16e}")
    print(f"d = {d:.16e}")
    print()
    for x, e in zip(xs, errs):
        print(f"x = {x: .5f} |cos(x)-p(x)| = {e:.16e}")
    print()
    print(f"Maximum error on the 4 points = {max_err:.16e}")

if __name__ == "__main__":
    main()
