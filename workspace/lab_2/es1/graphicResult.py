import pandas as pd
import matplotlib.pyplot as plt

#dataframe con risultati del test
df = pd.read_csv("result.csv", usecols=['size' ,'elapsed_time'], delimiter=",")


# Crea il grafico 
ax = df.plot(x="size", y="elapsed_time", kind="line", 
             marker="o", linestyle="-", color="royalblue", 
             linewidth=2, markersize=6, figsize=(10, 6), 
             title="Prestazioni insertionSort")

# Personalizza le etichette
ax.set_xlabel("Dimensione dell'array", fontsize=12)
ax.set_ylabel("Tempo di esecuzione medio (s)", fontsize=12)
ax.legend(["Tempo di esecuzione"], loc="upper left")

# Aggiungi una descrizione nel grafico
text_x = df["size"].max() * 0.6  
text_y = df["elapsed_time"].max() * 0.8
ax.text(text_x, text_y, 
        "min_size = 10 \nmax_size = 5000 \nrepetition = 10 \nstep = 25 \nseed = 250",
        fontsize=10, color="black", bbox=dict(facecolor='white', alpha=0.7))

# Mostra il grafico
plt.show()
