/*
 * =====================================================================
 *  FRACTALES 3D — Puntos y Recursion
 *  Piramide de Sierpinski (puntos, chaos game 3D)
 *  Esponja de Menger      (puntos recursivos)
 *  Conjunto de Julia      (puntos 3D escape-time)
 *
 *  Compilar Windows/MinGW:
 *    g++ r6_fractales.cpp -o fractales.exe -lfreeglut -lopengl32 -lglu32
 *  Compilar Linux:
 *    g++ r6_fractales.cpp -o fractales -lGL -lGLU -lglut -lm
 *
 *  Controles generales:
 *    ESPACIO / V      → Siguiente figura
 *    B                → Figura anterior
 *    1-3              → Ir directo a figura
 *    Tab              → Ciclar parametro de Julia (c)
 *    + / -            → Aumentar / disminuir nivel/detalle
 *    Arrastrar raton  → Rotar vista de camara
 *    Rueda raton      → Zoom in / out
 *    G                → Mostrar/ocultar cuadricula
 *    ESC              → Salir
 *
 *  Transformaciones afines (por figura activa):
 *    X/x  Rx/rx  Rotar +/- eje X
 *    Y/y  Ry/ry  Rotar +/- eje Y
 *    Z/z  Rz/rz  Rotar +/- eje Z
 *    T/t  Tx/tx  Trasladar +/- eje X
 *    U/u  Ty/ty  Trasladar +/- eje Y
 *    W/w  Tz/tz  Trasladar +/- eje Z
 *    E/e         Escalar mayor/menor
 *    H/h         Sesgo X positivo/negativo
 *    J/j         Sesgo Y positivo/negativo
 *    R           Restablecer transformaciones
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
#include <cstdlib>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

// ═════════════════════════════════════════════════════════════════════════════
//  ENUMERACIONES Y ESTADO GLOBAL
// ═════════════════════════════════════════════════════════════════════════════
enum Figure {
    FIG_SIERPINSKI_PYR = 0,
    FIG_MENGER_SPONGE,
    FIG_JULIA,
    FIG_COUNT
};
static Figure currentFig = FIG_SIERPINSKI_PYR;

static const char* figNames[] = {
    "PIRAMIDE DE SIERPINSKI",
    "ESPONJA DE MENGER",
    "CONJUNTO DE JULIA"
};

// ── Niveles de detalle por figura ─────────────────────────────────────────────
static int sierpLevel  = 6;   // iteraciones chaos game (x10000)
static int mengerLevel = 3;   // nivel de subdivision (1-4)
static int juliaPreset = 0;   // indice del preset c de Julia

// Constantes c para Julia (parte real, parte imaginaria)
static const float juliaCR[] = { -0.7f,  0.285f, -0.4f,  -0.8f,  0.0f,  -0.70176f };
static const float juliaCI[] = {  0.27f, 0.01f,  0.6f,    0.156f, 0.8f,  -0.3842f  };
static const int   juliaCount = 6;
static const char* juliaNames[] = {
    "c=(-0.70, 0.27)",
    "c=(0.285, 0.01)",
    "c=(-0.40, 0.60)",
    "c=(-0.80, 0.156)",
    "c=(0.00,  0.80)",
    "c=(-0.70, -0.38)"
};

// ── Nubes de puntos (generadas y cacheadas) ───────────────────────────────────
struct ColorPoint { float x,y,z; float r,g,b; };
static std::vector<ColorPoint> sierpPts;
static std::vector<ColorPoint> mengerPts;
static std::vector<ColorPoint> juliaPts;
static bool sierpDirty  = true;
static bool mengerDirty = true;
static bool juliaDirty  = true;

// ═════════════════════════════════════════════════════════════════════════════
//  TRANSFORMACIONES AFINES
// ═════════════════════════════════════════════════════════════════════════════
struct AffineTransform {
    float rotX=0,rotY=0,rotZ=0;
    float transX=0,transY=0,transZ=0;
    float scale=1.0f;
    float shearX=0,shearY=0;
    void reset(){ rotX=rotY=rotZ=transX=transY=transZ=shearX=shearY=0; scale=1.0f; }
};
static AffineTransform figTransforms[FIG_COUNT];

static const float ROT_STEP   = 5.0f;
static const float TRANS_STEP = 0.10f;
static const float SCALE_STEP = 0.10f;
static const float SHEAR_STEP = 0.05f;

static void applyAffineTransform(const AffineTransform& t)
{
    glTranslatef(t.transX, t.transY, t.transZ);
    glRotatef(t.rotX, 1,0,0);
    glRotatef(t.rotY, 0,1,0);
    glRotatef(t.rotZ, 0,0,1);
    if (t.shearX != 0.0f || t.shearY != 0.0f) {
        float sh[16] = {
            1.0f,    t.shearX, 0,0,
            t.shearY, 1.0f,   0,0,
            0,0,1,0, 0,0,0,1
        };
        glMultMatrixf(sh);
    }
    glScalef(t.scale, t.scale, t.scale);
}

// ═════════════════════════════════════════════════════════════════════════════
//  CAMARA
// ═════════════════════════════════════════════════════════════════════════════
static float camRotX =  25.0f;
static float camRotY = -35.0f;
static float zoom    =  1.0f;
static int   lastMX=0, lastMY=0;
static bool  dragging = false;
static bool  showGrid = true;
static int   winW=1200, winH=720;

// ═════════════════════════════════════════════════════════════════════════════
//  GENERACION: PIRAMIDE DE SIERPINSKI (Chaos Game 3D)
// ═════════════════════════════════════════════════════════════════════════════
static void genSierpinski()
{
    sierpPts.clear();
    // 4 vertices del tetraedro
    const float V[4][3] = {
        { 0.0f,  1.0f,  0.0f},
        {-1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f,  1.0f},
        { 0.0f, -1.0f, -1.4142f}
    };
    // Colores por vertice atractor
    const float VC[4][3] = {
        {0.20f, 0.80f, 1.00f},  // cian
        {1.00f, 0.40f, 0.20f},  // naranja
        {0.40f, 1.00f, 0.40f},  // verde
        {0.90f, 0.30f, 0.90f}   // magenta
    };

    int N = sierpLevel * 50000;
    if (N > 3000000) N = 3000000;
    sierpPts.reserve(N);

    float px=0,py=0,pz=0;
    // warm-up
    for (int i=0;i<20;i++){
        int v=rand()%4;
        px=(px+V[v][0])*0.5f;
        py=(py+V[v][1])*0.5f;
        pz=(pz+V[v][2])*0.5f;
    }
    for (int i=0;i<N;i++){
        int v=rand()%4;
        px=(px+V[v][0])*0.5f;
        py=(py+V[v][1])*0.5f;
        pz=(pz+V[v][2])*0.5f;
        // color: blend segun distancia a cada vertice
        float d[4];
        float dsum=0;
        for(int k=0;k<4;k++){
            float dx=px-V[k][0], dy=py-V[k][1], dz=pz-V[k][2];
            d[k]=1.0f/(0.001f+sqrtf(dx*dx+dy*dy+dz*dz));
            dsum+=d[k];
        }
        float r=0,g=0,b=0;
        for(int k=0;k<4;k++){ float w=d[k]/dsum; r+=w*VC[k][0]; g+=w*VC[k][1]; b+=w*VC[k][2]; }
        sierpPts.push_back({px,py,pz,r,g,b});
    }
    sierpDirty=false;
}

// ═════════════════════════════════════════════════════════════════════════════
//  GENERACION: ESPONJA DE MENGER (puntos recursivos)
// ═════════════════════════════════════════════════════════════════════════════
// Genera los centros de los cubos supervivientes a nivel N
static void mengerRecurse(float cx, float cy, float cz, float half,
                           int level, std::vector<ColorPoint>& pts)
{
    if (level == 0) {
        // emite puntos en las 8 esquinas + 6 caras del cubo unitario
        float s = half * 0.92f;
        int offsets[8][3] = {
            {-1,-1,-1},{1,-1,-1},{-1,1,-1},{1,1,-1},
            {-1,-1, 1},{1,-1, 1},{-1,1, 1},{1,1, 1}
        };
        for (auto& o : offsets) {
            float x=cx+o[0]*s, y=cy+o[1]*s, z=cz+o[2]*s;
            // color por altura
            float t = (y+1.5f)/3.0f;
            ColorPoint p;
            p.x=x; p.y=y; p.z=z;
            p.r = 0.10f + 0.85f*t;
            p.g = 0.30f + 0.50f*(1-t);
            p.b = 0.80f - 0.50f*t;
            pts.push_back(p);
        }
        return;
    }
    float h3 = half/3.0f;
    for (int ix=-1;ix<=1;ix++)
    for (int iy=-1;iy<=1;iy++)
    for (int iz=-1;iz<=1;iz++){
        // Eliminar los 7 cubos centrales de Menger:
        // los que tienen al menos 2 coordenadas == 0
        int zeros = (ix==0)+(iy==0)+(iz==0);
        if (zeros >= 2) continue;
        mengerRecurse(cx+ix*2*h3, cy+iy*2*h3, cz+iz*2*h3,
                      h3, level-1, pts);
    }
}

static void genMenger()
{
    mengerPts.clear();
    int lv = mengerLevel;
    if (lv<1) lv=1;
    if (lv>4) lv=4;
    mengerRecurse(0,0,0, 1.0f, lv, mengerPts);
    mengerDirty=false;
}

// ═════════════════════════════════════════════════════════════════════════════
//  GENERACION: CONJUNTO DE JULIA (escape-time 3D slice)
//  Se usa cuaterniones q -> q^2 + c proyectados en R^3
// ═════════════════════════════════════════════════════════════════════════════
static void genJulia()
{
    juliaPts.clear();
    float cr = juliaCR[juliaPreset];
    float ci = juliaCI[juliaPreset];
    int   maxIter = 64;
    float escR    = 2.0f;

    // Barrido de volumen 3D: (x,y) plano complejo, z como slice
    int   res  = 220;      // resolución de la cuadricula
    int   zRes = 40;       // numero de slices en z
    float span = 1.8f;
    float zSpan= 0.8f;

    juliaPts.reserve(res*res*zRes/4);

    for (int zi=0; zi<zRes; zi++){
        float zf = -zSpan + 2.0f*zSpan*zi/(zRes-1);
        for (int yi=0; yi<res; yi++){
            float y0 = -span + 2.0f*span*yi/(res-1);
            for (int xi=0; xi<res; xi++){
                float x0 = -span + 2.0f*span*xi/(res-1);

                // Cuaternion q = (x0, y0, zf, 0)
                float qr=x0, qi=y0, qj=zf, qk=0;
                int   it=0;
                float mag2;
                for(; it<maxIter; it++){
                    // q^2: (a+bi+cj+dk)^2
                    float nr = qr*qr - qi*qi - qj*qj - qk*qk;
                    float ni = 2*qr*qi;
                    float nj = 2*qr*qj;
                    float nk = 2*qr*qk;
                    qr=nr+cr; qi=ni+ci; qj=nj; qk=nk;
                    mag2=qr*qr+qi*qi+qj*qj+qk*qk;
                    if(mag2>escR*escR) break;
                }
                if (it==maxIter) {
                    // Punto del conjunto: colorear segun posicion
                    float t=(float)it/maxIter;
                    float zn=(zf+zSpan)/(2*zSpan);
                    ColorPoint p;
                    p.x=x0*1.3f; p.y=y0*1.3f; p.z=zf*2.5f;
                    p.r = 0.05f + 0.80f*fabsf(sinf((float)M_PI*zn*2));
                    p.g = 0.30f + 0.60f*t;
                    p.b = 0.70f + 0.30f*fabsf(cosf((float)M_PI*zn));
                    juliaPts.push_back(p);
                }
            }
        }
    }
    juliaDirty=false;
}

// ═════════════════════════════════════════════════════════════════════════════
//  DIBUJO DE NUBES DE PUNTOS
// ═════════════════════════════════════════════════════════════════════════════
static void drawPoints(const std::vector<ColorPoint>& pts, float ptSize)
{
    glDisable(GL_LIGHTING);
    glPointSize(ptSize);
    glBegin(GL_POINTS);
    for (const auto& p : pts){
        glColor3f(p.r, p.g, p.b);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    glPointSize(1.0f);
}

static void drawFigure(Figure f)
{
    switch(f){
        case FIG_SIERPINSKI_PYR:
            if(sierpDirty) genSierpinski();
            drawPoints(sierpPts, 1.0f);
            break;
        case FIG_MENGER_SPONGE:
            if(mengerDirty) genMenger();
            drawPoints(mengerPts, 3.5f);
            break;
        case FIG_JULIA:
            if(juliaDirty) genJulia();
            drawPoints(juliaPts, 1.2f);
            break;
        default: break;
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  CUADRICULA DE REFERENCIA
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
        glVertex3f(t,-1.5f,-ext); glVertex3f(t,-1.5f, ext);
        glVertex3f(-ext,-1.5f,t); glVertex3f( ext,-1.5f,t);
    }
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINES);
      glColor3f(0.85f,0.28f,0.28f);
      glVertex3f(0,-1.5f,0); glVertex3f(2.3f,-1.5f,0);
      glColor3f(0.28f,0.85f,0.28f);
      glVertex3f(0,-1.5f,0); glVertex3f(0,1.1f,0);
      glColor3f(0.28f,0.45f,0.90f);
      glVertex3f(0,-1.5f,0); glVertex3f(0,-1.5f,2.3f);
    glEnd();
    glLineWidth(1.0f);
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

    // ── Titulo ────────────────────────────────────────────────────────────
    glColor3f(0.85f,0.93f,1.0f);
    drawText(14, winH-34, "FRACTALES 3D  —  SIERPINSKI · MENGER · JULIA",
             GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.45f,0.82f,1.0f);
    char titulo[128];
    snprintf(titulo,sizeof(titulo),"Figura activa:  %s", figNames[currentFig]);
    drawText(16, winH-58, titulo, GLUT_BITMAP_HELVETICA_12);

    // Info extra segun figura
    glColor3f(0.35f,0.92f,0.55f);
    char info[128]="";
    if(currentFig==FIG_SIERPINSKI_PYR)
        snprintf(info,sizeof(info),"Puntos: %d   (+/- cambia densidad)",
                 (int)sierpPts.size());
    else if(currentFig==FIG_MENGER_SPONGE)
        snprintf(info,sizeof(info),"Nivel: %d   Puntos: %d   (+/- cambia nivel)",
                 mengerLevel,(int)mengerPts.size());
    else if(currentFig==FIG_JULIA)
        snprintf(info,sizeof(info),"Preset: %s   Tab=siguiente   Puntos: %d",
                 juliaNames[juliaPreset],(int)juliaPts.size());
    drawText(winW-600.0f, winH-34, info, GLUT_BITMAP_HELVETICA_12);

    // ── Estado de transformaciones (panel derecho) ────────────────────────
    const AffineTransform& tf = figTransforms[currentFig];
    float px = winW-320.0f, py = winH-80.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.06f,0.09f,0.13f,0.85f);
    glBegin(GL_QUADS);
      glVertex2f(px-8,py-150); glVertex2f(winW-8,py-150);
      glVertex2f(winW-8,py+14); glVertex2f(px-8,py+14);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.28f,0.50f,0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(px-8,py-150); glVertex2f(winW-8,py-150);
      glVertex2f(winW-8,py+14); glVertex2f(px-8,py+14);
    glEnd();

    glColor3f(0.72f,0.88f,1.0f);
    drawText(px,py,"ESTADO TRANSFORMACIONES",GLUT_BITMAP_HELVETICA_12);
    char buf[80];
    glColor3f(0.95f,0.70f,0.40f);
    snprintf(buf,sizeof(buf),"Rot X:%+.1f  Y:%+.1f  Z:%+.1f",tf.rotX,tf.rotY,tf.rotZ);
    drawText(px,py-20,buf,GLUT_BITMAP_HELVETICA_12);
    glColor3f(0.40f,0.90f,0.60f);
    snprintf(buf,sizeof(buf),"Trans X:%+.2f  Y:%+.2f  Z:%+.2f",tf.transX,tf.transY,tf.transZ);
    drawText(px,py-38,buf,GLUT_BITMAP_HELVETICA_12);
    glColor3f(0.60f,0.80f,1.00f);
    snprintf(buf,sizeof(buf),"Escala: %.2f",tf.scale);
    drawText(px,py-56,buf,GLUT_BITMAP_HELVETICA_12);
    glColor3f(1.00f,0.70f,0.90f);
    snprintf(buf,sizeof(buf),"Sesgo X:%+.2f  Y:%+.2f",tf.shearX,tf.shearY);
    drawText(px,py-74,buf,GLUT_BITMAP_HELVETICA_12);

    // ── Panel de instrucciones (izquierda) ────────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f,0.10f,0.14f,0.80f);
    glBegin(GL_QUADS);
      glVertex2f(12,8); glVertex2f(410,8);
      glVertex2f(410,370); glVertex2f(12,370);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.28f,0.50f,0.82f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(12,8); glVertex2f(410,8);
      glVertex2f(410,370); glVertex2f(12,370);
    glEnd();

    glColor3f(0.72f,0.88f,1.0f);
    drawText(18,352,"CONTROLES",GLUT_BITMAP_HELVETICA_12);

    struct Row { const char* k; const char* d; };
    static const Row rows[] = {
        {"ESPACIO / V / B",    "Siguiente / anterior figura"},
        {"1  Sierpinski Pyr",  "2  Esponja Menger   3  Julia"},
        {"--- FIGURA --------", ""},
        {"+ / -",              "Sierp: densidad | Menger: nivel"},
        {"Tab",                "Julia: siguiente preset c"},
        {"--- ROTACION ------", ""},
        {"X / x",              "Rotar +/- eje X  (Rx / rx)"},
        {"Y / y",              "Rotar +/- eje Y  (Ry / ry)"},
        {"Z / z",              "Rotar +/- eje Z  (Rz / rz)"},
        {"--- TRASLACION ----", ""},
        {"T / t",              "Trasladar +/- eje X  (Tx / tx)"},
        {"U / u",              "Trasladar +/- eje Y  (Ty / ty)"},
        {"W / w",              "Trasladar +/- eje Z  (Tz / tz)"},
        {"--- ESCALA --------", ""},
        {"E / e",              "Aumentar / disminuir tamanio"},
        {"--- SESGO ---------", ""},
        {"H / h",              "Sesgo X positivo / negativo"},
        {"J / j",              "Sesgo Y positivo / negativo"},
        {"--- VISTA ---------", ""},
        {"Arrastrar raton",    "Rotar camara"},
        {"Rueda raton",        "Zoom in / out"},
        {"G",                  "Mostrar/ocultar cuadricula"},
        {"R",                  "Restablecer transformaciones"},
        {"ESC",                "Salir"},
    };
    glColor3f(0.55f,0.72f,0.92f);
    int yy=334;
    for(auto& r:rows){
        if(r.k[0]=='-'){
            glColor3f(0.45f,0.65f,0.35f);
            drawText(18,(float)yy,r.k,GLUT_BITMAP_HELVETICA_12);
            glColor3f(0.55f,0.72f,0.92f);
        } else {
            char line[100];
            snprintf(line,sizeof(line),"%-22s%s",r.k,r.d);
            drawText(18,(float)yy,line,GLUT_BITMAP_HELVETICA_12);
        }
        yy-=14;
    }

    // ── Barra inferior de figuras ─────────────────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f,0.10f,0.14f,0.78f);
    glBegin(GL_QUADS);
      glVertex2f(0,0); glVertex2f((float)winW,0);
      glVertex2f((float)winW,38); glVertex2f(0,38);
    glEnd();
    glDisable(GL_BLEND);

    float bw=(float)winW/(float)FIG_COUNT;
    for(int i=0;i<FIG_COUNT;i++){
        float cx=bw*i+bw*0.5f;
        bool sel=(i==(int)currentFig);
        if(sel){
            glColor3f(0.28f,0.60f,0.95f);
            glBegin(GL_QUADS);
              glVertex2f(bw*i+2,2); glVertex2f(bw*(i+1)-2,2);
              glVertex2f(bw*(i+1)-2,36); glVertex2f(bw*i+2,36);
            glEnd();
            glColor3f(1.0f,1.0f,1.0f);
        } else {
            glColor3f(0.50f,0.65f,0.85f);
        }
        char lbl[48];
        snprintf(lbl,sizeof(lbl),"[%d] %s",i+1,figNames[i]);
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
    gluLookAt(0,0.3,6.0/zoom, 0,0,0, 0,1,0);
    glRotatef(camRotX,1,0,0);
    glRotatef(camRotY,0,1,0);

    if(showGrid) drawGrid();

    glPushMatrix();
    applyAffineTransform(figTransforms[currentFig]);
    drawFigure(currentFig);
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
    gluPerspective(45.0,(double)w/h,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
}

// ═════════════════════════════════════════════════════════════════════════════
//  TECLADO
// ═════════════════════════════════════════════════════════════════════════════
static void keyboard(unsigned char key, int, int)
{
    AffineTransform& tf = figTransforms[currentFig];
    switch(key){
        case 27: exit(0);
        case ' ':
        case 'v': case 'V':
            currentFig=(Figure)((currentFig+1)%FIG_COUNT); break;
        case 'b': case 'B':
            currentFig=(Figure)((currentFig+FIG_COUNT-1)%FIG_COUNT); break;
        case '1': currentFig=FIG_SIERPINSKI_PYR; break;
        case '2': currentFig=FIG_MENGER_SPONGE;  break;
        case '3': currentFig=FIG_JULIA;           break;

        // Detalle / parametros
        case '+': case '=':
            if(currentFig==FIG_SIERPINSKI_PYR){
                sierpLevel++; if(sierpLevel>60)sierpLevel=60;
                sierpDirty=true;
            } else if(currentFig==FIG_MENGER_SPONGE){
                mengerLevel++; if(mengerLevel>4)mengerLevel=4;
                mengerDirty=true;
            }
            break;
        case '-': case '_':
            if(currentFig==FIG_SIERPINSKI_PYR){
                sierpLevel--; if(sierpLevel<1)sierpLevel=1;
                sierpDirty=true;
            } else if(currentFig==FIG_MENGER_SPONGE){
                mengerLevel--; if(mengerLevel<1)mengerLevel=1;
                mengerDirty=true;
            }
            break;
        case '\t':
            if(currentFig==FIG_JULIA){
                juliaPreset=(juliaPreset+1)%juliaCount;
                juliaDirty=true;
            }
            break;

        // Rotacion
        case 'X': tf.rotX+=ROT_STEP;  break;
        case 'x': tf.rotX-=ROT_STEP;  break;
        case 'Y': tf.rotY+=ROT_STEP;  break;
        case 'y': tf.rotY-=ROT_STEP;  break;
        case 'Z': tf.rotZ+=ROT_STEP;  break;
        case 'z': tf.rotZ-=ROT_STEP;  break;
        // Traslacion
        case 'T': tf.transX+=TRANS_STEP; break;
        case 't': tf.transX-=TRANS_STEP; break;
        case 'U': tf.transY+=TRANS_STEP; break;
        case 'u': tf.transY-=TRANS_STEP; break;
        case 'W': tf.transZ+=TRANS_STEP; break;
        case 'w': tf.transZ-=TRANS_STEP; break;
        // Escala
        case 'E': tf.scale+=SCALE_STEP; break;
        case 'e': tf.scale-=SCALE_STEP; if(tf.scale<0.05f)tf.scale=0.05f; break;
        // Sesgo
        case 'H': tf.shearX+=SHEAR_STEP; break;
        case 'h': tf.shearX-=SHEAR_STEP; break;
        case 'J': tf.shearY+=SHEAR_STEP; break;
        case 'j': tf.shearY-=SHEAR_STEP; break;
        // Reset
        case 'R': tf.reset(); break;
        // Grid
        case 'g': case 'G': showGrid=!showGrid; break;
    }
    glutPostRedisplay();
}

// ═════════════════════════════════════════════════════════════════════════════
//  RATON
// ═════════════════════════════════════════════════════════════════════════════
static void mouse(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON){dragging=(state==GLUT_DOWN);lastMX=x;lastMY=y;}
    if(button==3){zoom*=1.08f;glutPostRedisplay();}
    if(button==4){zoom/=1.08f;if(zoom<0.05f)zoom=0.05f;glutPostRedisplay();}
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
    srand(42);
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(winW,winH);
    glutCreateWindow("Fractales 3D — Sierpinski · Menger · Julia");

    glClearColor(0.11f,0.14f,0.19f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    // Iluminacion minima (los puntos usan glColor, no material)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lp[]={3,5,6,1}; glLightfv(GL_LIGHT0,GL_POSITION,lp);

    // Pre-generar todas las figuras al inicio
    genSierpinski();
    genMenger();
    genJulia();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16,timer,0);
    glutMainLoop();
    return 0;
}
