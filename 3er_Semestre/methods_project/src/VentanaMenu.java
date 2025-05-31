import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;



public class VentanaMenu extends JFrame implements ActionListener {

    private JButton botonFalsaPosicion, botonNewton, botonSecante, botonGauss, botonJacobi, botonGaussS, botonCrout; //botones del menú

    public VentanaMenu() {
        super("Metodos Numericos"); //título de la ventana
        setSize(800, 600); //tamaño de la ventana
        setLocationRelativeTo(null); //centrar la ventana
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //cerrar el programa al cerrar la ventana
        setLayout(new GridLayout(2, 1)); //usar un layout de rejilla de 2 filas y 1 columna

        //crear los botones del menú
        botonFalsaPosicion = new JButton("Falsa Posicion");
        botonFalsaPosicion.setFont(new Font("Arial", Font.PLAIN, 20));
        botonFalsaPosicion.setForeground(new Color(128, 0, 128));
        botonNewton = new JButton("Newton");
        botonNewton.setFont(new Font("Arial", Font.PLAIN, 20));
        botonNewton.setForeground(new Color(128, 0, 128));
        botonSecante = new JButton("Secante");
        botonSecante.setFont(new Font("Arial", Font.PLAIN, 20));
        botonSecante.setForeground(new Color(128, 0, 128));
        botonGauss = new JButton("Gauss");
        botonGauss.setFont(new Font("Arial", Font.PLAIN, 20));
        botonGauss.setForeground(new Color(128, 0, 128));
        botonJacobi = new JButton("Jacobi");
        botonJacobi.setFont(new Font("Arial", Font.PLAIN, 20));
        botonJacobi.setForeground(new Color(128, 0, 128));
        botonGaussS = new JButton("Gauss-Seidel");
        botonGaussS.setFont(new Font("Arial", Font.PLAIN, 20));
        botonGaussS.setForeground(new Color(128, 0, 128));
        //botonGaussS.setBackground(new Color(173, 216, 230));
        botonGaussS.setOpaque(true);
        botonCrout = new JButton("Crout");
        botonCrout.setFont(new Font("Arial", Font.PLAIN, 20));
        botonCrout.setForeground(new Color(128, 0, 128));
        //agregar los botones a la ventana
        add(botonFalsaPosicion);
        add(botonNewton);
        add(botonSecante);
        add(botonGauss);
        add(botonJacobi);
        add(botonGaussS);
        add(botonCrout);
        //agregar un escuchador de acción a cada botón
        botonFalsaPosicion.addActionListener(this);
        botonNewton.addActionListener(this);
        botonSecante.addActionListener(this);
        botonGauss.addActionListener(this);
        botonJacobi.addActionListener(this);
        botonGaussS.addActionListener(this);
        botonCrout.addActionListener(this);
    }

    //método que se ejecuta al pulsar un botón
    @Override
    public void actionPerformed(ActionEvent e) {
        //obtener el botón que ha generado el evento
        JButton boton = (JButton) e.getSource();
        //obtener el texto del botón
        String texto = boton.getText();
        //si el botón es Gauss
        if (texto.equals("Gauss")) {
            //crear una instancia de la clase Gauss
            Gauss gauss = new Gauss();
            //llamar al método main de la clase Gauss con el argumento null
            gauss.main(null);
        }
        //si el botón es Falsa Posición
        if (texto.equals("Falsa Posicion")) {
            //crear una instancia de la clase FalsaPosicion
            FalsaPosicion fp = new FalsaPosicion();
            //llamar al método main de la clase FalsaPosicion con el argumento null
            fp.main(null);
        }
        //si el botón es Newton
        if (texto.equals("Newton")) {
            //crear una instancia de la clase Newton
            Newton fp = new Newton();
            //llamar al método main de la clase Newton con el argumento null
            fp.main(null);
        }
        //si el botón es Secante
        if (texto.equals("Secante")) {
            //crear una instancia de la clase Secante
            Secante fp = new Secante();
            //llamar al método main de la clase Secante con el argumento null
            fp.main(null);
        }
        //si el botón es Jacobi
        if (texto.equals("Jacobi")) {
            //crear una instancia de la clase Jacobi
            Jacobi fp = new Jacobi();
            //llamar al método main de la clase Jacobi con el argumento null
            fp.main(null);
        }
        //si el botón es Gauss-Seidel
        if (texto.equals("Gauss-Seidel")) {
            //crear una instancia de la clase Gauss-Seidel
            GaussSeidel fp = new GaussSeidel();
            //llamar al método main de la clase Gauss-Seidel con el argumento null
            fp.main(null);
        }
        //si el botón es Crout
        if (texto.equals("Crout")) {
            //crear una instancia de la clase Crout
            Crout fp = new Crout();
            //llamar al método main de la clase Crout con el argumento null
            fp.main(null);
        }
    }

    //método principal para ejecutar el programa
    public static void main(String[] args) {

        //Portada p = new Portada();
        //p.setVisible(true);
        //crear una instancia de la clase VentanaMenu y mostrarla
        VentanaMenu menu = new VentanaMenu();
        
        VentanaMenu vm = new VentanaMenu();
        Portada portada = new Portada(menu);
        vm.setVisible(true);
    }
}
