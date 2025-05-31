import javax.swing.JOptionPane;

public class FalsaPosicion {

    static double funcion(double x) {
        return Math.log(x * x * x + 2) * Math.tan(x);
    }

    static double metodoFalsaPosicion(double a, double b, double tolerancia, int maxIteraciones) {
        int iteracion = 1;

        while (iteracion <= maxIteraciones) {
            double fa = funcion(a);
            double fb = funcion(b);
            double c = (a * fb - b * fa) / (fb - fa);

            // Mostrar información de la iteración
            JOptionPane.showMessageDialog(null,
                    "Iteración " + iteracion + ": a=" + a + ", b=" + b + ", c=" + c);

            if (Math.abs(funcion(c)) < tolerancia) {
                JOptionPane.showMessageDialog(null,
                        "Solución encontrada en la iteración " + iteracion + ": " + c);
                return c;
            }

            // Actualizar los extremos del intervalo
            if (funcion(c) * fa < 0) {
                b = c;
            } else {
                a = c;
            }

            iteracion++;
        }

        JOptionPane.showMessageDialog(null,
                "El método no convergió después de " + maxIteraciones + " iteraciones.");
        return Double.NaN; // NaN representa que no se encontró una solución
    }

    public static void main(String[] args) {
        JOptionPane.showMessageDialog(null, "Falsa posicion (Metodo Iterativo)\n" + //
                "Utilizado para encontrar raíces de una función al buscar un intervalo donde la función cambie de signo y luego aproximando la raíz mediante una interpolación lineal.\n" + //
                "Como por ejemplo en la ecuación presentada ln(x^3 + 2) * tan(x), podemos utilizar de valores iniciales a->2 b->4\n" + //
                "", "Descripcion", 1);
        String aInput = JOptionPane.showInputDialog("Ingrese el extremo izquierdo del intervalo (a): ");
        double a = Double.parseDouble(aInput);

        String bInput = JOptionPane.showInputDialog("Ingrese el extremo derecho del intervalo (b): ");
        double b = Double.parseDouble(bInput);

        String toleranciaInput = JOptionPane.showInputDialog("Ingrese la tolerancia: ");
        double tolerancia = Double.parseDouble(toleranciaInput);

        String maxIteracionesInput = JOptionPane.showInputDialog("Ingrese el número máximo de iteraciones: ");
        int maxIteraciones = Integer.parseInt(maxIteracionesInput);

        double solucion = metodoFalsaPosicion(a, b, tolerancia, maxIteraciones);

        if (!Double.isNaN(solucion)) {
            JOptionPane.showMessageDialog(null, "La solución aproximada es: " + solucion);
        }
    }
}
