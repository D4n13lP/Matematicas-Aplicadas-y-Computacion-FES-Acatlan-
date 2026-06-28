/*
 * =====================================================================
 *  LUZ GLOBAL Y PUNTUAL
 *  Escena con icosaedro, tetera y dona
 *  Texturas procedurales, sombras y efectos de luz
 *
 *  Lee definiciones desde: definiciones_luz.txt
 *
 *  Compilar Windows/MinGW:
 *    g++ r9_luz.cpp -o luz.exe -lfreeglut -lopengl32 -lglu32
 *  Compilar Linux:
 *    g++ r9_luz.cpp -o luz -lGL -lGLU -lglut -lm
 *
 *  Controles:
 *    Arrastrar raton    → Rotar vista
 *    Rueda raton        → Zoom in/out
 *    G                  → Cuadricula
 *    R                  → Restablecer posicion
 *    L                  → Cambiar tipo de luz (Global / Puntual)
 *    ESC                → Salir
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
static bool  usePuntual = false;   // false=Global, true=Puntual
static float lightAngle = 0.0f;   // angulo para animar la luz puntual
static float lightHeight = 3.5f;

// ── Definiciones leidas del archivo ──────────────────────────────────────────
static std::string defGlobal = "Luz global: ilumina el conjunto de la escena, con un efecto mas uniforme.";
static std::string defPuntual = "Luz puntual: ilumina un area especifica, con haz mas estrecho y mayor concentracion de luz.";

// ── Texturas procedurales ─────────────────────────────────────────────────────
static GLuint texIco  = 0;
static GLuint texTeap = 0;
static GLuint texDona = 0;

// ── Quadric ───────────────────────────────────────────────────────────────────
static GLUquadric* gQuad = nullptr;

// ── Posicion luz puntual ──────────────────────────────────────────────────────
static float lightPX = 0.0f, lightPY = lightHeight, lightPZ = 2.5f;

// ─────────────────────────────────────────────────────────────────────────────
//  LEER DEFINICIONES DESDE ARCHIVO
// ─────────────────────────────────────────────────────────────────────────────
static void loadDefinitions()
{
    FILE* f = fopen("definiciones_luz.txt", "r");
    if (!f) return;
    char line[512];
    std::vector<std::string> lines;
    while (fgets(line, sizeof(line), f))
    {
        int len = (int)strlen(line);
        while (len > 0 && (line[len-1]=='\n'||line[len-1]=='\r')) line[--len]=0;
        if (len > 0) lines.push_back(line);
    }
    fclose(f);
    if (lines.size() >= 1) defGlobal  = lines[0];
    if (lines.size() >= 2) defPuntual = lines[1];
}

// ─────────────────────────────────────────────────────────────────────────────
//  GENERAR TEXTURA PROCEDURAL
// ─────────────────────────────────────────────────────────────────────────────
static GLuint makeCheckerTex(unsigned char r1,unsigned char g1,unsigned char b1,
                              unsigned char r2,unsigned char g2,unsigned char b2,
                              int cells=8)
{
    const int SZ = 256;
    std::vector<unsigned char> data(SZ*SZ*3);
    for(int y=0;y<SZ;y++) for(int x=0;x<SZ;x++){
        int cx=x*cells/SZ, cy=y*cells/SZ;
        bool even = ((cx+cy)%2==0);
        int idx=(y*SZ+x)*3;
        data[idx+0]=even?r1:r2;
        data[idx+1]=even?g1:g2;
        data[idx+2]=even?b1:b2;
    }
    GLuint t; glGenTextures(1,&t);
    glBindTexture(GL_TEXTURE_2D,t);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,SZ,SZ,0,GL_RGB,GL_UNSIGNED_BYTE,data.data());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    return t;
}

static GLuint makeStripeTex(unsigned char r1,unsigned char g1,unsigned char b1,
                             unsigned char r2,unsigned char g2,unsigned char b2,
                             int stripes=12)
{
    const int SZ=256;
    std::vector<unsigned char> data(SZ*SZ*3);
    for(int y=0;y<SZ;y++) for(int x=0;x<SZ;x++){
        int band=x*stripes/SZ;
        bool even=(band%2==0);
        int idx=(y*SZ+x)*3;
        data[idx+0]=even?r1:r2;
        data[idx+1]=even?g1:g2;
        data[idx+2]=even?b1:b2;
    }
    GLuint t; glGenTextures(1,&t);
    glBindTexture(GL_TEXTURE_2D,t);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,SZ,SZ,0,GL_RGB,GL_UNSIGNED_BYTE,data.data());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    return t;
}

static GLuint makeDotTex(unsigned char rb,unsigned char gb,unsigned char bb,
                          unsigned char rd,unsigned char gd,unsigned char bd)
{
    const int SZ=256;
    std::vector<unsigned char> data(SZ*SZ*3);
    for(int y=0;y<SZ;y++) for(int x=0;x<SZ;x++){
        // dots en rejilla de 32x32
        float cx=((x%32)-16)/16.0f;
        float cy=((y%32)-16)/16.0f;
        float d=sqrtf(cx*cx+cy*cy);
        bool dot=(d<0.55f);
        int idx=(y*SZ+x)*3;
        data[idx+0]=dot?rd:rb;
        data[idx+1]=dot?gd:gb;
        data[idx+2]=dot?bd:bb;
    }
    GLuint t; glGenTextures(1,&t);
    glBindTexture(GL_TEXTURE_2D,t);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,SZ,SZ,0,GL_RGB,GL_UNSIGNED_BYTE,data.data());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    return t;
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
//  SOMBRA PLANA (stencil shadow sobre el suelo y=−1.6)
// ─────────────────────────────────────────────────────────────────────────────
// Proyeccion de sombra plana hacia el suelo usando la posicion de la luz
static void getShadowMatrix(float m[16], float lightPos[4], float planeY)
{
    // Plano: y = planeY  →  normal (0,1,0,−planeY)
    float px=lightPos[0], py=lightPos[1], pz=lightPos[2], pw=lightPos[3];
    float ny=1.0f, nd=-planeY;
    float dot = py*ny + pw*nd;   // n·L (solo componente y importa)
    m[ 0]=dot - px*ny*0; m[ 1]=-py*ny*0; m[ 2]=-pz*ny*0; m[ 3]=-pw*ny*0;
    // columna 1 (y)
    m[ 4]=-px*ny;  m[ 5]=dot-py*ny; m[ 6]=-pz*ny;  m[ 7]=-pw*ny;
    m[ 8]=-px*0;   m[ 9]=-py*0;     m[10]=dot;      m[11]=-pw*0;
    m[12]=-px*nd;  m[13]=-py*nd;    m[14]=-pz*nd;   m[15]=dot-pw*nd;
    // Recalculo correcto (plano y=planeY, normal (0,1,0), d=-planeY)
    // shadow matrix formula:
    float lx=px,ly=py,lz=pz,lw=pw;
    float a=0,b=1,c=0,d2=-planeY;
    float dl=a*lx+b*ly+c*lz+d2*lw;
    m[ 0]=dl-lx*a; m[ 1]=-ly*a;   m[ 2]=-lz*a;   m[ 3]=-lw*a;
    m[ 4]=-lx*b;   m[ 5]=dl-ly*b; m[ 6]=-lz*b;   m[ 7]=-lw*b;
    m[ 8]=-lx*c;   m[ 9]=-ly*c;   m[10]=dl-lz*c; m[11]=-lw*c;
    m[12]=-lx*d2;  m[13]=-ly*d2;  m[14]=-lz*d2;  m[15]=dl-lw*d2;
}

// ─────────────────────────────────────────────────────────────────────────────
//  CUADRICULA (igual que en r8)
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
//  DIBUJAR ICOSAEDRO CON TEXTURA
// ─────────────────────────────────────────────────────────────────────────────
// Vértices del icosaedro unitario
static const float PHI = 1.6180339887f;

static float icoVerts[12][3] = {
    { 0, 1, PHI},{ 0,-1, PHI},{ 0, 1,-PHI},{ 0,-1,-PHI},
    { 1, PHI, 0},{-1, PHI, 0},{ 1,-PHI, 0},{-1,-PHI, 0},
    { PHI, 0, 1},{-PHI, 0, 1},{ PHI, 0,-1},{-PHI, 0,-1}
};

static int icoFaces[20][3] = {
    {0,1,8},{0,8,4},{0,4,5},{0,5,9},{0,9,1},
    {1,6,8},{8,6,10},{8,10,4},{4,10,2},{4,2,5},
    {5,2,11},{5,11,9},{9,11,7},{9,7,1},{1,7,6},
    {3,6,7},{3,10,6},{3,2,10},{3,11,2},{3,7,11}
};

static void drawIcosahedron()
{
    // Normalizar vertices
    float nv[12][3];
    for(int i=0;i<12;i++){
        float l=sqrtf(icoVerts[i][0]*icoVerts[i][0]+
                      icoVerts[i][1]*icoVerts[i][1]+
                      icoVerts[i][2]*icoVerts[i][2]);
        nv[i][0]=icoVerts[i][0]/l;
        nv[i][1]=icoVerts[i][1]/l;
        nv[i][2]=icoVerts[i][2]/l;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texIco);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBegin(GL_TRIANGLES);
    for(int f=0;f<20;f++){
        int a=icoFaces[f][0], b=icoFaces[f][1], c=icoFaces[f][2];
        // Normal de cara (flat para ver aristas)
        float ax=nv[b][0]-nv[a][0], ay=nv[b][1]-nv[a][1], az=nv[b][2]-nv[a][2];
        float bx=nv[c][0]-nv[a][0], by=nv[c][1]-nv[a][1], bz=nv[c][2]-nv[a][2];
        float nx=ay*bz-az*by, ny=az*bx-ax*bz, nz=ax*by-ay*bx;
        float nl=sqrtf(nx*nx+ny*ny+nz*nz);
        glNormal3f(nx/nl,ny/nl,nz/nl);

        glTexCoord2f(0.5f,1.0f); glVertex3fv(nv[a]);
        glTexCoord2f(0.0f,0.0f); glVertex3fv(nv[b]);
        glTexCoord2f(1.0f,0.0f); glVertex3fv(nv[c]);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// ─────────────────────────────────────────────────────────────────────────────
//  DIBUJAR DONA (torus) CON TEXTURA
// ─────────────────────────────────────────────────────────────────────────────
static void drawTorus(float R, float r, int nMaj, int nMin)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texDona);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    for(int i=0;i<nMaj;i++){
        float u0=2*M_PI*i/nMaj,     u1=2*M_PI*(i+1)/nMaj;
        float tu0=(float)i/nMaj,    tu1=(float)(i+1)/nMaj;
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<=nMin;j++){
            float v=2*M_PI*j/nMin;
            float tv=(float)j/nMin;
            for(int step=0;step<2;step++){
                float u=(step==0)?u0:u1;
                float tu=(step==0)?tu0:tu1;
                float cx=cosf(u), cy=sinf(u);
                float x=(R+r*cosf(v))*cx;
                float y=(R+r*cosf(v))*cy;
                float z=r*sinf(v);
                float nx=cosf(v)*cx, ny=cosf(v)*cy, nz=sinf(v);
                glNormal3f(nx,ny,nz);
                glTexCoord2f(tu*3.0f, tv*3.0f);
                glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

// ─────────────────────────────────────────────────────────────────────────────
//  DIBUJAR TETERA CON TEXTURA (proyeccion esferica)
// ─────────────────────────────────────────────────────────────────────────────
static void drawTeapotTextured()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texTeap);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // Generacion automatica de coordenadas de textura (esferica)
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    glutSolidTeapot(0.75);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

// ─────────────────────────────────────────────────────────────────────────────
//  CONFIGURAR LUCES
// ─────────────────────────────────────────────────────────────────────────────
static void setupLights()
{
    if (!usePuntual)
    {
        // ── LUZ GLOBAL (difusa, uniforme, varias fuentes) ─────────────────
        // Ambiente alta para simular iluminacion indirecta
        GLfloat globalAmb[] = {0.30f, 0.30f, 0.30f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

        // Luz 0: superior frontal suave
        glEnable(GL_LIGHT0);
        GLfloat pos0[] = {0.0f, 8.0f, 4.0f, 0.0f};  // w=0 → direccional
        GLfloat amb0[] = {0.20f,0.20f,0.20f,1.0f};
        GLfloat dif0[] = {0.75f,0.75f,0.75f,1.0f};
        GLfloat spc0[] = {0.40f,0.40f,0.40f,1.0f};
        glLightfv(GL_LIGHT0,GL_POSITION,pos0);
        glLightfv(GL_LIGHT0,GL_AMBIENT, amb0);
        glLightfv(GL_LIGHT0,GL_DIFFUSE, dif0);
        glLightfv(GL_LIGHT0,GL_SPECULAR,spc0);
        // Sin atenuacion (direccional)
        glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0f);
        glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.0f);
        glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0f);

        // Luz 1: lateral izquierda (relleno)
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

        // Luz 2: trasera contraluz
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

        // Apagar luz 3 (puntual no activa)
        glDisable(GL_LIGHT3);
    }
    else
    {
        // ── LUZ PUNTUAL (focalizada, con atenuacion) ──────────────────────
        // Ambiente muy baja para que las sombras sean pronunciadas
        GLfloat globalAmb[] = {0.04f, 0.04f, 0.06f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

        // Apagar luces de relleno
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);

        // Luz 0: puntual brillante con atenuacion
        glEnable(GL_LIGHT0);
        GLfloat pos0[] = {lightPX, lightPY, lightPZ, 1.0f};  // w=1 → puntual
        GLfloat amb0[] = {0.02f,0.02f,0.03f,1.0f};
        GLfloat dif0[] = {1.00f,0.95f,0.80f,1.0f};
        GLfloat spc0[] = {1.00f,1.00f,0.95f,1.0f};
        glLightfv(GL_LIGHT0,GL_POSITION,pos0);
        glLightfv(GL_LIGHT0,GL_AMBIENT, amb0);
        glLightfv(GL_LIGHT0,GL_DIFFUSE, dif0);
        glLightfv(GL_LIGHT0,GL_SPECULAR,spc0);
        // Atenuacion con la distancia
        glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION, 0.5f);
        glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,   0.15f);
        glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.02f);

        // Luz de relleno ambient muy debil (para no dejar zonas totalmente negras)
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
//  DIBUJAR ESFERA INDICADORA DE LUZ PUNTUAL
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
      // Rayos
      glColor4f(1.0f,0.95f,0.60f,0.5f);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glLineWidth(1.5f);
      glBegin(GL_LINES);
      for(int i=0;i<12;i++){
          float a=2*M_PI*i/12;
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
//  DIBUJAR SOMBRAS PROYECTADAS
// ─────────────────────────────────────────────────────────────────────────────
static void drawShadow(void (*drawFn)())
{
    // Obtenemos la posicion de la luz
    float lpos[4];
    if (usePuntual) {
        lpos[0]=lightPX; lpos[1]=lightPY; lpos[2]=lightPZ; lpos[3]=1.0f;
    } else {
        // Luz global → simular desde arriba
        lpos[0]=0.0f; lpos[1]=8.0f; lpos[2]=4.0f; lpos[3]=0.0f;
        // Para sombra plana con luz direccional, convertir a distante
        lpos[0]=0.0f; lpos[1]=1.0f; lpos[2]=0.5f; lpos[3]=0.0f;
    }

    float sm[16];
    getShadowMatrix(sm, lpos, -1.59f);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f,0.0f,0.0f,0.38f);

    glPushMatrix();
      glMultMatrixf(sm);
      drawFn();
    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// ─────────────────────────────────────────────────────────────────────────────
//  LAMBDAS / funciones de escena sin estado
// ─────────────────────────────────────────────────────────────────────────────
static void sceneIcosahedron()
{
    glPushMatrix();
      glTranslatef(-2.2f, -0.35f, 0.0f);
      glScalef(0.85f,0.85f,0.85f);
      setMat(0.10f,0.05f,0.20f,  0.45f,0.15f,0.85f,  0.80f,0.70f,1.00f, 80.0f);
      drawIcosahedron();
    glPopMatrix();
}

static void sceneTeapot()
{
    glPushMatrix();
      glTranslatef(0.2f, -0.88f, 0.2f);
      setMat(0.15f,0.08f,0.02f,  0.85f,0.50f,0.15f,  1.00f,0.90f,0.60f, 120.0f);
      drawTeapotTextured();
    glPopMatrix();
}

static void sceneDona()
{
    glPushMatrix();
      glTranslatef(2.3f, -1.0f, -0.2f);
      glRotatef(75, 1, 0.3f, 0);
      setMat(0.05f,0.15f,0.10f,  0.15f,0.75f,0.45f,  0.70f,1.00f,0.80f, 60.0f);
      drawTorus(0.55f, 0.22f, 40, 20);
    glPopMatrix();
}

// ─────────────────────────────────────────────────────────────────────────────
//  HUD
// ─────────────────────────────────────────────────────────────────────────────
static void drawText(float x, float y, const char* s, void* font)
{
    glRasterPos2f(x, y);
    for(const char* c=s; *c; c++) glutBitmapCharacter(font, *c);
}

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
    drawText(14, winH-36, "LUZ GLOBAL Y PUNTUAL", GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.55f,0.75f,1.0f);
    drawText(14, winH-56,
             "Icosaedro  |  Tetera  |  Dona     (Texturas + Sombras)",
             GLUT_BITMAP_HELVETICA_12);

    // ── Panel definicion activa (centro superior) ─────────────────────────
    const std::string& defActiva = usePuntual ? defPuntual : defGlobal;
    float panW = winW - 28.0f;
    float panX = 14.0f, panY = winH - 90.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    // Color del fondo del panel segun tipo de luz
    if (!usePuntual)
        glColor4f(0.08f,0.14f,0.22f,0.90f);
    else
        glColor4f(0.18f,0.12f,0.04f,0.90f);
    glBegin(GL_QUADS);
      glVertex2f(panX-4,panY-6); glVertex2f(panX+panW,panY-6);
      glVertex2f(panX+panW,panY+18); glVertex2f(panX-4,panY+18);
    glEnd();
    glDisable(GL_BLEND);

    // Borde del panel
    if (!usePuntual)
        glColor3f(0.28f,0.55f,0.90f);
    else
        glColor3f(0.90f,0.65f,0.15f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(panX-4,panY-6); glVertex2f(panX+panW,panY-6);
      glVertex2f(panX+panW,panY+18); glVertex2f(panX-4,panY+18);
    glEnd();

    // Indicador tipo de luz
    if (!usePuntual)
        glColor3f(0.50f,0.80f,1.00f);
    else
        glColor3f(1.00f,0.80f,0.25f);

    char tipoLabel[64];
    snprintf(tipoLabel, sizeof(tipoLabel), "[%s]  %s",
             usePuntual ? "PUNTUAL" : "GLOBAL",
             defActiva.c_str());
    drawText(panX+2, panY+4, tipoLabel, GLUT_BITMAP_HELVETICA_12);

    // ── Panel de controles (izquierda) ─────────────────────────────────────
    const float ctrlBottom = 44.0f;
    const float ctrlTop    = 290.0f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f,0.10f,0.14f,0.84f);
    glBegin(GL_QUADS);
      glVertex2f(12,ctrlBottom); glVertex2f(300,ctrlBottom);
      glVertex2f(300,ctrlTop);   glVertex2f(12,ctrlTop);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.28f,0.50f,0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(12,ctrlBottom); glVertex2f(300,ctrlBottom);
      glVertex2f(300,ctrlTop);   glVertex2f(12,ctrlTop);
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
        {"--- ESCENA -----------------","",true},
        {"Icosaedro",       "Izquierda (textura cuadros)",false},
        {"Tetera",          "Centro (textura rayas)",false},
        {"Dona",            "Derecha (textura puntos)",false},
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

    // ── Panel info objetos (derecha) ──────────────────────────────────────
    float ipX = winW - 220.0f, ipY = winH - 110.0f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.06f,0.09f,0.13f,0.88f);
    glBegin(GL_QUADS);
      glVertex2f(ipX-8,ipY-95); glVertex2f((float)winW-8,ipY-95);
      glVertex2f((float)winW-8,ipY+14); glVertex2f(ipX-8,ipY+14);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.28f,0.50f,0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(ipX-8,ipY-95); glVertex2f((float)winW-8,ipY-95);
      glVertex2f((float)winW-8,ipY+14); glVertex2f(ipX-8,ipY+14);
    glEnd();

    glColor3f(0.72f,0.88f,1.0f);
    drawText(ipX, ipY, "OBJETOS EN ESCENA", GLUT_BITMAP_HELVETICA_12);

    struct ObjRow { const char* name; const char* tex; float r,g,b; };
    static const ObjRow objs[] = {
        {"Icosaedro (20 caras)", "Cuadros violeta",  0.70f,0.40f,1.00f},
        {"Tetera    (smooth)",   "Rayas naranja",    1.00f,0.60f,0.20f},
        {"Dona      (torus)",    "Puntos turquesa",  0.20f,0.90f,0.60f},
    };
    float oy=ipY-20;
    for(auto& o:objs){
        glColor3f(o.r,o.g,o.b);
        drawText(ipX,oy,o.name,GLUT_BITMAP_HELVETICA_12);
        oy-=13;
        char buf[64]; snprintf(buf,sizeof(buf),"  Tex: %s",o.tex);
        glColor3f(o.r*0.7f,o.g*0.7f,o.b*0.7f);
        drawText(ipX,oy,buf,GLUT_BITMAP_HELVETICA_12);
        oy-=16;
    }

    // ── Indicador de tipo de luz grande (barra inferior) ─────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f,0.10f,0.14f,0.80f);
    glBegin(GL_QUADS);
      glVertex2f(0,0); glVertex2f((float)winW,0);
      glVertex2f((float)winW,40); glVertex2f(0,40);
    glEnd();
    glDisable(GL_BLEND);

    // Separacion en dos mitades
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

    // ── Sombras ───────────────────────────────────────────────────────────
    // Las sombras se dibujan primero (sobre la rejilla)
    glDepthMask(GL_FALSE);

    glPushMatrix();
      drawShadow([](){
          glPushMatrix(); glTranslatef(-2.2f,-0.35f,0.0f); glScalef(0.85f,0.85f,0.85f); drawIcosahedron(); glPopMatrix();
      });
    glPopMatrix();

    glPushMatrix();
      drawShadow([](){
          glPushMatrix(); glTranslatef(0.2f,-0.88f,0.2f); glutSolidTeapot(0.75); glPopMatrix();
      });
    glPopMatrix();

    glPushMatrix();
      drawShadow([](){
          glPushMatrix(); glTranslatef(2.3f,-1.0f,-0.2f); glRotatef(75,1,0.3f,0); drawTorus(0.55f,0.22f,40,20); glPopMatrix();
      });
    glPopMatrix();

    glDepthMask(GL_TRUE);

    // ── Objetos ───────────────────────────────────────────────────────────
    sceneIcosahedron();
    sceneTeapot();
    sceneDona();

    // ── Indicador de la luz puntual ───────────────────────────────────────
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
//  TIMER (animacion de la luz puntual)
// ─────────────────────────────────────────────────────────────────────────────
static void timer(int)
{
    if(usePuntual)
    {
        lightAngle += 0.008f;
        lightPX = 3.0f * cosf(lightAngle);
        lightPZ = 3.0f * sinf(lightAngle);
        lightPY = lightHeight;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main(int argc, char** argv)
{
    loadDefinitions();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Luz Global y Puntual");

    glClearColor(0.11f, 0.14f, 0.19f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Texturas procedurales
    texIco  = makeCheckerTex(100,40,200, 220,180,255, 10);   // violeta cuadros
    texTeap = makeStripeTex (200,90,20,  255,200,100, 14);   // naranja rayas
    texDona = makeDotTex    (20,140,100, 100,255,180);        // turquesa puntos

    gQuad = gluNewQuadric();
    gluQuadricNormals(gQuad, GLU_SMOOTH);
    gluQuadricTexture(gQuad, GL_TRUE);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();

    gluDeleteQuadric(gQuad);
    return 0;
}
