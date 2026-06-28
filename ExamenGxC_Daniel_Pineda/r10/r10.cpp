/*
 * =====================================================================
 *  FRACTAL DE JULIA 3D
 *  Quaternion Julia Set renderizado como nube de puntos 3D
 *  Colores: verdes, azules, morados y rojos
 *
 *  Compilar Windows/MinGW:
 *    g++ julia3d.cpp -o julia3d.exe -lfreeglut -lopengl32 -lglu32
 *  Compilar Linux:
 *    g++ julia3d.cpp -o julia3d -lGL -lGLU -lglut -lm
 *
 *  Controles:
 *    Arrastrar raton    -> Rotar vista
 *    Rueda raton        -> Zoom in/out
 *    G                  -> Cuadricula on/off
 *    R                  -> Restablecer posicion
 *    L                  -> Cambiar tipo de luz (Global / Puntual)
 *    A / S              -> Animar parametro C del fractal
 *    ESC                -> Salir
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
#include <string>
#include <vector>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

// ── Ventana ───────────────────────────────────────────────────────────────────
static int   winW = 1280, winH = 760;
static float camRotX = 18.0f, camRotY = -30.0f;
static float zoom = 1.0f;
static int   lastMX = 0, lastMY = 0;
static bool  dragging = false;
static bool  showGrid = true;

// ── Tipo de luz ───────────────────────────────────────────────────────────────
static bool  usePuntual = false;
static float lightAngle = 0.0f;
static float lightHeight = 3.5f;
static float lightPX = 0.0f, lightPY = lightHeight, lightPZ = 2.5f;

// ── Definiciones ──────────────────────────────────────────────────────────────
static std::string defGlobal  = "Luz global: ilumina el fractal de forma uniforme, revelando su estructura tridimensional completa.";
static std::string defPuntual = "Luz puntual: foco dinamico que orbita el fractal, resaltando las aristas y profundidad del conjunto de Julia.";

// ── Fractal de Julia cuaternionico ────────────────────────────────────────────
// Parametro C del conjunto de Julia (cuaternion)
static float juliaC[4] = { -0.2f, 0.6f, 0.2f, 0.2f };
static float juliaAnim = 0.0f;
static bool  animating = false;

// Nube de puntos del fractal
struct FractalPoint {
    float x, y, z;
    float r, g, b;
    float iter_norm; // iteracion normalizada [0,1]
};

static std::vector<FractalPoint> fractalCloud;
static bool fractalReady = false;

// ─────────────────────────────────────────────────────────────────────────────
//  CUATERNION HELPERS
// ─────────────────────────────────────────────────────────────────────────────
struct Quat { float x, y, z, w; };

static Quat qmul(Quat a, Quat b)
{
    return {
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
    };
}

static float qnorm2(Quat q)
{
    return q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
}

// Iteracion del fractal: q -> q^2 + c, retorna iteracion de escape (0..maxIter)
static int juliaIter(float px, float py, float pz, float pw,
                     float cx, float cy, float cz, float cw,
                     int maxIter, float bailout2)
{
    Quat q = {px, py, pz, pw};
    Quat c = {cx, cy, cz, cw};
    for (int i = 0; i < maxIter; i++) {
        q = qmul(q, q);
        q.x += c.x; q.y += c.y; q.z += c.z; q.w += c.w;
        if (qnorm2(q) > bailout2) return i;
    }
    return maxIter;
}

// ─────────────────────────────────────────────────────────────────────────────
//  PALETA DE COLORES: verdes, azules, morados, rojos
// ─────────────────────────────────────────────────────────────────────────────
static void paletaColor(float t, float& r, float& g, float& b)
{
    // t en [0,1]
    // Gradiente ciclico: rojo -> morado -> azul -> verde -> azul-verde
    float t4 = t * 4.0f;
    int seg = (int)t4 % 4;
    float f = t4 - (int)t4;

    if (seg == 0) {
        // rojo brillante -> morado
        r = 0.9f - 0.1f*f;
        g = 0.05f*f;
        b = 0.1f + 0.6f*f;
    } else if (seg == 1) {
        // morado -> azul
        r = 0.8f - 0.7f*f;
        g = 0.05f + 0.05f*f;
        b = 0.7f + 0.2f*f;
    } else if (seg == 2) {
        // azul -> verde-azul
        r = 0.1f - 0.05f*f;
        g = 0.1f + 0.75f*f;
        b = 0.9f - 0.2f*f;
    } else {
        // verde-azul -> rojo
        r = 0.05f + 0.85f*f;
        g = 0.85f - 0.80f*f;
        b = 0.7f - 0.6f*f;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  GENERAR NUBE DE PUNTOS DEL FRACTAL DE JULIA
// ─────────────────────────────────────────────────────────────────────────────
static void buildJuliaCloud()
{
    fractalCloud.clear();

    const int GRID   = 58;     // resolucion de la cuadricula 3D
    const int MAXITER = 12;
    const float BAILOUT2 = 4.0f;
    const float RANGE  = 1.5f;

    float cx = juliaC[0], cy = juliaC[1], cz = juliaC[2], cw = juliaC[3];

    for (int ix = 0; ix < GRID; ix++)
    for (int iy = 0; iy < GRID; iy++)
    for (int iz = 0; iz < GRID; iz++)
    {
        float px = -RANGE + 2.0f*RANGE*ix/(GRID-1);
        float py = -RANGE + 2.0f*RANGE*iy/(GRID-1);
        float pz = -RANGE + 2.0f*RANGE*iz/(GRID-1);

        // Solo muestrear puntos cerca de la superficie del fractal
        // haciendo dos resoluciones: interior y exterior
        int it = juliaIter(px, py, pz, 0.0f, cx, cy, cz, cw, MAXITER, BAILOUT2);

        // Solo puntos en la frontera (no muy adentro, no muy afuera)
        if (it == MAXITER || it == 0) continue;

        float t = (float)it / MAXITER;

        FractalPoint p;
        p.x = px * 1.25f;
        p.y = py * 1.25f;
        p.z = pz * 1.25f;
        p.iter_norm = t;
        paletaColor(t, p.r, p.g, p.b);
        fractalCloud.push_back(p);
    }

    fractalReady = true;
}

// ─────────────────────────────────────────────────────────────────────────────
//  DIBUJAR FRACTAL
// ─────────────────────────────────────────────────────────────────────────────
static void drawJuliaFractal()
{
    if (!fractalReady || fractalCloud.empty()) return;

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Puntos con iluminacion aproximada: el material depende del color
    glDisable(GL_LIGHTING);

    glPointSize(2.8f);
    glBegin(GL_POINTS);
    for (const auto& p : fractalCloud) {
        // Modulamos alpha segun capa: puntos mas interiores mas opacos
        float alpha = 0.55f + 0.45f * p.iter_norm;
        glColor4f(p.r, p.g, p.b, alpha);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    // Segunda pasada: puntos mas grandes y brillantes para el borde exterior
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (const auto& p : fractalCloud) {
        if (p.iter_norm > 0.7f) {
            float bright = (p.iter_norm - 0.7f) / 0.3f;
            glColor4f(
                p.r * 0.5f + 0.5f * bright,
                p.g * 0.5f + 0.5f * bright,
                p.b * 0.5f + 0.5f * bright,
                0.35f * bright
            );
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();

    glPointSize(1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// ─────────────────────────────────────────────────────────────────────────────
//  MATERIALES
// ─────────────────────────────────────────────────────────────────────────────
static void setMat(float ar,float ag,float ab,
                   float dr,float dg,float db,
                   float sr,float sg,float sb,
                   float shine)
{
    GLfloat amb[]={ar,ag,ab,1.f};
    GLfloat dif[]={dr,dg,db,1.f};
    GLfloat spc[]={sr,sg,sb,1.f};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  amb);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  dif);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, spc);
    glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,shine);
}

// ─────────────────────────────────────────────────────────────────────────────
//  CUADRICULA
// ─────────────────────────────────────────────────────────────────────────────
static void drawGrid()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.20f,0.28f,0.38f);
    glLineWidth(1.0f);
    float ext=4.0f; int n=16;
    glBegin(GL_LINES);
    for(int i=-n;i<=n;i++){
        float t=ext*i/n;
        glVertex3f(t,-1.6f,-ext); glVertex3f(t,-1.6f, ext);
        glVertex3f(-ext,-1.6f,t); glVertex3f( ext,-1.6f,t);
    }
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINES);
      glColor3f(0.85f,0.28f,0.28f);
      glVertex3f(0,-1.6f,0); glVertex3f(2.0f,-1.6f,0);
      glColor3f(0.28f,0.85f,0.28f);
      glVertex3f(0,-1.6f,0); glVertex3f(0,1.2f,0);
      glColor3f(0.28f,0.45f,0.90f);
      glVertex3f(0,-1.6f,0); glVertex3f(0,-1.6f,2.0f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

// ─────────────────────────────────────────────────────────────────────────────
//  CONFIGURAR LUCES
// ─────────────────────────────────────────────────────────────────────────────
static void setupLights()
{
    if (!usePuntual)
    {
        GLfloat globalAmb[] = {0.30f, 0.30f, 0.30f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

        glEnable(GL_LIGHT0);
        GLfloat pos0[] = {0.0f, 8.0f, 4.0f, 0.0f};
        GLfloat amb0[] = {0.20f,0.20f,0.20f,1.0f};
        GLfloat dif0[] = {0.75f,0.75f,0.75f,1.0f};
        GLfloat spc0[] = {0.40f,0.40f,0.40f,1.0f};
        glLightfv(GL_LIGHT0,GL_POSITION,pos0);
        glLightfv(GL_LIGHT0,GL_AMBIENT, amb0);
        glLightfv(GL_LIGHT0,GL_DIFFUSE, dif0);
        glLightfv(GL_LIGHT0,GL_SPECULAR,spc0);
        glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0f);
        glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.0f);
        glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0f);

        glEnable(GL_LIGHT1);
        GLfloat pos1[] = {-6.0f, 3.0f, 2.0f, 0.0f};
        GLfloat dif1[] = {0.45f,0.45f,0.50f,1.0f};
        GLfloat spc1[] = {0.10f,0.10f,0.12f,1.0f};
        glLightfv(GL_LIGHT1,GL_POSITION,pos1);
        glLightfv(GL_LIGHT1,GL_AMBIENT, (GLfloat[]){0,0,0,1});
        glLightfv(GL_LIGHT1,GL_DIFFUSE, dif1);
        glLightfv(GL_LIGHT1,GL_SPECULAR,spc1);
        glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1.0f);
        glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.0f);
        glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.0f);

        glEnable(GL_LIGHT2);
        GLfloat pos2[] = {2.0f, 5.0f, -8.0f, 0.0f};
        GLfloat dif2[] = {0.35f,0.35f,0.40f,1.0f};
        GLfloat spc2[] = {0.08f,0.08f,0.10f,1.0f};
        glLightfv(GL_LIGHT2,GL_POSITION,pos2);
        glLightfv(GL_LIGHT2,GL_AMBIENT, (GLfloat[]){0,0,0,1});
        glLightfv(GL_LIGHT2,GL_DIFFUSE, dif2);
        glLightfv(GL_LIGHT2,GL_SPECULAR,spc2);
        glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION,1.0f);
        glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,0.0f);
        glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0.0f);

        glDisable(GL_LIGHT3);
    }
    else
    {
        GLfloat globalAmb[] = {0.04f, 0.04f, 0.06f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);

        glEnable(GL_LIGHT0);
        GLfloat pos0[] = {lightPX, lightPY, lightPZ, 1.0f};
        GLfloat amb0[] = {0.02f,0.02f,0.03f,1.0f};
        GLfloat dif0[] = {1.00f,0.95f,0.80f,1.0f};
        GLfloat spc0[] = {1.00f,1.00f,0.95f,1.0f};
        glLightfv(GL_LIGHT0,GL_POSITION,pos0);
        glLightfv(GL_LIGHT0,GL_AMBIENT, amb0);
        glLightfv(GL_LIGHT0,GL_DIFFUSE, dif0);
        glLightfv(GL_LIGHT0,GL_SPECULAR,spc0);
        glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION, 0.5f);
        glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,   0.15f);
        glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.02f);

        glEnable(GL_LIGHT3);
        GLfloat pos3[] = {-3.0f, 2.0f, -2.0f, 1.0f};
        GLfloat dif3[] = {0.10f,0.10f,0.14f,1.0f};
        GLfloat spc3[] = {0.02f,0.02f,0.03f,1.0f};
        glLightfv(GL_LIGHT3,GL_POSITION,pos3);
        glLightfv(GL_LIGHT3,GL_AMBIENT, (GLfloat[]){0,0,0,1});
        glLightfv(GL_LIGHT3,GL_DIFFUSE, dif3);
        glLightfv(GL_LIGHT3,GL_SPECULAR,spc3);
        glLightf(GL_LIGHT3,GL_CONSTANT_ATTENUATION,1.0f);
        glLightf(GL_LIGHT3,GL_LINEAR_ATTENUATION,0.12f);
        glLightf(GL_LIGHT3,GL_QUADRATIC_ATTENUATION,0.01f);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  ESFERA INDICADORA DE LUZ PUNTUAL
// ─────────────────────────────────────────────────────────────────────────────
static void drawLightBulb()
{
    if (!usePuntual) return;
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
      glTranslatef(lightPX, lightPY, lightPZ);
      glColor3f(1.0f, 0.95f, 0.60f);
      glutSolidSphere(0.10, 12, 8);
      glColor4f(1.0f,0.95f,0.60f,0.5f);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glLineWidth(1.5f);
      glBegin(GL_LINES);
      for(int i=0;i<12;i++){
          float a=2*(float)M_PI*i/12;
          float dx=cosf(a)*0.25f, dz=sinf(a)*0.25f;
          glVertex3f(0,0,0); glVertex3f(dx,-0.12f,dz);
      }
      glEnd();
      glDisable(GL_BLEND);
      glLineWidth(1.0f);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// ─────────────────────────────────────────────────────────────────────────────
//  TEXTO 2D
// ─────────────────────────────────────────────────────────────────────────────
static void drawText(float x, float y, const char* s, void* font)
{
    glRasterPos2f(x, y);
    for(const char* c=s; *c; c++) glutBitmapCharacter(font, *c);
}

// ─────────────────────────────────────────────────────────────────────────────
//  HUD
// ─────────────────────────────────────────────────────────────────────────────
static void drawHUD()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    // ── Titulo ────────────────────────────────────────────────────────────
    glColor3f(0.85f,0.93f,1.0f);
    drawText(14, winH-36, "FRACTAL DE JULIA 3D", GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.55f,0.75f,1.0f);
    drawText(14, winH-56,
             "Conjunto de Julia cuaternionico  (Nube de puntos volumetrica)",
             GLUT_BITMAP_HELVETICA_12);

    // ── Panel definicion activa ────────────────────────────────────────────
    const std::string& defActiva = usePuntual ? defPuntual : defGlobal;
    float panX = 14.0f;
    float panW = winW - 28.0f;
    float lineH   = 16.0f;
    float padV    = 6.0f;
    float panH    = lineH * 2 + padV * 2;
    float panY    = winH - 90.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    if (!usePuntual)
        glColor4f(0.08f,0.14f,0.22f,0.92f);
    else
        glColor4f(0.18f,0.12f,0.04f,0.92f);
    glBegin(GL_QUADS);
      glVertex2f(panX-4, panY-panH); glVertex2f(panX+panW, panY-panH);
      glVertex2f(panX+panW, panY+padV); glVertex2f(panX-4, panY+padV);
    glEnd();
    glDisable(GL_BLEND);

    if (!usePuntual)
        glColor3f(0.28f,0.55f,0.90f);
    else
        glColor3f(0.90f,0.65f,0.15f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(panX-4, panY-panH); glVertex2f(panX+panW, panY-panH);
      glVertex2f(panX+panW, panY+padV); glVertex2f(panX-4, panY+padV);
    glEnd();

    const char* modoStr = usePuntual ? "[ LUZ PUNTUAL ]" : "[ LUZ GLOBAL ]";
    if (!usePuntual)
        glColor3f(0.50f,0.85f,1.00f);
    else
        glColor3f(1.00f,0.82f,0.25f);
    drawText(panX+4, panY+2, modoStr, GLUT_BITMAP_HELVETICA_12);

    glColor3f(0.88f,0.92f,0.96f);
    drawText(panX+4, panY+2-lineH, defActiva.c_str(), GLUT_BITMAP_HELVETICA_12);

    // ── Panel de controles (izquierda) ─────────────────────────────────────
    const float ctrlBottom = 44.0f;
    const float ctrlTop    = 330.0f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f,0.10f,0.14f,0.84f);
    glBegin(GL_QUADS);
      glVertex2f(12,ctrlBottom); glVertex2f(310,ctrlBottom);
      glVertex2f(310,ctrlTop);   glVertex2f(12,ctrlTop);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.28f,0.50f,0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(12,ctrlBottom); glVertex2f(310,ctrlBottom);
      glVertex2f(310,ctrlTop);   glVertex2f(12,ctrlTop);
    glEnd();

    glColor3f(0.72f,0.88f,1.0f);
    drawText(18, ctrlTop-18, "CONTROLES", GLUT_BITMAP_HELVETICA_12);

    struct CtrlRow { const char* k; const char* d; bool header; };
    static const CtrlRow ctrl[] = {
        {"--- VISTA -------------------","",true},
        {"Arrastrar raton", "Rotar camara 3D",false},
        {"Rueda raton",     "Zoom in / out",false},
        {"G",               "Cuadricula on/off",false},
        {"R",               "Restablecer camara",false},
        {"--- LUZ --------------------","",true},
        {"L",               "Cambiar: Global / Puntual",false},
        {"--- FRACTAL ----------------","",true},
        {"A",               "Animar parametro C (on/off)",false},
        {"S",               "Siguiente preset de Julia",false},
        {"ESC",             "Salir",false},
    };
    int cy=(int)(ctrlTop-36);
    for(auto& cr:ctrl){
        if(cr.header){
            glColor3f(0.45f,0.68f,0.35f);
            drawText(18,(float)cy,cr.k,GLUT_BITMAP_HELVETICA_12);
        } else {
            char line[128];
            snprintf(line,sizeof(line),"  %-18s%s",cr.k,cr.d);
            glColor3f(0.55f,0.72f,0.92f);
            drawText(18,(float)cy,line,GLUT_BITMAP_HELVETICA_12);
        }
        cy -= 14;
    }

    // ── Panel info fractal (derecha) ──────────────────────────────────────
    float ipX = winW - 235.0f, ipY = winH - 110.0f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.06f,0.09f,0.13f,0.88f);
    glBegin(GL_QUADS);
      glVertex2f(ipX-8,ipY-112); glVertex2f((float)winW-8,ipY-112);
      glVertex2f((float)winW-8,ipY+14); glVertex2f(ipX-8,ipY+14);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.28f,0.50f,0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(ipX-8,ipY-112); glVertex2f((float)winW-8,ipY-112);
      glVertex2f((float)winW-8,ipY+14); glVertex2f(ipX-8,ipY+14);
    glEnd();

    glColor3f(0.72f,0.88f,1.0f);
    drawText(ipX, ipY, "FRACTAL EN ESCENA", GLUT_BITMAP_HELVETICA_12);

    char buf[128];
    float oy = ipY - 20;
    glColor3f(0.55f,0.80f,1.0f);
    drawText(ipX, oy, "Julia Cuaternionico 3D", GLUT_BITMAP_HELVETICA_12); oy-=13;
    glColor3f(0.40f,0.60f,0.80f);
    snprintf(buf,sizeof(buf),"  Cx=%.3f  Cy=%.3f", juliaC[0], juliaC[1]);
    drawText(ipX, oy, buf, GLUT_BITMAP_HELVETICA_12); oy-=13;
    snprintf(buf,sizeof(buf),"  Cz=%.3f  Cw=%.3f", juliaC[2], juliaC[3]);
    drawText(ipX, oy, buf, GLUT_BITMAP_HELVETICA_12); oy-=16;

    glColor3f(0.60f,0.40f,0.90f);
    drawText(ipX, oy, "Paleta:", GLUT_BITMAP_HELVETICA_12); oy-=13;

    // Mini barra de color paleta
    int bw = (int)(winW - 16 - (int)(ipX));
    float bx0 = ipX, bx1 = bx0 + bw;
    float by0 = oy - 8, by1 = oy + 2;
    glDisable(GL_LIGHTING);
    glBegin(GL_QUAD_STRIP);
    int STEPS = 32;
    for (int i = 0; i <= STEPS; i++) {
        float t = (float)i / STEPS;
        float cr, cg, cb;
        paletaColor(t, cr, cg, cb);
        glColor3f(cr, cg, cb);
        float bx = bx0 + t * (bx1 - bx0);
        glVertex2f(bx, by0);
        glVertex2f(bx, by1);
    }
    glEnd();
    oy -= 22;

    glColor3f(0.35f,0.90f,0.50f);
    snprintf(buf,sizeof(buf),"Puntos: %d", (int)fractalCloud.size());
    drawText(ipX, oy, buf, GLUT_BITMAP_HELVETICA_12); oy-=13;
    glColor3f(0.70f,0.70f,0.70f);
    drawText(ipX, oy, animating ? "  Animando C..." : "  [A] para animar",
             GLUT_BITMAP_HELVETICA_12);

    // ── Barra inferior: selector de luz ───────────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f,0.10f,0.14f,0.80f);
    glBegin(GL_QUADS);
      glVertex2f(0,0); glVertex2f((float)winW,0);
      glVertex2f((float)winW,40); glVertex2f(0,40);
    glEnd();
    glDisable(GL_BLEND);

    float halfW = winW * 0.5f;
    // Izquierda: GLOBAL
    {
        bool active = !usePuntual;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.10f, 0.20f, 0.45f, active ? 0.90f : 0.35f);
        glBegin(GL_QUADS);
          glVertex2f(2,2); glVertex2f(halfW-2,2);
          glVertex2f(halfW-2,38); glVertex2f(2,38);
        glEnd();
        glDisable(GL_BLEND);
        if(active){
            glColor3f(0.50f,0.80f,1.00f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
              glVertex2f(2,2); glVertex2f(halfW-2,2);
              glVertex2f(halfW-2,38); glVertex2f(2,38);
            glEnd();
            glLineWidth(1.0f);
        }
        glColor3f(active?1.0f:0.50f, active?1.0f:0.60f, active?1.0f:0.65f);
        const char* lbl = active ? ">>> LUZ GLOBAL  [L para cambiar] <<<" : "    LUZ GLOBAL";
        int tw=0; for(const char* c=lbl;*c;c++) tw+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_12,*c);
        drawText(halfW*0.5f - tw*0.5f, 14, lbl, GLUT_BITMAP_HELVETICA_12);
    }
    // Derecha: PUNTUAL
    {
        bool active = usePuntual;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.45f, 0.28f, 0.04f, active ? 0.90f : 0.35f);
        glBegin(GL_QUADS);
          glVertex2f(halfW+2,2); glVertex2f((float)winW-2,2);
          glVertex2f((float)winW-2,38); glVertex2f(halfW+2,38);
        glEnd();
        glDisable(GL_BLEND);
        if(active){
            glColor3f(1.00f,0.80f,0.25f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
              glVertex2f(halfW+2,2); glVertex2f((float)winW-2,2);
              glVertex2f((float)winW-2,38); glVertex2f(halfW+2,38);
            glEnd();
            glLineWidth(1.0f);
        }
        glColor3f(active?1.0f:0.60f, active?0.85f:0.55f, active?0.30f:0.30f);
        const char* lbl = active ? ">>> LUZ PUNTUAL  [L para cambiar] <<<" : "    LUZ PUNTUAL";
        int tw=0; for(const char* c=lbl;*c;c++) tw+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_12,*c);
        float cx = halfW + (winW - halfW)*0.5f;
        drawText(cx - tw*0.5f, 14, lbl, GLUT_BITMAP_HELVETICA_12);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
}

// ─────────────────────────────────────────────────────────────────────────────
//  DISPLAY
// ─────────────────────────────────────────────────────────────────────────────
static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1.5, 6.5/zoom,  0, -0.3, 0,  0, 1, 0);
    glRotatef(camRotX, 1, 0, 0);
    glRotatef(camRotY, 0, 1, 0);

    setupLights();

    if (showGrid) drawGrid();

    // ── Fractal de Julia 3D ───────────────────────────────────────────────
    glPushMatrix();
      // Centrar fractal ligeramente elevado sobre la cuadricula
      glTranslatef(0.0f, -0.15f, 0.0f);
      drawJuliaFractal();
    glPopMatrix();

    drawLightBulb();
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
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(40.0, (double)w/h, 0.05, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

// ─────────────────────────────────────────────────────────────────────────────
//  PRESETS DEL FRACTAL
// ─────────────────────────────────────────────────────────────────────────────
static const float juliaPresets[][4] = {
    { -0.2f,  0.6f,  0.2f,  0.2f },
    { -0.4f,  0.5f,  0.0f,  0.0f },
    {  0.1f,  0.7f, -0.3f,  0.1f },
    { -0.6f,  0.4f,  0.3f, -0.2f },
    {  0.0f,  0.8f,  0.0f, -0.2f },
    { -0.3f,  0.5f,  0.4f,  0.3f },
};
static int presetIdx = 0;
static const int NUM_PRESETS = 6;

// ─────────────────────────────────────────────────────────────────────────────
//  TECLADO
// ─────────────────────────────────────────────────────────────────────────────
static void keyboard(unsigned char key, int, int)
{
    switch(key)
    {
    case 27: exit(0);
    case 'g': case 'G': showGrid = !showGrid; break;
    case 'r': case 'R':
        camRotX=18.0f; camRotY=-30.0f; zoom=1.0f; break;
    case 'l': case 'L':
        usePuntual = !usePuntual;
        break;
    case 'a': case 'A':
        animating = !animating;
        break;
    case 's': case 'S':
        presetIdx = (presetIdx + 1) % NUM_PRESETS;
        juliaC[0] = juliaPresets[presetIdx][0];
        juliaC[1] = juliaPresets[presetIdx][1];
        juliaC[2] = juliaPresets[presetIdx][2];
        juliaC[3] = juliaPresets[presetIdx][3];
        buildJuliaCloud();
        break;
    }
    glutPostRedisplay();
}

// ─────────────────────────────────────────────────────────────────────────────
//  RATON
// ─────────────────────────────────────────────────────────────────────────────
static void mouse(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON){ dragging=(state==GLUT_DOWN); lastMX=x; lastMY=y; }
    if(button==3){ zoom*=1.08f; glutPostRedisplay(); }
    if(button==4){ zoom/=1.08f; if(zoom<0.1f)zoom=0.1f; glutPostRedisplay(); }
}
static void motion(int x, int y)
{
    if(!dragging) return;
    camRotY += (x-lastMX)*0.5f;
    camRotX += (y-lastMY)*0.5f;
    lastMX=x; lastMY=y;
    glutPostRedisplay();
}

// ─────────────────────────────────────────────────────────────────────────────
//  TIMER
// ─────────────────────────────────────────────────────────────────────────────
static int animFrame = 0;
static void timer(int)
{
    // Animar luz puntual
    if(usePuntual)
    {
        lightAngle += 0.008f;
        lightPX = 3.0f * cosf(lightAngle);
        lightPZ = 3.0f * sinf(lightAngle);
        lightPY = lightHeight;
    }

    // Animar parametro C del fractal
    if (animating) {
        animFrame++;
        if (animFrame % 6 == 0) {  // recalcular cada 6 frames (~10fps para el fractal)
            juliaAnim += 0.04f;
            juliaC[0] = juliaPresets[presetIdx][0] + 0.18f * sinf(juliaAnim * 0.7f);
            juliaC[1] = juliaPresets[presetIdx][1] + 0.12f * cosf(juliaAnim * 0.5f);
            juliaC[2] = juliaPresets[presetIdx][2] + 0.10f * sinf(juliaAnim * 0.9f);
            buildJuliaCloud();
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Fractal de Julia 3D");

    // Mismo color de fondo oscuro que el original
    glClearColor(0.11f, 0.14f, 0.19f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);  // puntos suavizados

    // Generar nube de puntos inicial
    buildJuliaCloud();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();

    return 0;
}
