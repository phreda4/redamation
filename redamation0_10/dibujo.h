#ifndef DIBUJO_H
#define DIBUJO_H

#include "entrada.h"

//-----------------------------------------------------------------------------
struct Vis {
	short angle,sc,xt,yt;

	void clear()				{ angle=0;sc=1<<FRACMAT;xt=0;yt=0; }
	void position(short x,short y)	{ xt=x;yt=y; }
	void move(short x,short y)		{ xt+=x;yt+=y; }
	void scale(short s)			{ sc=s; }
	void rota(short a)			{ angle=a; }

	void makeMatrix(Matrix &m)
	{
	Matrix m1;
	m.scale(sc,sc);
	m1.rotate(angle);
	m.mult(m1);
	m1.traslate(xt,yt);
	m.mult(m1);
	}
};

//-----------------------------------------------------------------------------
struct NodoDibujo {
	BYTE tipo;
	short x,y;
};

struct TipoTrazo {
	char tipo;
	WORD color;// por ahora solo color solido
	short xt,yt,r,s;

	void inic(char t,WORD c)	{ tipo=t;color=c;xt=yt=r=0;s=1<<FRACMAT; }
};

// tipo de cambio
// 0 borra trazo (nro trazo)
//*** 1 mueve trazo (nro,x,y)
// 2 escala trazo (nro,x,y)
// 3 rota trazo (nro,x)
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

struct NodoCambio {
	char tipo;// 
	unsigned short nro;// nodo
	short x,y;// mueve
};

struct NodoCambioD {
	short x,y;// origen
};

struct TipoTrazoD { // datos que no se necesita guardar
	unsigned short nnodo,cnodo;// primer nodo y cantidad
	short x1,y1,x2,y2;//bb
	short xc,yc;// centro del trazo
};

/*
struct TipoSeleccion {
	unsigned short cntpnt;
	unsigned short pnts[512];
	short xc,yc;
};
*/

struct Cabecera {
	char tipo;
	unsigned short tiempo,cntp,cntt,cntc;
};

#define MAXPNTDIBUJO 2048
#define MAXTRA 255
#define MAXCAM 1024

struct Dibujo {
	char tipo;
	unsigned short tiempo;
	NodoDibujo pnt[MAXPNTDIBUJO];
	unsigned short cntp;//puntos
	TipoTrazo trz[MAXTRA];
	unsigned short cntt;// trazos
	NodoCambio cmb[MAXCAM];
	unsigned short cntc;// cambios de los trazos anteriores;
	//.....................................
	unsigned short marca,marcat;
	TipoTrazoD trzD[MAXTRA];// para comienzo y largo de trazos
	NodoCambioD cmbD[MAXCAM];// para interpolar puntos
	Vis vista;
	Matrix mat,imat;

//	unsigned short csel;// selecciones
//	TipoSeleccion sel;
	//..........................
	void clear() { tipo=0;cntp=0;cntt=0;marca=0;marcat=0;tiempo=10; };

	int size();// tamanio en bytes
	void setmem(char *p);// graba en memoria
	void getmem(char *p);// carga de memoria
	void cargaCambios();

	bool esmarca(unsigned short nt) { return (nt<marcat); };
	void setSub();

	void setVista(Vis &v) { vista=v;vista.makeMatrix(mat);mat.invert(imat); };
	void draw();
	void drawEdit(unsigned short nt,unsigned short np);
	void drawBox(unsigned short nt);
	void setSensor();

	void addTrazo(Entrada *e,char t,WORD c);
	void addCircle(short x,short y,short rx,short ry,char t,WORD c);
	void addBox(short x1,short y1,short x2,short y2,char t,WORD c);
	void delTrazo(unsigned short nt);
	
	void moveTrazo(unsigned short nt,short xa,short ya,short x,short y);
	void moveNodo(unsigned short nt,unsigned short np,int x,int y);

	void delNodo(unsigned short nt,unsigned short np);
	void insNodo(unsigned short nt,unsigned short np);

	void changeWin(Vis &v);
	void moveWin(short xz,short yz,short x,short y);
	void rsWin(short xz,short yz,short x,short y);

	void changeLC(unsigned short nt,unsigned short np);

	void regenera();// info para edicion
	void regeneraBBox();

	void Literal(char *n);

	void settiempo();
	void settiempot(unsigned short t);
	unsigned short buscaCmb(char t,short nro);

	void clearSel();
	void setSel(short x1,short y1,short x2,short y2,Vis &vista);
	void moveSel(short dx,short dy);// mueve los seleccionados
	void rotaSel(short ang);
	void scaleSel(short sx,short sy);
	// falta cargar y grabar seleccion
};

#endif