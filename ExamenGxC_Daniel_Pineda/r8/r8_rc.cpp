/*
 * =====================================================================
 *  ARTICULACIONES DE UNA MANO
 *  14 articulaciones: 4 dedos x 3 + pulgar x 2
 *
 *  Compilar Windows/MinGW:
 *    g++ r7_mano.cpp -o mano.exe -lfreeglut -lopengl32 -lglu32
 *  Compilar Linux:
 *    g++ r7_mano.cpp -o mano -lGL -lGLU -lglut -lm
 *
 *  Controles:
 *    Arrastrar raton    → Rotar vista
 *    Rueda raton        → Zoom in/out
 *    G                  → Cuadricula
 *    R                  → Restablecer posicion
 *    ESPACIO (mantener) → Cerrar puño (todos los dedos); al soltar se abre
 *    ESC                → Salir
 *
 *  Seleccion y movimiento de articulaciones:
 *    0..4   → Selecciona el dedo (0 Pulgar, 1 Indice, 2 Medio, 3 Anular, 4 Menique)
 *    Q      → Selecciona articulacion MCP (la mas cercana a la palma)
 *    W      → Selecciona articulacion PIP / IP
 *    D      → Selecciona articulacion DIP (no existe en el pulgar)
 *    E      → Extiende  la articulacion seleccionada
 *    C      → Contrae   la articulacion seleccionada
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

// ── Ventana ───────────────────────────────────────────────────────────────────
static int   winW=1280, winH=760;
static float camRotX=20.0f, camRotY=-20.0f;
static float zoom=1.0f;
static int   lastMX=0, lastMY=0;
static bool  dragging=false;
static bool  showGrid=true;

// ── Angulos de articulaciones (grados, 0=extendido, max=doblado hacia palma) ─
// Indices: 0=Pulgar, 1=Indice, 2=Medio, 3=Anular, 4=Menique
// Sub-indices por dedo: Pulgar[0]=MCP [1]=IP
//                       Resto  [0]=MCP [1]=PIP [2]=DIP
static float angles[5][3] = {
    {0,0,0},   // Pulgar  MCP, IP
    {0,0,0},   // Indice  MCP, PIP, DIP
    {0,0,0},   // Medio   MCP, PIP, DIP
    {0,0,0},   // Anular  MCP, PIP, DIP
    {0,0,0}    // Menique MCP, PIP, DIP
};

// Limites maximos por articulacion (hacia la palma)
static const float maxAngle[5][3] = {
    {70, 80, 0},    // Pulgar
    {90, 110, 80},  // Indice
    {90, 110, 80},  // Medio
    {90, 110, 80},  // Anular
    {90, 110, 80}   // Menique
};

static const float STEP = 5.0f;  // grados por pulsacion

// ── Seleccion activa para controlar articulaciones con teclado ──────────────
// selFinger: 0=Pulgar,1=Indice,2=Medio,3=Anular,4=Menique
// selJoint : 0=MCP, 1=PIP/IP, 2=DIP (el pulgar solo tiene 0 y 1)
static int  selFinger = 1;
static int  selJoint  = 0;
static const char* fingerNames[5] = {"PULGAR","INDICE","MEDIO","ANULAR","MENIQUE"};
static const char* jointNamesLong[3] = {"MCP","PIP","DIP"};
static const char* jointNamesThumb[2] = {"MCP","IP"};

static int jointCount(int fi){ return (fi==0)?2:3; }

static const char* currentJointName(){
    if(selFinger==0) return jointNamesThumb[selJoint];
    return jointNamesLong[selJoint];
}

// ── Utilidades de color / material ───────────────────────────────────────────
static void setSkinMaterial()
{
    // Piel humana — tono calido con especular suave
    GLfloat amb[] = {0.25f, 0.15f, 0.10f, 1.0f};
    GLfloat dif[] = {0.88f, 0.62f, 0.45f, 1.0f};
    GLfloat spc[] = {0.35f, 0.25f, 0.20f, 1.0f};
    GLfloat emi[] = {0.03f, 0.02f, 0.01f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,   amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  spc);
    glMaterialfv(GL_FRONT, GL_EMISSION,  emi);
    glMaterialf (GL_FRONT, GL_SHININESS, 18.0f);
}

static void setNailMaterial()
{
    GLfloat amb[] = {0.30f, 0.22f, 0.20f, 1.0f};
    GLfloat dif[] = {0.80f, 0.70f, 0.65f, 1.0f};
    GLfloat spc[] = {0.70f, 0.65f, 0.60f, 1.0f};
    GLfloat emi[] = {0.02f, 0.02f, 0.02f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,   amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  spc);
    glMaterialfv(GL_FRONT, GL_EMISSION,  emi);
    glMaterialf (GL_FRONT, GL_SHININESS, 60.0f);
}

static void setPalmMaterial()
{
    GLfloat amb[] = {0.22f, 0.12f, 0.08f, 1.0f};
    GLfloat dif[] = {0.82f, 0.58f, 0.40f, 1.0f};
    GLfloat spc[] = {0.20f, 0.14f, 0.10f, 1.0f};
    GLfloat emi[] = {0.02f, 0.01f, 0.01f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,   amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  spc);
    glMaterialfv(GL_FRONT, GL_EMISSION,  emi);
    glMaterialf (GL_FRONT, GL_SHININESS, 8.0f);
}

// ── Cilindro redondeado (falange) ─────────────────────────────────────────────
// Dibuja un cilindro con tapas esfericas en ambos extremos usando GLU + GLUT
static GLUquadric* gQuad = nullptr;

static void drawRoundedCylinder(float radius, float length, int slices=20)
{
    // Cuerpo cilindrico
    gluCylinder(gQuad, radius, radius, length, slices, 4);
    // Tapa inferior (esfera media)
    glutSolidSphere(radius, slices, slices/2);
    // Tapa superior
    glPushMatrix();
      glTranslatef(0,0,length);
      glutSolidSphere(radius, slices, slices/2);
    glPopMatrix();
}

// Dibuja una uña plana sobre la falange distal
static void drawNail(float radius, float length)
{
    setNailMaterial();
    glPushMatrix();
      // Posicionar encima de la falange
      glTranslatef(0, radius*0.7f, length*0.15f);
      glScalef(radius*0.85f, radius*0.18f, length*0.65f);
      // Cuadricula curva aproximada con esfera achatada
      glScalef(1.0f, 0.5f, 1.0f);
      glutSolidSphere(1.0f, 16, 8);
    glPopMatrix();
}

// ── Nudillo esfera ────────────────────────────────────────────────────────────
static void drawKnuckle(float radius)
{
    setSkinMaterial();
    glutSolidSphere(radius*1.08f, 16, 16);
}

// ── Falange individual ────────────────────────────────────────────────────────
// Dibuja la falange y deja el origen en la articulacion DISTAL
static void drawPhalanx(float radius, float length, bool hasnail=false)
{
    setSkinMaterial();
    // Alineamos el cilindro a lo largo de Y local
    glPushMatrix();
      glRotatef(-90,1,0,0);  // cilindro va en Z de GLU, lo rotamos a Y
      drawRoundedCylinder(radius, length);
    glPopMatrix();
    if(hasnail) drawNail(radius, length);
}

// ── DEDO GENERICO (3 falanges) ────────────────────────────────────────────────
// finger: 0=indice,1=medio,2=anular,3=menique  (mapeado a angles[1..4])
// Proporciones: proximal > media > distal
static void drawFinger(int fIdx,
                       float rProx, float lProx,
                       float rMid,  float lMid,
                       float rDist, float lDist)
{
    // MCP — articulacion metacarpofalangica
    drawKnuckle(rProx*1.1f);
    glRotatef(angles[fIdx][0], 1,0,0);
    drawPhalanx(rProx, lProx);
    glTranslatef(0, lProx, 0);

    // PIP — articulacion interfalangica proximal
    drawKnuckle(rMid*1.05f);
    glRotatef(angles[fIdx][1], 1,0,0);
    drawPhalanx(rMid, lMid);
    glTranslatef(0, lMid, 0);

    // DIP — articulacion interfalangica distal
    drawKnuckle(rDist);
    glRotatef(angles[fIdx][2], 1,0,0);
    drawPhalanx(rDist, lDist, true);
}

// ── PULGAR (2 falanges) ───────────────────────────────────────────────────────
static void drawThumb()
{
    float rProx=0.115f, lProx=0.38f;
    float rDist=0.095f, lDist=0.30f;

    // MCP
    drawKnuckle(rProx*1.1f);
    glRotatef(angles[0][0], 1,0,0);
    drawPhalanx(rProx, lProx);
    glTranslatef(0, lProx, 0);

    // IP
    drawKnuckle(rDist*1.05f);
    glRotatef(angles[0][1], 1,0,0);
    drawPhalanx(rDist, lDist, true);
}

// ── MANO COMPLETA ─────────────────────────────────────────────────────────────
static void drawHand()
{
    // Proporciones de dedos (radio, longitud falange prox/med/dist)
    // indices: 1=Indice, 2=Medio, 3=Anular, 4=Menique
    struct FingerDef { float rP,lP, rM,lM, rD,lD; };
    static const FingerDef FD[4] = {
        {0.100f,0.420f, 0.088f,0.320f, 0.075f,0.240f}, // Indice
        {0.108f,0.460f, 0.095f,0.350f, 0.080f,0.260f}, // Medio
        {0.100f,0.440f, 0.088f,0.330f, 0.075f,0.250f}, // Anular
        {0.088f,0.360f, 0.078f,0.270f, 0.068f,0.210f}, // Menique
    };

    // ── PALMA ──────────────────────────────────────────────────────────────
    setPalmMaterial();
    glPushMatrix();
      // Cuerpo principal de la palma: caja redondeada
      glScalef(0.62f, 0.70f, 0.18f);
      glutSolidSphere(1.0f, 32, 24);
    glPopMatrix();

    // Relleno trapezoidal de la palma (volumen)
    setPalmMaterial();
    glPushMatrix();
      glTranslatef(0, 0.05f, 0);
      glScalef(0.58f, 0.52f, 0.15f);
      glutSolidSphere(1.0f, 24, 16);
    glPopMatrix();

    // ── MUÑECA / BASE ──────────────────────────────────────────────────────
    setPalmMaterial();
    glPushMatrix();
      glTranslatef(0, -0.72f, 0);
      glScalef(0.50f, 0.22f, 0.14f);
      glutSolidSphere(1.0f, 20, 12);
    glPopMatrix();

    // ── PULGAR ─────────────────────────────────────────────────────────────
    glPushMatrix();
      // Posicion base del pulgar (lateral izquierdo, inclinado)
      glTranslatef(-0.52f, -0.28f, 0.0f);
      glRotatef(-55, 0,0,1);   // apertura lateral
      glRotatef( 10, 1,0,0);   // ligeramente hacia adelante
      drawThumb();
    glPopMatrix();

    // ── 4 DEDOS LARGOS ─────────────────────────────────────────────────────
    // Offsets X para cada dedo (de izquierda a derecha: indice, medio, anular, menique)
    static const float fingerX[4] = { -0.33f, -0.11f, 0.11f, 0.33f };
    // Altura de salida desde la palma (el menique nace mas bajo)
    static const float fingerY[4] = {  0.60f,  0.64f, 0.60f, 0.50f };
    // Inclinacion de cada dedo
    static const float fingerTilt[4] = { 5.0f, 0.0f, -2.0f, -6.0f };

    for(int i=0;i<4;i++){
        glPushMatrix();
          glTranslatef(fingerX[i], fingerY[i], 0.0f);
          glRotatef(fingerTilt[i], 0,0,1);
          drawFinger(i+1,
                     FD[i].rP, FD[i].lP,
                     FD[i].rM, FD[i].lM,
                     FD[i].rD, FD[i].lD);
        glPopMatrix();
    }

    // ── VENAS / PLIEGUES decorativos en palma ─────────────────────────────
    // Pliegues en los nudillos (surcos oscuros sutiles)
    // Se omiten en esta version para mantener el render limpio
}

// ═════════════════════════════════════════════════════════════════════════════
//  CUADRICULA
// ═════════════════════════════════════════════════════════════════════════════
static void drawGrid()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.20f,0.28f,0.38f);
    glLineWidth(1.0f);
    float ext=3.0f; int n=12;
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

// ═════════════════════════════════════════════════════════════════════════════
//  HUD
// ═════════════════════════════════════════════════════════════════════════════
static void drawText(float x, float y, const char* s, void* font)
{
    glRasterPos2f(x,y);
    for(const char* c=s;*c;c++) glutBitmapCharacter(font,*c);
}

static void drawHUD()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0,winW,0,winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // ── Titulo principal ──────────────────────────────────────────────────
    glColor3f(0.85f,0.93f,1.0f);
    drawText(14, winH-36, "ARTICULACIONES DE UNA MANO",
             GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.55f,0.75f,1.0f);
    drawText(14, winH-58,
             "14 articulaciones  |  Pulgar: 2  |  Indice/Medio/Anular/Menique: 3 c/u",
             GLUT_BITMAP_HELVETICA_12);

    // ── Estado de angulos (panel derecho) ────────────────────────────────
    float px = winW-310.0f, py = winH-80.0f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.06f,0.09f,0.13f,0.88f);
    glBegin(GL_QUADS);
      glVertex2f(px-8,py-180); glVertex2f(winW-8,py-180);
      glVertex2f(winW-8,py+14); glVertex2f(px-8,py+14);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.28f,0.50f,0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(px-8,py-180); glVertex2f(winW-8,py-180);
      glVertex2f(winW-8,py+14); glVertex2f(px-8,py+14);
    glEnd();

    glColor3f(0.72f,0.88f,1.0f);
    drawText(px, py, "ANGULOS DE ARTICULACIONES", GLUT_BITMAP_HELVETICA_12);

    // Barra de progreso auxiliar
    auto drawBar = [&](float bx, float by, float val, float maxv, float r, float g, float b){
        float w=90.0f, h=7.0f;
        float fill=w*(val/maxv);
        glColor3f(0.18f,0.18f,0.22f);
        glBegin(GL_QUADS);
          glVertex2f(bx,by); glVertex2f(bx+w,by);
          glVertex2f(bx+w,by+h); glVertex2f(bx,by+h);
        glEnd();
        glColor3f(r,g,b);
        glBegin(GL_QUADS);
          glVertex2f(bx,by); glVertex2f(bx+fill,by);
          glVertex2f(bx+fill,by+h); glVertex2f(bx,by+h);
        glEnd();
    };

    struct JointRow { const char* name; int fi; int ji; float maxA; float cr,cg,cb; };
    static const JointRow rows[] = {
        {"Pulgar  MCP", 0,0, 70,  0.95f,0.65f,0.30f},
        {"Pulgar  IP ", 0,1, 80,  0.95f,0.65f,0.30f},
        {"Indice  MCP", 1,0, 90,  0.35f,0.85f,0.55f},
        {"Indice  PIP", 1,1,110,  0.35f,0.85f,0.55f},
        {"Indice  DIP", 1,2, 80,  0.35f,0.85f,0.55f},
        {"Medio   MCP", 2,0, 90,  0.40f,0.70f,1.00f},
        {"Medio   PIP", 2,1,110,  0.40f,0.70f,1.00f},
        {"Medio   DIP", 2,2, 80,  0.40f,0.70f,1.00f},
        {"Anular  MCP", 3,0, 90,  0.85f,0.45f,0.85f},
        {"Anular  PIP", 3,1,110,  0.85f,0.45f,0.85f},
        {"Anular  DIP", 3,2, 80,  0.85f,0.45f,0.85f},
        {"Menique MCP", 4,0, 90,  1.00f,0.50f,0.40f},
        {"Menique PIP", 4,1,110,  1.00f,0.50f,0.40f},
        {"Menique DIP", 4,2, 80,  1.00f,0.50f,0.40f},
    };
    float yy=py-22;
    for(auto& jr:rows){
        char buf[64];
        snprintf(buf,sizeof(buf),"%-12s %3.0f°", jr.name, angles[jr.fi][jr.ji]);
        glColor3f(jr.cr, jr.cg, jr.cb);
        drawText(px, yy, buf, GLUT_BITMAP_HELVETICA_12);
        drawBar(px+140, yy+1, angles[jr.fi][jr.ji], jr.maxA, jr.cr, jr.cg, jr.cb);
        yy -= 13;
    }

    // ── Panel de controles (izquierda) ────────────────────────────────────
    // Subido para no sobreponerse con la barra inferior (altura 38px)
    const float panelBottom = 50.0f;
    const float panelTop    = 452.0f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f,0.10f,0.14f,0.82f);
    glBegin(GL_QUADS);
      glVertex2f(12,panelBottom); glVertex2f(415,panelBottom);
      glVertex2f(415,panelTop);   glVertex2f(12,panelTop);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.28f,0.50f,0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(12,panelBottom); glVertex2f(415,panelBottom);
      glVertex2f(415,panelTop);   glVertex2f(12,panelTop);
    glEnd();

    glColor3f(0.72f,0.88f,1.0f);
    drawText(18, panelTop-18, "CONTROLES", GLUT_BITMAP_HELVETICA_12);

    // Indicador de seleccion actual (dedo + articulacion activos)
    {
        char selbuf[64];
        snprintf(selbuf,sizeof(selbuf),"Seleccion actual:  %s / %s",
                  fingerNames[selFinger], currentJointName());
        glColor3f(0.95f,0.85f,0.35f);
        drawText(18, panelTop-34, selbuf, GLUT_BITMAP_HELVETICA_12);
    }

    struct CtrlRow { const char* k; const char* d; bool header; };
    static const CtrlRow ctrl[] = {
        {"--- VISTA -----------------------","",true},
        {"Arrastrar raton",  "Rotar camara 3D",false},
        {"Rueda raton",      "Zoom in / out",false},
        {"G",                "Mostrar / ocultar cuadricula",false},
        {"--- GLOBALES -------------------","",true},
        {"R",                "Restablecer posicion",false},
        {"ESPACIO (mantener)","Cerrar puno; al soltar se abre",false},
        {"ESC",              "Salir",false},
        {"--- SELECCIONAR DEDO -----------","",true},
        {"0",                "Pulgar",false},
        {"1",                "Indice",false},
        {"2",                "Medio",false},
        {"3",                "Anular",false},
        {"4",                "Menique",false},
        {"--- SELECCIONAR ARTICULACION ---","",true},
        {"Q",                "Articulacion MCP",false},
        {"W",                "Articulacion PIP / IP",false},
        {"D",                "Articulacion DIP (no en pulgar)",false},
        {"--- MOVER ARTICULACION ---------","",true},
        {"E",                "Extender la seleccionada",false},
        {"C",                "Contraer la seleccionada",false},
    };
    int cy2=(int)(panelTop-54);
    for(auto& cr2:ctrl){
        if(cr2.header){
            glColor3f(0.45f,0.68f,0.35f);
            drawText(18,(float)cy2,cr2.k,GLUT_BITMAP_HELVETICA_12);
        } else {
            char line[100];
            snprintf(line,sizeof(line),"  %-18s%s",cr2.k,cr2.d);
            glColor3f(0.55f,0.72f,0.92f);
            drawText(18,(float)cy2,line,GLUT_BITMAP_HELVETICA_12);
        }
        cy2-=14;
    }

    // ── Barra inferior ────────────────────────────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f,0.10f,0.14f,0.78f);
    glBegin(GL_QUADS);
      glVertex2f(0,0); glVertex2f((float)winW,0);
      glVertex2f((float)winW,38); glVertex2f(0,38);
    glEnd();
    glDisable(GL_BLEND);

    // Indicadores de dedos en barra inferior
    static const char* dedos[] = {"PULGAR","INDICE","MEDIO","ANULAR","MENIQUE"};
    static const float dcr[] = {0.95f,0.35f,0.40f,0.85f,1.00f};
    static const float dcg[] = {0.65f,0.85f,0.70f,0.45f,0.50f};
    static const float dcb[] = {0.30f,0.55f,1.00f,0.85f,0.40f};
    float bw=(float)winW/5.0f;
    for(int i=0;i<5;i++){
        float cx=bw*i+bw*0.5f;
        bool active = (i==selFinger);
        // Fondo del dedo (resaltado si es el seleccionado)
        float bgA = active ? 0.95f : 0.6f;
        float bgMul = active ? 0.65f : 0.3f;
        glColor4f(dcr[i]*bgMul, dcg[i]*bgMul, dcb[i]*bgMul, bgA);
        glEnable(GL_BLEND);
        glBegin(GL_QUADS);
          glVertex2f(bw*i+2,2); glVertex2f(bw*(i+1)-2,2);
          glVertex2f(bw*(i+1)-2,36); glVertex2f(bw*i+2,36);
        glEnd();
        glDisable(GL_BLEND);
        if(active){
            glColor3f(1.0f,1.0f,1.0f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
              glVertex2f(bw*i+2,2); glVertex2f(bw*(i+1)-2,2);
              glVertex2f(bw*(i+1)-2,36); glVertex2f(bw*i+2,36);
            glEnd();
            glLineWidth(1.0f);
        }
        glColor3f(dcr[i],dcg[i],dcb[i]);
        char lbl[32];
        // Promedio de angulos del dedo como indicador
        float avg=0;
        int   cnt=(i==0)?2:3;
        for(int j=0;j<cnt;j++) avg+=angles[i][j];
        avg/=cnt;
        snprintf(lbl,sizeof(lbl),"%s  %.0f°",dedos[i],avg);
        int tw=0;
        for(const char* c=lbl;*c;c++) tw+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_12,*c);
        drawText(cx-tw*0.5f,13,lbl,GLUT_BITMAP_HELVETICA_12);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
}

// ═════════════════════════════════════════════════════════════════════════════
//  DISPLAY
// ═════════════════════════════════════════════════════════════════════════════
static void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0.5,5.5/zoom, 0,0.2,0, 0,1,0);
    glRotatef(camRotX,1,0,0);
    glRotatef(camRotY,0,1,0);

    if(showGrid) drawGrid();

    glPushMatrix();
      glTranslatef(0,-0.4f,0);
      drawHand();
    glPopMatrix();

    drawHUD();
    glutSwapBuffers();
}

// ═════════════════════════════════════════════════════════════════════════════
//  RESHAPE
// ═════════════════════════════════════════════════════════════════════════════
static void reshape(int w, int h)
{
    winW=w; winH=h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(40.0,(double)w/h,0.05,50.0);
    glMatrixMode(GL_MODELVIEW);
}

// ═════════════════════════════════════════════════════════════════════════════
//  HELPERS de angulo con clamp
// ═════════════════════════════════════════════════════════════════════════════
static void bend(int fi, int ji, float delta)
{
    angles[fi][ji] += delta;
    if(angles[fi][ji] < 0) angles[fi][ji]=0;
    if(angles[fi][ji] > maxAngle[fi][ji]) angles[fi][ji]=maxAngle[fi][ji];
}

// ═════════════════════════════════════════════════════════════════════════════
//  TECLADO
// ═════════════════════════════════════════════════════════════════════════════
static void keyboard(unsigned char key, int, int)
{
    switch(key){
    case 27: exit(0);

    // Vista / globales
    case 'g': case 'G': showGrid=!showGrid; break;
    case 'R': case 'r':
        for(int i=0;i<5;i++) angles[i][0]=angles[i][1]=angles[i][2]=0; break;

    // ESPACIO: cerrar puño completo MIENTRAS se mantiene presionado
    case ' ':
        for(int i=0;i<5;i++)
            for(int j=0;j<jointCount(i);j++)
                angles[i][j] = maxAngle[i][j];
        break;

    // Seleccion de dedo (0..4)
    case '0': selFinger=0; if(selJoint>1) selJoint=1; break;
    case '1': selFinger=1; break;
    case '2': selFinger=2; break;
    case '3': selFinger=3; break;
    case '4': selFinger=4; break;

    // Seleccion de articulacion dentro del dedo activo
    case 'q': case 'Q': selJoint=0; break;                       // MCP
    case 'w': case 'W': selJoint=1; break;                       // PIP / IP
    case 'd': case 'D':                                          // DIP
        if(jointCount(selFinger)>2) selJoint=2;
        break;

    // Mover la articulacion seleccionada
    case 'e': case 'E':  // Extender
        bend(selFinger, selJoint, -STEP);
        break;
    case 'c': case 'C':  // Contraer
        bend(selFinger, selJoint, +STEP);
        break;
    }
    glutPostRedisplay();
}

// Al soltar el ESPACIO, el puño se abre por completo
static void keyboardUp(unsigned char key, int, int)
{
    if(key==' '){
        for(int i=0;i<5;i++)
            for(int j=0;j<jointCount(i);j++)
                angles[i][j] = 0;
        glutPostRedisplay();
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  RATON
// ═════════════════════════════════════════════════════════════════════════════
static void mouse(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON){dragging=(state==GLUT_DOWN);lastMX=x;lastMY=y;}
    if(button==3){zoom*=1.08f;glutPostRedisplay();}
    if(button==4){zoom/=1.08f;if(zoom<0.1f)zoom=0.1f;glutPostRedisplay();}
}
static void motion(int x, int y)
{
    if(!dragging)return;
    camRotY+=(x-lastMX)*0.5f;
    camRotX+=(y-lastMY)*0.5f;
    lastMX=x;lastMY=y;
    glutPostRedisplay();
}
static void timer(int){glutPostRedisplay();glutTimerFunc(16,timer,0);}

// ═════════════════════════════════════════════════════════════════════════════
//  MAIN
// ═════════════════════════════════════════════════════════════════════════════
int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(winW,winH);
    glutCreateWindow("Articulaciones de una Mano");

    glClearColor(0.11f,0.14f,0.19f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    // Luz principal (frontal superior)
    glEnable(GL_LIGHT0);
    {
        GLfloat pos[]={1.5f, 4.0f, 5.0f, 1.0f};
        GLfloat amb[]={0.08f,0.06f,0.05f,1.0f};
        GLfloat dif[]={1.00f,0.95f,0.88f,1.0f};
        GLfloat spc[]={0.90f,0.88f,0.80f,1.0f};
        glLightfv(GL_LIGHT0,GL_POSITION,pos);
        glLightfv(GL_LIGHT0,GL_AMBIENT, amb);
        glLightfv(GL_LIGHT0,GL_DIFFUSE, dif);
        glLightfv(GL_LIGHT0,GL_SPECULAR,spc);
    }
    // Luz de relleno (lateral)
    glEnable(GL_LIGHT1);
    {
        GLfloat pos[]={-3.0f, 1.0f, 2.0f, 1.0f};
        GLfloat dif[]={0.35f,0.28f,0.22f,1.0f};
        GLfloat spc[]={0.10f,0.08f,0.06f,1.0f};
        glLightfv(GL_LIGHT1,GL_POSITION,pos);
        glLightfv(GL_LIGHT1,GL_DIFFUSE, dif);
        glLightfv(GL_LIGHT1,GL_SPECULAR,spc);
    }
    // Luz trasera de contorno
    glEnable(GL_LIGHT2);
    {
        GLfloat pos[]={0.0f,-2.0f,-4.0f,1.0f};
        GLfloat dif[]={0.18f,0.14f,0.10f,1.0f};
        GLfloat spc[]={0.05f,0.04f,0.03f,1.0f};
        glLightfv(GL_LIGHT2,GL_POSITION,pos);
        glLightfv(GL_LIGHT2,GL_DIFFUSE, dif);
        glLightfv(GL_LIGHT2,GL_SPECULAR,spc);
    }

    // Modelo de luz global
    GLfloat globalAmb[]={0.12f,0.08f,0.06f,1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,globalAmb);

    // Quadric compartido
    gQuad=gluNewQuadric();
    gluQuadricNormals(gQuad,GLU_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16,timer,0);
    glutMainLoop();

    gluDeleteQuadric(gQuad);
    return 0;
}
