import numpy as np
import matplotlib.pyplot as plt
#   cd ~/5250
#   source venv5250/bin/activate

#   cd ~/5250/lecture08
#   python3 plot_lab8.py


data = np.loadtxt("exp_output.data")

x = data[:, 0]
y = data[:, 1]

plt.figure(figsize=(8, 5))
plt.plot(x, y, marker='o', linestyle='-', color='blue', label='exp(x) approx')

plt.title("Exponential Function exp(x) from Taylor Expansion")
plt.xlabel("x")
plt.ylabel("exp(x)")
plt.grid(True)
plt.legend()

plt.tight_layout()

plt.tight_layout()
plt.savefig("lab8_plot.png", dpi=300)

