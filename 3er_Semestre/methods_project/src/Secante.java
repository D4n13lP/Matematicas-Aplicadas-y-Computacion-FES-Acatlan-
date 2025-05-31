import javax.swing.JOptionPane;

public class Secante {
    static double funcion(double x) {
        return x * x - 5 * x + 6;
    }

    static double metodoSecante(double x0, double x1, double exactitud, int maxIteraciones) {
        int iteracion = 1;

        while (iteracion <= maxIteraciones) {
            double x2 = x1 - (funcion(x1) * (x1 - x0)) / (funcion(x1) - funcion(x0));

            // Mostramos cada iteracion
            JOptionPane.showMessageDialog(null,
                    "Iteración " + iteracion + ": x0=" + x0 + ", x1=" + x1 + ", x2=" + x2);

            if (Math.abs(x2 - x1) < exactitud) {
                JOptionPane.showMessageDialog(null, "Solución  " + iteracion + ": " + x2);
                return x2;
            }

            x0 = x1;
            x1 = x2;
            iteracion++;
        }

        JOptionPane.showMessageDialog(null,
                "El método no convergió después de " + maxIteraciones + " iteraciones.");
        return Double.NaN;
    }

    public static void main(String[] args) {
        JOptionPane.showMessageDialog(null, "Secante (Metodo Iterativo)\n" + //
                "Se utiliza para encontrar raíces de una función, estima la tangente mediante una secante que pasa por dos puntos cercanos.\n" + //
                "Como por ejemplo en la ecuación presentada x^2 - 5x + 6, podemos utilizar de valores iniciales 1 y 1.2\n" + //
                "", "Descripcion", 1);

        String x0Input = JOptionPane.showInputDialog("Ingrese el primer valor inicial (x0): ");
        double x0 = Double.parseDouble(x0Input);

        String x1Input = JOptionPane.showInputDialog("Ingrese el segundo valor inicial (x1): ");
        double x1 = Double.parseDouble(x1Input);

        String exactitudInput = JOptionPane.showInputDialog("Ingrese la tolerancia: ");
        double exactitud = Double.parseDouble(exactitudInput);

        String maxIteracionesInput = JOptionPane.showInputDialog("Ingrese el número máximo de iteraciones: ");
        int maxIteraciones = Integer.parseInt(maxIteracionesInput);

        double solucion = metodoSecante(x0, x1, exactitud, maxIteraciones);

        if (!Double.isNaN(solucion)) {
            JOptionPane.showMessageDialog(null, "La solución aproximada es: " + solucion);
        }
    }
}
