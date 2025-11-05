import numpy as np
import matplotlib.pyplot as plt

t_2 = np.array([4.44, 3.83, 3.13, 2.63, 1.93])
dx = np.array([367, 318, 267, 217, 166])

n = len(t_2)
b = (n * np.sum(t_2 * dx) - np.sum(t_2) * np.sum(dx)) / (n * np.sum(t_2 ** 2) - np.sum(t_2) ** 2)
a = (np.sum(dx) - b * np.sum(t_2)) / n
poly = np.poly1d([b, a])

plt.figure(figsize=(8, 5))

plt.scatter(t_2, dx, color='blue', s=60, label='Данные', alpha=0.7)

t_2_line = np.linspace(min(t_2) - 0.5, max(t_2) + 0.5, 100)
plt.plot(t_2_line, poly(t_2_line), 'r-', linewidth=2,
         label=f'y = {b:.3f}x + {a:.3f}')

plt.xlabel('t², c²', fontsize=12)
plt.ylabel('Δx, мм', fontsize=12)
plt.title('Δx(t²)', fontsize=14)

plt.grid(True, linestyle='-', alpha=0.7)

plt.legend()
plt.tight_layout()
plt.show()

print(f"Уравнение линии: y = {b:.3f}x + {a:.3f}")
print(f"Коэффициент корреляции: {np.corrcoef(t_2, dx)[0,1]:.3f}")
