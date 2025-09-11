import matplotlib.pyplot as plt

frequencies = [10**5, 3*10**6, 6*10**6, 9*10**6, 13*10**6, 17*10**6, 21*10**6, 24*10**6, 27*10**6, 30*10**6]
L_values = [40,42,44,46,52,57,54,47,37,16]
L_L0_values = [1, 1.05, 1.1, 1.15, 1.3, 1.425, 1.35, 1.175, 0.925, 0.4]

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