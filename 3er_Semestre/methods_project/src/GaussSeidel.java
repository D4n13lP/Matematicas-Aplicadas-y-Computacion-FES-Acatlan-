//package gaussSeidel;
import javax.swing.JOptionPane;

public class GaussSeidel {

    public static void main(String[] args) {

        JOptionPane.showMessageDialog(null, "Gauss-Seidel (Metodo Iterativo)\n" + //
                "Utilizado para resolver sistemas de ecuaciones lineales mediante aproximaciones sucesivas, actualizando cada variable en cada iteración.\n" + //
                "", "Descripcion", 1);
        int n = Integer.parseInt(JOptionPane.showInputDialog("Ingrese el número de ecuaciones en el sistema:"));
        JOptionPane.showMessageDialog(null, "Ten en cuenta tener una matriz diagonal dominante =)");

        double[][] coeficientes = new double[n][n];
        double[] constantes = new double[n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                coeficientes[i][j] = Double.parseDouble(JOptionPane.showInputDialog("Ingrese el coeficiente para x" + (j + 1) + " de la ecuación " + (i + 1) + ":"));
            }
            constantes[i] = Double.parseDouble(JOptionPane.showInputDialog("Ingrese el término constante para la ecuación " + (i + 1) + ":"));
        }

        double[] valoresIniciales = new double[n];
        for (int i = 0; i < n; i++) {
            valoresIniciales[i] = Double.parseDouble(JOptionPane.showInputDialog("Ingrese el valor inicial para x" + (i + 1) + ":"));
        }

        int iteracionesMaximas = Integer.parseInt(JOptionPane.showInputDialog("Ingrese el número máximo de iteraciones:"));
        double tolerancia = Double.parseDouble(JOptionPane.showInputDialog("Ingrese la tolerancia(se recomienda 0.0001 =) ):"));

        double[] resultados = gaussSeidel(coeficientes, constantes, valoresIniciales, iteracionesMaximas, tolerancia);

        StringBuilder resultadosStr = new StringBuilder("Resultados:\n");
        for (int i = 0; i < resultados.length; i++) {
            resultadosStr.append(String.format("x%d = %.4f%n", i + 1, resultados[i]));
        }
        JOptionPane.showMessageDialog(null, resultadosStr.toString());
    }

    public static double[] gaussSeidel(double[][] coeficientes, double[] constantes, double[] valoresIniciales, int iteracionesMaximas, double tolerancia) {
        int n = constantes.length;
        double[] resultados = new double[n];
        double[] resultadosAnteriores = new double[n];

        System.arraycopy(valoresIniciales, 0, resultados, 0, n);

        for (int iteracion = 0; iteracion < iteracionesMaximas; iteracion++) {
            for (int i = 0; i < n; i++) {
                resultadosAnteriores[i] = resultados[i];

                double suma = constantes[i];
                for (int j = 0; j < n; j++) {
                    if (j != i) {
                        suma -= coeficientes[i][j] * resultados[j];
                    }
                }
                resultados[i] = suma / coeficientes[i][i];
            }

            StringBuilder iteracionStr = new StringBuilder("Iteración ").append(iteracion + 1).append(":\n");
            for (int i = 0; i < resultados.length; i++) {
                iteracionStr.append(String.format("x%d = %.4f%n", i + 1, resultados[i]));
            }
            JOptionPane.showMessageDialog(null, iteracionStr.toString());

            boolean convergencia = true;
            for (int i = 0; i < n && convergencia; i++) {
                if (Math.abs(resultados[i] - resultadosAnteriores[i]) > tolerancia) {
                    convergencia = false;
                }
            }

            if (convergencia) {
                JOptionPane.showMessageDialog(null, "Convergencia alcanzada después de " + (iteracion + 1) + " iteraciones.");
                return resultados;
            }
        }

        JOptionPane.showMessageDialog(null, "El método de Gauss-Seidel no converge en " + iteracionesMaximas + " iteraciones.");
        return resultados;
    }
}