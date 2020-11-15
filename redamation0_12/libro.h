#ifndef LIBRO_H
#define LIBRO_H

#include "dibujo.h"

#define LIBROMEM 65536 //64k
#define MAXHOJAS 64

struct Hoja {
	char *memoria;
	int largo;
};

struct Libro {
	char mem[LIBROMEM];
	int cntmem;

	Hoja hojas[MAXHOJAS];
	short cnthojas;

	void create(void);
	void destroy(void);

	void clear(void);

	inline char *get(short n) {	return hojas[n].memoria; };
	void get(short n,Dibujo &d);
	void put(short n,Dibujo &d);
};

#endif