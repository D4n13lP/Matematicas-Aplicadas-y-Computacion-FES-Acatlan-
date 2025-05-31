#!/usr/bin/env python3
# algorithms/method_policy_improvement.py

"""
Iteración de Políticas para PMD de costo medio.

Contiene dos métodos:
  1) Sin descuento:
     - Resuelve g y v_0…v_{n-1} fijando v_{n-1}=0
     - Mejora usando Δ₍i,a₎ = c_{i,a} + Σ_j P_a[i,j]·v_j – v_i

  2) Con descuento α:
     - Resuelve (I – αP^π)·v = c^π
     - Mejora usando c_{i,a} + α Σ_j P_a[i,j]·v_j
"""

import numpy as np
from read import leer_datos_manualmente
from lim_pant import limpiar_pantalla

def evaluar_politica_sin_desc(pol, datos):

    """Evalúa la política SIN descuento (igual a tu evaluar_politica)."""
    n      = datos["num_estados"]
    probs  = datos["probabilidades"]
    costos = datos["costos"]

    # Construir Pmat y cvec
    Pmat = np.zeros((n, n))
    cvec = np.zeros(n)
    for i, a in enumerate(pol):
        Pmat[i, :] = probs[a][i]
        cvec[i]    = costos[a][i]

    # Montar sistema A x = b con x = [v_0…v_{n-1}, g]
    N = n + 1
    A = np.zeros((N, N))
    b = np.zeros(N)

    # Eqs i=0…n-1
    for i in range(n):
        for j in range(n):
            A[i, j] = (1.0 if i == j else 0.0) - Pmat[i, j]
        A[i, n] = -1.0
        b[i]    = -cvec[i]

    # Eq n: v_{n-1} = 0
    A[n, n-1] = 1.0
    b[n]       = 0.0

    sol     = np.linalg.solve(A, b)
    v_raw   = sol[:n]
    g       = sol[n]
    # Invertir signo para que Δ funcione
    v_signed = -v_raw
    return g, v_signed

def metodo_policy_improvement(datos):
    limpiar_pantalla()
    """Mejoramiento de Políticas SIN descuento."""
    problema_tipo = datos["problema_tipo"].lower()
    n             = datos["num_estados"]
    politicas     = datos["politicas"]
    probs         = datos["probabilidades"]
    costos        = datos["costos"]

    # Construir D(i)
    D = {i: [] for i in range(n)}
    for a, estados in politicas.items():
        for i in estados:
            D[i].append(a)

    # Leer política inicial
    while True:
        entrada = input(f"Ingrese política inicial ({n} acciones, coma-sep.): ")
        try:
            pol = tuple(int(x) for x in entrada.split(","))
            if len(pol)!=n or any(pol[i] not in D[i] for i in range(n)):
                raise ValueError
            break
        except:
            print("Política inválida, inténtalo de nuevo.")

    iter_count = 0
    while True:
        iter_count += 1
        print(f"\n--- Iteración {iter_count} (sin descuento) ---")

        # Evaluar
        g, v = evaluar_politica_sin_desc(pol, datos)
        print(f" g = {g:.6f}")
        for i, vi in enumerate(v):
            print(f" v_{i} = {vi:.6f}")

        # Mejorar
        nueva = []
        for i in range(n):
            candidatos = {}
            for a in D[i]:
                delta = costos[a][i] + np.dot(probs[a][i], v) - v[i]
                candidatos[a] = delta
            mejor = (max if problema_tipo.startswith("max") else min)(candidatos, key=candidatos.get)
            nueva.append(mejor)
        nueva = tuple(nueva)

        print(" Política mejorada:", nueva)

        # Convergencia
        if nueva==pol:
            print(f"\nConvergió en {iter_count} iteraciones.")
            print("Política Óptima (sin descuento):", pol)
            print(f" g* = {g:.6f}")
            for i, vi in enumerate(v):
                print(f" v*_{i} = {vi:.6f}")
            pfin = np.zeros((n, n))
            for i, a in enumerate(pol):
                pfin[i, :] = probs[a][i]
            A = pfin.T - np.eye(n); A[-1, :] = 1.0
            b = np.zeros(n); b[-1] = 1.0
            pi_est = np.linalg.solve(A, b)
            costo_medio = sum(pi_est[i] * costos[pol[i]][i] for i in range(n))
            print(f"\nCosto medio E[C_(R)] = {costo_medio:.6f}")
            break
        pol = nueva

def evaluar_politica_con_desc(pol, datos, alpha):
    """Evalúa la política CON descuento α resolviendo (I – αP) v = c."""
    n      = datos["num_estados"]
    probs  = datos["probabilidades"]
    costos = datos["costos"]

    Pmat = np.zeros((n, n))
    cvec = np.zeros(n)
    for i, a in enumerate(pol):
        Pmat[i, :] = probs[a][i]
        cvec[i]    = costos[a][i]

    A = np.eye(n) - alpha * Pmat
    v = np.linalg.solve(A, cvec)
    return v

def metodo_policy_improvement_desc(datos):
    limpiar_pantalla()
    """Mejoramiento de Políticas CON descuento."""
    problema_tipo = datos["problema_tipo"].lower()
    n             = datos["num_estados"]
    politicas     = datos["politicas"]
    probs         = datos["probabilidades"]
    costos        = datos["costos"]

    # Leer α
    while True:
        try:
            alpha = float(input("Ingrese factor de descuento α (0 ≤ α < 1): ").strip())
            if 0 <= alpha < 1: break
        except:
            pass
        print("α inválido; debe ser 0 ≤ α < 1.")

    # Construir D(i)
    D = {i: [] for i in range(n)}
    for a, estados in politicas.items():
        for i in estados:
            D[i].append(a)

    # Política inicial
    while True:
        entrada = input(f"Ingrese política inicial ({n} acciones, coma-sep.): ")
        try:
            pol = tuple(int(x) for x in entrada.split(","))
            if len(pol)!=n or any(pol[i] not in D[i] for i in range(n)):
                raise ValueError
            break
        except:
            print("Política inválida, inténtalo de nuevo.")

    iter_count = 0
    while True:
        iter_count += 1
        print(f"\n--- Iteración {iter_count} (con descuento α={alpha}) ---")

        # Evaluar con descuento
        v = evaluar_politica_con_desc(pol, datos, alpha)
        for i, vi in enumerate(v):
            print(f" v_{i} = {vi:.6f}")

        # Mejorar
        nueva = []
        for i in range(n):
            candidatos = {}
            for a in D[i]:
                q = costos[a][i] + alpha * np.dot(probs[a][i], v)
                candidatos[a] = q
            mejor = (max if problema_tipo.startswith("max") else min)(
                candidatos, key=candidatos.get)
            nueva.append(mejor)
        nueva = tuple(nueva)

        print(" Política mejorada:", nueva)

        # Convergencia
        if nueva==pol:
            print(f"\nConvergió en {iter_count} iteraciones.")
            print("Política Óptima (con descuento):", pol)
            print("Valores finales v*: ")
            for i, vi in enumerate(v):
                print(f" v*_{i} = {vi:.6f}")
            pfin = np.zeros((n, n))
            for i, a in enumerate(pol):
                pfin[i, :] = probs[a][i]
            A = pfin.T - np.eye(n); A[-1, :] = 1.0
            b = np.zeros(n); b[-1] = 1.0
            pi_est = np.linalg.solve(A,b)
            costo_medio = sum(pi_est[i] * costos[pol[i]][i] for i in range(n))
            print(f"\nCosto medio E[C_(R)] = {costo_medio:.6f}")
            break
        pol = nueva
