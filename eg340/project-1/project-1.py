import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
from scipy.stats import multivariate_normal


fig = plt.figure(figsize=(18, 6))


def gen_joint_pdf(
        mean: tuple[float, float],
        variance: tuple[float, float],
        correlation_coefficient: float):
    cov = ((variance[0], correlation_coefficient), (correlation_coefficient, variance[1]))
    return multivariate_normal(mean, cov)


def gen_subplot(number, x, y, pos):
    ax: Axes3D = fig.add_subplot(number, projection="3d")
    ax.plot_surface(x, y, rv.pdf(pos), cmap="viridis", linewidth=0)
    ax.set_xlabel("X axis")
    ax.set_ylabel("Y axis")
    ax.set_zlabel("Z axis")


x = y = np.linspace(-6, 6, 500)
X, Y = np.meshgrid(x, y)
pos = np.empty(X.shape + (2,))
pos[:, :, 0], pos[:, :, 1] = X, Y

rv = gen_joint_pdf(mean=(2, 1), variance=(1, 2), correlation_coefficient=0.5)

x, y = X, Y
gen_subplot(131, x, y, pos)

x, y = X, np.ones(len(X))
gen_subplot(132, x, y, pos)

x, y = np.ones(len(X))*2, Y
gen_subplot(133, x, y, pos)

plt.tight_layout()
plt.show()
