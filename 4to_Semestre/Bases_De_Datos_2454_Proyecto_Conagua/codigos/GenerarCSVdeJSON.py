""" 
Programa que crea un csv en base a todos los json para poder cargar en otro programa
el csv a la base de datos
"""
import pandas as pd

#Se crea el diccionario con los datos relevantes
dict_aux = {"mesurementAgency": [], "parametro": [], "fecha": [], "hora": [],"estacion":[], "valor": [], "anio": []}

#Auxiliares para las llaves de las fechas
s = ""
t = ""

#Ciclo que recorre los JSON puesto que solo se diferencían por el año
for i in range(2013, 2024):
    #Auxiliar para recorrer las fechas
    aux_fecha = -1
    #Se crea el path en base al año
    path = "datos-base/históricos-20240524T201155Z-001/históricos/meteorología_"+str(i)+".json"
    df = pd.read_json(path)
    #Se genera una lista con todas las fechas del JSON
    dias = [key for key in dict(df['pollutionMeasurements']['date']).keys()]
    #Enero
    #Bucle que recorre los días del mes
    for j in range(1,32):
        #Condicional para las llaves en las fechas por parte de los días
        if j<10:
            s = "0"
        else:
            s = ""
        #Bucle que recorre las horas del día
        for k in range(1,25):
            # Se aumenta la fecha puesto que éstas se van diferenciando por una hora
            aux_fecha += 1
            #Condicional para las llaves en las fechas por parte de las horas
            if k<10:
                t = "0"
            else:
                t = ""
            #Bucle que recorre las llaves de RH
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                #Se agrega al diccionario cada registro
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                #Puesto que la fecha se divide por un espacio el dia y la hora, se dividen en datos distintos con split
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            #Bucle que recorre las llaves de TMP
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            #Bucle que recorre las llaves de WSP
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            #Bucle que recorre las llaves de WDR
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            #Bucle que recorre las llaves de PBa
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-01-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Febrero
    #Por si es bisiesto el año, entonces el bucle irá hasta 29 días
    if i%4 == 0:
        for j in range(1,30):
            if j<10:
                s = "0"
            else:
                s = ""
            for k in range(1,25):
                aux_fecha += 1
                if k<10:
                    t = "0"
                else:
                    t = ""
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('RH')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('TMP')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('WSP')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('WDR')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('PBa')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
    else:
        for j in range(1,29):
            if j<10:
                s = "0"
            else:
                s = ""
            for k in range(1,25):
                aux_fecha += 1
                if k<10:
                    t = "0"
                else:
                    t = ""
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('RH')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('TMP')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('WSP')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('WDR')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
                for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                    dict_aux["mesurementAgency"].append("SIMAT")
                    dict_aux["parametro"].append('PBa')
                    dict_aux["estacion"].append(key)
                    dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-02-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                    dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                    dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                    dict_aux["anio"].append(i)
    #Todos los siguientes bucles son iguales, solo cambia el for de j dependiendo de la cantidad de días del mes
    #Marzo
    for j in range(1,32):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-03-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Abril
    for j in range(1,31):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-04-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Los registros de 2023 llegan hasta abril, por lo que se corta el bucle 
    if i == 2023:
        break
    #Mayo
    for j in range(1,32):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-05-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Junio
    for j in range(1,31):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-06-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Julio
    for j in range(1,32):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-07-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Agosto
    for j in range(1,32):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-08-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Septiembre
    for j in range(1,31):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-09-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Octubre
    for j in range(1,32):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-10-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Noviembre
    for j in range(1,31):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-11-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
    #Diciembre
    for j in range(1,32):
        if j<10:
            s = "0"
        else:
            s = ""
        for k in range(1,25):
            aux_fecha += 1
            if k<10:
                t = "0"
            else:
                t = ""
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['RH']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('RH')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['RH'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('TMP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['TMP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WSP')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['WSP'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('WDR')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['WDR'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)
            for key, value_date in dict(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa']).items():
                dict_aux["mesurementAgency"].append("SIMAT")
                dict_aux["parametro"].append('PBa')
                dict_aux["estacion"].append(key)
                dict_aux["valor"].append(df['pollutionMeasurements']['date'][str(i) + '-12-' + s + str(j) + ' ' + t + str(k) + ':00']['PBa'][key])
                dict_aux["fecha"].append(str(dias[aux_fecha]).split(" ")[0])
                dict_aux["hora"].append(str(dias[aux_fecha]).split(" ")[1])
                dict_aux["anio"].append(i)

#Se genera el df conforme al diccionario creado con los bucles      
df = pd.DataFrame(dict_aux)
#Se genera el csv en base al df
df.to_csv("meteorologia_json.csv", index = False)
   