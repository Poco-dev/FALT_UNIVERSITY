import numpy as np
import matplotlib.pyplot as plt

Vpp = np.array([20,18,16,14,12,10,8,6,4,2])
L = np.array([21,19,17,15,13,11,9,7,5,3])

n = len(Vpp)
b = (n * np.sum(Vpp * L) - np.sum(Vpp) * np.sum(L)) / (n * np.sum(Vpp**2) - np.sum(Vpp)**2)
a = (np.sum(L) - b * np.sum(Vpp)) / n

poly = np.poly1d([b, a])

plt.figure(figsize=(6, 3))

plt.scatter(Vpp, L, color='blue', s=60, label='Данные', alpha=0.7)

Vpp_line = np.linspace(min(Vpp)-1, max(Vpp)+1, 100)
plt.plot(Vpp_line, poly(Vpp_line), 'r-', linewidth=2,
         label=f'y = {b:.3f}x + {a:.3f}')

plt.xlabel('Vpp, мВ', fontsize=12)
plt.ylabel('L, мм', fontsize=12)

plt.gca().xaxis.set_major_locator(plt.MultipleLocator(1))

plt.gca().yaxis.set_major_locator(plt.MultipleLocator(2))

plt.grid(True, linestyle='-', alpha=0.7, which='major')
plt.grid(True, linestyle=':', alpha=0.4, which='minor')

plt.legend()
plt.tight_layout()
plt.show()