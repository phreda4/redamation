#ifndef ESCENA_H
#define ESCENA_H

#include "dibujo.h"
#include "libro.h"

#define UNIDIB 0
#define UNITEX 1
#define UNINUM 2
#define UNIVAR 3
#define UNITAB 4


struct Elemento {
	char tipo;
	short hoja;// y grupo
	Vista vista;
	ColorRGBA color;
// referencias
	int padre;
// calculos internos
	int menort,mayort;
	int menorc,mayorc;
	Matrix mat,imat;

	inline void adjust(void) { vista.makeMatrix(mat,imat); };
};

struct Relacion {
	char a,b;//
	int vx,vy;
//	void (*calc)();
};

// modo
//	0 infinite loop,1 infinite pingpong,2 una sola vez
//  0 adelante, 1 reverse
//	0 release, 1 debug/export 
//----------------------------------------------------------------
// Cuadros.orden
//
// 0 borra trazo (nro trazo)
//*** 1 mueve trazo (nro,x,y)
//*** 2 escala trazo (nro,x,y) y rota
//----- 3 rota trazo (nro,x) o skew (futuro)
// 4 borra punto (nro punto)
//*** 5 mueve punto (nro,x,y)
//*** 6 mueve ventana
//*** 7 rota y escala ventana
// 8 color trazo (nro,tipo,color2)
// 9 fade in 
// 10 fade out
// 64 goto frame nro
// 65 espera por click
// 65 si se toca trazo (nro) goto frame x
// 66 fin
// 67 chain animacion
// 68 chain ani paralel
// 128 play wav
// 129 play sound
// 130 stop sound
// 131 stop all sound
// 240 espera trazo
struct Cuadros {
	int tiempo;
//	char orden;
	unsigned short nro;
	short x,y;
	short angulo,escala;
};

struct Escena {
	Libro *libro;

	short cntelementos;
	short elementos_index[128];
	Elemento elementos[128];

	short cntrelaciones;
	Relacion relaciones[256];
	short cntcuadros;
	Cuadros cuadros[1000];
	
	Vista vista;
	Matrix mat,imat;
	int tiempoactual;

	int escalatiempo;// escala de tiempo 10 = 1 segundo

	void create(Libro *l);
	void destroy(void);

	void clear(void);
	void addObj(short nro);
	void addTxt(char *n);
	void addNro(int nro);
	void addVar(void);
	void addTab(void);

	void draw(void);
	void drawSelect(short n);
	void play(void);

	void move(short n,short x,short y);
	void rotar(short n,short a);
	void escala(short n,short s);

	void addSensorO(void);//Matrix &mat);

// set keyframes
	void rew(void);// edita siempre en rew
	void addKeyF(short nro,WORD t,Vista &v);
	void delKeyF(short nro,WORD i);

	void drawKey(short nro);
	void sett(int t);// ubica en tiempo t
	void setto(int t);

	void moveWin(short xz,short yz,short x,short y);
	void rsWin(short x,short y);

	void cargar(char *n);
	void grabar(char *n);
};

#endif