public class Curso {
    private Integer clave;
    private Integer duracionHoras;
    private String nombreCurso;

    public Curso(Integer clave, Integer duracionHoras, String nombreCurso) {
        this.clave = clave;
        this.duracionHoras = duracionHoras;
        this.nombreCurso = nombreCurso;
    }

    public Integer getClave() {
        return clave;
    }

    public void setClave(Integer clave) {
        this.clave = clave;
    }

    public Integer getDuracionHoras() {
        return duracionHoras;
    }

    public void setDuracionHoras(Integer duracionHoras) {
        this.duracionHoras = duracionHoras;
    }

    public String getNombreCurso() {
        return nombreCurso;
    }

    public void setNombreCurso(String nombreCurso) {
        this.nombreCurso = nombreCurso;
    }

    
}
