import javax.swing.JOptionPane;

public class Newton {

    public static double funcion(double x) {
        return Math.pow(x, 2) - Math.exp(x) - 3 * x + 2;
    }

    public static double derivadaFuncion(double x) {
        return 2 * x - Math.exp(x) - 3;
    }

    public static double newton(double x0, int ciclomaximo, double valor) {
        double x = x0;
        int iteracion = 0;
        JOptionPane.showMessageDialog(null, "Newton (Metodo Iterativo)\n" + //
                "Utilizado para encontrar raíces de una función mediante aproximaciones sucesivas basadas en la tangente a la curva de la función.\n" + //
                "Comienza con una suposición inicial y, en cada iteración, mejora la aproximación a la raíz utilizando la tangente a la curva de la función en el punto actual.\n" + //
                "", "Descripcion", 1);

        do {
            double fx = funcion(x);
            double dfx = derivadaFuncion(x);

            if (dfx == 0) {
                JOptionPane.showMessageDialog(null, "Valor de derivada = 0, no se puede seguir.");
                return Double.NaN;
            }

            double xSiguiente = x - fx / dfx;

            if (Math.abs(xSiguiente - x) < valor) {
                JOptionPane.showMessageDialog(null, "Convergencia buscada después de " + iteracion + " iteraciones.");
                return xSiguiente;
            }

            JOptionPane.showMessageDialog(null,
                    "Iteración " + iteracion + ": x = " + x + ", f(x) = " + fx + ", f'(x) = " + dfx);

            x = xSiguiente;
            iteracion++;

        } while (iteracion < ciclomaximo);

        JOptionPane.showMessageDialog(null, "Ciclos máximos alcanzados.");
        return Double.NaN;
    }

    public static void main(String[] args) {

        double x0 = 1.0;
        int ciclomaximo = 100;
        double valor = 1e-6;
        double raiz = newton(x0, ciclomaximo, valor);
        JOptionPane.showMessageDialog(null, "Raíz: " + raiz);
    }
}
