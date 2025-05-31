""" 
Programa que carga a la base de datos todos los csv base
"""
from sqlalchemy import create_engine
import pandas as pd
import numpy as np

#Se establecen las credenciales de la base
db = "conagua"
path = 'datos-base/catálogos-20240524T201146Z-001/catálogos/cat_unidades.csv'
url = "mysql+mysqlconnector://bd_conagua:HkW0eiFxfT5iUv31@34.72.69.10/"

table = "Catalogo_unidades"
#Se crea la conexión a la base
engine = create_engine(url + db, echo = False)

df = pd.read_csv(path)

#Se carga el csv a la tabla Catalogo_unidades
#df.to_sql(name = table, con = engine, if_exists = "append", index = False)

#Se cambia el path
path = 'datos-base/catálogos-20240524T201146Z-001/catálogos/cat_parametros.csv'

#Se cambia la tabla
table = "Catalogo_de_parametros"

df = pd.read_csv(path, skiprows=1)
#Se cambia el df para que cuadre con la tabla en la base de datos
df = df.drop('unidades_param', axis=1)
df = df.rename(columns = {"nom_param":"cve_nom"})

#Se carga el csv a la base
#df.to_sql(name = table, con = engine, if_exists = "append", index = False)

#Se cambia el path y la tabla
path = 'datos-base/catálogos-20240524T201146Z-001/catálogos/cat_estacion.csv'
table = "Estado_estacion"

df = pd.read_csv(path, skiprows=1)
#Se eliminan columnas innecesarias
df = df.drop(columns = ["cve_estac", "nom_estac", "longitud", "latitud", "alt", "id_station"])
df = df.rename(columns = {"obs_estac":"desc_fecha"})

#Se sube el df a la tabla deseada
#df.to_sql(name = table, con = engine, if_exists = "append", index = False)

#Se cambian el path y la tabla
path = 'datos-base/históricos-20240524T201155Z-001/históricos/meteorología_2013.json'
table = "Catalogo_ciudades"

#Se crea un df auxiliar para obtener la ciudad y país
df_aux = pd.read_json(path)
data = {"city": [df_aux['pollutionMeasurements']['city']], "country": [df_aux['pollutionMeasurements']['country']]}
#Se crea el df con el diccionario auxiliar
df = pd.DataFrame(data)

#Se carga el catalogo de ciudades (Solo hay una)
#df.to_sql(name = table, con = engine, if_exists = "append", index = False)

#Se cambia el path
path = 'datos-base/catálogos-20240524T201146Z-001/catálogos/cat_estacion.csv'

#Se crean distintas tablas para las llaves foráneas
parent_table_one = "Estado_estacion"
parent_table_two = "Catalogo_ciudades"
table = "Catalogo_de_estaciones"

#Se leen las tablas que poseen las llaves foráneas
df_parent_one = pd.read_sql_table(parent_table_one, engine)
df_parent_two = pd.read_sql_table(parent_table_two, engine)
df = pd.read_csv(path, skiprows=1)
df = df.rename(columns = {"id_station":"id_estac"})
df = df.drop(columns = ['obs_estac'])
#Se unen las tablas con respecto al orden que tienen
df.insert(6, 'id_obs_estac', df_parent_one['id_obs_estac'])
#Como solo hay una ciudad, se duplica la lista en base al numero de estaciones
aux = df_parent_two['id_city'].tolist() * len(df)
df.insert(7, 'id_city', aux)
#Si hay altitud nula, se cambia a 0
for i in range(0,len(df)):
    if np.isnan(df['alt'][i]):
        df.loc[i, "alt"] = int(0)

#Se sube el catálogo de estaciones    
#df.to_sql(name = table, con = engine, if_exists = "append", index = False)
