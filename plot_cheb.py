import numpy as np
import matplotlib.pyplot as plt

def plot_cheb():
    data = np.loadtxt("cheb_output.data")
    x = data[:,0]
    y = data[:,1]

    plt.figure(figsize=(7,5))
    plt.plot(x, y, '-o', markersize=4)
    plt.title("Chebyshev Polynomial Expansion")
    plt.xlabel("x")
    plt.ylabel("p_N(x)")
    plt.grid(True)
    plt.savefig("cheb_plot.png", dpi=300, bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    plot_cheb()
