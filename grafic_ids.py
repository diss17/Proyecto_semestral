import pandas as pd
import matplotlib.pyplot as plt

# Leer los datos desde el archivo CSV
df = pd.read_csv('results_ids.csv', sep=';', names=["hash_type", "operation", "n", "duration"])

# Convertir la columna 'duration' a valores numéricos
df['duration'] = pd.to_numeric(df['duration'])

# Calcular el promedio de los tiempos de duración para cada combinación de hash_type, operation y n
avg_df = df.groupby(['hash_type', 'operation', 'n']).mean().reset_index()

# Crear subgráficas para inserciones y búsquedas
fig, ax = plt.subplots(2, 1, figsize=(12, 10), sharex=True)

# Graficar los tiempos de inserción promedio
insert_df = avg_df[avg_df['operation'].str.contains('insert')]
for key, grp in insert_df.groupby(['hash_type', 'operation']):
    ax[0].plot(grp['n'], grp['duration'], marker='o', linestyle='-', label=f'{key[0]} - {key[1]}')

ax[0].set_title('Tiempos de Inserción Promedio')
ax[0].set_ylabel('Duración Promedio (ns)')
ax[0].set_yscale('log')
#ax[0].set_xscale('log')
ax[0].legend()

# Graficar los tiempos de búsqueda promedio
search_df = avg_df[avg_df['operation'].str.contains('search')]
for key, grp in search_df.groupby(['hash_type', 'operation']):
    ax[1].plot(grp['n'], grp['duration'], marker='o', linestyle='-', label=f'{key[0]} - {key[1]}')

ax[1].set_title('Tiempos de Búsqueda Promedio')
ax[1].set_ylabel('Duración Promedio (ns)')
ax[1].set_yscale('log')
#ax[1].set_xscale('log')
ax[1].legend()

# Ajustar la rotación de las etiquetas en el eje x
plt.xticks(rotation=45)
plt.xlabel('Cantidad de Datos (n)')

# Mostrar la gráfica
plt.tight_layout()
plt.show()
