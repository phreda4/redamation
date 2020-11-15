//
//  Trazo y figura para editar
//
#ifndef FIGURA_H
#define FIGURA_H

#include "graph.h"

// punto del dibujo
struct NODOP {
	//BYTE tipo; // inicio,line,curve
	long x,y;
	int bx,by;
	// otros atributos de la linea (grosor, color???
};

struct Vista {
	long xt,yt;
	long zoom;

	long getX(long x)	{ return (x-xt)/zoom; }
	long getY(long y)	{ return (y-yt)/zoom; }
	long setX(long x)	{ return x*zoom+xt; }
	long setY(long y)	{ return y*zoom+yt; }
	void set(long x,long y,long z) { xt=x;yt=y;zoom=z; }
};

struct Estado {
	unsigned short *pnt; // puntos resultado
	unsigned short *c;		// referencia
	unsigned short cnt;	// cantidad de cambios
	Color color;// nuevo color
	unsigned short tiempo;	// tiempo que pasa
	};

struct Trazo {
	char tipo;	// tipo de trazo
	NODOP *pnt;	// punto de inicio
	unsigned short cntp;	// cantidad de puntos
	Color color;// color de trazo

	Trazo()					{ clear(); }
	void clear()			{ cntp=0; }
	void ini(int x,int y,Vista &v);
	void add(int x,int y,Vista &v);
	void del(int nro);
	void move(int x,int y,Vista &v);
	void moveNodo(int e,int x,int y,Vista &v);
//	void setChange(int e);
	void drawDraw(Vista &v);
	void drawEdit(Vista &v,int nro);
	void drawLast();
	void reduce();
};

#define MAXPNTFIG 512

struct Figura {
// puntos
  NODOP pnt[MAXPNTFIG];	// puntos
  unsigned short cntpnt;
// poligonos
  Trazo pol[256];		// poligonos
  unsigned short cntpol;// cnt de poli
// estados de la figura
  int ref[MAXPNTFIG/4];		// 32 estados
  Estado estado[32];
  short cntest;
  
  int x1,y1,x2,y2; // bounding box
  long a,b,c,d,tx,ty;

  Figura()     { init(); }
  void init()  { cntpnt=cntpol=cntest=0; }
  void add(Trazo &tr);
  void set(short n,Trazo &tr);
  void get(short n,Trazo *tr);
  void del(short n);
  void draw(Vista &v);

  void save(char *t);
  void load(char *t);
};

union UNIDAD {
	long l;
	unsigned short s[2];
	unsigned char c[4];
};

// forma del archivo
// control: 
// ----inicio  
//	'l' lineas
//	'p' poligono solido
//  'q' poligono radial
//	'r' poligono lineal



// line to c[0]='l' s[1]=cnt de lines
// spline  c[0]='s' s[1]=cnt de spline (pnt*3)+2

#endif