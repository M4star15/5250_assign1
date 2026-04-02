import sys
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

def load_xy(fname):
    data = np.loadtxt(fname)
    if data.ndim == 1:
        data = data.reshape(1, 2)
    return data[:, 0], data[:, 1]

def load_intervals(fname):
    data = np.loadtxt(fname)
    if data.ndim == 1:
        data = data.reshape(1, 2)
    return data[:, 0], data[:, 1]

def make_plots(mode, prefix):
    values_file = f"{prefix}_values.data"
    intervals_file = f"{prefix}_intervals.data"

    x, y = load_xy(values_file)
    a, b = load_intervals(intervals_file)

    # -------- function plot --------
    plt.figure(figsize=(9, 5))

    if mode == "potential":
        plt.plot(x, y, 'o-', linewidth=1.2, markersize=3.5)
        plt.title(f"Electrostatic Potential V(x) [{prefix}]")
        plt.xlabel("x")
        plt.ylabel("V(x)")
    elif mode == "bessel":
        plt.plot(x, y, 'o-', linewidth=1.2, markersize=3.5)
        plt.title(f"Bessel Function J0(x) [{prefix}]")
        plt.xlabel("x")
        plt.ylabel("J0(x)")
    else:
        raise ValueError("mode must be potential or bessel")

    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"{prefix}_function.png", dpi=400, bbox_inches="tight")
    plt.close()

    # -------- interval structure plot --------
    plt.figure(figsize=(10, 6))
    for k in range(len(a)):
        plt.plot([a[k], b[k]], [k, k], linewidth=2)

    plt.title(f"Adaptive Simpson Subintervals [{prefix}]")
    plt.xlabel("x")
    plt.ylabel("Interval index")
    plt.grid(True, axis="x", linestyle="--", alpha=0.6)

    xmin = np.min(a)
    xmax = np.max(b)
    plt.xlim(xmin, xmax)
    plt.tight_layout()
    plt.savefig(f"{prefix}_intervals.png", dpi=400, bbox_inches="tight")
    plt.close()

    print(f"Saved {prefix}_function.png")
    print(f"Saved {prefix}_intervals.png")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python plot_results.py <mode> <prefix>")
        sys.exit(1)

    make_plots(sys.argv[1], sys.argv[2])