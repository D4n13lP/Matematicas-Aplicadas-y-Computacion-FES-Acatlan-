//package Daniel;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridLayout;

import java.awt.BorderLayout;
import java.awt.Color;
import javax.swing.*;
import javax.swing.border.TitledBorder;

public class PanelDatos extends JPanel{

	//etiqueta y zona de texto para el nombre
	private JLabel lbNombre;
	private JTextField txtNombre;
	//etiqueta y texto para el apellido paterno
	private JLabel lbPaterno;
	private JTextField txtPaterno;
	//etiqueta y texto para el apellido Materno
	private JLabel lbMaterno;
	private JTextField txtMaterno;
	//etiqueta y texto para el genero
	private JLabel lbGenero;
	private JTextField txtGenero;
	//etiqueta y texto para la fecha de nacimiento
	private JLabel lbFechaNac;
	private JTextField txtFechaNac;

	private JLabel lbImagen;
	
	public PanelDatos() {
		setLayout(new BorderLayout());
		TitledBorder border=BorderFactory.createTitledBorder("Datos Alumno");
		border.setTitleColor(Color.BLUE);
		setBorder(border);

		//definicion y creacion de paneles para la informacion del alumno y foto
		JPanel panelDatosAlumno = new JPanel();
		JPanel panelFotoAlumno = new JPanel();

		//agregar los paneles al Frame
		add(panelDatosAlumno, BorderLayout.CENTER);
		add(panelFotoAlumno, BorderLayout.EAST);

		//Creacion de todas las etiquetas
		lbNombre=new JLabel("Nombre:");
		lbPaterno=new JLabel("Apellido Paterno:");
		lbMaterno=new JLabel("Apellido Materno:");
		lbGenero=new JLabel("Sexo:");
		lbFechaNac=new JLabel("Fecha de nacimiento:");

		//Creacion y configuracion de la zona de texto para el nombre
		txtNombre=new JTextField();
		txtNombre.setEditable(false);
		txtNombre.setBackground(Color.LIGHT_GRAY);
		txtNombre.setForeground(Color.BLUE);

		//Creacion y configuracion de la zona de texto para el apellido paterno
		txtPaterno = new JTextField();
		txtPaterno.setEditable(false);
		txtPaterno.setBackground(Color.LIGHT_GRAY);
		txtPaterno.setForeground(Color.BLUE);

		//Creacion y configuracion de la zona de texto para el apellido materno
		txtMaterno = new JTextField();
		txtMaterno.setEditable(false);
		txtMaterno.setBackground(Color.LIGHT_GRAY);
		txtMaterno.setForeground(Color.BLUE);

		//Creacion y configuracion de la zona de texto para el genero
		txtGenero = new JTextField();
		txtGenero.setEditable(false);
		txtGenero.setBackground(Color.LIGHT_GRAY);
		txtGenero.setForeground(Color.BLUE);

		//Creacion y configuracion de la zona de texto para la fecha de nacimiento
		txtFechaNac = new JTextField();
		txtFechaNac.setEditable(false);
		txtFechaNac.setBackground(Color.LIGHT_GRAY);
		txtFechaNac.setForeground(Color.BLUE);

		//distribuidor grafico
		panelDatosAlumno.setLayout(new GridLayout(6,2));

		//se agregan las etiquetas y las cajas de texto al panelDatosAlumno
		panelDatosAlumno.add(lbNombre);
		panelDatosAlumno.add(txtNombre);
		panelDatosAlumno.add(lbPaterno);
		panelDatosAlumno.add(txtPaterno);
		panelDatosAlumno.add(lbMaterno);
		panelDatosAlumno.add(txtMaterno);
		panelDatosAlumno.add(lbGenero);
		panelDatosAlumno.add(txtGenero);
		panelDatosAlumno.add(lbFechaNac);
		panelDatosAlumno.add(txtFechaNac);

		//crea la etiqueta para mostrar la foto
		lbImagen = new JLabel("FOTO ALUMNO");
		//agraga al panel de foto la etiqueta
		panelFotoAlumno.add(lbImagen);

	}


}
