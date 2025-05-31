""" 
Programa que carga el JSON a la base a partir del csv creado en GenerarCSVdeJSON.py
"""
from sqlalchemy import create_engine
import pandas as pd

#Se establecen las credenciales de la base
db = "conagua"
path = 'meteorologia_json.csv'
url = "mysql+mysqlconnector://bd_conagua:HkW0eiFxfT5iUv31@34.72.69.10/"

#Se establecen las tablas a usar de la base
table = "Meteorologia"
parent_table_one = "Catalogo_de_estaciones"
parent_table_two = "Catalogo_de_parametros"

#Se crea la conexión a la base
engine = create_engine(url + db, echo = False)

#Se leen las tablas a usar en la base y el csv
df = pd.read_csv(path)
df_parent_one = pd.read_sql_table(parent_table_one, engine)
df_parent_two = pd.read_sql_table(parent_table_two, engine)
#Se eliminan tablas innecesarias
df_parent_one = df_parent_one.drop(columns = ['nom_estac', 'longitud', 'latitud', 'alt', 'id_obs_estac', 'id_city'])
df_parent_two = df_parent_two.drop(columns = ['cve_nom'])
#Se unen las tablas padre a la tabla meteorología para usar llaver foráneas
df = df.merge(df_parent_one, left_on= "estacion", right_on= "cve_estac", how="inner")
df = df.merge(df_parent_two, left_on= "parametro", right_on= "cve_param", how="inner")
#Se eliminan columnas para que sea igual la tabla con el df
df = df.drop(columns=["parametro", "estacion", "cve_estac", "cve_param"])

#Se define un método para subir los datos por partes (Son casi 9 millones de registros)
def subir_datos_por_lotes(dataframe: pd.DataFrame,
                          nombre_tabla: str,
                          tamano_lote: int):
    """
    Sube un DataFrame a una tabla MySQL por lotes usando SQLAlchemy.

    Args:
        dataframe (pandas.DataFrame): El DataFrame que contiene los datos a subir.
        nombre_tabla (str): El nombre de la tabla en la base de datos MySQL.
        tamano_lote (int): El tamaño de cada lote de datos a subir.
    """
    #Se crea una nueva conexión para subir los datos
    db = "conagua"
    url = "mysql+mysqlconnector://bd_conagua:HkW0eiFxfT5iUv31@34.72.69.10/"
    engine = create_engine(url + db, echo = False)
    #Bucle para cargar los datos por lotes
    for i in range(0, len(dataframe), tamano_lote):
        #Variable el cual es una fragmento del df dependiento del tamaño del lote
        lote_datos = dataframe[i:i + tamano_lote]
        #Se sube el fragmento del df a la base
        #lote_datos.to_sql(name = nombre_tabla, con = engine, if_exists = "append", index = False)
            
#Se llama a la función con lotes de tamaño igual a 10000
TAM = int(10000)
subir_datos_por_lotes(df, table, TAM)