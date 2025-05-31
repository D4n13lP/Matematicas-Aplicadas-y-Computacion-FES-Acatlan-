#!/usr/bin/env python3
# algorithms/enumerate_policies.py

import numpy as np
from itertools import product
from lim_pant import limpiar_pantalla

def calcular_vector_estacionario(P):
    n = P.shape[0]
    A = P.T - np.eye(n)
    A[-1, :] = 1.0
    b = np.zeros(n)
    b[-1] = 1.0
    return np.linalg.solve(A, b)

def metodo_enumeracion(datos):
    limpiar_pantalla()
    problema_tipo = datos["problema_tipo"]
    n = datos["num_estados"]
    politicas = datos["politicas"]
    costos_data = datos["costos"]
    probs = datos["probabilidades"]

    # Construir decisiones viables por estado
    decisiones_por_estado = {i: [] for i in range(n)}
    for j, estados in politicas.items():
        for i in estados:
            decisiones_por_estado[i].append(j)

    print("\n=== ENUMERACIÓN EXHAUSTIVA DE POLÍTICAS ===\n")
    resultados = []
    for pol in product(*(decisiones_por_estado[i] for i in range(n))):
        # Matriz de transición P^pol
        P_pol = np.zeros((n, n))
        for i, dj in enumerate(pol):
            P_pol[i, :] = probs[dj][i]

        # Vector estacionario y costo medio
        pi = calcular_vector_estacionario(P_pol)
        c_pol = np.array([costos_data[dj][i] for i, dj in enumerate(pol)], dtype=float)
        g = float(pi.dot(c_pol))
        resultados.append((pol, pi, g))

        print(f"Política: {pol}")
        print("  π =", np.round(pi, 6).tolist())
        print(f"  Costo medio = {g:.6f}\n")

    # Selección óptima
    if problema_tipo.lower().startswith("max"):
        pol_opt, pi_opt, g_opt = max(resultados, key=lambda x: x[2])
    else:
        pol_opt, pi_opt, g_opt = min(resultados, key=lambda x: x[2])

    print("="*60)
    print("POLÍTICA ÓPTIMA".center(60))
    print("="*60)
    print(f"R* = {pol_opt}")
    print("π* =", np.round(pi_opt, 6).tolist())
    print(f"E(C_{{R*}}) = {g_opt:.6f}\n")
