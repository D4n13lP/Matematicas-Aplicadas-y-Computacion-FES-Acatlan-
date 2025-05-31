//package Daniel;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;

import javax.swing.*;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.border.TitledBorder;

public class PanelNavegacion extends JPanel{

	//botones
	private JButton btnSiguiente;
	private JButton btnAnterior;

	//Constructor del panel	
	public PanelNavegacion() {

		setLayout(new FlowLayout());
		
		TitledBorder border=BorderFactory.createTitledBorder("Navegacion");
		border.setTitleColor(Color.BLUE);
		setBorder(border);

		//creacion de botones
		btnAnterior = new JButton("<");
		btnSiguiente = new JButton(">");

		//se agregan los botones
		add(btnAnterior);
		add(btnSiguiente);
	}

}
