#ifndef ANIMACION_H
#define ANIMACION_H

#include "dibujo.h"

//#define MAXPNTANI 8192
//#define MAXTRAANI 512
#define MAXCUA 128
#define MAXDIBUJOS 15536 // 64k

class Animacion {
public:
	char fps;
	char mem[MAXDIBUJOS];
	int cmem;
	Vis vista;// vista del dibujo
	WORD bColor;

	char *cuadros[MAXCUA];
	int scuadros[MAXCUA];
	unsigned short ccuadros;

	Animacion()			{ clear(25); };

	void clear(char f)	
	{ 
	fps=f;cmem=0;ccuadros=0;
	vista.clear();fColor=0;
	}

	void cargar(char *n);
	void grabar(char *n);
	void recostruye(); 

	int sizein(char *p);// tamanio del cuadro
	char tipoin(char *p);// tipo del cuadro

	void getDibujo(Dibujo *d,unsigned short n);
	void setDibujo(Dibujo *d,unsigned short n);

	int sizeDibujo(unsigned short n);

	void trim();

	void play();
};

#endif