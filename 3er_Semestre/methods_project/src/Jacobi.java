import javax.swing.*;
import java.text.DecimalFormat;

public class Jacobi {

    public static void main(String[] args) {
        DecimalFormat d=new DecimalFormat("0.000000");
        double [] elementos=new double[4];
        double [][] matriz = new double[4][5];
        double [] x=new double[4]; // vector para almacenar las soluciones
        double Ea=0.00001; // error absoluto relativo máximo deseado
        int n=1; // contador de iteraciones
        int maxIter = 100; // número máximo de iteraciones permitidas

        JOptionPane.showMessageDialog(null, "Jacobi (Metodo Iterativo)\n" + //
                "Se utiliza para resolver sistemas de ecuaciones lineales, pero actualizando todas las variables simultáneamente en cada iteración.\n" + //
                "", "Descripcion", 1);

        // Pedir el número de ecuaciones
        int nEcuaciones = Integer.parseInt(JOptionPane.showInputDialog("Ingresa el número de ecuaciones"));

        // Leer los datos de la matriz
        for(int i=0; i<nEcuaciones; i++)
        {
            for(int j=0; j<nEcuaciones+1; j++)
            {
                if (j < nEcuaciones) {
                    // Pedir el coeficiente de la variable x_j
                    matriz[i][j]=Double.parseDouble(JOptionPane.showInputDialog("Ingresa el coeficiente de x"+j+" en la ecuación "+i));
                } else {
                    // Pedir el término independiente de la ecuación i
                    matriz[i][j]=Double.parseDouble(JOptionPane.showInputDialog("Ingresa el término independiente de la ecuación "+i));
                }
            }
        }

        // Mostrar la matriz por pantalla
        mostrarMatriz(matriz, nEcuaciones);

        for(int i=0; i<nEcuaciones; i++)  //ingresar valores iniciales de x
        {
            elementos[i]=Double.parseDouble(JOptionPane.showInputDialog("Ingresa el valor inicial de x"+i+""));
        }

        // Crear una variable para almacenar el resultado
        String resultado = "";

        resultado += "\nlos valores iniciales de x son: \n";

        for(int i=0; i<nEcuaciones; i++){
            resultado += elementos[i]+"\t";
        }

        resultado += "\n";
        double [] x_old; // declarar la variable x_old
        double err; // declarar la variable para el error absoluto relativo máximo
        do{
            x_old = elementos.clone(); // copiar los valores anteriores de x
            for (int i = 0; i < nEcuaciones; i++) {
                x[i] = (matriz[i][nEcuaciones] - sum(matriz[i], x_old, i))/matriz[i][i]; // calcular la nueva solución
                resultado += "X"+(i+1)+" = "+d.format(x[i])+ ";\t ";
            }
            resultado += "\n";
            n++;
            elementos = x.clone(); // actualizar los valores de x
            err = error(x, x_old); // calcular el error absoluto relativo máximo
            resultado += "Iteración: "+n+", Error: "+d.format(err)+"\n"; // mostrar el número de iteración y el error
        }while(err > Ea && n < maxIter); // verificar la condición de parada

        // Mostrar el resultado en una ventana de JOptionPane
        resultado += "El método ha terminado\n"; // mostrar el mensaje final
        JOptionPane.showMessageDialog(null, resultado);
    }

    // método para calcular la suma de los productos de los coeficientes y las soluciones
    public static double sum(double[] coef, double[] sol, int i) {
        double sum = 0.0;
        for (int j = 0; j < coef.length-1; j++) {
            if (i != j) {
                sum += coef[j]*sol[j];
            }
        }
        return sum;
    }

    // método para calcular el error absoluto relativo máximo
    public static double error(double[] x, double[] x_old) {
        double max = 0.0;
        for (int i = 0; i < x.length; i++) {
            double err = Math.abs((x[i]-x_old[i])/x[i]);
            if (err > max) {
                max = err;
            }
        }
        return max;
    }

    // método para mostrar la matriz por pantalla
    public static void mostrarMatriz(double[][] matriz, int nEcuaciones) {
        for(int i=0; i<nEcuaciones; i++)
        {
            for(int j=0; j<nEcuaciones+1; j++)
            {
                System.out.print(matriz[i][j]+"\t");
            }
            System.out.println("");
        }
    }
}
