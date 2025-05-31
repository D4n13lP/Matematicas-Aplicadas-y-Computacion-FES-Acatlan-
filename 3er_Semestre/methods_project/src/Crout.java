//package biblioteca;
import javax.swing.JOptionPane;

public class Crout {

    public static void main(String[] args) {

        JOptionPane.showMessageDialog(null, "Crout (Metodo Directo)\n" + //
                "Utilizado para la descomposición LU, que es una forma de factorización de una matriz en el producto de una matriz triangular inferior (L) y una matriz triangular superior (U).", "Descripcion", 1);
        int n = Integer.parseInt(JOptionPane.showInputDialog("Ingrese el número de ecuaciones en el sistema:"));
        JOptionPane.showMessageDialog(null, "Ingrese los coeficientes y el término constante para cada ecuación.");

        double[][] matriz = new double[n][n];
        double[] b = new double[n];

        // Ingresar los coeficientes de la matriz y el término constante
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matriz[i][j] = Double.parseDouble(JOptionPane.showInputDialog("Ingrese el coeficiente para x" + (j + 1) + " de la ecuación " + (i + 1) + ":"));
            }
            b[i] = Double.parseDouble(JOptionPane.showInputDialog("Ingrese el término constante para la ecuación " + (i + 1) + ":"));
        }

        double[] resultados = resolverSistema(matriz, b);

        StringBuilder resultadosStr = new StringBuilder("Resultados:\n");
        for (int i = 0; i < resultados.length; i++) {
            resultadosStr.append(String.format("x%d = %.4f%n", i + 1, resultados[i]));
        }
        JOptionPane.showMessageDialog(null, resultadosStr.toString());
    }

    public static double[] resolverSistema(double[][] matriz, double[] b) {
        int n = b.length;

        double[][] L = new double[n][n]; // Matriz triangular inferior
        double[][] U = new double[n][n]; // Matriz triangular superior

        // Inicialización de matrices L y U
        for (int i = 0; i < n; i++) {
            U[i][i] = 1; // Diagonal de U es 1
        }

        // Algoritmo Crout para factorización LU
        for (int i = 0; i < n; i++) {
            // Calculando la matriz U
            for (int k = i; k < n; k++) {
                double sum = 0;
                for (int j = 0; j < i; j++) {
                    sum += L[i][j] * U[j][k];
                }
                U[i][k] = matriz[i][k] - sum;
            }

            // Calculando la matriz L
            for (int k = i; k < n; k++) {
                if (i == k) {
                    L[i][i] = 1; // Diagonal de L es 1
                } else {
                    double sum = 0;
                    for (int j = 0; j < i; j++) {
                        sum += L[k][j] * U[j][i];
                    }
                    L[k][i] = (matriz[k][i] - sum) / U[i][i];
                }
            }

            // Mostrar la matriz L en una ventana
            StringBuilder matrizLStr = new StringBuilder("Matriz L (Iteración ").append(i + 1).append("):\n");
            for (int l = 0; l < n; l++) {
                for (int m = 0; m < n; m++) {
                    matrizLStr.append(String.format("%.4f \t", L[l][m]));
                }
                matrizLStr.append("\n ");
            }
            JOptionPane.showMessageDialog(null, matrizLStr.toString(), "Matriz L (Iteración " + (i + 1) + ")", JOptionPane.INFORMATION_MESSAGE);

            // Mostrar la matriz U en una ventana
            double[][] matrizU = obtenerMatrizU(U);
            StringBuilder matrizUStr = new StringBuilder("Matriz U (Iteración ").append(i + 1).append("):\n");
            for (int l = 0; l < n; l++) {
                for (int m = 0; m < n; m++) {
                    matrizUStr.append(String.format("%.4f \t", matrizU[l][m]));
                }
                matrizUStr.append("\n ");
            }
            JOptionPane.showMessageDialog(null, matrizUStr.toString(), "Matriz U (Iteración " + (i + 1) + ")", JOptionPane.INFORMATION_MESSAGE);
        }

        // Resolver Ly = b usando sustitución hacia adelante
        double[] y = new double[n];
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < i; j++) {
                sum += L[i][j] * y[j];
            }
            y[i] = (b[i] - sum) / L[i][i];
        }

        // Resolver Ux = y usando sustitución hacia atrás
        double[] x = new double[n];
        for (int i = n - 1; i >= 0; i--) {
            double sum = 0;
            for (int j = i + 1; j < n; j++) {
                sum += U[i][j] * x[j];
            }
            x[i] = (y[i] - sum) / U[i][i];
        }

        return x;
    }

    public static double[][] obtenerMatrizU(double[][] matriz) {
        int n = matriz.length;
        double[][] U = new double[n][n];

        // Copiar elementos de la matriz original a la matriz U
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                U[i][j] = matriz[i][j];
            }
        }

        return U;
    }
}