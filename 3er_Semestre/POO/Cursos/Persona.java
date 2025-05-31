import java.util.Date;

import javax.swing.JOptionPane;

public class Persona {

    protected Date fechNac;
    protected Genero genero;
    protected String nombre;
    protected String paterno;
    protected String materno;
    
    //constructor con todos los parametros 
    public Persona(Date fechNac, Genero genero, String nombre, String paterno, String materno) {
        this.fechNac = fechNac;
        this.genero = genero;
        this.nombre = nombre;
        this.paterno = paterno;
        this.materno = materno;
    }

    public Date getFechNac() {
        return fechNac;
    }

    public void setFechNac(Date fechNac) {
        this.fechNac = fechNac;
    }

    public Genero getGenero() {
        return genero;
    }

    public void setGenero(Genero genero) {
        this.genero = genero;
    }

    public String getNombre() {
        return nombre;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public String getPaterno() {
        return paterno;
    }

    public void setPaterno(String paterno) {
        this.paterno = paterno;
    }

    public String getMaterno() {
        return materno;
    }

    public void setMaterno(String materno) {
        this.materno = materno;
    }

    
    public void imprimirDatos() {
        JOptionPane.showMessageDialog(null, "Nombre: " + this.nombre +"\nApellido Paterno: " + this.paterno + "\nApellido Materno: " + this.materno
        +"\nGenero: "+this.genero+"\nFecha de Nacimiento: "+this.fechNac);
    }

    

}
