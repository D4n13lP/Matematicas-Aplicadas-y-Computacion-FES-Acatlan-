import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Portada extends JFrame {

    private VentanaMenu menu;

    public Portada(VentanaMenu menu) {
        super("Pantalla de inicio");
        this.menu = menu;
        setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        setSize(800, 600);
        setAlwaysOnTop(true);

        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 10, 5, 10);

        panel.setBackground(new Color(173, 216, 230));

        // Cargar la imagen
        ImageIcon iconoUNAM = new ImageIcon(getClass().getResource("logo-azul.png"));

        // Obtener el ancho y el alto de la imagen
        int ancho = iconoUNAM.getIconWidth();
        int alto = iconoUNAM.getIconHeight();

        // Reducir las dimensiones a la mitad
        int nuevoAncho = ancho / 3;
        int nuevoAlto = alto / 3;

        // Escalar la imagen a las nuevas dimensiones
        Image imagen = iconoUNAM.getImage().getScaledInstance(nuevoAncho, nuevoAlto, Image.SCALE_SMOOTH);

        // Crear un nuevo ImageIcon con la imagen escalada
        ImageIcon imagenEscalada = new ImageIcon(imagen);

        JLabel imagenLabel = new JLabel(imagenEscalada);
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 2;
        panel.add(imagenLabel, gbc);

        JLabel tituloLabel = new JLabel("Universidad Nacional Autonoma de México");
        tituloLabel.setHorizontalAlignment(SwingConstants.CENTER);
        tituloLabel.setFont(new Font("Arial", Font.BOLD, 24));
        gbc.gridy = 1;
        panel.add(tituloLabel, gbc);

        JLabel subtituloLabel = new JLabel("Métodos Numéricos: Proyecto final");
        subtituloLabel.setHorizontalAlignment(SwingConstants.CENTER);
        subtituloLabel.setFont(new Font("Arial", Font.PLAIN, 18));
        gbc.gridy = 2;
        panel.add(subtituloLabel, gbc);

        JLabel nombreEscuelaLabel = new JLabel("Integrantes de equipo:");
        nombreEscuelaLabel.setHorizontalAlignment(SwingConstants.CENTER);
        nombreEscuelaLabel.setFont(new Font("Arial", Font.PLAIN, 18));
        gbc.gridy = 3;
        panel.add(nombreEscuelaLabel, gbc);

        JLabel integrante1Label = new JLabel("Garrido Vazquez Fernanda");
        integrante1Label.setFont(new Font("Arial", Font.PLAIN, 16));
        gbc.gridy = 4;
        panel.add(integrante1Label, gbc);

        JLabel integrante2Label = new JLabel("Martinez Cortes Camila Montserrat");
        integrante2Label.setFont(new Font("Arial", Font.PLAIN, 16));
        gbc.gridy = 5;
        panel.add(integrante2Label, gbc);

        JLabel integrante3Label = new JLabel("Martínez Garcia Roberto");
        integrante3Label.setFont(new Font("Arial", Font.PLAIN, 16));
        gbc.gridy = 6;
        panel.add(integrante3Label, gbc);

        JLabel integrante4Label = new JLabel("Pineda Ortega Daniel");
        integrante4Label.setFont(new Font("Arial", Font.PLAIN, 16));
        gbc.gridy = 7;
        panel.add(integrante4Label, gbc);

        JButton cerrarButton = new JButton("Iniciar Programa");
        cerrarButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                dispose();
            }
        });
        gbc.gridy = 8;
        panel.add(cerrarButton, gbc);

        add(panel);
        setLocationRelativeTo(null);
        setVisible(true);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                menu.setVisible(true);
            }
        });
    }
}
