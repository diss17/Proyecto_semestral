import pandas as pd
import matplotlib.pyplot as plt

# Función para leer y graficar los datos
def plot_results(csv_file, title):
    # Leer el archivo CSV
    df = pd.read_csv(csv_file, sep=';', names=["archivo", "iteracion", "tiempo", "unidad", "tamaño"])

    # Convertir 'iteracion' a cadena si no lo es
    if not pd.api.types.is_string_dtype(df['iteracion']):
        df['iteracion'] = df['iteracion'].astype(str)

    # Extraer número de iteración y convertir a entero
    df['iteracion'] = df['iteracion'].str.extract('(\d+)').astype(int)

    # Ordenar por iteración
    df = df.sort_values(by='iteracion')

    # Preparar datos para graficar
    iteraciones = df['iteracion'].unique()
    tiempos = df['tiempo']
    tamaños = df['tamaño']

    # Graficar los tiempos
    plt.figure(figsize=(10, 6))
    plt.plot(iteraciones, tiempos, marker='o', linestyle='-', color='b', label='Tiempo (ms)')
    plt.title(title)
    plt.xlabel('Iteración')
    plt.ylabel('Tiempo (ms)')
    plt.xticks(iteraciones)
    plt.grid(True)
    plt.legend()

    # Añadir barra de tamaño del archivo (opcional)
    if not tamaños.empty:
        ax2 = plt.gca().twinx()
        ax2.bar(iteraciones, tamaños, alpha=0.3, color='g', label='Tamaño (bytes)')
        ax2.set_ylabel('Tamaño (bytes)')
        ax2.legend(loc='upper right')

    plt.tight_layout()
    plt.show()

# Rutas de los archivos CSV
archivo_codificacion = 'resultados_tiempos_codificacionhuffman.csv'
archivo_decodificacion = 'resultados_tiempos_decodificacionhuffman.csv'

# Graficar tiempos de codificación
plot_results(archivo_codificacion, 'Tiempos de Codificación Huffman')

# Graficar tiempos de decodificación
plot_results(archivo_decodificacion, 'Tiempos de Decodificación Huffman')
