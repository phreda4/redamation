//
//  Trazo y Figura
//
#include "stdafx.h"
#include "flib.h"
#include "graph.h"
//................
#include "trazo.h"


//------------------------- TRAZO --------------------------------
void Linea::ini(int x,int y)
{
NodoLinea *p=pnt;
p->x=x;p->y=y;cntp=1;
}

void Linea::add(int x,int y)
{
NodoLinea *a,*p=&pnt[cntp-1];
if (p->x==x && p->y==y) return;// no agregar puntos iguales
a=p-1;
//int ang=angle(a->x,a->y,p->x,p->y,x,y);
int ang=recta2(a->x,a->y,p->x,p->y,x,y);
if (ang<15)
	{ 
	p->x=x;p->y=y;
	return; 
	}
p++;p->x=x;p->y=y;
if (cntp<PUNTOSPORLINEA) cntp++;  
return;
}

void Linea::addDraw(int x,int y)
{
NodoLinea *a,*p=&pnt[cntp-1];
if (p->x==x && p->y==y) return;// no agregar puntos iguales
SetPixelXOR();
a=p-1;
//int ang=angle(a->x,a->y,p->x,p->y,x,y);
int ang=recta2(a->x,a->y,p->x,p->y,x,y);
if (ang<10)
	{ 
	Line(a->x,a->y,p->x,p->y);
	p->x=x;p->y=y;
	Line(a->x,a->y,x,y);
	SetPixelCopy();
	return; 
	}
Line(p->x,p->y,x,y);	
SetPixelCopy();
p++;p->x=x;p->y=y;
if (cntp<PUNTOSPORLINEA) cntp++;  
return;
}

void Linea::move(int x,int y)
{
pnt[cntp-1].x=x;pnt[cntp-1].y=y;
}

void Linea::moveNodo(unsigned short e,int x,int y)
{
pnt[e].x=x;pnt[e].y=y;
}

void Linea::draw()
{
if (cntp<1) return;
NodoLinea *p=pnt;
int x1,y1,x=p->x,y=p->y;p++;
for (unsigned short i=1;i<cntp-1;i++,p++)
	{
	x1=p->x;y1=p->y;
	Line(x,y,x1,y1);
	x=x1;y=y1;
	}
}

void Linea::drawLast()
{
if (cntp<2) return;
SetColor(0xffff);
NodoLinea *a=&pnt[cntp-2],*p=&pnt[cntp-1];
Line(a->x,a->y,p->x,p->y);
}

void Linea::del(unsigned short nro)
{
if (nro<0 || nro>=cntp) return;
for (unsigned short i=nro+1;i<cntp;i++)
  pnt[i-1]=pnt[i];
cntp--;
}

