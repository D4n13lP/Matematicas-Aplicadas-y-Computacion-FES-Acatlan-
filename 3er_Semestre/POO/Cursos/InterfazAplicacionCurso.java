//Daniel Pineda Ortega
//package Daniel;

import java.awt.BorderLayout;
import javax.swing.JFrame;

public class InterfazAplicacionCurso extends JFrame{
	
	//panel para registrar los datos del alumno 
	private PanelDatos panelDatos;
	
	//Panel para registrar curso y tutor
	private PanelRegistro panelRegistro;
	
	//panel para los botones de navegacion
	private PanelNavegacion panelNavegacion;
	
	public InterfazAplicacionCurso() {
		setTitle("Aplicacion Cursos");
		setSize(700,450);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setLocationRelativeTo(null);
		setResizable(false);
		setLayout(new BorderLayout());
		
		//crear las instancias de los paneles
		panelDatos = new PanelDatos();
		panelRegistro = new PanelRegistro();
		panelNavegacion = new PanelNavegacion();
		
		//agregar los paneles al Frame
		add(panelDatos, BorderLayout.NORTH);
		add(panelRegistro, BorderLayout.CENTER);
		add(panelNavegacion, BorderLayout.SOUTH);
		
	}
	
	public static void main(String[] args) {
		InterfazAplicacionCurso interfaz = new InterfazAplicacionCurso();
		interfaz.setVisible(true);   // Hay que cambiar a visible siempre 
	}
}
