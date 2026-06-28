/*
 * =====================================================================
 *  CARDIOIDE CILÍNDRICO — Metodología NURBS
 *  Compilar (Windows/MinGW):
 *    g++ cardioid_nurbs.cpp -o cardioid_nurbs.exe -lfreeglut -lopengl32 -lglu32
 *  Compilar (Linux):
 *    g++ cardioid_nurbs.cpp -o cardioid_nurbs -lGL -lGLU -lglut -lm
 * =====================================================================
 */

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include <windows.h>
#endif

#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>
#include <vector>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

// ── Renombrar para evitar colision con POINTS de windef.h ─────────────────
enum RenderMode { RM_SOLID, RM_WIRE, RM_POINTS };
static RenderMode renderMode = RM_SOLID;

// ── Parámetros de la superficie ───────────────────────────────────────────
static const int   NU   = 60;
static const int   NV   = 60;
static const float ZMIN = -1.5f;
static const float ZMAX =  1.5f;
static const float A    =  1.0f;

// ── Cámara ────────────────────────────────────────────────────────────────
static float rotX   =  30.0f;
static float rotY   = -30.0f;
static float zoom   =  1.0f;
static int   lastMX = 0, lastMY = 0;
static bool  dragging = false;
static bool  showGrid = true;
static int   winW = 1000, winH = 700;

// ── Datos NURBS ───────────────────────────────────────────────────────────
static std::vector<float> ctrlPts;
static std::vector<float> knotsU;
static std::vector<float> knotsV;
static int orderU = 4;
static int orderV = 4;
static int numU, numV;
static GLUnurbsObj* nurbsObj = nullptr;

// ─────────────────────────────────────────────────────────────────────────
//  Knot vector clamped uniforme
// ─────────────────────────────────────────────────────────────────────────
static std::vector<float> makeKnots(int N, int p)
{
    int nk    = N + p;
    int inner = N - p;
    std::vector<float> k(nk, 0.0f);
    for (int i = 0; i < p; ++i) k[nk - 1 - i] = 1.0f;
    for (int i = 0; i < inner; ++i)
        k[p + i] = (float)(i + 1) / (inner + 1);
    return k;
}

// ─────────────────────────────────────────────────────────────────────────
//  Construir malla de control  r(t) = a(1 - cos t)
// ─────────────────────────────────────────────────────────────────────────
static void buildSurface()
{
    numU = NU;
    numV = NV;
    ctrlPts.resize(numU * numV * 3);

    for (int j = 0; j < numV; ++j) {
        float z = ZMIN + (ZMAX - ZMIN) * j / (numV - 1);
        for (int i = 0; i < numU; ++i) {
            float theta = 2.0f * (float)M_PI * i / (numU - 1);
            float r     = A * (1.0f - cosf(theta));
            int   idx   = (j * numU + i) * 3;
            ctrlPts[idx + 0] = r * cosf(theta);
            ctrlPts[idx + 1] = r * sinf(theta);
            ctrlPts[idx + 2] = z;
        }
    }

    knotsU = makeKnots(numU, orderU);
    knotsV = makeKnots(numV, orderV);
}

// ─────────────────────────────────────────────────────────────────────────
//  HUD — título + instrucciones
// ─────────────────────────────────────────────────────────────────────────
static void drawText(float x, float y, const char* s, void* font)
{
    glRasterPos2f(x, y);
    for (const char* c = s; *c; ++c)
        glutBitmapCharacter(font, *c);
}

static void drawHUD()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // ── Título ────────────────────────────────────────────────────────────
    glColor3f(0.85f, 0.92f, 1.0f);
    drawText(14, winH - 34,
             "CARDIOIDE CILINDRICO  -  NURBS",
             GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.50f, 0.72f, 1.0f);
    drawText(16, winH - 56,
             "r(t) = a(1 - cos t)   |   a = 1.0   |   z en [-1.5, 1.5]",
             GLUT_BITMAP_HELVETICA_12);

    // ── Panel semitransparente ────────────────────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f, 0.10f, 0.14f, 0.78f);
    glBegin(GL_QUADS);
      glVertex2f(12,  8);
      glVertex2f(318, 8);
      glVertex2f(318, 192);
      glVertex2f(12,  192);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0.30f, 0.52f, 0.85f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(12,  8);
      glVertex2f(318, 8);
      glVertex2f(318, 192);
      glVertex2f(12,  192);
    glEnd();

    // ── Encabezado controles ──────────────────────────────────────────────
    glColor3f(0.70f, 0.88f, 1.0f);
    drawText(18, 174, "CONTROLES", GLUT_BITMAP_HELVETICA_12);

    struct Row { const char* key; const char* desc; };
    static const Row rows[] = {
        { "Arrastrar raton",          "Rotar la figura"              },
        { "Rueda raton",              "Zoom in / out"                },
        { "A / D",                    "Rotar en eje Y"               },
        { "W / S",                    "Rotar en eje X"               },
        { "R",                        "Restablecer vista"            },
        { "G",                        "Mostrar/ocultar cuadricula"   },
        { "N",                        "Solido / Alambre / Puntos"    },
        { "ESC",                      "Salir"                        },
    };

    glColor3f(0.55f, 0.72f, 0.92f);
    int y = 156;
    for (auto& r : rows) {
        char buf[80];
        snprintf(buf, sizeof(buf), "%-24s%s", r.key, r.desc);
        drawText(18, (float)y, buf, GLUT_BITMAP_HELVETICA_12);
        y -= 18;
    }

    // ── Modo actual ───────────────────────────────────────────────────────
    const char* modeStr = (renderMode == RM_SOLID)  ? "SOLIDO"
                        : (renderMode == RM_WIRE)   ? "ALAMBRE"
                        :                             "PUNTOS";
    char mline[64];
    snprintf(mline, sizeof(mline), "Modo actual: %s", modeStr);
    glColor3f(0.35f, 0.92f, 0.55f);
    drawText(18, 22, mline, GLUT_BITMAP_HELVETICA_12);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ─────────────────────────────────────────────────────────────────────────
//  Cuadrícula de referencia
// ─────────────────────────────────────────────────────────────────────────
static void drawGrid()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.22f, 0.30f, 0.40f);
    glLineWidth(1.0f);
    float ext = 2.8f;
    int   n   = 10;
    glBegin(GL_LINES);
    for (int i = -n; i <= n; ++i) {
        float t = ext * i / n;
        glVertex3f(t,    0, -ext); glVertex3f(t,    0,  ext);
        glVertex3f(-ext, 0,  t  ); glVertex3f( ext, 0,  t  );
    }
    glEnd();

    // Ejes X Y Z
    glLineWidth(2.0f);
    glBegin(GL_LINES);
      glColor3f(0.9f, 0.3f, 0.3f);
      glVertex3f(0,0,0); glVertex3f(2.2f,0,0);
      glColor3f(0.3f, 0.9f, 0.3f);
      glVertex3f(0,0,0); glVertex3f(0,2.2f,0);
      glColor3f(0.3f, 0.5f, 0.9f);
      glVertex3f(0,0,0); glVertex3f(0,0,2.2f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

// ─────────────────────────────────────────────────────────────────────────
//  Render NURBS
// ─────────────────────────────────────────────────────────────────────────
static void renderNURBS()
{
    if (!nurbsObj) return;
    gluNurbsProperty(nurbsObj, GLU_SAMPLING_TOLERANCE, 6.0f);

    switch (renderMode) {
        case RM_SOLID:
            gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, (GLfloat)GLU_FILL);
            break;
        case RM_WIRE:
            gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, (GLfloat)GLU_OUTLINE_POLYGON);
            break;
        case RM_POINTS:
            gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, (GLfloat)GLU_OUTLINE_PATCH);
            break;
    }

    gluBeginSurface(nurbsObj);
    gluNurbsSurface(nurbsObj,
        (GLint)knotsU.size(), knotsU.data(),
        (GLint)knotsV.size(), knotsV.data(),
        3,
        numU * 3,
        ctrlPts.data(),
        orderU, orderV,
        GL_MAP2_VERTEX_3);
    gluEndSurface(nurbsObj);
}

// ─────────────────────────────────────────────────────────────────────────
//  Callbacks GLUT
// ─────────────────────────────────────────────────────────────────────────
static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 7.0 / zoom, 0, 0, 0, 0, 1, 0);
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);

    if (showGrid) drawGrid();

    if (renderMode == RM_SOLID) {
        GLfloat amb[]  = { 0.12f, 0.18f, 0.28f, 1.0f };
        GLfloat diff[] = { 0.22f, 0.58f, 1.00f, 1.0f };
        GLfloat spec[] = { 1.00f, 1.00f, 1.00f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  spec);
        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 80.0f);
    } else {
        glDisable(GL_LIGHTING);
        if (renderMode == RM_WIRE)   glColor3f(0.28f, 0.75f, 1.0f);
        else                         glColor3f(0.95f, 0.90f, 0.35f);
    }

    renderNURBS();

    if (renderMode != RM_SOLID) glEnable(GL_LIGHTING);

    drawHUD();
    glutSwapBuffers();
}

static void reshape(int w, int h)
{
    winW = w; winH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

static void keyboard(unsigned char key, int, int)
{
    switch (key) {
        case 27:                      exit(0);
        case 'a': case 'A': rotY -= 5.0f; break;
        case 'd': case 'D': rotY += 5.0f; break;
        case 'w': case 'W': rotX -= 5.0f; break;
        case 's': case 'S': rotX += 5.0f; break;
        case 'r': case 'R':
            rotX = 30.0f; rotY = -30.0f; zoom = 1.0f; break;
        case 'g': case 'G': showGrid = !showGrid; break;
        case 'n': case 'N':
            renderMode = (RenderMode)((renderMode + 1) % 3); break;
    }
    glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        dragging = (state == GLUT_DOWN);
        lastMX = x; lastMY = y;
    }
    if (button == 3) { zoom *= 1.08f; glutPostRedisplay(); }
    if (button == 4) { zoom /= 1.08f; if (zoom < 0.05f) zoom = 0.05f;
                       glutPostRedisplay(); }
}

static void motion(int x, int y)
{
    if (!dragging) return;
    rotY += (x - lastMX) * 0.5f;
    rotX += (y - lastMY) * 0.5f;
    lastMX = x; lastMY = y;
    glutPostRedisplay();
}

static void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ─────────────────────────────────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────────────────────────────────
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Cardioide Cilindrico - NURBS");

    glClearColor(0.11f, 0.14f, 0.19f, 1.0f);

    // Iluminación
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lpos[] = { 3.0f,  5.0f,  7.0f, 1.0f };
    GLfloat lamb[] = { 0.08f, 0.10f, 0.18f, 1.0f };
    GLfloat ldif[] = { 0.90f, 0.90f, 1.00f, 1.0f };
    GLfloat lspc[] = { 1.00f, 1.00f, 1.00f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lamb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  ldif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspc);

    glEnable(GL_LIGHT1);
    GLfloat l1pos[] = { -4.0f, -2.0f, -3.0f, 1.0f };
    GLfloat l1dif[] = {  0.18f, 0.28f,  0.55f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, l1pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  l1dif);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    buildSurface();

    nurbsObj = gluNewNurbsRenderer();
    gluNurbsProperty(nurbsObj, GLU_AUTO_LOAD_MATRIX, GL_TRUE);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();

    gluDeleteNurbsRenderer(nurbsObj);
    return 0;
}
