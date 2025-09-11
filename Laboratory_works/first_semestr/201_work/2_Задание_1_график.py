import numpy as np
import matplotlib.pyplot as plt

Vpp = np.array([4,3.6,3.2,2.8,2.4,2,1.6,1.2,0.8,0.4])
L = np.array([80,71,64,56,48,40,33,25,17,9])

n = len(Vpp)
b = (n * np.sum(Vpp * L) - np.sum(Vpp) * np.sum(L)) / (n * np.sum(Vpp**2) - np.sum(Vpp)**2)
a = (np.sum(L) - b * np.sum(Vpp)) / n

poly = np.poly1d([b, a])

plt.figure(figsize=(6, 3))

plt.scatter(Vpp, L, color='blue', s=60, label='Данные', alpha=0.7)

Vpp_line = np.linspace(0.2, 4.2, 100)
plt.plot(Vpp_line, poly(Vpp_line), 'r-', linewidth=2,
         label=f'y = {b:.3f}x + {a:.3f}')

plt.xlabel('Vpp, В', fontsize=12)
plt.ylabel('L, мм', fontsize=12)

plt.gca().xaxis.set_major_locator(plt.MultipleLocator(0.4))
plt.gca().yaxis.set_major_locator(plt.MultipleLocator(5))

plt.grid(True, linestyle='-', alpha=0.7, which='major')
plt.grid(True, linestyle=':', alpha=0.4, which='minor')

plt.legend()
plt.tight_layout()
plt.show()