#ifndef ENTRADA_H
#define ENTRADA_H

#include "trazo.h"

struct Entrada {
	bool dibujando;
	Linea trazo;		

	void clear() { dibujando=false;trazo.clear(); };
	bool bucle();	// bucle principal
};

struct EntradaBox {
	bool dibujando;
	short x1,y1,x2,y2;

	void clear() { dibujando=false; };
	bool bucle();	// bucle principal
};

struct EntradaCir {
	bool dibujando;
	short xc,yc,rx,ry;

	void clear() { dibujando=false; };
	bool bucle();	// bucle principal
};

#endif