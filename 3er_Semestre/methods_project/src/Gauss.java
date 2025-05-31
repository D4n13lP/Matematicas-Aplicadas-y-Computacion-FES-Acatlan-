import javax.swing.*;

public class Gauss {
    public static void main(String[] args) {

        JOptionPane.showMessageDialog(null, "Gauss (Metodo Directo)\n" + //
                "Utilizado para la eliminación de Gauss, que resuelve sistemas de ecuaciones lineales mediante operaciones de fila en una matriz aumentada.\n" + //
                "", "Descripcion", 1);
        //pedir al usuario que introduzca el número de ecuaciones
        String input = JOptionPane.showInputDialog(null, "Introduce el número de ecuaciones:");
        //convertir el input a un número entero
        int m = Integer.parseInt(input);
        //pedir al usuario que introduzca el número de incógnitas
        input = JOptionPane.showInputDialog(null, "Introduce el número de incógnitas:");
        //convertir el input a un número entero
        int n = Integer.parseInt(input);
        double[][] matriz = new double[m][n+1];
        double[] solucion = new double[n];

        //pedir al usuario que introduzca los coeficientes de las ecuaciones y los términos independientes
        for (int i = 0; i < m; i++) {
            for (int j = 0; j <= n; j++) {
                input = JOptionPane.showInputDialog(null, "Introduce el coeficiente de la fila " + (i+1) + " y la columna " + (j+1) + ":");
                //convertir el input a un número real
                matriz[i][j] = Double.parseDouble(input);
            }
        }

        for (int i = 0; i < m; i++) {
            for (int j = i+1; j < m; j++) {
                if (i < n) {
                    double factor = matriz[j][i] / matriz[i][i];
                    for (int k = i; k <= n; k++) {
                        matriz[j][k] -= factor * matriz[i][k];
                    }
                }
            }
        }

        //crear una variable para almacenar la matriz escalonada como una cadena
        String escalonada = "La matriz escalonada es:\n";
        for (int i = 0; i < m; i++) {
            for (int j = 0; j <= n; j++) {
                escalonada += matriz[i][j] + " ";
            }
            escalonada += "\n";
        }
        //mostrar la matriz escalonada por una ventana
        JOptionPane.showMessageDialog(null, escalonada);

        for (int i = Math.min(m, n)-1; i >= 0; i--) {
            double suma = 0;
            for (int j = i+1; j < n; j++) {
                suma += matriz[i][j] * solucion[j];
            }
            solucion[i] = (matriz[i][n] - suma) / matriz[i][i];
        }

        //crear una variable para almacenar la solución como una cadena
        String solucionStr = "La solución es:\n";
        for (int i = 0; i < n; i++) {
            solucionStr += "x" + (i+1) + " = " + solucion[i] + "\n";
        }
        //mostrar la solución por una ventana
        JOptionPane.showMessageDialog(null, solucionStr);
    }
}
