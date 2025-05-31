import java.util.Date;

public class Profesor extends Persona {
    
    private Integer noTrabajador;
    private String cedula;

    //Constructor con todos los parametros y herencia
    public Profesor(Date fechNac, Genero genero, String nombre, String paterno, String materno, Integer noTrabajador,
            String cedula) {

        super(fechNac, genero, nombre, paterno, materno);
        this.noTrabajador = noTrabajador;
        this.cedula = cedula;
    }

    public Integer getNoTrabajador() {
        return noTrabajador;
    }

    public void setNoTrabajador(Integer noTrabajador) {
        this.noTrabajador = noTrabajador;
    }

    public String getCedula() {
        return cedula;
    }

    public void setCedula(String cedula) {
        this.cedula = cedula;
    }
    
    
}
