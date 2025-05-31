import os 

def limpiar_pantalla():
    
    comando = 'cls' if os.name == 'nt' else 'clear'

    os.system(comando)
