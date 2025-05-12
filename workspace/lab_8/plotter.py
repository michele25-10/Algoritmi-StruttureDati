import matplotlib.pyplot as plt
import numpy as np

# leggo tutte le righe del file contenente i risultati degli esperimenti
with open("report.txt", "r") as file:
    lines = file.readlines()

x_data = list(map(int, lines[0].split()))
y_data = [list(map(float, line.split())) for line in lines[1:]]

algos = ["INSERTION", "MERGE", "HYBRIDMERGE", "QUICK", "MOTQUICK", "TAILQUICK", "HEAPSORT"]

# creiamo un plot per ogni lista in y_data
for i, time in enumerate(y_data):
    plt.plot(x_data, time, label=algos[i])

# aggiungiamo curve teoriche per confronto (normalizzate per visibilità)
x = np.array(x_data)
scaling_factor = max(max(y) for y in y_data) / max(x)

plt.xlabel("Dimensione")
plt.ylabel("Tempo trascorso [s]")
plt.title("Esperimenti sugli ordinamenti")
plt.legend()
plt.grid(True)
plt.show()
