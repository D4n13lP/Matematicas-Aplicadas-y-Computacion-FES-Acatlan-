# Examen Extraordinario de Graficación por Computadora

## Descripción general

El presente repositorio contiene el desarrollo correspondiente al Examen Extraordinario de la asignatura de Graficación por Computadora, periodo 2026-2. Cada uno de los reactivos del examen es resuelto mediante un programa independiente, desarrollado en lenguaje C++ con la biblioteca OpenGL (GLUT/freeglut). Adicionalmente, se incluye un programa Launcher que permite la ejecución unificada de la totalidad de los reactivos desde un único menú gráfico.

## Fecha de entrega

```
Fecha de entrega: 27 de Junio de 2026 a más tardar a las 23:59 hrs. en
camo6812@acatlan.unam.mx con el Asunto (subject) Examen Extraordinario
de GC 2026-2. Se debe enviar también ejecutable.
```

## Estructura del repositorio

El repositorio se encuentra organizado en diez carpetas, numeradas de `r1` a `r10`, donde cada una corresponde a un reactivo del examen. Cada carpeta contiene el código fuente correspondiente y su respectivo ejecutable, nombrado de forma idéntica a la carpeta que lo contiene (por ejemplo, la carpeta `r1` contiene el ejecutable `r1.exe`).

Adicionalmente, en la raíz del repositorio se encuentra el programa `launcher.exe`, junto con su código fuente `launcher.cpp`, el cual permite acceder a cada uno de los diez programas desde un menú gráfico desarrollado en OpenGL.

```
.
├── launcher.cpp
├── launcher.exe
├── r1\
│      r1.cpp
│      r1.exe
├── r2\
│      r2.cpp
│      r2.exe
├── r3\
│      r3.cpp
│      r3.exe
├── r4\
│      r4.cpp
│      r4.exe
├── r5\
│      r5.cpp
│      r5.exe
├── r6\
│      r6.cpp
│      r6.exe
├── r7\
│      r7.cpp
│      r7.exe
├── r8\
│      r8.cpp
│      r8.exe
├── r9\
│      r9.cpp
│      r9.exe
└── r10\
       r10.cpp
       r10.exe
```

## Correspondencia entre carpetas y reactivos

A continuación se describe el contenido de cada carpeta en relación con el reactivo del examen que resuelve.

### r1 — Historia de OpenGL

Se atiende el reactivo: *¿Cuándo y dónde se presentó OpenGL?* En esta carpeta se presenta la respuesta correspondiente a través de salida de consola.

### r2 — Primitivas 3D de OpenGL

Se atiende el reactivo: *Explique brevemente las primitivas gráficas 3D de OpenGL y elabore un programa con ejemplos de cada una de ellas.* Se incluye un programa que ejemplifica las primitivas tridimensionales disponibles en OpenGL.

### r3 — Algoritmos DDA y Bresenham

Se atiende el reactivo: *Elabore dos programas que dibujen líneas a partir de puntos, utilizando los algoritmos DDA y Bresenham.* Se incluye la implementación de ambos algoritmos de trazado de líneas.

### r4 — Cardioide Cilíndrico (NURBS)

Se atiende el reactivo: *Elabore el Cardioide Cilíndrico con metodología NURBS.* Se presenta la construcción de dicha superficie mediante curvas NURBS.

### r5 — Objetos 3D en forma alámbrica y sólida

Se atiende el reactivo: *Los objetos en 3D se pueden presentar en forma alámbrica y sólida. Elabore un programa que, a partir de una tecla, cambie de un objeto a otro.* Se establece la tecla `w` como mecanismo de alternancia entre ambas representaciones.

### r6 — Transformaciones afines con teclado

Se atiende el reactivo: *Investigue sobre las transformaciones afines (traslación, rotación, escalamiento y sesgo) y permita su aplicación mediante teclas indicadas.* Se implementa el control de rotación, traslación y escalamiento sobre los objetos del reactivo anterior, conforme al siguiente esquema de teclas:

| Transformación | Eje | Tecla (positivo) | Tecla (negativo) |
|---|---|---|---|
| Rotación | X | `Rx` | `rx` |
| Rotación | Y | `Ry` | `ry` |
| Rotación | Z | `Rz` | `rz` |
| Traslación | X | `Tx` | `tx` |
| Traslación | Y | `Ty` | `ty` |
| Traslación | Z | `Tz` | `tz` |
| Escalamiento | — | `E` | `e` |

### r7 — Fractales: Pirámide de Sierpinski y Conjuntos de Julia

Se atiende el reactivo: *Investigue sobre la pirámide de Sierpinski y los conjuntos de Julia, y prográmelos con puntos, al igual que la esponja de Sierpinski.* Se incluyen las implementaciones correspondientes a dichos fractales mediante representación por puntos.

### r8 — Mano con catorce articulaciones

Se atiende el reactivo: *Elabore un programa que permita doblar las catorce articulaciones de una mano.* Se contempla que los dedos meñique, anular, medio e índice cuentan con tres articulaciones cada uno, mientras que el pulgar cuenta con dos, con desplazamiento exclusivo hacia la palma.

### r9 — Luz global y luz puntual

Se atiende el reactivo: *Defina lo que es una luz global y una luz puntual. Haga un programa donde cambie a cada tipo de luz al oprimir la tecla L.* Se establece la tecla `L` como mecanismo de alternancia entre ambos modelos de iluminación.

### r10 — Fractal de Julia en 3D

Se atiende el reactivo: *Elabore el programa de cualquier fractal de Julia en 3D y que se pueda rotar con las flechas del teclado o bien con el mouse.* Se implementa el control de rotación mediante ambos periféricos.

## Programa Launcher

El programa `launcher.cpp` constituye un menú gráfico desarrollado en OpenGL/GLUT, con tipografía Helvetica, mediante el cual se centraliza el acceso a los diez programas anteriormente descritos. Se contempla la siguiente operación:

- La selección del programa se efectúa mediante las flechas de dirección (arriba/abajo), las teclas numéricas (0 a 9) o el cursor del mouse.
- La ejecución del programa seleccionado se efectúa mediante la tecla `ENTER`.
- Al ejecutarse un programa, la ventana del Launcher se minimiza y permanece en espera hasta que dicho programa es cerrado, momento en el cual se restaura el menú principal.
- El cierre definitivo del Launcher se efectúa mediante la tecla `ESC`.

Las instrucciones de compilación y la especificación detallada de la estructura de carpetas requerida se encuentran descritas en el archivo `README.md` correspondiente al Launcher.

## Requisitos de compilación

Se requiere un compilador compatible con C++ (MinGW) y la biblioteca freeglut. La compilación de cada programa, así como del Launcher, se efectúa conforme a lo siguiente:

```bash
g++ programa.cpp -o programa.exe -lfreeglut -lopengl32 -lglu32 -lwinmm
```

Se sustituye `programa` por el nombre correspondiente a cada carpeta (`r1`, `r2`, ..., `r10`) o por `launcher`, según el archivo a compilar.
