/*
 * =====================================================================
 *  FIGURAS 3D — Alambre y Solido con sombras
 *  Piramide Hexagonal, Prisma Hexagonal, Octaedro,
 *  Icosaedro, Dodecaedro, Dona (Torus)
 *
 *  Compilar Windows/MinGW:
 *    g++ r5_transformaciones.cpp -o figuras3d.exe -lfreeglut -lopengl32 -lglu32
 *  Compilar Linux:
 *    g++ r5_transformaciones.cpp -o figuras3d -lGL -lGLU -lglut -lm
 *
 *  Controles generales:
 *    ESPACIO / V      → Siguiente figura
 *    B                → Figura anterior
 *    1-6              → Ir directo a figura
 *    Tab              → Cambiar modo: Solido / Alambre / Ambos
 *    Arrastrar raton  → Rotar vista de camara
 *    Rueda raton      → Zoom in / out
 *    G                → Mostrar/ocultar cuadricula
 *    ESC              → Salir
 *
 *  Transformaciones afines (por figura activa):
 *    -- ROTACION --
 *    Rx / rx          → Rotar +/- grados alrededor del eje X
 *    Ry / ry          → Rotar +/- grados alrededor del eje Y
 *    Rz / rz          → Rotar +/- grados alrededor del eje Z
 *    -- TRASLACION --
 *    Tx / tx          → Trasladar +/- en X
 *    Ty / ty          → Trasladar +/- en Y
 *    Tz / tz          → Trasladar +/- en Z
 *    -- ESCALAMIENTO --
 *    E                → Aumentar tamaño (escalar +)
 *    e                → Disminuir tamaño (escalar -)
 *    -- SESGO --
 *    H / h            → Sesgo en X (shear positivo/negativo)
 *    J / j            → Sesgo en Y (shear positivo/negativo)
 *    -- RESET --
 *    R                → Restablecer TODAS las transformaciones de la figura
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
#include <cstring>
#include <vector>
#include <string>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

// ── Modo de render ────────────────────────────────────────────────────────────
enum RenderMode { RM_SOLID, RM_WIRE, RM_BOTH };
static RenderMode renderMode = RM_SOLID;

// ── Figuras disponibles ───────────────────────────────────────────────────────
enum Figure {
    FIG_PIRAMIDE_HEX = 0,
    FIG_PRISMA_HEX,
    FIG_OCTAEDRO,
    FIG_ICOSAEDRO,
    FIG_DODECAEDRO,
    FIG_DONA,
    FIG_COUNT
};
static Figure currentFig = FIG_PIRAMIDE_HEX;

static const char* figNames[] = {
    "PIRAMIDE HEXAGONAL",
    "PRISMA HEXAGONAL",
    "OCTAEDRO",
    "ICOSAEDRO",
    "DODECAEDRO",
    "DONA (TORUS)"
};

// ── Transformaciones afines por figura ───────────────────────────────────────
struct AffineTransform {
    float rotX   = 0.0f;   // rotacion acumulada en X (grados)
    float rotY   = 0.0f;   // rotacion acumulada en Y (grados)
    float rotZ   = 0.0f;   // rotacion acumulada en Z (grados)
    float transX = 0.0f;   // traslacion en X
    float transY = 0.0f;   // traslacion en Y
    float transZ = 0.0f;   // traslacion en Z
    float scale  = 1.0f;   // factor de escala uniforme
    float shearX = 0.0f;   // sesgo en X (shear XY)
    float shearY = 0.0f;   // sesgo en Y (shear YX)

    void reset() {
        rotX=rotY=rotZ=0.0f;
        transX=transY=transZ=0.0f;
        scale=1.0f;
        shearX=shearY=0.0f;
    }
};

static AffineTransform figTransforms[FIG_COUNT];

// Incrementos de cada transformacion
static const float ROT_STEP   = 5.0f;   // grados por tecla
static const float TRANS_STEP = 0.10f;  // unidades por tecla
static const float SCALE_STEP = 0.10f;  // factor por tecla
static const float SHEAR_STEP = 0.05f;  // sesgo por tecla

// ── Camara ────────────────────────────────────────────────────────────────────
static float camRotX  =  25.0f;
static float camRotY  = -35.0f;
static float zoom     =  1.0f;
static int   lastMX   =  0, lastMY = 0;
static bool  dragging = false;
static bool  showGrid = true;
static int   winW     = 1200, winH = 720;

// ─────────────────────────────────────────────────────────────────────────────
//  Aplica la matriz de transformacion afin acumulada de la figura
//  Orden: Traslacion * Rotacion * Sesgo * Escalamiento
// ─────────────────────────────────────────────────────────────────────────────
static void applyAffineTransform(const AffineTransform& t)
{
    // 1. Traslacion
    glTranslatef(t.transX, t.transY, t.transZ);

    // 2. Rotacion (X luego Y luego Z)
    glRotatef(t.rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(t.rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(t.rotZ, 0.0f, 0.0f, 1.0f);

    // 3. Sesgo (shear): matriz de sesgo en columna mayor de OpenGL
    //    | 1  shY  0  0 |
    //    | shX  1  0  0 |
    //    | 0    0  1  0 |
    //    | 0    0  0  1 |
    if (t.shearX != 0.0f || t.shearY != 0.0f) {
        float shear[16] = {
            1.0f,    t.shearX, 0.0f, 0.0f,
            t.shearY, 1.0f,   0.0f, 0.0f,
            0.0f,    0.0f,    1.0f, 0.0f,
            0.0f,    0.0f,    0.0f, 1.0f
        };
        glMultMatrixf(shear);
    }

    // 4. Escalamiento uniforme
    glScalef(t.scale, t.scale, t.scale);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Calcula normal de un triangulo (v0->v1, v0->v2)
// ─────────────────────────────────────────────────────────────────────────────
static void computeNormal(const float* v0, const float* v1, const float* v2)
{
    float ax = v1[0]-v0[0], ay = v1[1]-v0[1], az = v1[2]-v0[2];
    float bx = v2[0]-v0[0], by = v2[1]-v0[1], bz = v2[2]-v0[2];
    float nx = ay*bz - az*by;
    float ny = az*bx - ax*bz;
    float nz = ax*by - ay*bx;
    glNormal3f(nx, ny, nz);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Configura material segun figura y modo
// ─────────────────────────────────────────────────────────────────────────────
struct MatDef { float amb[4]; float dif[4]; float spc[4]; float shi; };

static const MatDef mats[FIG_COUNT] = {
    // Piramide hex — turquesa
    { {0.05f,0.18f,0.22f,1}, {0.15f,0.72f,0.85f,1}, {0.9f,1.0f,1.0f,1}, 96 },
    // Prisma hex — dorado
    { {0.22f,0.16f,0.02f,1}, {0.85f,0.65f,0.10f,1}, {1.0f,0.95f,0.7f,1}, 80 },
    // Octaedro — coral
    { {0.22f,0.06f,0.04f,1}, {0.90f,0.30f,0.22f,1}, {1.0f,0.8f,0.8f,1}, 110 },
    // Icosaedro — lila
    { {0.12f,0.05f,0.22f,1}, {0.55f,0.30f,0.95f,1}, {1.0f,0.9f,1.0f,1}, 90 },
    // Dodecaedro — verde esmeralda
    { {0.02f,0.20f,0.08f,1}, {0.15f,0.80f,0.35f,1}, {0.8f,1.0f,0.8f,1}, 85 },
    // Dona — naranja
    { {0.20f,0.10f,0.02f,1}, {0.95f,0.50f,0.10f,1}, {1.0f,0.9f,0.7f,1}, 100 },
};

static void setMaterial(Figure f)
{
    const MatDef& m = mats[f];
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   m.amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   m.dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m.spc);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, m.shi);
}

// ═════════════════════════════════════════════════════════════════════════════
//  PIRAMIDE HEXAGONAL
// ═════════════════════════════════════════════════════════════════════════════
static void drawPiramideHex(bool wire)
{
    const int N  = 6;
    const float R = 1.0f, H = 1.8f, Y0 = -0.9f;
    float bx[N], bz[N];
    for (int i = 0; i < N; i++) {
        float a = 2.0f*(float)M_PI*i/N;
        bx[i] = R*cosf(a);
        bz[i] = R*sinf(a);
    }
    float apex[3] = {0, Y0+H, 0};

    // Caras laterales
    for (int i = 0; i < N; i++) {
        int j = (i+1)%N;
        float v0[3] = {bx[i], Y0, bz[i]};
        float v1[3] = {bx[j], Y0, bz[j]};
        computeNormal(v0, v1, apex);
        glBegin(wire ? GL_LINE_LOOP : GL_TRIANGLES);
          glVertex3fv(v0);
          glVertex3fv(v1);
          glVertex3fv(apex);
        glEnd();
    }
    // Base hexagonal (como abanico)
    float nBase[3]  = {0,-1, 0};
    glNormal3fv(nBase);
    for (int i = 0; i < N; i++) {
        int j = (i+1)%N;
        glBegin(wire ? GL_LINE_LOOP : GL_TRIANGLES);
          glVertex3f(0, Y0, 0);
          glVertex3f(bx[j], Y0, bz[j]);
          glVertex3f(bx[i], Y0, bz[i]);
        glEnd();
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  PRISMA HEXAGONAL
// ═════════════════════════════════════════════════════════════════════════════
static void drawPrismaHex(bool wire)
{
    const int N  = 6;
    const float R = 1.0f, H = 1.6f;
    float bx[N], bz[N];
    for (int i = 0; i < N; i++) {
        float a = 2.0f*(float)M_PI*i/N + (float)M_PI/6.0f;
        bx[i] = R*cosf(a);
        bz[i] = R*sinf(a);
    }
    float Y0 = -H*0.5f, Y1 = H*0.5f;

    // Caras laterales
    for (int i = 0; i < N; i++) {
        int j = (i+1)%N;
        float v0[3]={bx[i],Y0,bz[i]}, v1[3]={bx[j],Y0,bz[j]};
        float v2[3]={bx[j],Y1,bz[j]}, v3[3]={bx[i],Y1,bz[i]};
        float nx = 0.5f*(bx[i]+bx[j]), nz = 0.5f*(bz[i]+bz[j]);
        float len = sqrtf(nx*nx+nz*nz);
        glNormal3f(nx/len, 0, nz/len);
        glBegin(wire ? GL_LINE_LOOP : GL_QUADS);
          glVertex3fv(v0); glVertex3fv(v1);
          glVertex3fv(v2); glVertex3fv(v3);
        glEnd();
    }
    // Tapas
    for (int side = 0; side < 2; side++) {
        float y  = (side==0) ? Y0 : Y1;
        float ny = (side==0) ? -1.0f : 1.0f;
        glNormal3f(0, ny, 0);
        for (int i = 0; i < N; i++) {
            int j = (side==0) ? (i+1)%N : (N-1-i+N)%N;
            int k = (side==0) ? i        : (N-i)%N;
            glBegin(wire ? GL_LINE_LOOP : GL_TRIANGLES);
              glVertex3f(0, y, 0);
              glVertex3f(bx[j], y, bz[j]);
              glVertex3f(bx[k], y, bz[k]);
            glEnd();
        }
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  OCTAEDRO
// ═════════════════════════════════════════════════════════════════════════════
static void drawOctaedro(bool wire)
{
    const float s = 1.0f;
    float v[6][3] = {
        { s, 0, 0}, {-s, 0, 0},
        { 0, s, 0}, { 0,-s, 0},
        { 0, 0, s}, { 0, 0,-s}
    };
    int f8[8][3] = {
        {0,2,4},{2,1,4},{1,3,4},{3,0,4},
        {2,0,5},{1,2,5},{3,1,5},{0,3,5}
    };
    for (int i = 0; i < 8; i++) {
        float* a = v[f8[i][0]];
        float* b = v[f8[i][1]];
        float* c = v[f8[i][2]];
        computeNormal(a,b,c);
        glBegin(wire ? GL_LINE_LOOP : GL_TRIANGLES);
          glVertex3fv(a); glVertex3fv(b); glVertex3fv(c);
        glEnd();
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  ICOSAEDRO
// ═════════════════════════════════════════════════════════════════════════════
static void drawIcosaedro(bool wire)
{
    const float t = (1.0f + sqrtf(5.0f)) * 0.5f;
    const float s = 1.0f / sqrtf(1.0f + t*t);
    float vv[12][3] = {
        {-s, t*s, 0},{ s, t*s, 0},{-s,-t*s, 0},{ s,-t*s, 0},
        { 0,-s, t*s},{ 0, s, t*s},{ 0,-s,-t*s},{ 0, s,-t*s},
        { t*s, 0,-s},{ t*s, 0, s},{-t*s, 0,-s},{-t*s, 0, s}
    };
    int fi[20][3] = {
        {0,11,5},{0,5,1},{0,1,7},{0,7,10},{0,10,11},
        {1,5,9},{5,11,4},{11,10,2},{10,7,6},{7,1,8},
        {3,9,4},{3,4,2},{3,2,6},{3,6,8},{3,8,9},
        {4,9,5},{2,4,11},{6,2,10},{8,6,7},{9,8,1}
    };
    for (int i = 0; i < 20; i++) {
        float* a = vv[fi[i][0]];
        float* b = vv[fi[i][1]];
        float* c = vv[fi[i][2]];
        computeNormal(a,b,c);
        glBegin(wire ? GL_LINE_LOOP : GL_TRIANGLES);
          glVertex3fv(a); glVertex3fv(b); glVertex3fv(c);
        glEnd();
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  DODECAEDRO  (12 pentagonos)
// ═════════════════════════════════════════════════════════════════════════════
static void drawDodecaedro(bool wire)
{
    const float phi = (1.0f + sqrtf(5.0f)) * 0.5f;
    const float sc  = 0.92f;
    const float p   = sc * phi;
    const float ip  = sc / phi;
    const float u   = sc * 1.0f;

    float vv[20][3] = {
        { u, u, u},  // 0
        { u, u,-u},  // 1
        { u,-u, u},  // 2
        { u,-u,-u},  // 3
        {-u, u, u},  // 4
        {-u, u,-u},  // 5
        {-u,-u, u},  // 6
        {-u,-u,-u},  // 7
        { 0, ip, p}, // 8
        { 0, ip,-p}, // 9
        { 0,-ip, p}, // 10
        { 0,-ip,-p}, // 11
        { ip, p, 0}, // 12
        { ip,-p, 0}, // 13
        {-ip, p, 0}, // 14
        {-ip,-p, 0}, // 15
        { p, 0, ip}, // 16
        { p, 0,-ip}, // 17
        {-p, 0, ip}, // 18
        {-p, 0,-ip}, // 19
    };

    int fi[12][5] = {
        { 0, 8, 4,14,12},
        { 0,12, 1,17,16},
        { 0,16, 2,10, 8},
        { 4, 8,10, 6,18},
        { 4,18,19, 5,14},
        { 1,12,14, 5, 9},
        { 2,16,17, 3,13},
        { 2,13,15, 6,10},
        { 6,15, 7,19,18},
        { 3,17, 1, 9,11},
        { 5,19, 7,11, 9},
        { 3,11, 7,15,13},
    };

    for (int i = 0; i < 12; i++) {
        float cx=0, cy=0, cz=0;
        for (int k = 0; k < 5; k++) {
            cx += vv[fi[i][k]][0];
            cy += vv[fi[i][k]][1];
            cz += vv[fi[i][k]][2];
        }
        float len = sqrtf(cx*cx + cy*cy + cz*cz);
        glNormal3f(cx/len, cy/len, cz/len);

        float* p0 = vv[fi[i][0]];
        for (int k = 1; k < 4; k++) {
            glBegin(wire ? GL_LINE_LOOP : GL_TRIANGLES);
              glVertex3fv(p0);
              glVertex3fv(vv[fi[i][k]]);
              glVertex3fv(vv[fi[i][k+1]]);
            glEnd();
        }
        if (wire) {
            glBegin(GL_LINE_LOOP);
            for (int k = 0; k < 5; k++) glVertex3fv(vv[fi[i][k]]);
            glEnd();
        }
    }
    (void)p; (void)ip; (void)u;
}

// ═════════════════════════════════════════════════════════════════════════════
//  DONA (TORUS)
// ═════════════════════════════════════════════════════════════════════════════
static void drawDona(bool wire)
{
    const float R  = 0.80f;
    const float r  = 0.35f;
    const int   NS = 48;
    const int   NT = 64;

    for (int i = 0; i < NT; i++) {
        float u0 = 2.0f*(float)M_PI*i/NT;
        float u1 = 2.0f*(float)M_PI*(i+1)/NT;

        glBegin(wire ? GL_LINE_LOOP : GL_QUAD_STRIP);
        for (int j = 0; j <= NS; j++) {
            float v = 2.0f*(float)M_PI*j/NS;
            float cv = cosf(v), sv = sinf(v);
            for (int k = 0; k < 2; k++) {
                float u  = (k==0) ? u0 : u1;
                float cu = cosf(u), su = sinf(u);
                float x = (R + r*cv)*cu;
                float y = r*sv;
                float z = (R + r*cv)*su;
                float nx = cv*cu;
                float ny = sv;
                float nz = cv*su;
                glNormal3f(nx, ny, nz);
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  Dispatch de figuras
// ═════════════════════════════════════════════════════════════════════════════
static void drawFigure(Figure f, bool wire)
{
    switch (f) {
        case FIG_PIRAMIDE_HEX: drawPiramideHex(wire); break;
        case FIG_PRISMA_HEX:   drawPrismaHex(wire);   break;
        case FIG_OCTAEDRO:     drawOctaedro(wire);     break;
        case FIG_ICOSAEDRO:    drawIcosaedro(wire);    break;
        case FIG_DODECAEDRO:   drawDodecaedro(wire);   break;
        case FIG_DONA:         drawDona(wire);          break;
        default: break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Cuadricula de referencia
// ─────────────────────────────────────────────────────────────────────────────
static void drawGrid()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.20f, 0.28f, 0.38f);
    glLineWidth(1.0f);
    float ext = 3.0f;
    int   n   = 12;
    glBegin(GL_LINES);
    for (int i = -n; i <= n; i++) {
        float t = ext*i/n;
        glVertex3f( t,   -1.2f, -ext); glVertex3f( t,   -1.2f,  ext);
        glVertex3f(-ext, -1.2f,  t  ); glVertex3f( ext, -1.2f,  t  );
    }
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINES);
      glColor3f(0.85f,0.28f,0.28f);
      glVertex3f(0,-1.2f,0); glVertex3f(2.3f,-1.2f,0);
      glColor3f(0.28f,0.85f,0.28f);
      glVertex3f(0,-1.2f,0); glVertex3f(0,1.1f,0);
      glColor3f(0.28f,0.45f,0.90f);
      glVertex3f(0,-1.2f,0); glVertex3f(0,-1.2f,2.3f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

// ─────────────────────────────────────────────────────────────────────────────
//  HUD
// ─────────────────────────────────────────────────────────────────────────────
static void drawText(float x, float y, const char* s, void* font)
{
    glRasterPos2f(x, y);
    for (const char* c = s; *c; c++)
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

    // ── Titulo principal ──────────────────────────────────────────────────
    glColor3f(0.85f, 0.93f, 1.0f);
    drawText(14, winH - 34,
             "FIGURAS 3D  —  TRANSFORMACIONES AFINES",
             GLUT_BITMAP_HELVETICA_18);

    // Nombre de la figura actual
    glColor3f(0.45f, 0.82f, 1.0f);
    char titulo[80];
    snprintf(titulo, sizeof(titulo), "Figura activa:  %s", figNames[currentFig]);
    drawText(16, winH - 58, titulo, GLUT_BITMAP_HELVETICA_12);

    // Modo actual
    const char* modeStr = (renderMode==RM_SOLID) ? "SOLIDO"
                        : (renderMode==RM_WIRE)  ? "ALAMBRE"
                        :                          "AMBOS";
    char modeLabel[40];
    snprintf(modeLabel, sizeof(modeLabel), "Modo: %s", modeStr);
    glColor3f(0.35f, 0.92f, 0.55f);
    drawText(winW - 160.0f, winH - 34, modeLabel, GLUT_BITMAP_HELVETICA_12);

    // ── Estado de transformaciones de la figura activa ────────────────────
    const AffineTransform& tf = figTransforms[currentFig];

    // Panel de estado (derecha)
    float px = winW - 320.0f;
    float py = winH - 80.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.06f, 0.09f, 0.13f, 0.85f);
    glBegin(GL_QUADS);
      glVertex2f(px-8, py-150); glVertex2f(winW-8, py-150);
      glVertex2f(winW-8, py+14); glVertex2f(px-8, py+14);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0.28f, 0.50f, 0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(px-8, py-150); glVertex2f(winW-8, py-150);
      glVertex2f(winW-8, py+14); glVertex2f(px-8, py+14);
    glEnd();

    glColor3f(0.72f, 0.88f, 1.0f);
    drawText(px, py, "ESTADO TRANSFORMACIONES", GLUT_BITMAP_HELVETICA_12);

    char buf[80];
    glColor3f(0.95f, 0.70f, 0.40f);
    snprintf(buf, sizeof(buf), "Rot X: %+.1f  Y: %+.1f  Z: %+.1f",
             tf.rotX, tf.rotY, tf.rotZ);
    drawText(px, py-20, buf, GLUT_BITMAP_HELVETICA_12);

    glColor3f(0.40f, 0.90f, 0.60f);
    snprintf(buf, sizeof(buf), "Trans X: %+.2f  Y: %+.2f  Z: %+.2f",
             tf.transX, tf.transY, tf.transZ);
    drawText(px, py-38, buf, GLUT_BITMAP_HELVETICA_12);

    glColor3f(0.60f, 0.80f, 1.00f);
    snprintf(buf, sizeof(buf), "Escala: %.2f", tf.scale);
    drawText(px, py-56, buf, GLUT_BITMAP_HELVETICA_12);

    glColor3f(1.00f, 0.70f, 0.90f);
    snprintf(buf, sizeof(buf), "Sesgo X: %+.2f  Y: %+.2f",
             tf.shearX, tf.shearY);
    drawText(px, py-74, buf, GLUT_BITMAP_HELVETICA_12);

    // ── Panel de instrucciones (izquierda) ────────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f, 0.10f, 0.14f, 0.80f);
    glBegin(GL_QUADS);
      glVertex2f(12,  8); glVertex2f(400, 8);
      glVertex2f(400,350); glVertex2f(12,350);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0.28f, 0.50f, 0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(12,  8); glVertex2f(400, 8);
      glVertex2f(400,350); glVertex2f(12,350);
    glEnd();

    glColor3f(0.72f, 0.88f, 1.0f);
    drawText(18, 332, "CONTROLES", GLUT_BITMAP_HELVETICA_12);

    struct Row { const char* k; const char* d; };
    static const Row rows[] = {
        {"ESPACIO / V / B",         "Siguiente / anterior figura"},
        {"1-6",                     "Ir directamente a figura"},
        {"Tab",                     "Cambiar modo Solido/Alambre/Ambos"},
        {"Arrastrar raton",         "Rotar vista de camara"},
        {"Rueda raton",             "Zoom in / out"},
        {"G",                       "Mostrar/ocultar cuadricula"},
        {"--- ROTACION FIGURA ---", ""},
        {"Rx / rx",                 "Rotar +/- alrededor eje X"},
        {"Ry / ry",                 "Rotar +/- alrededor eje Y"},
        {"Rz / rz",                 "Rotar +/- alrededor eje Z"},
        {"--- TRASLACION --------", ""},
        {"Tx / tx",                 "Trasladar +/- en eje X"},
        {"Ty / ty",                 "Trasladar +/- en eje Y"},
        {"Tz / tz",                 "Trasladar +/- en eje Z"},
        {"--- ESCALA ------------", ""},
        {"E / e",                   "Aumentar / disminuir tamanio"},
        {"--- SESGO -------------", ""},
        {"H / h",                   "Sesgo en X (positivo/negativo)"},
        {"J / j",                   "Sesgo en Y (positivo/negativo)"},
        {"--- RESET -------------", ""},
        {"R",                       "Restablecer todas las transform."},
        {"ESC",                     "Salir"},
    };
    glColor3f(0.55f, 0.72f, 0.92f);
    int yy = 314;
    for (auto& r : rows) {
        if (r.k[0] == '-') {
            glColor3f(0.45f, 0.65f, 0.35f);
            drawText(18, (float)yy, r.k, GLUT_BITMAP_HELVETICA_12);
            glColor3f(0.55f, 0.72f, 0.92f);
        } else {
            char line[80];
            snprintf(line, sizeof(line), "%-20s%s", r.k, r.d);
            drawText(18, (float)yy, line, GLUT_BITMAP_HELVETICA_12);
        }
        yy -= 14;
    }

    // ── Miniaturas de figuras (barra inferior) ────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f, 0.10f, 0.14f, 0.78f);
    glBegin(GL_QUADS);
      glVertex2f(0,0); glVertex2f((float)winW,0);
      glVertex2f((float)winW,38); glVertex2f(0,38);
    glEnd();
    glDisable(GL_BLEND);

    float bw = (float)winW / (float)FIG_COUNT;
    for (int i = 0; i < FIG_COUNT; i++) {
        float cx = bw*i + bw*0.5f;
        bool  sel = (i == (int)currentFig);
        if (sel) {
            glColor3f(0.28f, 0.60f, 0.95f);
            glBegin(GL_QUADS);
              glVertex2f(bw*i+2, 2); glVertex2f(bw*(i+1)-2, 2);
              glVertex2f(bw*(i+1)-2,36); glVertex2f(bw*i+2,36);
            glEnd();
            glColor3f(1.0f, 1.0f, 1.0f);
        } else {
            glColor3f(0.50f, 0.65f, 0.85f);
        }
        char lbl[32];
        snprintf(lbl, sizeof(lbl), "[%d] %s", i+1, figNames[i]);
        int tw = 0;
        for (const char* c = lbl; *c; c++)
            tw += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, *c);
        drawText(cx - tw*0.5f, 13, lbl, GLUT_BITMAP_HELVETICA_12);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ─────────────────────────────────────────────────────────────────────────────
//  DISPLAY
// ─────────────────────────────────────────────────────────────────────────────
static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camara global
    gluLookAt(0, 0.3, 6.0/zoom, 0, 0, 0, 0, 1, 0);
    glRotatef(camRotX, 1,0,0);
    glRotatef(camRotY, 0,1,0);

    if (showGrid) drawGrid();

    // Aplicar transformaciones afines de la figura activa
    glPushMatrix();
    applyAffineTransform(figTransforms[currentFig]);

    // ── Render solido ─────────────────────────────────────────────────────
    if (renderMode == RM_SOLID || renderMode == RM_BOTH) {
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (renderMode == RM_BOTH) {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.5f, 1.5f);
        }
        setMaterial(currentFig);
        drawFigure(currentFig, false);
        if (renderMode == RM_BOTH)
            glDisable(GL_POLYGON_OFFSET_FILL);
    }

    // ── Render alambre ────────────────────────────────────────────────────
    if (renderMode == RM_WIRE || renderMode == RM_BOTH) {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (renderMode == RM_BOTH) {
            glColor3f(0.05f, 0.05f, 0.08f);
            glLineWidth(1.2f);
        } else {
            const float* c = mats[currentFig].dif;
            glColor3f(c[0], c[1], c[2]);
            glLineWidth(1.4f);
        }
        drawFigure(currentFig, true);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();

    drawHUD();
    glutSwapBuffers();
}

// ─────────────────────────────────────────────────────────────────────────────
//  RESHAPE
// ─────────────────────────────────────────────────────────────────────────────
static void reshape(int w, int h)
{
    winW=w; winH=h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// ─────────────────────────────────────────────────────────────────────────────
//  TECLADO — Transformaciones afines por figura
// ─────────────────────────────────────────────────────────────────────────────
static void keyboard(unsigned char key, int, int)
{
    AffineTransform& tf = figTransforms[currentFig];

    switch (key) {
        // ── Sistema / Navegacion ─────────────────────────────────────────
        case 27: exit(0);
        case ' ':
        case 'v': case 'V':
            currentFig = (Figure)((currentFig + 1) % FIG_COUNT); break;
        case 'b': case 'B':
            currentFig = (Figure)((currentFig + FIG_COUNT - 1) % FIG_COUNT); break;
        case '\t':
            renderMode = (RenderMode)((renderMode + 1) % 3); break;
        case 'g': case 'G': showGrid=!showGrid; break;
        case '1': currentFig=FIG_PIRAMIDE_HEX; break;
        case '2': currentFig=FIG_PRISMA_HEX;   break;
        case '3': currentFig=FIG_OCTAEDRO;     break;
        case '4': currentFig=FIG_ICOSAEDRO;    break;
        case '5': currentFig=FIG_DODECAEDRO;   break;
        case '6': currentFig=FIG_DONA;         break;

        // ── ROTACION ─────────────────────────────────────────────────────
        // Eje X
        case 'X': tf.rotX += ROT_STEP; break;   // Rx — positivo
        case 'x': tf.rotX -= ROT_STEP; break;   // rx — negativo
        // Eje Y
        case 'Y': tf.rotY += ROT_STEP; break;   // Ry — positivo
        case 'y': tf.rotY -= ROT_STEP; break;   // ry — negativo
        // Eje Z
        case 'Z': tf.rotZ += ROT_STEP; break;   // Rz — positivo
        case 'z': tf.rotZ -= ROT_STEP; break;   // rz — negativo

        // ── TRASLACION ───────────────────────────────────────────────────
        // Eje X
        case 'T': tf.transX += TRANS_STEP; break;  // Tx — positivo (usa Shift+T = 'T')
        case 't': tf.transX -= TRANS_STEP; break;  // tx — negativo

        // Nota: para Ty y Tz usamos combinaciones alternativas
        // Eje Y: U positivo, u negativo
        case 'U': tf.transY += TRANS_STEP; break;  // Ty — positivo
        case 'u': tf.transY -= TRANS_STEP; break;  // ty — negativo
        // Eje Z: W positivo, w negativo
        case 'W': tf.transZ += TRANS_STEP; break;  // Tz — positivo
        case 'w': tf.transZ -= TRANS_STEP; break;  // tz — negativo

        // ── ESCALAMIENTO ─────────────────────────────────────────────────
        case 'E': tf.scale += SCALE_STEP; break;   // E — aumentar
        case 'e':                                   // e — disminuir (minimo 0.05)
            tf.scale -= SCALE_STEP;
            if (tf.scale < 0.05f) tf.scale = 0.05f;
            break;

        // ── SESGO (SHEAR) ─────────────────────────────────────────────────
        case 'H': tf.shearX += SHEAR_STEP; break;  // H — sesgo X positivo
        case 'h': tf.shearX -= SHEAR_STEP; break;  // h — sesgo X negativo
        case 'J': tf.shearY += SHEAR_STEP; break;  // J — sesgo Y positivo
        case 'j': tf.shearY -= SHEAR_STEP; break;  // j — sesgo Y negativo

        // ── RESET de figura activa ────────────────────────────────────────
        case 'R': tf.reset(); break;
    }
    glutPostRedisplay();
}

// ─────────────────────────────────────────────────────────────────────────────
//  RATON — rotacion de camara y zoom
// ─────────────────────────────────────────────────────────────────────────────
static void mouse(int button, int state, int x, int y)
{
    if (button==GLUT_LEFT_BUTTON) {
        dragging=(state==GLUT_DOWN);
        lastMX=x; lastMY=y;
    }
    if (button==3) { zoom*=1.08f; glutPostRedisplay(); }
    if (button==4) { zoom/=1.08f; if(zoom<0.05f)zoom=0.05f; glutPostRedisplay(); }
}

static void motion(int x, int y)
{
    if (!dragging) return;
    camRotY += (x-lastMX)*0.5f;
    camRotX += (y-lastMY)*0.5f;
    lastMX=x; lastMY=y;
    glutPostRedisplay();
}

static void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Figuras 3D - Transformaciones Afines");

    glClearColor(0.11f, 0.14f, 0.19f, 1.0f);

    // ── Iluminacion: 3 luces ──────────────────────────────────────────────
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    GLfloat l0pos[] = { 3.0f, 5.0f, 6.0f, 1.0f };
    GLfloat l0amb[] = { 0.05f,0.05f,0.08f,1.0f };
    GLfloat l0dif[] = { 0.90f,0.90f,1.00f,1.0f };
    GLfloat l0spc[] = { 1.00f,1.00f,1.00f,1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  l0amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  l0dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);

    glEnable(GL_LIGHT1);
    GLfloat l1pos[] = {-5.0f, 2.0f, 1.0f, 1.0f };
    GLfloat l1dif[] = { 0.30f,0.35f,0.55f,1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, l1pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  l1dif);

    glEnable(GL_LIGHT2);
    GLfloat l2pos[] = { 0.0f,-3.0f,-5.0f, 1.0f };
    GLfloat l2dif[] = { 0.20f,0.22f,0.35f,1.0f };
    GLfloat l2spc[] = { 0.60f,0.60f,0.70f,1.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, l2pos);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  l2dif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, l2spc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
