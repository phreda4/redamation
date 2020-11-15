#ifndef DIBUJO_H
#define DIBUJO_H

#include "punto.h"

struct Dibujo {
	Punto2D *pnts;
	WORD cntp,maxp;
	Trazo2D *trazos;
	WORD cntt,maxt;
	Grupo grupos[64];
	int x1,y1,x2,y2;// bb

	void create(WORD mp,WORD mt)
	{ 
	maxp=mp;
	pnts=new Punto2D[maxp];
	maxt=mt;
	trazos=new Trazo2D[maxt];
	}
	void destroy()
	{
	delete [] pnts;
	delete [] trazos;
	}

	void clear() {cntp=cntt=0;};

// crear trazos
	void addClon(WORD t);
	void addLinea(Matrix &imat,Linea &l,char t,ColorRGBA &c,short g);
	//void addCircle(short x,short y,short r,ColorRGBA &c,short g);
	void addEllipse(Matrix &imat,short x,short y,short xr,short yr,ColorRGBA &c,short g);
	void addRect(Matrix &imat,short x1,short y1,short x2,short y2,ColorRGBA &c,short g);
//	void addText(short x1,short y1,char *n);

	// trazo
	void drawTrazo(Matrix &mat,WORD t);
	void delTrazo(WORD t);

	void moveTrazo(WORD t,short x,short y)	
	{
	trazos[t].move(pnts,x,y);
	}

	void SwapPnts(WORD t);// interno
	void subeTrazo(WORD t);
	void bajaTrazo(WORD t);
	void tipoTrazo(WORD t);
	// nodo
	void drawNodo(Matrix &mat,WORD t,WORD p);
	void delNodo(WORD t,WORD p);
	void insNodo(WORD t,WORD p);
	inline void changeNodo(WORD t,WORD p)	
	{ 
	if (cntt==0) return;
	trazos[t].changeNodo(pnts,p); 
	}
	inline void changeNodoVisible(WORD t,WORD p)	
	{ 
	if (cntt==0) return;
	trazos[t].changeNodoVisible(pnts,p); 
	}
	inline void moveNodo(WORD t,WORD p,short x,short y)	
	{ 
	pnts[trazos[t].puntos+p].move(x,y);
	}

// agregar sensores
	void addSensorObj(Matrix &mat);
	void addSensorPnt(Matrix &mat,short tr);

// mostrar dibujo
	void drawu(Matrix &mat);// sin grupos
	void draw(Matrix &mat);

// manejo de grupos
	void makegroup(void);
	void drawg(Matrix &mat);
	void moveg(Matrix &imat,byte g,short dx,short dy);

	void BBox(Matrix &mat);

	void unirUltimo(void);
// manejo de memoria
	int memsize(void);
	char *memput(char *l);
	char *memget(char *l);

	void memset(char *l);// sin alojamiento
};

#endif