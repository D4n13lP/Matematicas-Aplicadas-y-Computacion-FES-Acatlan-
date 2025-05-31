#include <stdio.h>
#include <stdlib.h>
#include <winbgim.h>
#include <math.h>
#include <time.h>

//Porcentaje de espacio para que quede un "margen" en la pantalla
#define PORC 5
//Cantidad de datos
#define CANTIDAD 8

#define PI 3.14159


typedef struct {
	char Nombre[50];
	char Apellidos[100];
	int Edad;
	char Direccion[100];
}Persona;

typedef struct{
	int clave;
	Persona DatosPersonales;
	char borrado;
}Paciente;

typedef struct{
	int NumeroTrabajador;
	Persona DatosPersonales;
	char Especialidad[50];
	int Experiencia;
	char borrado;
}Doctor;

typedef struct{
	int NoConsulta;
	float costo;
	int clave;
	int NumeroTrabajador;
	char borrado;
}Consulta;

typedef struct{
	int NumeroTrabajador;
	Persona DatosPersonales;
	float ganancia;
}Gananciadoc;

typedef struct{                  //Esta estructura guarda un punto del plano de 2 dimensiones
    double x;                    //Se usa para guardar el centro de las rebanadas del crculo y
    double y;                    //poder colocar en el centro de cada rebanada de la grafica
}Point;                          //un texto con el porcentaje que representa la rebanada


int generarAleatorio() {
    return rand() % 256;
}


void graficaBarras(Gananciadoc *datos, int nbarras){

    int Xf, Xi, Yf, Yi, EspacioX, EspacioY, AnchoBarra, CantidadBarras,X,Y;
    int AlturaBarra, YBarra, XBarra, i;
    float ValorBarra, ValorBarraMax;

    //Aqu� son datos fijos, pero se deben tomar de un archivo
    //float Datos[CANTIDAD]={150.2, 180.3, 230, 125, 58.97, 450.67, 7.5, 75.6};
    float Datos[nbarras];
    for(int i=0; i<nbarras; ++i){
        Datos[i]=datos[i].ganancia;
    }

    //La cantidad depende de los datos guardados en el archivo
    CantidadBarras=nbarras;

    //Se debe encontrar el dato m�s grande
    //ValorBarraMax=450.67;
    ValorBarraMax=datos[0].ganancia;
    for(int i=1; i<nbarras; ++i){
        if(datos[i].ganancia>ValorBarraMax)
            ValorBarraMax=datos[i].ganancia;
    }

    X=getmaxwidth();
    Y=getmaxheight();

    initwindow(X,Y,"Ganancias por doctor");
    setbkcolor(COLOR(255,255,255));
	cleardevice();

    Xi=X*PORC/100;
    Xf=X-Xi;
    Yi=Y*PORC/100;
    Yf=Y-Yi;

    EspacioX=Xf-Xi;
    EspacioY=Yf-Yi;

    AnchoBarra=EspacioX/(CantidadBarras*2-1);

    XBarra=Xi;

    setcolor(COLOR(0,0,0));
    outtextxy(Xi, 0, "Dinero por doctor");

    setlinestyle(SOLID_LINE,0,3);
    //setcolor(YELLOW);
    setcolor(BLACK);
    line(Xi,Yf+2,Xf,Yf+2);

    /* for(i=0; i<nbarras; i++){
        setcolor(i+1);
        AlturaBarra=(Datos[i]/ValorBarraMax)*EspacioY;
        YBarra=Yi+(EspacioY-AlturaBarra);
        rectangle(XBarra,YBarra,XBarra+AnchoBarra,Yf);
        XBarra+=AnchoBarra*2;
    } */
    char ganancia2string[20];
    char fullname[50];

    setlinestyle(SOLID_LINE,0,1);
    for (i = 0; i < nbarras; i++) {
        setcolor(i + 1);
        AlturaBarra = (Datos[i] / ValorBarraMax) * EspacioY;
        YBarra = Yi + (EspacioY - AlturaBarra);
        setfillstyle(SOLID_FILL, i + 1); // Establecer el color de relleno
        bar(XBarra, YBarra, XBarra + AnchoBarra, Yf); // Utilizar la función bar() en lugar de rectangle()
        sprintf(ganancia2string, "$ %.2f", datos[i].ganancia);
        outtextxy(XBarra, YBarra-20, ganancia2string);
        sprintf(fullname, "Dr. %s %s", datos[i].DatosPersonales.Nombre, datos[i].DatosPersonales.Apellidos);
        outtextxy(XBarra, Yf+5, fullname);
        line(XBarra, YBarra, Xf, YBarra);                   //Linea que resalta la altura de las barras
        XBarra += AnchoBarra * 2;
    }

    //Ciclo para que se vea la gr�fica antes de que  se cierre la ventana
    while(!kbhit());

    closegraph();

}

Point getSliceCenter(Point center, double radius, double startAngle, double endAngle) {
    Point sliceCenter;
    double angle = (startAngle + endAngle) / 2;
    sliceCenter.x = center.x + radius * cos(angle);
    sliceCenter.y = center.y + radius * sin(angle);
    return sliceCenter;
}

void graficaPastel(Gananciadoc *datos, int nbarras) {

        int Xf, Xi, Yf, Yi;

        //Primero calculamos el total de ingresos que genera el hospital con los costos de las consultas
        float ingresosTotal=0.0;
        for(int i=0; i<nbarras; ++i){
            ingresosTotal+=datos[i].ganancia;
        }
        printf("\nIngresos totales del Hospital %f\n", ingresosTotal);

        //Ahora calculamos el porcentaje que representan las ganancias de cada doctor
        float porcentajes[nbarras];
        for(int i=0; i<nbarras; ++i){
            porcentajes[i]=(datos[i].ganancia*100)/ingresosTotal;
            printf("\n%f\n", porcentajes[i]);
        }

        int ancho=getmaxwidth(), alto=getmaxheight();
        Xi=ancho*PORC/100;
        Xf=ancho-Xi;
        Yi=alto*PORC/100;
        Yf=alto-Yi;

        // Resolución de referencia
        int referenceWidth = 1920;  // Resolución de referencia
        int referenceHeight = 1080;
        // Calcular relación de escala
        float scaleX = (float)ancho / referenceWidth;
        float scaleY = (float)alto / referenceHeight;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;  // Usar la escala más pequeña para mantener la proporción

        // Tamaño de fuente original
        int originalFontSize = 5;

        // Ajustar el tamaño de fuente
        int adjustedFontSize = (int)(originalFontSize * scale);

        initwindow(ancho,alto);
        setbkcolor(COLOR(255,255,255));
	    cleardevice();
        settextstyle(GOTHIC_FONT, HORIZ_DIR, adjustedFontSize);
        setcolor(BLACK);
        outtextxy(Xi, Yi, "Porcentaje de las ganancias totales");
        // Inicializar la semilla del generador de números aleatorios con la hora actual
        srand(time(NULL));

        int r, g, b, radio=alto/3;
        float start, end, anterior=0;
        Point center = {ancho/2, alto/2};
        double textslideRadius = alto/4;

        setcolor(COLOR(255,255,255));

        int squareSize = alto*.05;
        int gap = squareSize;
        char nombreYporcentaje[50];

        // Tamaño de fuente original
        originalFontSize = 1;

        // Ajustar el tamaño de fuente
        adjustedFontSize = (int)(originalFontSize * scale);

        for(int i=0; i<nbarras; i++){
            r = generarAleatorio();
            g = generarAleatorio();
            b = generarAleatorio();
            setcolor(COLOR(255,255,255));
            int left = ancho*.75;
            int top = squareSize + i * (squareSize + gap);
            int right = left + squareSize;
            int bottom = top + squareSize;
            setfillstyle(SOLID_FILL, COLOR(r,g,b));
            start=anterior;
            end=start+(360*(porcentajes[i]/100));
            pieslice(ancho/2, alto/2, start, end, radio);
            bar(left, top, right, bottom);
            sprintf(nombreYporcentaje, "(%.2f%%) %s %s", porcentajes[i], datos[i].DatosPersonales.Nombre, datos[i].DatosPersonales.Apellidos);
            setcolor(BLACK);
            //settextstyle(GOTHIC_FONT, HORIZ_DIR, adjustedFontSize);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, adjustedFontSize);
            outtextxy(left, top+squareSize, nombreYporcentaje);
            anterior=end;

        }


    for (int i = 0; i < nbarras; i++)
    {

        start=anterior;
        end=start-(360*(porcentajes[i]/100));


        char buffer[10];
        sprintf(buffer, "%.2f%%", porcentajes[i]);
        outtextxy(center.x + (textslideRadius * cos((start + end) * PI / 360)), center.y + (textslideRadius * sin((start + end) * PI / 360)), buffer);
        anterior=end;
    }


        while(!kbhit());

        closegraph();

    }
