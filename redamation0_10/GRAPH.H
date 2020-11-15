#ifndef GRAPH_H
#define GRAPH_H

#include "mat.h"
#include "stgapi.h"

// variables
extern WORD fColor;

// estructuras de grafico
struct Rect {
int xmin,xmax,ymin,ymax;
};

struct Color { // color con alpha
WORD color;
BYTE alpha;
};

struct GradColor {
Color ramp[8];
int ratio[8];
char cnt;
};

struct PatronColor {
Color ramp[256]; // gradiente
Matrix imat; // lugar y estala
// bool tienealpha; 
};


void SetClip(int x1,int y1,int x2,int y2);
void ClrSrc(WORD color);

inline void SetColor(WORD c)
{
fColor=c;
}

inline WORD ColorRGB(BYTE r,BYTE g,BYTE b)
{
return screen.setRGBColor(r,g,b);
}

inline void SetRGB(BYTE r,BYTE g,BYTE b)
{
fColor=ColorRGB(r,g,b);
}

void SetPixelCopy();
void SetPixelXOR();

// color fColor
void Line(int x1,int y1,int x2,int y2);
void VLine(int x1,int y1,int y2);
void HLine(int x1,int y1,int x2);
void HLineD(int x1,int y1,int x2,WORD c1,WORD c2);
void Box(int x1,int y1,int x2,int y2);
void BoxFill(int x1,int y1,int x2,int y2);
void Circle(int x,int y,int r);
void Ellipse(int x, int y, int rx, int ry);
void EllipseFill(int x,int y,int rx,int ry);
void SPline(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
void SPlin(int x1,int y1,int x2,int y2,int x3,int y3);

// crear patrones
void SetLinearPat(PatronColor &p,long a1,long b1,long tx1,long ty1);
void SetRadialPat(PatronColor &p,long a1,long b1,long c1,long d1,long tx1,long ty1);

// set color o patrones
void SetSolidColor(WORD c);
void SetSolidAColor(Color &c);
void SetLinearColor(PatronColor *p);
void SetRadialColor(PatronColor *p);
//void SetBitmapColor();

// texto
short TexLargoN(char *n);
short TexLargo(char *n);
void writeTexB(short x,short y,char *n);
void writeTexNA(short x,short y,char *n);
void writeTexA(short x,short y,char *n);
void writeTexN(short x,short y,char *n);
void writeTex(short x,short y,char *n);
void writeCar(short x,short y,char c);
void writeCarN(short x,short y,char c);

#endif