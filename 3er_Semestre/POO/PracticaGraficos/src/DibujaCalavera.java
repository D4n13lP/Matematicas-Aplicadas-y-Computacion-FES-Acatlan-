import javax.swing.JFrame;

public class DibujaCalavera {
    public static void main(String[] args) {
        Calavera dibujoCalavera = new Calavera();
        JFrame ventana = new JFrame();
        ventana.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        ventana.add(dibujoCalavera);
        ventana.setSize(600, 600);
        ventana.setVisible(true);
    }
}