import java.util.Date;

import javax.swing.JOptionPane;

public class Alumno extends Persona{
    
    public String foto;
    public String promedio;
    public String cuenta;

    //Constructor con todos los parametros y herencia.
    public Alumno(Date fechNac, Genero genero, String nombre, String paterno, String materno, String foto,
            String promedio, String cuenta) {

        super(fechNac, genero, nombre, paterno, materno);
        this.foto = foto;
        this.promedio = promedio;
        this.cuenta = cuenta;
 
    }

    //Getters y Setters
    
    
    Curso curso1 = new Curso(null, null, nombre);

    public String getFoto() {
        return foto;
    }

    public void setFoto(String foto) {
        this.foto = foto;
    }

    public String getPromedio() {
        return promedio;
    }

    public void setPromedio(String promedio) {
        this.promedio = promedio;
    }

    public String getCuenta() {
        return cuenta;
    }

    public void setCuenta(String cuenta) {
        this.cuenta = cuenta;
    }

    public Curso getCurso1() {
        return curso1;
    }

    public void setCurso1(Curso curso1) {
        this.curso1 = curso1;
    }

    public Profesor getProfe1() {
        return profe1;
    }

    public void setProfe1(Profesor profe1) {
        this.profe1 = profe1;
    }

    // Metodos --------------------------------------------------------------------------------------------------------------------------//
    public void registrarCurso(Curso curso1){
        curso1.setClave(Integer.parseInt(JOptionPane.showInputDialog("Ingresa la clave del curso.")));
        curso1.setDuracionHoras(Integer.parseInt(JOptionPane.showInputDialog("Ingresa las horas de duración del curso")));
        curso1.setNombreCurso(JOptionPane.showInputDialog("Ingresa el nombre del curso."));
    }
    
    public void eliminarCurso(){
    
    }
    Profesor profe1 = new Profesor(fechNac, genero, nombre, paterno, materno, null, materno);
    public void agregarTutor(Profesor profe1){

    }

    public void eliminarTutor(){

    }
}
