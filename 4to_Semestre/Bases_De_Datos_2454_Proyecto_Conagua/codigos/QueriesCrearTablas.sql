-- Se utiliza la base conagua
Use conagua;

-- Se crea la tabla para los parámetros
CREATE TABLE Catalogo_de_parametros (
  id_parameter INT PRIMARY KEY AUTO_INCREMENT,
  cve_param VARCHAR(10) NOT NULL,
  cve_nom VARCHAR(50) NOT NULL
);

-- Se crea la tabla de unidades
CREATE TABLE Catalogo_unidades(
	id_unidad INT PRIMARY KEY AUTO_INCREMENT,
	clave_unidad VARCHAR(10) NOT NULL,
	nombre_unidad VARCHAR(50) NOT NULL
);

-- Se crea la tabla que relaciona unidades con parámetros
CREATE TABLE Unidad_parametro(
	id_unidad_parametro INT PRIMARY KEY AUTO_INCREMENT,
	id_parameter INT NOT NULL,
	id_unidad INT NOT NULL,
	FOREIGN KEY(id_parameter) REFERENCES Catalogo_de_parametros(id_parameter)
		ON DELETE RESTRICT
		ON UPDATE CASCADE,
	FOREIGN KEY(id_unidad) REFERENCES Catalogo_unidades(id_unidad)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

-- Se crea la tabla para las ciudades, aunque la única ciudad que aparece es la Ciudad de México
CREATE TABLE Catalogo_ciudades(
	id_city INT PRIMARY KEY AUTO_INCREMENT,
	city VARCHAR(20) NOT NULL,
	country VARCHAR(20) NOT NULL
);

-- Se crea la tabla para sabes en qué estado se encuentra la estación, desc_fecha = NULL funciona, desc_fecha != NULL no funciona
CREATE TABLE Estado_estacion(
	id_obs_estac INT PRIMARY KEY AUTO_INCREMENT,
	desc_fecha VARCHAR(40)
);

-- Se crea tabla para las estaciones
CREATE TABLE Catalogo_de_estaciones(
	id_estac BIGINT PRIMARY KEY,
	cve_estac VARCHAR(5) NOT NULL,
	nom_estac VARCHAR(100) NOT NULL,
	longitud FLOAT NOT NULL,
	latitud FLOAT NOT NULL,
	alt SMALLINT NOT NULL,
	id_obs_estac INT NOT NULL,
	id_city INT NOT NULL,
	FOREIGN KEY(id_obs_estac) REFERENCES Estado_estacion(id_obs_estac)
		ON DELETE RESTRICT
		ON UPDATE CASCADE,
	FOREIGN KEY(id_city) REFERENCES Catalogo_ciudades(id_city)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

-- Se crea la tabla con el registro de los datos por estación
CREATE TABLE Meteorologia(
	id_meteorologia INT PRIMARY KEY AUTO_INCREMENT,
	id_estac BIGINT NOT NULL,
	mesurementAgency VARCHAR(10) NOT NULL,
	id_parameter INT NOT NULL,
	fecha DATE NOT NULL,
	hora TIME NOT NULL,
	valor FLOAT,
	anio INT NOT NULL,
    FOREIGN KEY(id_estac) REFERENCES Catalogo_de_estaciones(id_estac)
		ON DELETE RESTRICT
		ON UPDATE CASCADE,
	FOREIGN KEY(id_parameter) REFERENCES Catalogo_de_parametros(id_parameter)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);
