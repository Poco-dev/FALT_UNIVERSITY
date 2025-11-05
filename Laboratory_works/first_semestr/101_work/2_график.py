import numpy as np
import matplotlib.pyplot as plt

t_2 = np.array([1,2,3,4,5])
dx = np.array([1.00,0.87,0.73,0.59,0.45])

n = len(t_2)
b = (n * np.sum(t_2 * dx) - np.sum(t_2) * np.sum(dx)) / (n * np.sum(t_2 ** 2) - np.sum(t_2) ** 2)
a = (np.sum(dx) - b * np.sum(t_2)) / n
poly = np.poly1d([b, a])

plt.figure(figsize=(8, 5))

plt.scatter(t_2, dx, color='blue', s=60, label='Данные', alpha=0.7)

t_2_line = np.linspace(min(t_2) - 0.5, max(t_2) + 0.5, 100)
plt.plot(t_2_line, poly(t_2_line), 'r-', linewidth=2,
         label=f'y = {b:.3f}x + {a:.3f}')

plt.xlabel('i', fontsize=12)
plt.ylabel('Δx_i/Δx_0', fontsize=12)
plt.title('f(i)', fontsize=14)

plt.grid(True, linestyle='-', alpha=0.7)

plt.legend()
plt.tight_layout()
plt.show()

print(f"Уравнение линии: y = {b:.3f}x + {a:.3f}")
print(f"Коэффициент корреляции: {np.corrcoef(t_2, dx)[0,1]:.3f}")