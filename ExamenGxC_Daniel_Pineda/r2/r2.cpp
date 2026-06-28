#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int modo = 0; // Índice de la primitiva actual
std::vector<std::string> explicaciones;

void cargarExplicaciones() {
    std::ifstream archivo("explicacion.txt");
    std::string linea;
    while (std::getline(archivo, linea)) explicaciones.push_back(linea);
}

void dibujarPrimitiva(int m) {
    glBegin(m);
    glColor3f(0.29f, 0.53f, 0.91f);
    // Vértices de ejemplo
    glVertex3f(-0.5, 0.5, 0.0); glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0); glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.0, 0.8, 0.0);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Dibujar Primitiva
    GLenum tipos[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, 
                      GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON};
    
    dibujarPrimitiva(tipos[modo]);

    // UI: Título y Leyenda
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9, 0.9);
    for(char c : explicaciones[modo]) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    glColor3f(0.7f, 0.7f, 0.7f);
    glRasterPos2f(-0.9, -0.9);
    std::string ley = "Instrucciones: Usa las flechas IZQ/DER para cambiar de primitiva.";
    for(char c : ley) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);

    glutSwapBuffers();
}

void teclado(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) modo = (modo + 1) % 10;
    if (key == GLUT_KEY_LEFT) modo = (modo - 1 + 10) % 10;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    cargarExplicaciones();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Primitivas OpenGL");
    glClearColor(0.11f, 0.14f, 0.19f, 1.0f);
    glutDisplayFunc(display);
    glutSpecialFunc(teclado);
    glutMainLoop();
    return 0;
}