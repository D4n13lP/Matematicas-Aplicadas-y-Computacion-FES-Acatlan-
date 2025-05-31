//Daniel Pineda Ortega
//package Daniel;
import java.util.*; 

public class AplicacionCursos {

   
    private Integer posicionAlumnoActual;

    //Constructor con todos lo parametros -----------------

    public AplicacionCursos(Integer posicionAlumnoActual) {
        this.posicionAlumnoActual = posicionAlumnoActual;
    }

    //Getter y Setter -----------------------
    public Integer getPosicionAlumnoActual() {
        return posicionAlumnoActual;
    }

    public void setPosicionAlumnoActual(Integer posicionAlumnoActual) {
        this.posicionAlumnoActual = posicionAlumnoActual;
    }

    //------- Metodos -----------------------------------------------------
    Alumno AlumnoAnterior = new Alumno(null, null, null, null, null, null, null, null);
    public Alumno getAlumnoAnterior(){
        return AlumnoAnterior;
    }

    Alumno AlumnoSig = new Alumno(null, null, null, null, null, null, null, null);
    public Alumno getAlumnoSiguiente(){
        return AlumnoSig;
    }

    Alumno AlumnoActual = new Alumno(null, null, null, null, null, null, null, null);
    public Alumno getAlumnoActual(){
        return AlumnoActual;
    }


    //-******************************************************************************************************************************************************
     //Llenado de cursos
    List<Curso>cursos=new ArrayList<Curso>();

    Curso cursoTmp=new Curso(101, 100, "Programacion 1");
    cursos.add(cursoTmp);

    cursoTmp=new Curso(102, 90, "Diseño 1");
    cursos.add(cursoTmp);

    cursoTmp=new Curso(103, 80, "Programacion 2");
    cursos.add(cursoTmp);

    cursoTmp=new Curso(104, 50, "Redes");
    cursos.add(cursoTmp);

    cursoTmp=new Curso(105, 95, "Diseño Base de datos");
    cursos.add(cursoTmp);

    //Llenado de profesores
    profesores=new ArrayList<Profesor>();
    //TODO: Llenar 5 profesores y agregarlos a la lista

    //Llenado de alumnos
    alumnos=new ArrayList<Alumno>();

    Alumno alumnoTmp=new Alumno("mujer.png", "101010", "9", "Karla", "Romero", "Cruz", Genero.FEMENINO, new Date());
    alumnos.add(alumnoTmp);

    alumnoTmp=new Alumno("hombre.png", "101010", "9", "Ignacio", "Luna", "Rojas", Genero.MASCULINO, new Date());
    alumnos.add(alumnoTmp);

    //TODO: Llenar 8 alumnos y agregarlos a la lista 

    //establece la posicion actual
    posicionAlumnoActual=0;
    
}
