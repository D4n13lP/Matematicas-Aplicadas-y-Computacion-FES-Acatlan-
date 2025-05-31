import java.text.DecimalFormat;
import javax.swing.JOptionPane;

public class Jacobi {

    public static void main(String[] args) {
        DecimalFormat d=new DecimalFormat("0.000000");
        double [] elementos=new double[4];
        double [][] matriz = new double[4][5];
        double x1=0, x2=0, x3=0, x4=0, Ea1, Ea2, Ea3, Ea4, Ea=0.00001;
        int n=1;

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<5; j++)
            {
                matriz[i][j]=Double.parseDouble(JOptionPane.showInputDialog("Ingresa los valores ["+i +"]["+j+"]"));
            }
        }

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<5; j++)
            {
                System.out.print(matriz[i][j]+"\t");
            }
            System.out.println("");
        }

        for(int x=0; x<4; x++)  //ingresar valores iniciales de x
        {
            elementos[x]=Double.parseDouble(JOptionPane.showInputDialog("Ingresa el valor inicial de x"+x+""));
        }

        System.out.println("");
        System.out.print("los valores iniciales de x son: \n");

        for(int x=0; x<4; x++){
            System.out.print(elementos[x]+"\t");
        }

        System.out.println("");

        do{
            double aux1= elementos[0]; 
            double aux2= elementos[1];
            double aux3= elementos[2];
            double aux4= elementos[3];

            x1= ( matriz[0][4] - (matriz[0][1]*elementos[1]) - (matriz[0][2]*elementos[2])-(matriz[0][3]*elementos[3]))/matriz[0][0];
            System.out.print("X1="+d.format(x1)+ "\t");
            x2= ( matriz[1][4] - (matriz[1][0]*elementos[0]) - (matriz[1][2]*elementos[2])-(matriz[1][3]*elementos[3]))/matriz[1][1];
            System.out.print("X2="+d.format(x2)+ "\t");
            x3= ( matriz[2][4] - (matriz[2][0]*elementos[0]) - (matriz[2][1]*elementos[1])-(matriz[2][3]*elementos[3]))/matriz[2][2];
            System.out.print("X3="+d.format(x3)+ "\t");
            x4= ( matriz[3][4] - (matriz[3][0]*elementos[0]) - (matriz[3][1]*elementos[1])-(matriz[3][2]*elementos[2]))/matriz[3][3];
            System.out.print("X4="+d.format(x4)+ "\t");
            System.out.println("");
            Ea1=Math.abs((x1-aux1)/x1);
            Ea2=Math.abs((x2-aux2)/x2);
            Ea3=Math.abs((x3-aux3)/x3);
            Ea4=Math.abs((x4-aux4)/x1);
            n++;
            elementos[0]=x1;
            elementos[1]=x2;
            elementos[2]=x3;
            elementos[3]=x4;
        }while((Ea3>Ea || Ea1>Ea || Ea2>Ea || Ea4>Ea) && n<50);

    }
}
