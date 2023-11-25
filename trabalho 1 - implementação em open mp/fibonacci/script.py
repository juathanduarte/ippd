import subprocess

import matplotlib.pyplot as plt
import pandas as pd

executable1 = "./fibo-1"
executable2 = "./fibo-2"

num_executions = 20

arg_value = "35"

times_algoritmo1 = []
times_algoritmo2 = []

for _ in range(num_executions):
    result = subprocess.run([executable1, arg_value], capture_output=True, text=True)
    time = float(result.stdout.split()[-1])
    times_algoritmo1.append(time)

for _ in range(num_executions):
    result = subprocess.run([executable2, arg_value], capture_output=True, text=True)
    time = float(result.stdout.split()[-1])
    times_algoritmo2.append(time)

mean_algoritmo1 = round(sum(times_algoritmo1) / num_executions, 3)
mean_algoritmo2 = round(sum(times_algoritmo2) / num_executions, 3)

times_algoritmo1.append(mean_algoritmo1)
times_algoritmo2.append(mean_algoritmo2)

data = {
    "Algoritmo 1": times_algoritmo1,
    "Algoritmo 2": times_algoritmo2
}

df = pd.DataFrame(data)

execucao_index = ["Média" if i == num_executions else i + 1 for i in range(num_executions + 1)]
df.insert(0, "Execução", execucao_index)

fig, ax = plt.subplots(figsize=(10, 4))
ax.set_axis_off()

col_colors = ['#D3D3D3', 'red', 'blue']

table = ax.table(cellText=df.values, colLabels=df.columns, loc='center', cellLoc='center',
                 colColours=col_colors)

for i, key in enumerate(df.columns):
    col_width = max([len(str(val)) for val in df[key]])
    table.auto_set_column_width([i])
    table.get_celld()[(0, i)].set_fontsize(10)

table.auto_set_font_size(False)
table.set_fontsize(10)

plt.savefig("tabela_comparativa.png", bbox_inches='tight', pad_inches=0.5)
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(df["Execução"][:-1], df["Algoritmo 1"][:-1], marker='o', linestyle='-', color='red', label='Algoritmo 1')
plt.plot(df["Execução"][:-1], df["Algoritmo 2"][:-1], marker='o', linestyle='-', color='blue', label='Algoritmo 2')
plt.xlabel("Execução")
plt.ylabel("Tempo de Execução")
plt.title("Comparação dos Tempos de Execução")
plt.legend()
plt.grid(True)
plt.savefig("grafico_comparativo.png")
plt.show()
