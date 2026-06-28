#include <GL/glut.h>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

int modo = 0; 
std::vector<std::string> lineasExplicacion;

void dibujarRejilla(int w, int h) {
    glColor4f(0.2f, 0.25f, 0.3f, 0.5f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for(int i = 0; i <= w; i += 40) { glVertex2i(i, 0); glVertex2i(i, h); }
    for(int i = 0; i <= h; i += 40) { glVertex2i(0, i); glVertex2i(w, i); }
    glEnd();
}

void mostrarTexto(int x, int y, std::string texto) {
    glRasterPos2i(x, y);
    for(char c : texto) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void mostrarExplicacion(std::string texto) {
    size_t pos = texto.find(':');
    glColor3f(1.0f, 1.0f, 1.0f);
    mostrarTexto(20, 40, texto.substr(0, pos + 1));
    mostrarTexto(20, 65, texto.substr(pos + 1));
}

void drawDDA(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0, dy = y1 - y0;
    int steps = std::abs(dx) > std::abs(dy) ? std::abs(dx) : std::abs(dy);
    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;
    float x = x0, y = y0;
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i(round(x), round(y));
        x += xInc; y += yInc;
    }
    glEnd();
}

void drawBresenham(int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0), dy = -std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    while (true) {
        glVertex2i(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    dibujarRejilla(800, 600);

    int coords[4][4] = {{100, 100, 700, 500}, {100, 500, 700, 100}, {400, 100, 400, 500}, {100, 300, 700, 300}};
    float colores[4][3] = {{0.29f, 0.53f, 0.91f}, {0.91f, 0.29f, 0.29f}, {0.29f, 0.91f, 0.53f}, {0.91f, 0.91f, 0.29f}};

    for(int i = 0; i < 4; i++) {
        glColor3fv(colores[i]);
        if (modo == 0) drawDDA(coords[i][0], coords[i][1], coords[i][2], coords[i][3]);
        else drawBresenham(coords[i][0], coords[i][1], coords[i][2], coords[i][3]);
    }

    mostrarExplicacion(lineasExplicacion[modo]);
    
    // Leyenda inferior
    glColor3f(0.7f, 0.7f, 0.7f);
    mostrarTexto(20, 570, "Instrucciones: Flechas IZQ/DER para cambiar. Observa la precision en la rejilla.");

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0); 
    glMatrixMode(GL_MODELVIEW);
}

void teclado(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT) modo = 1 - modo;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    std::ifstream ar("explicacion_algoritmos.txt");
    std::string l;
    while(std::getline(ar, l)) lineasExplicacion.push_back(l);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rasterizacion: DDA vs Bresenham");
    glClearColor(0.11f, 0.14f, 0.19f, 1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(teclado);
    glutMainLoop();
    return 0;
}