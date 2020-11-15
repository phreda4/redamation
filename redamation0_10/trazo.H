//
//  Trazo sobre pantalla
//
#ifndef TRAZO_H
#define TRAZO_H

#include "graph.h"

// punto del dibujo
struct NodoLinea {
	int x,y; // posicion
//	int t;	// tiempo
};

#define PUNTOSPORLINEA 512

struct Linea {
	NodoLinea pnt[PUNTOSPORLINEA];
	unsigned short cntp;

	Linea()					{ clear(); }
	void clear()			{ cntp=0; }
	void ini(int x,int y);
	void add(int x,int y);
	void addDraw(int x,int y);
	void del(unsigned short nro);
	void move(int x,int y);
	void moveNodo(unsigned short e,int x,int y);

	void draw();
	void drawLast();
};


#endif