/* ============================================================
   LAUNCHER - Menu unificado para programas OpenGL (r1..r10)
   ============================================================
   Compilar (MinGW):
       g++ launcher.cpp -o launcher.exe -lfreeglut -lopengl32 -lglu32 -lwinmm

   Estructura esperada (todas en la misma carpeta que launcher.exe):
       launcher.exe
       r1\r1.exe
       r2\r2.exe
       ...
       r10\r10.exe

   Controles:
       Flechas arriba/abajo : moverse entre programas
       ENTER                : ejecutar programa seleccionado
       Numeros 0-9          : acceso directo (0 = programa 10)
       ESC                  : salir del launcher
   ============================================================ */

#include <windows.h>
#include <GL/glut.h>
#include <gl/gl.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// ----------------------------------------------------------
// Datos de los programas
// ----------------------------------------------------------
struct Programa {
    std::string numero;   // "1".."10"
    std::string carpeta;  // "r1".."r10"
    std::string exe;      // "r1.exe".."r10.exe"
    std::string nombre;   // nombre limpio (sin parentesis/corchetes)
};

std::vector<Programa> programas = {
    {"1",  "r1",  "r1.exe",  "Historia de OpenGL"},
    {"2",  "r2",  "r2.exe",  "Primitivas 3D de OpenGL"},
    {"3",  "r3",  "r3.exe",  "Algoritmos DDA y Bresenham"},
    {"4",  "r4",  "r4.exe",  "Cardioide Cilindrico"},
    {"5",  "r5",  "r5.exe",  "Objetos 3D alambre/solido"},
    {"6",  "r6",  "r6.exe",  "Transformaciones afines con teclado"},
    {"7",  "r7",  "r7.exe",  "Fractales Sierpinski + Julia 2D"},
    {"8",  "r8",  "r8.exe",  "Mano con 14 articulaciones"},
    {"9",  "r9",  "r9.exe",  "Luces global/puntual"},
    {"10", "r10", "r10.exe", "Fractal Julia 3D"}
};

// ----------------------------------------------------------
// Estado global
// ----------------------------------------------------------
int seleccion = 0;                 // indice del programa resaltado
int ventanaAncho = 1000;
int ventanaAlto  = 720;

enum EstadoLauncher { MENU, EJECUTANDO, MENSAJE_ERROR };
EstadoLauncher estado = MENU;
std::string mensajeError = "";
float tiempoMensaje = 0.0f;

// ----------------------------------------------------------
// Utilidades de texto con GLUT (Helvetica)
// ----------------------------------------------------------
void dibujarTexto(float x, float y, void* fuente, const std::string& texto,
                   float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : texto) {
        glutBitmapCharacter(fuente, c);
    }
}

float anchoTexto(void* fuente, const std::string& texto)
{
    float ancho = 0.0f;
    for (char c : texto) {
        ancho += glutBitmapWidth(fuente, c);
    }
    return ancho;
}

void dibujarTextoCentrado(float xCentro, float y, void* fuente,
                           const std::string& texto, float r, float g, float b)
{
    float w = anchoTexto(fuente, texto);
    dibujarTexto(xCentro - w / 2.0f, y, fuente, texto, r, g, b);
}

// ----------------------------------------------------------
// Dibuja un rectangulo relleno (para resaltar seleccion / fondo)
// ----------------------------------------------------------
void rectangulo(float x1, float y1, float x2, float y2,
                 float r, float g, float b, float alpha = 1.0f)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, alpha);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
    glDisable(GL_BLEND);
}

void rectanguloBorde(float x1, float y1, float x2, float y2,
                      float r, float g, float b, float ancho = 1.5f)
{
    glColor3f(r, g, b);
    glLineWidth(ancho);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

// ----------------------------------------------------------
// Lanza un programa externo y ESPERA a que termine
// (bloqueante: el launcher se "congela" / oculta logicamente
//  hasta que el proceso hijo cierra, luego regresa al menu)
// ----------------------------------------------------------
bool ejecutarProgramaExterno(const Programa& p, std::string& errorOut)
{
    char rutaActual[MAX_PATH];
    GetModuleFileNameA(NULL, rutaActual, MAX_PATH);
    std::string dirBase = rutaActual;
    size_t pos = dirBase.find_last_of("\\/");
    dirBase = dirBase.substr(0, pos);

    std::string dirPrograma = dirBase + "\\" + p.carpeta;
    std::string rutaExe     = dirPrograma + "\\" + p.exe;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // El "directorio de trabajo" del proceso hijo es su propia carpeta,
    // asi cada programa encuentra sus propios recursos si los necesita.
    std::string cmdLine = "\"" + rutaExe + "\"";

    BOOL ok = CreateProcessA(
        rutaExe.c_str(),          // aplicacion
        &cmdLine[0],              // linea de comando (modificable)
        NULL, NULL,
        FALSE,
        0,
        NULL,
        dirPrograma.c_str(),      // directorio de trabajo = carpeta del programa
        &si,
        &pi
    );

    if (!ok) {
        DWORD err = GetLastError();
        std::ostringstream oss;
        oss << "No se pudo iniciar " << p.exe
            << " (codigo de error " << err << "). "
            << "Verifica que exista en: " << rutaExe;
        errorOut = oss.str();
        return false;
    }

    // Esperar a que el proceso hijo termine antes de regresar al menu
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

// ----------------------------------------------------------
// Callback de despliegue
// ----------------------------------------------------------
void display()
{
    glClearColor(0.07f, 0.08f, 0.11f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, ventanaAncho, 0, ventanaAlto, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float centroX = ventanaAncho / 2.0f;

    // ---------- Encabezado ----------
    rectangulo(0, ventanaAlto - 110, (float)ventanaAncho, (float)ventanaAlto,
               0.10f, 0.13f, 0.20f);
    dibujarTextoCentrado(centroX, ventanaAlto - 55, GLUT_BITMAP_HELVETICA_18,
                         "LAUNCHER - PROGRAMAS DE GRAFICACION (OpenGL / C++)",
                         0.85f, 0.92f, 1.0f);
    dibujarTextoCentrado(centroX, ventanaAlto - 85, GLUT_BITMAP_HELVETICA_12,
                         "Usa las flechas Arriba/Abajo o los numeros 0-9 para elegir, ENTER para ejecutar, ESC para salir",
                         0.55f, 0.62f, 0.72f);

    // ---------- Lista de programas ----------
    float yInicio = ventanaAlto - 150.0f;
    float alturaItem = 50.0f;
    float margenX = 90.0f;
    float anchoItem = ventanaAncho - 2 * margenX;

    for (size_t i = 0; i < programas.size(); i++) {
        float y = yInicio - i * alturaItem;
        float yTop = y + 32;
        float yBot = y - 8;

        bool sel = ((int)i == seleccion);

        if (sel) {
            rectangulo(margenX, yBot, margenX + anchoItem, yTop,
                       0.20f, 0.45f, 0.80f, 0.85f);
            rectanguloBorde(margenX, yBot, margenX + anchoItem, yTop,
                           0.55f, 0.80f, 1.0f, 2.0f);
        } else {
            rectangulo(margenX, yBot, margenX + anchoItem, yTop,
                       0.13f, 0.15f, 0.20f, 0.6f);
        }

        // Numero del programa (chip)
        std::string num = programas[i].numero;
        dibujarTexto(margenX + 18, y + 6, GLUT_BITMAP_HELVETICA_18, num,
                    sel ? 1.0f : 0.6f, sel ? 1.0f : 0.7f, sel ? 1.0f : 0.85f);

        // Nombre del programa
        dibujarTexto(margenX + 70, y + 6, GLUT_BITMAP_HELVETICA_18,
                    programas[i].nombre,
                    sel ? 1.0f : 0.85f, sel ? 1.0f : 0.88f, sel ? 1.0f : 0.92f);

        // Carpeta/ejecutable de referencia, alineado a la derecha
        std::string ref = "[" + programas[i].carpeta + "\\" + programas[i].exe + "]";
        float wRef = anchoTexto(GLUT_BITMAP_HELVETICA_12, ref);
        dibujarTexto(margenX + anchoItem - wRef - 16, y + 10,
                    GLUT_BITMAP_HELVETICA_12, ref,
                    sel ? 0.90f : 0.45f, sel ? 0.95f : 0.50f, sel ? 1.0f : 0.55f);
    }

    // ---------- Pie / instrucciones de navegacion ----------
    rectangulo(0, 0, (float)ventanaAncho, 46, 0.10f, 0.13f, 0.20f);
    dibujarTextoCentrado(centroX, 16, GLUT_BITMAP_HELVETICA_12,
        "Navegacion: [Arriba/Abajo] mover seleccion   [ENTER] ejecutar   [0-9] salto directo   [ESC] salir del launcher",
        0.6f, 0.85f, 0.65f);

    // ---------- Mensaje de error temporal ----------
    if (estado == MENSAJE_ERROR) {
        rectangulo(centroX - 380, ventanaAlto / 2.0f - 60,
                   centroX + 380, ventanaAlto / 2.0f + 40,
                   0.55f, 0.12f, 0.12f, 0.92f);
        rectanguloBorde(centroX - 380, ventanaAlto / 2.0f - 60,
                        centroX + 380, ventanaAlto / 2.0f + 40,
                        1.0f, 0.6f, 0.6f, 2.0f);
        dibujarTextoCentrado(centroX, ventanaAlto / 2.0f + 10, GLUT_BITMAP_HELVETICA_18,
                            "ERROR AL EJECUTAR", 1.0f, 0.85f, 0.85f);
        dibujarTextoCentrado(centroX, ventanaAlto / 2.0f - 20, GLUT_BITMAP_HELVETICA_12,
                            mensajeError, 1.0f, 0.92f, 0.92f);
    }

    glutSwapBuffers();
}

// ----------------------------------------------------------
// Ejecuta el programa seleccionado, manejando el ciclo
// "ocultar launcher -> correr hijo -> regresar al menu"
// ----------------------------------------------------------
void lanzarSeleccionado()
{
    estado = EJECUTANDO;

    // Minimizamos la ventana del launcher mientras el hijo corre
    HWND hwnd = FindWindowA(NULL, NULL); // se reemplaza abajo por glutGet
    int win = glutGetWindow();
    (void)win;

    glutIconifyWindow(); // minimiza el launcher

    std::string error;
    bool ok = ejecutarProgramaExterno(programas[seleccion], error);

    glutShowWindow();    // al volver, restauramos el launcher
    glutPopWindow();

    if (!ok) {
        mensajeError = error;
        estado = MENSAJE_ERROR;
        tiempoMensaje = 0.0f;
    } else {
        estado = MENU;
    }

    glutPostRedisplay();
}

// ----------------------------------------------------------
// Teclado especial (flechas)
// ----------------------------------------------------------
void teclasEspeciales(int key, int x, int y)
{
    if (estado == MENSAJE_ERROR) {
        estado = MENU;
        glutPostRedisplay();
        return;
    }

    if (key == GLUT_KEY_UP) {
        seleccion = (seleccion - 1 + (int)programas.size()) % (int)programas.size();
        glutPostRedisplay();
    } else if (key == GLUT_KEY_DOWN) {
        seleccion = (seleccion + 1) % (int)programas.size();
        glutPostRedisplay();
    }
}

// ----------------------------------------------------------
// Teclado normal
// ----------------------------------------------------------
void teclado(unsigned char key, int x, int y)
{
    if (estado == MENSAJE_ERROR) {
        estado = MENU;
        glutPostRedisplay();
        return;
    }

    if (key == 27) { // ESC
        exit(0);
    }
    else if (key == 13 || key == ' ') { // ENTER o espacio
        lanzarSeleccionado();
    }
    else if (key >= '0' && key <= '9') {
        int n = key - '0';
        // '1'..'9' -> indices 0..8 ; '0' -> programa 10 -> indice 9
        int idx = (n == 0) ? 9 : (n - 1);
        if (idx >= 0 && idx < (int)programas.size()) {
            seleccion = idx;
            glutPostRedisplay();
        }
    }
}

// ----------------------------------------------------------
// Mouse: click en un item lo selecciona y doble click lo ejecuta
// ----------------------------------------------------------
void mouse(int button, int buttonState, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || buttonState != GLUT_DOWN) return;
    if (estado != MENU) return;

    int yGL = ventanaAlto - y; // invertir coordenada Y (GLUT usa origen arriba-izq)

    float yInicio = ventanaAlto - 150.0f;
    float alturaItem = 50.0f;
    float margenX = 90.0f;
    float anchoItem = ventanaAncho - 2 * margenX;

    for (size_t i = 0; i < programas.size(); i++) {
        float yC = yInicio - i * alturaItem;
        float yTop = yC + 32;
        float yBot = yC - 8;

        if (x >= margenX && x <= margenX + anchoItem && yGL >= yBot && yGL <= yTop) {
            seleccion = (int)i;
            glutPostRedisplay();
            break;
        }
    }
}

// ----------------------------------------------------------
// Redimension de ventana
// ----------------------------------------------------------
void reshape(int w, int h)
{
    ventanaAncho = w;
    ventanaAlto = h;
    glViewport(0, 0, w, h);
}

// ----------------------------------------------------------
// main
// ----------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(ventanaAncho, ventanaAlto);
    glutInitWindowPosition(100, 60);
    glutCreateWindow("Launcher - Menu de Programas OpenGL");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciales);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
