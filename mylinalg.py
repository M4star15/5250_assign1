# terminal code to run in HPC:  cd ~/5250 && source venv5250/bin/activate && python mylinalg.py

import math
import matplotlib.pyplot as plt

# this part same as last one.
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


def _linspace(a, b, m):

    if m <= 1:
        return [float(a)]
    h = (b - a) / (m - 1)
    return [a + i * h for i in range(m)]


def _poly_eval(coeffs, x):

    val = 0.0
    for c in reversed(coeffs):
        val = val * x + c
    return val


def LeastSquareApprox(x, f, n):

    m = len(x)
    # when it doenst contain any node
    if m == 0:
        raise ValueError("x must contain at least one node.")
    if n < 0:
        raise ValueError("n must be nonnegative.")
    if m < n + 1:
        raise ValueError("Need at least n+1 data points for degree n fit.")

    # Build y values
    if callable(f):
        y = [float(f(xi)) for xi in x]
    else:
        if len(f) != m:
            raise ValueError("If f is a list, it must have same length as x.")
        y = [float(v) for v in f]

    # Design matrix V where V[i][k] = x_i^k, for k = 0 .... as said
    # Normal equations: (V^T V) c = V^T y
    # Build G = V^T V (size (n+1)x(n+1)) and g = V^T y (size n+1)...

    deg = n
    size = deg + 1

    # to compute powers up to 2n for each x_i (so to be more efficint)
    # powers[i][p] = x_i^p for p= 0  to 2n
    powers = []
    for xi in x:
        row = [1.0]
        for p in range(1, 2 * deg + 1):
            row.append(row[-1] * xi)
        powers.append(row)

    # G[j][k] = sum_i x_i^(j+k)
    G = [[0.0 for _ in range(size)] for __ in range(size)]
    for j in range(size):
        for k in range(size):
            s = 0.0
            power = j + k
            for i in range(m):
                s += powers[i][power]
            G[j][k] = s

    # g[j] = sum_i y_i x_i^j
    g = [0.0 for _ in range(size)]
    for j in range(size):
        s = 0.0
        for i in range(m):
            s += y[i] * powers[i][j]
        g[j] = s

    # Solve for coefficients
    coeffs = GaussElimination(G, g)
    return coeffs


def _rmse(y_true, y_pred):
    m = len(y_true)
    s = 0.0
    for yt, yp in zip(y_true, y_pred):
        diff = yt - yp
        s += diff * diff
    return math.sqrt(s / m)


if __name__ == "__main__":
    # Application: least squares degree <= 5 for cos(x) on 51 nodes in [-pi, pi]....
    n = 5
    xs = _linspace(-math.pi, math.pi, 51)

    coeffs = LeastSquareApprox(xs, math.cos, n)

    ys_true = [math.cos(x) for x in xs]
    ys_fit = [_poly_eval(coeffs, x) for x in xs]

    max_err = max(abs(a - b) for a, b in zip(ys_true, ys_fit))
    rmse = _rmse(ys_true, ys_fit)

    print("Least Squares polynomial fit to cos(x)")
    print(f"Degree n = {n}")
    print("p(x) = c0 + c1 x + ... + cn x^n")
    for k, ck in enumerate(coeffs):
        print(f"c{k} = {ck:.16e}")
    print(f"\nMax error on the 51 nodes = {max_err:.16e}")
    print(f"RMSE on the 51 nodes      = {rmse:.16e}")

    # Plot on a denser grid for a nicer curve
    xd = _linspace(-math.pi, math.pi, 400)
    yd_true = [math.cos(x) for x in xd]
    yd_fit = [_poly_eval(coeffs, x) for x in xd]

    plt.figure()
    plt.plot(xd, yd_true, label="cos(x)")
    plt.plot(xd, yd_fit, label="Least Squares poly deg<=5")
    plt.scatter(xs, ys_true, marker="o", label="nodes (51)")
    plt.title("Least squares approximation of cos(x) on [-pi, pi]")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Save figure for submission
    plt.savefig("lab6_fig.png", dpi=200)
    plt.show()
