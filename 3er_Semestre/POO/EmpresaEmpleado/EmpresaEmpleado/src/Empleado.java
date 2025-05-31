import java.util.Date;

public class Empleado {

    protected String nombre;
    protected String paterno;
    protected String materno;
    protected Integer horasTrabajadas;
    protected String curp;
    protected Date fechaNac;
    protected String rfc;
    protected Double salario;

    //-----  Constructor ----------------------------------------------
    public Empleado(String nombre, String paterno, String materno, Integer horasTrabajadas, String curp, Date fechaNac,
            String rfc, Double salario) {
        this.nombre = nombre;
        this.paterno = paterno;
        this.materno = materno;
        this.horasTrabajadas = horasTrabajadas;
        this.curp = curp;
        this.fechaNac = fechaNac;
        this.rfc = rfc;
        this.salario = salario;
    }

    //Getters y Setters -----------------------------------------------
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
    public Integer getHorasTrabajadas() {
        return horasTrabajadas;
    }
    public void setHorasTrabajadas(Integer horasTrabajadas) {
        this.horasTrabajadas = horasTrabajadas;
    }
    public String getCurp() {
        return curp;
    }
    public void setCurp(String curp) {
        this.curp = curp;
    }
    public Date getFechaNac() {
        return fechaNac;
    }
    public void setFechaNac(Date fechaNac) {
        this.fechaNac = fechaNac;
    }
    public String getRfc() {
        return rfc;
    }
    public void setRfc(String rfc) {
        this.rfc = rfc;
    }
    public Double getSalario() {
        return salario;
    }
    public void setSalario(Double salario) {
        this.salario = salario;
    }

    //Metodos ----------------------------------------------
    public void imprimirDatos(){

    }
    

}
