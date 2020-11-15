#ifndef PUNTO_H
#define PUNTO_H

#include "graficos.h"

struct Punto2D {
	short x,y;
	char tipo;

	inline void move(short x1,short y1) {x=x1;y=y1;};
};

struct Caja2D {
	short x1,y1,x2,y2;
};

struct Grupo {// solo en tiempo de ejecucion
	short xc,yc;// centro del grupo
	// angulo ,escala?
	short ancho,alto;
	WORD cntp;
	short x,y;// posicion relativa al centro (actual
	};

struct Trazo2D {
	WORD puntos;
	WORD cntpuntos;
	char tipo;
	ColorRGBA color;

	void drawu(Matrix &mat,Punto2D *pnts);
	void draw(Matrix &mat,Grupo *gr,Punto2D *pnts);

	void drawEdit(Matrix &mat,Punto2D *pnts,WORD p);
	void drawEditT(Matrix &mat,Punto2D *pnts);
	void getBox(Matrix &mat,Punto2D *pnts,long *x1,long *y1,long *x2,long *y2);
	void move(Punto2D *pnts,short x,short y);
	//inline void moveNodo(WORD p,short x,short y) { puntos[p].x=x;puntos[p].y=y; }
	void changeNodo(Punto2D *pnts,WORD p);
	void changeNodoVisible(Punto2D *pnts,WORD p);
	
//	void BBox(Matrix &mat);
};

#define PUNTOSPORLINEA 512

struct Linea {
	Punto2D pp;
	Punto2D pnt[PUNTOSPORLINEA];
	WORD cntp;

	Linea()					{ clear(); }
	void clear()			{ cntp=0; }
	void ini(short x,short y);
	void add(short x,short y);
	void addPinta(short x,short y,short g);

	void del(unsigned short nro);
	void move(short x,short y);
	void moveNodo(unsigned short e,short x,short y);

	void draw(void);
	void drawPoli(void);
	void drawant(void);
	void optimize(void);
};

#endif