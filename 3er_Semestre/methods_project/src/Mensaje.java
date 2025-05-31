import javax.swing.*;
import java.awt.event.*;

public class Mensaje {

    public static void main(String[] args) {
        //crear el mensaje que se quiere mostrar
        String mensaje = "Presionar enter para continuar";
        //crear el cuadro de diálogo con el mensaje
        JOptionPane dialogo = new JOptionPane(mensaje, JOptionPane.INFORMATION_MESSAGE);
        //crear un JDialog a partir del JOptionPane
        JDialog dialog = dialogo.createDialog(null, "Mensaje");
        //agregar un escuchador de ventana al JDialog
        dialog.addWindowListener(new WindowListener() {
            @Override
            public void windowOpened(WindowEvent e) {
                //obtener el panel raíz del cuadro de diálogo
                JRootPane rootPane = dialogo.getRootPane();
                //obtener el mapa de entrada del panel raíz
                InputMap inputMap = rootPane.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
                //obtener el mapa de acción del panel raíz
                ActionMap actionMap = rootPane.getActionMap();
                //asociar la tecla Enter con una cadena de identificación
                inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "ENTER");
                //asociar la cadena de identificación con una acción
                actionMap.put("ENTER", new AbstractAction() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        //cerrar el cuadro de diálogo
                        dialog.dispose();
                        //ejecutar el código de la clase VentanaMenu
                        VentanaMenu.main(args);
                    }
                });
            }

            //los demás métodos de la interfaz WindowListener no se implementan
            @Override
            public void windowClosing(WindowEvent e) {}

            @Override
            public void windowClosed(WindowEvent e) {}

            @Override
            public void windowIconified(WindowEvent e) {}

            @Override
            public void windowDeiconified(WindowEvent e) {}

            @Override
            public void windowActivated(WindowEvent e) {}

            @Override
            public void windowDeactivated(WindowEvent e) {}
        });
        //mostrar el JDialog
        dialog.setVisible(true);
    }
}
