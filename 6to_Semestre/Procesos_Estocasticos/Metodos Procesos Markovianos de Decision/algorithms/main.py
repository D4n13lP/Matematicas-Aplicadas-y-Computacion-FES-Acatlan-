#!/usr/bin/env python3
# algorithms/main.py

from read import leer_datos_manualmente
from enumeration import metodo_enumeracion
from pro_lineal import metodo_programacion_lineal
from pol_mejoradas import metodo_policy_improvement
from pol_mejoradas import metodo_policy_improvement_desc
from aproximaciones import metodo_value_iteration


def main():
    print("=== LECTURA DE DATOS PARA PMD ===\n")
    datos = leer_datos_manualmente()

    while True:
        print("\n=== MENÚ PRINCIPAL ===")
        print("1) Enumeración Exhaustiva de Políticas")
        print("2) Programación Lineal")
        print("3) Mejoramiento de políticas")
        print("4) Mejoramiento de políticas con descuento")
        print("5) Aproximaciones sucesivas")
        print("Q) Salir")
        opc = input("Elige una opción: ").strip().lower()

        if opc == '1':
            metodo_enumeracion(datos)
        elif opc == '2':
            metodo_programacion_lineal(datos)
        elif opc == '3':
            metodo_policy_improvement(datos)
        elif opc == '4':
            metodo_policy_improvement_desc(datos)
        elif opc == '5':
            metodo_value_iteration(datos)
        elif opc in ('q', 'salir'):
            print("¡Hasta luego!")
            break
        else:
            print("Opción no válida. Intenta de nuevo.")

if __name__ == "__main__":
    main()
