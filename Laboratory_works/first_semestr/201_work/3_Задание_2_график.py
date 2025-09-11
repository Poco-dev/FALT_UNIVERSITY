import matplotlib.pyplot as plt

frequencies = [10**5, 3*10**6, 6*10**6, 9*10**6, 13*10**6, 17*10**6, 21*10**6, 24*10**6, 27*10**6, 30*10**6]
L_values = [21,22,23,24,27,29,29,25,20,8]
L_L0_values = [1, 1.0476190476190477, 1.0952380952380953, 1.1428571428571428, 1.2857142857142858, 1.380952380952381, 1.380952380952381, 1.1904761904761905, 0.9523809523809523, 0.38095238095238093]

plt.figure(figsize=(7, 4))
plt.plot(frequencies, L_L0_values, 'bo-', linewidth=2, markersize=8)

plt.xscale('log')

plt.xlabel('f, Hz', fontsize=12)
plt.ylabel('L/L₀', fontsize=12)

plt.xlim(10**5, 10**8)
plt.ylim(0, max(L_L0_values) * 1.1)

plt.grid(True, alpha=0.3, which='both')

plt.xticks([10**5, 10**6, 10**7, 10**8],
           ['$10^5$', '$10^6$', '$10^7$', '$10^8$'],
           fontsize=10)

plt.tight_layout()
plt.show()