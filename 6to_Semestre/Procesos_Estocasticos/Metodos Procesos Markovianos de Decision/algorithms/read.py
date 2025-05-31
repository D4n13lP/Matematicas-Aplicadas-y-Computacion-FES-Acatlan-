#!/usr/bin/env python3
"""
Algoritmo de lectura para el proyecto de Procesos Estocásticos.

Este programa permite configurar:
    - El tipo de problema: Maximizar ("max") o Minimizar ("min").
    - Número de estados y de decisiones.
    - Políticas viables para cada decisión.
    - Costos asociados: Solo se piden c_{i,j} si la decisión j se aplica al estado i.
    - Probabilidades para las matrices de transición (solo se piden si la decisión j aplica al estado i).

Se permite ingresar fracciones como '1/3' o '2/5'.
"""

from fractions import Fraction

def parse_float(value_str):
    """Convierte una cadena que puede representar un número o fracción a float."""
    try:
        return float(Fraction(value_str)) if '/' in value_str else float(value_str)
    except Exception as e:
        raise ValueError(f"No se pudo interpretar el número: {value_str}") from e

def leer_datos_manualmente():
    # Tipo de problema
    while True:
        problema_tipo = input("Indique el tipo de problema ('max' o 'min'): ").strip().lower()
        if problema_tipo in ['max', 'min']:
            break
        print("Entrada inválida. Use 'max' o 'min'.")
    
    # Estados y decisiones
    while True:
        try:
            num_estados = int(input("Ingrese el número de estados: "))
            if num_estados > 0:
                break
        except: pass
        print("Ingrese un número entero positivo.")
    
    while True:
        try:
            num_decisiones = int(input("Ingrese el número de decisiones: "))
            if num_decisiones > 0:
                break
        except: pass
        print("Ingrese un número entero positivo.")

    # Políticas viables (decisiones -> estados)
    politicas = {}
    print("\n=== Ingreso de Políticas Viables ===")
    for j in range(1, num_decisiones + 1):
        while True:
            entrada = input(f"Estados donde se aplica la decisión {j} (ej: 0,1,2): ")
            try:
                estados = [int(e.strip()) for e in entrada.split(',') if e.strip()]
                if all(0 <= e < num_estados for e in estados):
                    politicas[j] = estados
                    break
                print("Error: estados fuera de rango.")
            except:
                print("Error: entrada inválida.")

    # Costos c_{i,j} solo si decisión j aplica a estado i
    print("\n=== Ingreso de Costos Asociados ===")
    costos = {j: {} for j in range(1, num_decisiones + 1)}
    for i in range(num_estados):
        for j in range(1, num_decisiones + 1):
            if i in politicas.get(j, []):
                while True:
                    entrada = input(f"Ingrese el costo de c_{{{i},{j}}}: ")
                    try:
                        costos[j][i] = parse_float(entrada)
                        break
                    except ValueError as e:
                        print("  Error:", e)

    # Probabilidades de transición solo si la decisión j aplica al estado i
    print("\n=== Ingreso de Probabilidades de Transición ===")
    probabilidades = {}
    for j in range(1, num_decisiones + 1):
        print(f"\n--- Matriz de transición para la decisión {j} ---")
        matriz = []
        for i in range(num_estados):
            if i in politicas.get(j, []):
                while True:
                    entrada = input(f"Probabilidades desde estado {i} (fracciones o decimales, separados por comas): ")
                    try:
                        fila = [parse_float(x.strip()) for x in entrada.split(",") if x.strip()]
                        if len(fila) != num_estados:
                            print(f"Error: se esperaban {num_estados} valores.")
                            continue
                        if abs(sum(fila) - 1.0) > 1e-3:
                            print("Advertencia: la suma no es 1.")
                        matriz.append(fila)
                        break
                    except ValueError as e:
                        print("  Error:", e)
            else:
                matriz.append([0.0] * num_estados)
        probabilidades[j] = matriz

    return {
        "problema_tipo": "Maximizar" if problema_tipo == "max" else "Minimizar",
        "num_estados": num_estados,
        "num_decisiones": num_decisiones,
        "politicas": politicas,
        "costos": costos,
        "probabilidades": probabilidades,
    }

def mostrar_resumen(datos):
    print("\n" + "=" * 60)
    print("RESUMEN DE LOS DATOS INGRESADOS".center(60))
    print("=" * 60)
    print(f"Tipo de problema: {datos['problema_tipo']}")
    print(f"Estados: {datos['num_estados']} | Decisiones: {datos['num_decisiones']}")

    print("\nPolíticas viables:")
    for j, estados in datos['politicas'].items():
        print(f"  Decisión {j}: estados {estados}")

    print("\nCostos c_{i,j}:")
    for j in sorted(datos["costos"]):
        for i in sorted(datos["costos"][j]):
            print(f"  c_{{{i},{j}}} = {datos['costos'][j][i]:.4f}")

    print("\nMatrices de transición:")
    for j, matriz in datos["probabilidades"].items():
        print(f"\n  Decisión {j}:")
        for i, fila in enumerate(matriz):
            fila_str = " | ".join(f"{v:.4f}" for v in fila)
            print(f"    Estado {i}: [ {fila_str} ]")
    print("=" * 60)

def main():
    print("=== Lectura para Procesos de Decisión de Markov ===\n")
    datos = leer_datos_manualmente()
    mostrar_resumen(datos)

if __name__ == "__main__":
    main()
