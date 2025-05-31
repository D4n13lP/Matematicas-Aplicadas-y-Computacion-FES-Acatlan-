/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

//package com.metodos_numericos2.proy2;

/**
 *
 * @author USUARIO
 */

import java.util.InputMismatchException;
import java.util.Scanner;

/**
 *
 * @author USUARIO
 */
public class Proy2 {

    public static void main(String[] args) {
        Scanner sn = new Scanner(System.in);
        
        //grafico pizarra = new grafico();
        //pizarra.setVisible(true);
        
        System.out.println("\tUniversidad Autonoma de Mexico");
        System.out.println("\t\tFES. Acatlan");
        System.out.println("\n");
	     System.out.println("\t\tMetodos Numericos\t\t");
        System.out.println("\n");
	     System.out.println("\tPresenta:");
	     System.out.println("camila");
	     System.out.println("fernanda");
	     System.out.println("daniel");
	     System.out.println("roberto");
	     System.out.println("josue");
        
        System.out.println("\n\t\t---Pulse ENTER para continuar---");
         sn.nextLine();

        boolean salir = false;
      int opcion;

      while(!salir){

         System.out.println("\n\t-- Programa que muestra un menu con opciones de metodos numericos --");

         System.out.println("1.- Falsa posicion");
         System.out.println("2.- Newton");
         System.out.println("3.- Secante");
         System.out.println("4.- Gauss");
         System.out.println("5.- Jacobi");
         System.out.println("6.- Gauss - Seidel");
         System.out.println("7.- Crout");
         System.out.println("8.- Salir");

         try{

            System.out.println("Seleccione la opcion deseada: ");
            opcion = Integer.parseInt(sn.nextLine());

            switch (opcion){
            case 1:
               //Ejercicios.funciones();
                System.out.println("\t METODO DE FALSA POSICION");
                System.out.println("Se trata de un procedimiento aritmético que"
                        + "\npermite resolver ecuaciones lineales. Para ello "
                        + "\nparte de un valor cualquiera (método simple) o de dos"
                        + "\nvalores(doble falsa posición). A partir de estas "
                        + "\nfalsas posiciones se obtiene la solución de la"
                        + "\necuación por proporcionalidad.");
                //aqui van los metodos
                System.out.println("\n\t\t---Pulse ENTER para continuar---");
                sn.nextLine();
               break;
            case 2:
                System.out.println("\t METODO DE NEWTON");
                System.out.println("Es otro método que se utiliza para calcular"
                        + "\nlos ceros de una función real de variable real."
                        + "\nAunque no sea siempre el mejor método para un"
                        + "\nproblema dado, su simplicidad formal y su rapidez"
                        + "\nde convergencia hacen que, con frecuencia, sea el"
                        + "\nprimer algoritmo a considerar para esta tarea.");
                //aqui van los metodos
                System.out.println("\n\t\t---Pulse ENTER para continuar---");
                sn.nextLine();
               break;
            case 3:
               System.out.println("\t METODO DE LA SECANTE");
               System.out.println("El método de la secante es un algoritmo de la"
                       + "\nraíz de investigación que utiliza una serie de raíces"
                       + "\nde las líneas secantes para aproximar mejor la raíz"
                       + "\nde una función f. El método de la secante se puede"
                       + "\nconsiderar como una aproximación en diferencias"
                       + "\nfinitas del método de Newton.");
               //aqui van los metodos
                System.out.println("\n\t\t---Pulse ENTER para continuar---");
                sn.nextLine();
               break;
            case 4:
                System.out.println("\t METODO DE GAUSS");
                System.out.println("Consiste en usar operaciones elementales en"
                        + "\nlas ecuaciones para transformar un sistema de"
                        + "\necuaciones lineales n × n , en un sistema reducido"
                        + "\nde forma triangular; que es, por tanto, equivalente"
                        + "\nal original. Este nuevo sistema triangular es equivalente"
                        + "\nal sistema original: tiene las mismas soluciones.");
                //aqui van los metodos
                System.out.println("\n\t\t---Pulse ENTER para continuar---");
                sn.nextLine();
               break;
            case 5:
                System.out.println("\t METODO DE JACOBI");
                System.out.println("La base del método consiste en construir una"
                        + "\nsucesión convergente definida iterativamente. El límite"
                        + "\nde esta sucesión es precisamente la solución del sistema."
                        + "\nA efectos prácticos si el algoritmo se detiene después"
                        + "\nde un número finito de pasos se llega a una aproximación"
                        + "\nal valor de x de la solución del sistema.");
                //aqui van los metodos
                System.out.println("\n\t\t---Pulse ENTER para continuar---");
                sn.nextLine();
               break;
            case 6:
                System.out.println("\t METODO DE GAUSS-SEIDEL");
                System.out.println("En el método de Gauss-Seidel se propone ir"
                        + "\nsustituyendo los nuevos valores de la aproximación"
                        + "\nsiguiente conforme se vayan obteniendo sin esperar a"
                        + "\ntener un vector completo. De esta forma se acelera la convergencia.");
                //aqui van los metodos
                System.out.println("\n\t\t---Pulse ENTER para continuar---");
                sn.nextLine();
               break;
            case 7:
                System.out.println("\t METODO DE CROUT");
                System.out.println("""
                                   La descomposicion LU es un metodo anal\u0131tico
                                   muy recurrido por las ventajas de su programacion, tal
                                   como el almacenamiento en memoria, su convergencia
                                   y su algoritmo relativamente
                                   sencillo de elaborar. Se mostrara como se descompone
                                   una matriz de orden nxn y posteriormente
                                   la forma en que esta descomposicion se aplica en la solucion
                                   de sistemas de ecuaciones lineales.""");
                //aqui van los metodos
                System.out.println("\n\t\t---Pulse ENTER para continuar---");
                sn.nextLine();
               break;
            case 8:
               salir=true;
               break;   
            default:   
               System.out.println("Las opciones son entre 1 y 8");

            }
         }
         catch(InputMismatchException | NumberFormatException e){
            System.out.println("Debes seleccionar un numero de opcion");
         }
         
         //Pausa en la ejecución del programa
         System.out.println("\n\t\t---Pulse ENTER para continuar---");
         sn.nextLine();
      }
    }
}