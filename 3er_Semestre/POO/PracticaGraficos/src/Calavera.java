import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.Color;
import java.awt.Graphics;

public class Calavera extends JPanel {
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        // Cambia el color de fondo a negro
        setBackground(Color.BLACK);
        // Cambia el color del dibujo a beige
        g.setColor(new Color(245, 245, 220));
        // Dibuja la parte superior de la calavera (un círculo)
        g.fillOval(150, 100, 200, 200);
        // Dibuja la parte inferior de la calavera (un rectángulo redondeado más angosto)
        g.fillRoundRect(190, 265, 120, 75, 50, 50);
        g.setColor(Color.BLACK);
        g.fillOval(190, 200, 50, 50); // Ojo izquierdo
        g.fillOval(260, 200, 50, 50); // Ojo derecho
        // Dibuja la nariz como un triángulo
        g.fillPolygon(new int[] {235, 250, 265}, new int[] {260, 290, 260}, 3);
        // Dibuja los dientes
        for (int i = 210; i <= 290; i += 20) {
            g.drawLine(i, 325, i, 350);
        }
        // Cambia el color del dibujo a marrón
        g.setColor(new Color(139, 69, 19));
        // Dibuja la parte superior del sombrero (un óvalo)
        g.fillOval(125, 70, 250, 100);
        // Dibuja la parte inferior del sombrero (un óvalo más grande)
        g.fillOval(100, 120, 300, 50);
        // Cambia el color del dibujo a blanco para las velas
        g.setColor(Color.WHITE);
        // Dibuja la vela de la izquierda
        g.fillRect(80, 200, 30, 150);
        // Dibuja la vela de la derecha
        g.fillRect(390, 200, 30, 150);
        // Cambia el color del dibujo a amarillo para las llamas de las velas
        g.setColor(Color.YELLOW);
        // Dibuja la llama de la vela de la izquierda
        g.fillOval(85, 170, 20, 30);
        // Cambia el color del dibujo a rojo para las llamas de las velas
        g.setColor(Color.RED);
        // Dibuja la llama de la vela de la izquierda
        g.fillOval(90, 175, 10, 20);
        // Cambia el color del dibujo a amarillo para las llamas de las velas
        g.setColor(Color.YELLOW);
        // Dibuja la llama de la vela de la derecha
        g.fillOval(395, 170, 20, 30);
        // Cambia el color del dibujo a rojo para las llamas de las velas
        g.setColor(Color.RED);
        // Dibuja la llama de la vela de la derecha
        g.fillOval(400, 175, 10, 20);
        // Cambia el color del dibujo a naranja para las flores de cempasúchil
        g.setColor(Color.ORANGE);
        // Dibuja las flores de cempasúchil
        for (int i = 100; i <= 400; i += 50) {
            g.fillOval(i, 370, 40, 40); // Círculo exterior
            g.setColor(Color.YELLOW);
            g.fillOval(i+10, 380, 20, 20); // Círculo interior
            g.setColor(Color.ORANGE);
        }
        // Dibuja los cuadrados de diferentes colores
        for (int i = 50; i <= 450; i += 100) {
            g.setColor(new Color((int)(Math.random() * 0x1000000))); // Color aleatorio
            g.fillRect(i, 420, 80, 80); // Cuadrado
        }
        // Dibuja el arco de círculos de diferentes colores
        for (int i = 50; i <= 450; i += 50) {
            g.setColor(new Color((int)(Math.random() * 0x1000000))); // Color aleatorio
            g.drawOval(i, 20, 50, 50); // Círculo
        }
    }
}


