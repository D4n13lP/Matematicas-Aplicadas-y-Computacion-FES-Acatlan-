import java.util.Scanner;

public class Gauss {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Ingrese el número de ecuaciones:");
        int n = sc.nextInt(); //número de ecuaciones
        double[][] A = new double[n][n]; //matriz de coeficientes
        double[] b = new double[n]; //vector de términos independientes
        double[] x = new double[n]; //vector de soluciones

        //Lectura de los datos
        System.out.println("Ingrese los coeficientes de la matriz A:");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] = sc.nextDouble();
            }
        }
        System.out.println("Ingrese los términos independientes del vector b:");
        for (int i = 0; i < n; i++) {
            b[i] = sc.nextDouble();
        }

        //Aplicación del método de Gauss
        for (int k = 0; k < n - 1; k++) { //para cada columna
            for (int i = k + 1; i < n; i++) { //para cada fila debajo de la diagonal
                double m = A[i][k] / A[k][k]; //factor de multiplicación
                for (int j = k; j < n; j++) { //para cada elemento de la fila
                    A[i][j] = A[i][j] - m * A[k][j]; //restar la fila k multiplicada por m
                }
                b[i] = b[i] - m * b[k]; //restar el término independiente k multiplicado por m
            }
        }

        //Resolución por sustitución hacia atrás
        x[n - 1] = b[n - 1] / A[n - 1][n - 1]; //última solución
        for (int i = n - 2; i >= 0; i--) { //para cada fila de abajo hacia arriba
            double s = 0; //suma de los productos de los coeficientes por las soluciones
            for (int j = i + 1; j < n; j++) { //para cada columna a la derecha de la diagonal
                s = s + A[i][j] * x[j]; //acumular el producto
            }
            x[i] = (b[i] - s) / A[i][i]; //calcular la solución
        }

        //Mostrar la matriz reducida y las soluciones
        System.out.println("La matriz reducida es:");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                System.out.print(A[i][j] + "\t");
            }
            System.out.println("| " + b[i]);
        }
        System.out.println("Las soluciones son:");
        for (int i = 0; i < n; i++) {
            System.out.println("x" + (i + 1) + " = " + x[i]);
        }
    }
}
