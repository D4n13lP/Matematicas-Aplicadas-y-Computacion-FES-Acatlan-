#!/usr/bin/env python3
# algorithms/method_value_iteration.py

"""
Aproximaciones Sucesivas (Value Iteration) para PMD con descuento.

Ecuación de iteración:
  v^{k+1}_i = min_{a∈D(i)} [ c_{i,a} + α ∑_j P_a[i,j] · v^k_j ]
(o max si problema es de maximizar).

El usuario introduce:
  • α (0 ≤ α < 1)
  • iter_max (número máximo de iteraciones)
  • tol (tolerancia para detenerse)

En cada iteración se imprimen v⁽ᵏ⁾_0…v⁽ᵏ⁾_{n-1}. Al final, se extrae
la política π(i)=argmin_a[…], se muestra y se termina.
"""

import numpy as np
from read import leer_datos_manualmente
from lim_pant import limpiar_pantalla

def metodo_value_iteration(datos):
    limpiar_pantalla()
    problema_tipo = datos["problema_tipo"].lower()  # “maximizar” o “minimizar”
    n             = datos["num_estados"]
    politicas     = datos["politicas"]              # dict: acción→[estados]
    probs         = datos["probabilidades"]         # dict: acción→matriz n×n
    costos        = datos["costos"]                 # dict: acción→{i:c_{i,a}}

    # 1) Leer α, iter_max, tol
    while True:
        try:
            alpha = float(input("Ingrese factor de descuento α (0 ≤ α < 1): ").strip())
            if 0 <= alpha < 1: break
        except:
            pass
        print("α inválido; debe ser 0 ≤ α < 1.")
    while True:
        try:
            iter_max = int(input("Ingrese número máximo de iteraciones: ").strip())
            if iter_max > 0: break
        except:
            pass
        print("iter_max inválido; debe ser entero positivo.")
    while True:
        try:
            tol = float(input("Ingrese tolerancia tol (>0): ").strip())
            if tol > 0: break
        except:
            pass
        print("tol inválida; debe ser número positivo.")

    # Construir D(i)
    D = {i: [] for i in range(n)}
    for a, estados in politicas.items():
        for i in estados:
            D[i].append(a)

    # 2) Inicializar v
    v = np.zeros(n)
    print("\n--- Valores de la iteración ---")
    for k in range(1, iter_max+1):
        v_new = np.zeros(n)
        for i in range(n):
            valores = []
            for a in D[i]:
                q = costos[a][i] + alpha * np.dot(probs[a][i], v)
                valores.append(q)
            if problema_tipo.startswith("max"):
                v_new[i] = max(valores)
            else:
                v_new[i] = min(valores)
        # Imprimir iteración
        print(f"\nIteración {k}:")
        for i, vi in enumerate(v_new):
            print(f" v_{i} = {vi:.6f}")
        # Chequear convergencia
        if np.max(np.abs(v_new - v)) < tol:
            print(f"\nConvergió por tol={tol} en {k} iteraciones.")
            v = v_new
            break
        v = v_new
    else:
        print(f"\nAlcanzado iter_max={iter_max} sin converger.")

    # 3) Extraer política final
    pi = []
    for i in range(n):
        candidatos = {}
        for a in D[i]:
            q = costos[a][i] + alpha * np.dot(probs[a][i], v)
            candidatos[a] = q
        mejor = (max if problema_tipo.startswith("max") else min)(
            candidatos, key=candidatos.get)
        pi.append(mejor)

    # 4) Mostrar política lograda
    print("\nLa política resultante es:", tuple(pi))
    print("Valores finales v*:")
    for i, vi in enumerate(v):
        print(f" v*_{i} = {vi:.6f}")

    pfin = np.zeros((n, n))
    for i, a in enumerate(pi):
        pfin[i, :] = probs[a][i]
    A = pfin.T - np.eye(n); A[-1, :] = 1.0
    b = np.zeros(n); b[-1] = 1.0
    pi_est = np.linalg.solve(A, b)
    costo_medio = sum(pi_est[i] * costos[pi[i]][i] for i in range(n))
    print(f"\nCosto medio E[C_(R)] = {costo_medio:.6f}")
