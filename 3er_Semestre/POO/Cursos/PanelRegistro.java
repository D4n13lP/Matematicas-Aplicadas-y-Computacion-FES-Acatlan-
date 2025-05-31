//package Daniel;
import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.TitledBorder;

public class PanelRegistro extends JPanel{
	private JLabel lbCurso;
	private JComboBox<String> cbCurso;

	private JLabel lbTutor;
	private JComboBox<String> cbTutor;

	//botones
	private JButton btnRegistrarCurso;
	private JButton btnEliminarCurso;
	private JButton btnRegistrarTutor;
	private JButton btnEliminarTutor;

	public PanelRegistro() {
		setLayout(null);
		TitledBorder border=BorderFactory.createTitledBorder("Registro a Cursos");
		border.setTitleColor(Color.BLUE);
		setBorder(border);

		lbCurso = new JLabel("Curso: ");
		lbTutor = new JLabel("Tutor: ");

		//Creacion listas desplegables
		cbCurso = new JComboBox<>();
		cbCurso.addItem("CURSO 1");
		cbCurso.addItem("CURSO 2");
		cbCurso.addItem("CURSO 3");
		cbCurso.addItem("CURSO 4");

		cbTutor = new JComboBox<String>();
		cbTutor.addItem("TUTOR 1");
		cbTutor.addItem("TUTOR 2");
		cbTutor.addItem("TUTOR 3");

		//Creacion de botones
		btnRegistrarCurso = new JButton("Registrar Curso");
		btnEliminarCurso = new JButton("Eliminar Curso");
		btnRegistrarTutor = new JButton("Registrar Tutor");
		btnEliminarTutor = new JButton("Eliminar Tutor");

		//Posiciones para cada objeto, etiqueta, lista desplegable y botones
		lbCurso.setBounds(5,50,50,20);
		cbCurso.setBounds(60,50,200,20);
		lbTutor.setBounds(5,100,500,20);
		cbTutor.setBounds(60,100,200,20);

		btnRegistrarCurso.setBounds(10,150,140,20);
		btnEliminarCurso.setBounds(160,150,140,20);
		btnRegistrarTutor.setBounds(310,150,140,20);
		btnEliminarTutor.setBounds(460,150,140,20);

		//se agregan las etiquetas y las listas desplegables al panel
		add(lbCurso);
		add(cbCurso);
		add(lbTutor);
		add(cbTutor);
		add(btnRegistrarCurso);
		add(btnEliminarCurso);
		add(btnRegistrarTutor);
		add(btnEliminarTutor);
	}

}
