#include <GL/glut.h>
#include <string>
#include <vector>

int width = 800, height = 400;

// Nueva funcion para dibujar texto alineado a la izquierda dentro de una coordenada x dada
void renderizarTextoIzquierda(float x, float y, void* fuente, const std::string& texto) {
    glRasterPos2f(x, y);
    for (char c : texto) {
        glutBitmapCharacter(fuente, c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    std::string titulo = "1. Cuando y donde se presento OpenGL?";
    std::vector<std::string> cuerpo = {
        "OpenGL se presento en 1992 por Silicon Graphics, Inc. (SGI),",
        "en Estados Unidos; su primera version publica se lanzo",
        "el 30 de junio de 1992."
    };

    // --- CONFIGURACIÓN DE MEDIDAS ---
    float anchoChar = 11.5f;
    float largoTitulo = titulo.length() * anchoChar;
    // Definimos el ancho de la caja basándonos en la línea más larga (la primera del cuerpo)
    float anchoCaja = 600.0f; 
    float xCaja = (width / 2.0f) - (anchoCaja / 2.0f);
    float yBase = (height / 2.0f) + 50.0f;

    // --- DIBUJAR CAJA (Fondo sutil para el texto) ---
    glColor4f(0.18f, 0.22f, 0.28f, 0.5f); // Color caja un poco mas claro que el fondo
    glBegin(GL_QUADS);
        glVertex2f(xCaja - 20, yBase + 40);
        glVertex2f(xCaja + anchoCaja + 20, yBase + 40);
        glVertex2f(xCaja + anchoCaja + 20, yBase - 120);
        glVertex2f(xCaja - 20, yBase - 120);
    glEnd();

    // --- DIBUJAR TÍTULO (Centrado sobre la caja) ---
    float xTitulo = (width / 2.0f) - (largoTitulo / 2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderizarTextoIzquierda(xTitulo, yBase + 20.0f, GLUT_BITMAP_HELVETICA_18, titulo);

    // --- LÍNEA SUBRAYADO ---
    glColor3f(0.29f, 0.53f, 0.91f);
    glBegin(GL_LINES);
        glVertex2f(xTitulo, yBase + 10.0f);
        glVertex2f(xTitulo + largoTitulo, yBase + 10.0f);
    glEnd();

    // --- DIBUJAR CUERPO (Alineado a la izquierda dentro de la caja) ---
    glColor3f(0.85f, 0.88f, 0.92f);
    float yCuerpo = yBase - 30.0f;
    for (const std::string& linea : cuerpo) {
        renderizarTextoIzquierda(xCaja, yCuerpo, GLUT_BITMAP_HELVETICA_18, linea);
        yCuerpo -= 30.0f;
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    width = w; height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Proyecto Graficacion - Caja Centrada");
    glClearColor(0.11f, 0.14f, 0.19f, 1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}