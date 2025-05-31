#!/usr/bin/env python3
# algorithms/method_lp.py

"""
Resolución de PMD por Programación Lineal.

Formulación:
  Min (o Max) z = Σ_{i,j} c_{i,j} · y_{i,j}

Sujeto a:
    Σ_{j∈D(i)} y_{i,j}  – Σ_{k, j∈D(k)} p_j[k][i] · y_{k,j} = 0   ∀ i
    Σ_{i,j} y_{i,j} = 1
    y_{i,j} ≥ 0   (para todo par i,j, incluso si vale 0)
"""

from pulp import (
    LpProblem, LpVariable, lpSum,
    LpStatus, LpMinimize, LpMaximize, PULP_CBC_CMD
)
from lim_pant import limpiar_pantalla

def metodo_programacion_lineal(datos):
    limpiar_pantalla()

    problema_tipo = datos["problema_tipo"]       # "Maximizar" o "Minimizar"
    n = datos["num_estados"]
    politicas = datos["politicas"]               # dict: j → [estados]
    costos = datos["costos"]                     # dict: j → {i: c_{i,j}}
    probs = datos["probabilidades"]              # dict: j → matriz n×n

    # Creamos el LP
    sentido = LpMinimize if problema_tipo == "Minimizar" else LpMaximize
    prob = LpProblem("PMD_AverageCost", sentido)

    # Variables y_{i,j} ≥ 0, incluso si luego resultan 0
    y = {}
    for j, estados in politicas.items():
        for i in estados:
            y[(i, j)] = LpVariable(f"y_{i}_{j}", lowBound=0)

    # Objetivo: z = Σ c_{i,j} · y_{i,j}
    objetivo = lpSum(costos[j][i] * y[(i, j)] for (i, j) in y)
    prob += objetivo, "z"

    # Restricciones de flujo estacionario para cada estado i
    restricciones = []
    for i in range(n):
        salidas = [y[(i, j)] for j in politicas if (i, j) in y]
        entradas = []
        for j, estados in politicas.items():
            for k in estados:
                # flujo de k->i bajo acción j
                entradas.append(probs[j][k][i] * y[(k, j)])
        restr = lpSum(salidas) - lpSum(entradas) == 0
        name = f"Restricción {i}"
        prob += restr, name
        restricciones.append((name, restr))

    # Restricción de normalización:
    norm = lpSum(y[(i, j)] for (i, j) in y) == 1
    prob += norm, "Normalización"
    restricciones.append(("Normalización", norm))

    # 1) Mostrar el PPL (manualmente)
    sentido_str = "Minimizar" if sentido == LpMinimize else "Maximizar"
    print(f"{sentido_str} z = ", end="")
    # Objetivo impreso a mano:
    terms = [f"{costos[j][i]}·y_{i}_{j}" for (i, j) in y]
    print(" + ".join(terms))

    print("\nSujeto a:")
    for name, restr in restricciones:
        # Cada restricción con tabulador
        print(f"\t{name}: {restr}")

    print("\nVariables y_{i,j} ≥ 0:")
    # Listamos todas las variables, incluso si se quedan en cero
    for (i, j), var in y.items():
        print(f"\t y_{i}_{j} ≥ 0")

    # 2) Resolver
    prob.solve(PULP_CBC_CMD(msg=0))
    print(f"\nEstado de la solución: {LpStatus[prob.status]}\n")

    # 3) Mostrar todas las y_{i,j} con su valor (incluso ceros)
    print("Valores de variables y_{i,j}:")
    sol_y = {}
    for (i, j), var in y.items():
        val = var.value() or 0.0
        sol_y[(i, j)] = val
        print(f"\t y_{i}_{j} = {val:.6f}")

    # 4) Derivar política óptima: para cada estado i, elegir j con mayor y_{i,j}
    pol_opt = []
    for i in range(n):
        candidatos = {j: sol_y.get((i, j), 0.0) for j in politicas if (i, j) in y}
        mejor_j = max(candidatos, key=candidatos.get)
        pol_opt.append(mejor_j)

    # 5) Distribución estacionaria π[i] = Σ_{j∈D(i)} y_{i,j}
    pi = [sum(sol_y.get((i, j), 0.0) for j in politicas if (i, j) in y)
          for i in range(n)]

    # 6) Costo esperado E(C_{R*})
    costo_esperado = sum(costos[j][i] * sol_y[(i, j)] for (i, j) in sol_y)

    # 7) Impresión final
    print("\n" + "=" * 60)
    print("POLÍTICA ÓPTIMA (LP)".center(60))
    print("=" * 60)
    print(f"R* = {tuple(pol_opt)}")
    print("π* =", [round(v, 6) for v in pi])
    print(f"E(C_{{R*}}) = {costo_esperado:.6f}\n")

