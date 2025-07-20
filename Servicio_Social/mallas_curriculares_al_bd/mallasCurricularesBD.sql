-- Crear base de datos
CREATE DATABASE IF NOT EXISTS MallasCurricularesDB;
USE MallasCurricularesDB;

-- 1. País
CREATE TABLE Pais (
    ID_pais VARCHAR(10) PRIMARY KEY,
    Nombre_pais VARCHAR(100) NOT NULL
);

-- 2. División_Administrativa
CREATE TABLE Division_Administrativa (
    ID_division_administrativa VARCHAR(10) PRIMARY KEY,
    Nombre_division_admin VARCHAR(100) NOT NULL,
    numeroHabitantes INT,
    ID_pais VARCHAR(10),
    FOREIGN KEY (ID_pais) REFERENCES Pais(ID_pais)
);

-- 3. Institución
CREATE TABLE Institucion (
    ID_institucion VARCHAR(10) PRIMARY KEY,
    Nombre_institucion VARCHAR(100) NOT NULL,
    codigo_institucion VARCHAR(20),
    tipoInstitucion ENUM('publica', 'privada'),
    ID_division_administrativa VARCHAR(10),
    FOREIGN KEY (ID_division_administrativa) REFERENCES Division_Administrativa(ID_division_administrativa)
);

-- 4. Distribución_de_Instituciones (opcional)
CREATE TABLE Distribucion_de_Instituciones (
    ID_division_administrativa VARCHAR(10),
    ID_institucion VARCHAR(10),
    PRIMARY KEY (ID_division_administrativa, ID_institucion),
    FOREIGN KEY (ID_division_administrativa) REFERENCES Division_Administrativa(ID_division_administrativa),
    FOREIGN KEY (ID_institucion) REFERENCES Institucion(ID_institucion)
);

-- 5. Carrera
CREATE TABLE Carrera (
    ID_carrera VARCHAR(10) PRIMARY KEY,
    Nombre_carrera VARCHAR(100) NOT NULL,
    creditos_totales INT,
    area_de_conocimiento VARCHAR(100),
    cantidad_alumnos INT,
    Nombre_titulo VARCHAR(100)
);

-- 6. Institución_Carrera
CREATE TABLE Institucion_Carrera (
    ID_institucion VARCHAR(10),
    ID_carrera VARCHAR(10),
    oferta_lugares INT,
    PRIMARY KEY (ID_institucion, ID_carrera),
    FOREIGN KEY (ID_institucion) REFERENCES Institucion(ID_institucion),
    FOREIGN KEY (ID_carrera) REFERENCES Carrera(ID_carrera)
);

-- 7. Aspirantes
CREATE TABLE Aspirantes (
    ID_aspirantes VARCHAR(10) PRIMARY KEY,
    aceptados INT,
    rechazados INT,
    totales INT
);

-- 8. Aspirantes_Carrera
CREATE TABLE Aspirantes_Carrera (
    ID_aspirantes VARCHAR(10),
    ID_carrera VARCHAR(10),
    PRIMARY KEY (ID_aspirantes, ID_carrera),
    FOREIGN KEY (ID_aspirantes) REFERENCES Aspirantes(ID_aspirantes),
    FOREIGN KEY (ID_carrera) REFERENCES Carrera(ID_carrera)
);

-- 9. Versión_Malla_Curricular
CREATE TABLE Version_Mapa_Curricular (
    ID_version_mapa_curricular VARCHAR(10) PRIMARY KEY,
    starting_year INT,
    ending_year INT,
    ID_carrera VARCHAR(10),
    FOREIGN KEY (ID_carrera) REFERENCES Carrera(ID_carrera)
);

-- 10. Contenido_Mapa_Curricular
CREATE TABLE Contenido_Mapa_Curricular (
    ID_cont_mapa_curricular VARCHAR(10) PRIMARY KEY,
    ID_version_mapa_curricular VARCHAR(10),
    FOREIGN KEY (ID_version_mapa_curricular) REFERENCES Version_Mapa_Curricular(ID_version_mapa_curricular)
);

-- 11. Tipo_de_Créditos
CREATE TABLE Tipo_de_Creditos (
    ID_tipo_creditos VARCHAR(10) PRIMARY KEY,
    Nombre_tipo_creditos ENUM('Obligatorio', 'formación_básica', 'optativa')
);

-- 12. Distribución_Tipo_de_Créditos
CREATE TABLE Distribucion_Tipo_de_Creditos (
    ID_tipo_creditos VARCHAR(10),
    ID_cont_mapa_curricular VARCHAR(10),
    cantidad_tipo_creditos INT,
    PRIMARY KEY (ID_tipo_creditos, ID_cont_mapa_curricular),
    FOREIGN KEY (ID_tipo_creditos) REFERENCES Tipo_de_Creditos(ID_tipo_creditos),
    FOREIGN KEY (ID_cont_mapa_curricular) REFERENCES Contenido_Mapa_Curricular(ID_cont_mapa_curricular)
);

-- 13. Periodo_Academico
CREATE TABLE Periodo_Academico (
    ID_periodo_academico VARCHAR(10) PRIMARY KEY,
    Numero_periodo INT,
    duracion_periodo VARCHAR(50),
    etapa VARCHAR(50)
);

-- 14. Tipo_Asignatura
CREATE TABLE Tipo_Asignatura (
    ID_tipo_asignatura VARCHAR(10) PRIMARY KEY,
    nombre_tipo_asignatura ENUM('Obligatoria', 'formación_básica', 'optativa')
);

-- 15. Asignatura
CREATE TABLE Asignatura (
    ID_asignatura VARCHAR(10) PRIMARY KEY,
    Nombre_asignatura VARCHAR(100) NOT NULL,
    Horas_teoricas INT,
    Horas_practicas INT,
    cantidad_creditos INT,
    ID_tipo_asignatura VARCHAR(10),
    ID_tipo_creditos VARCHAR(10),
    ID_periodo_academico VARCHAR(10),
    FOREIGN KEY (ID_tipo_asignatura) REFERENCES Tipo_Asignatura(ID_tipo_asignatura),
    FOREIGN KEY (ID_tipo_creditos) REFERENCES Tipo_de_Creditos(ID_tipo_creditos),
    FOREIGN KEY (ID_periodo_academico) REFERENCES Periodo_Academico(ID_periodo_academico)
);

-- 16. Mapa_Curricular_Asignatura
CREATE TABLE Mapa_Curricular_Asignatura (
    ID_cont_mapa_curricular VARCHAR(10),
    ID_asignatura VARCHAR(10),
    PRIMARY KEY (ID_cont_mapa_curricular, ID_asignatura),
    FOREIGN KEY (ID_cont_mapa_curricular) REFERENCES Contenido_Mapa_Curricular(ID_cont_mapa_curricular),
    FOREIGN KEY (ID_asignatura) REFERENCES Asignatura(ID_asignatura)
);
