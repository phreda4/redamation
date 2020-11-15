#include "stdafx.h"
#include "punto.h"
#include "graficos.h"

void Trazo2D::draw(Matrix &mat,Grupo *gr,Punto2D *pnts)
{
WORD i;
long x1,y1,x2,y2,xc,yc,xa,ya;
Punto2D *p=&pnts[puntos];
Grupo *grg=&gr[p->tipo>>2];
mat.transforma(p->x+grg->x,p->y+grg->y,&x1,&y1);
p++;
xa=x1;ya=y1;
// color
Pantalla.Color(color);
Pantalla.Alpha(color.a);
switch(tipo) {
case 0:// poligono 
	for (i=1;i<cntpuntos;i++,p++)
		{
		grg=&gr[p->tipo>>2];
		mat.transforma(p->x+grg->x,p->y+grg->y,&x2,&y2);
		switch (p->tipo&3) {
	      case 0:
			Pantalla.pSegmento(x1,y1,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 1:
			xc=x2;yc=y2;	
			break;
		  case 2:
			Pantalla.pSPlineC(x1,y1,xc,yc,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 3:
			Pantalla.pSegmento(x1,y1,xa,ya);
			xa=x1=x2;ya=y1=y2;
			break;
			};
		}
	p--;
	if (p->tipo==1)	Pantalla.pSPlineC(x1,y1,xc,yc,xa,ya); else Pantalla.pSegmento(x1,y1,xa,ya);
	Pantalla.drawPoli();
//	Pantalla.drawLines();
	break;
case 1:// borde
	for (i=1;i<cntpuntos;i++,p++)
		{
		grg=&gr[p->tipo>>2];
		mat.transforma(p->x+grg->x,p->y+grg->y,&x2,&y2);
		switch (p->tipo&3) {
	      case 0:
			Pantalla.Line(x1,y1,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 1:
			xc=x2;yc=y2;	
			break;
		  case 2:
			Pantalla.SPlineC(x1,y1,xc,yc,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 3:
			x1=x2;y1=y2;
			break;
			};
		}
	p--;
	if (p->tipo==1)	Pantalla.SPlineC(x1,y1,xc,yc,xa,ya); else Pantalla.Line(x1,y1,xa,ya);
	break;
case 2:// lineas
	for (i=1;i<cntpuntos;i++,p++)
		{
		grg=&gr[p->tipo>>2];
		mat.transforma(p->x+grg->x,p->y+grg->y,&x2,&y2);
		switch (p->tipo&3) {
	      case 0:
			Pantalla.Line(x1,y1,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 1:
			xc=x2;yc=y2;	
			break;
		  case 2:
			Pantalla.SPlineC(x1,y1,xc,yc,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 3:
			x1=x2;y1=y2;
			break;
			};
		}
	break;
case 4:// circulos
	//Pantalla.Circle(x1,y1,p->x);// falta escala y eso
//	mat.transforma(p->x,p->y,&x2,&y2);
	Pantalla.Ellipse(x1,y1,p->x,p->y,true);
	break;
case 5:// linea gruesa
	for (i=1;i<cntpuntos;i++,p++)
		{
		grg=&gr[p->tipo>>2];
		mat.transforma(p->x+grg->x,p->y+grg->y,&x2,&y2);
		switch (p->tipo&3) {
	      case 0:
			Pantalla.Line(x1,y1,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 1:
			xc=x2;yc=y2;	
			break;
		  case 2:
			Pantalla.SPlineC(x1,y1,xc,yc,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 3:
			x1=x2;y1=y2;
			break;
			};
		}
	break;
};
Pantalla.Alpha(255);
}

void Trazo2D::drawu(Matrix &mat,Punto2D *pnts)
{
WORD i;
long x1,y1,x2,y2,xc,yc,xa,ya;
Punto2D *p=&pnts[puntos];
mat.transforma(p->x,p->y,&x1,&y1);
p++;
xa=x1;ya=y1;
// color
Pantalla.Color(color);
Pantalla.Alpha(color.a);
switch(tipo) {
case 0:// poligono 
	for (i=1;i<cntpuntos;i++,p++)
		{
		mat.transforma(p->x,p->y,&x2,&y2);
		switch (p->tipo&3) {
	      case 0:
			Pantalla.pSegmento(x1,y1,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 1:
			xc=x2;yc=y2;	
			break;
		  case 2:
			Pantalla.pSPlineC(x1,y1,xc,yc,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 3:
			Pantalla.pSegmento(x1,y1,xa,ya);
			xa=x1=x2;ya=y1=y2;
			break;
			};
		}
	p--;
	if (p->tipo==1)	Pantalla.pSPlineC(x1,y1,xc,yc,xa,ya); else Pantalla.pSegmento(x1,y1,xa,ya);
	Pantalla.drawPoli();
//	Pantalla.drawLines();
	break;
case 1:// borde
	for (i=1;i<cntpuntos;i++,p++)
		{
		mat.transforma(p->x,p->y,&x2,&y2);
		switch (p->tipo&3) {
	      case 0:
			Pantalla.Line(x1,y1,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 1:
			xc=x2;yc=y2;	
			break;
		  case 2:
			Pantalla.SPlineC(x1,y1,xc,yc,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 3:
			x1=x2;y1=y2;
			break;
			};
		}
	p--;
	if (p->tipo==1)	Pantalla.SPlineC(x1,y1,xc,yc,xa,ya); else Pantalla.Line(x1,y1,xa,ya);
	break;
case 2:// lineas
	for (i=1;i<cntpuntos;i++,p++)
		{
		mat.transforma(p->x,p->y,&x2,&y2);
		switch (p->tipo&3) {
	      case 0:
			Pantalla.Line(x1,y1,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 1:
			xc=x2;yc=y2;	
			break;
		  case 2:
			Pantalla.SPlineC(x1,y1,xc,yc,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 3:
			x1=x2;y1=y2;
			break;
			};
		}
	break;
case 4:// circulos
	//Pantalla.Circle(x1,y1,p->x);// falta escala y eso
//	mat.transforma(p->x,p->y,&x2,&y2);
	Pantalla.Ellipse(x1,y1,p->x,p->y,true);
	break;
case 5:// linea gruesa
	for (i=1;i<cntpuntos;i++,p++)
		{
		mat.transforma(p->x,p->y,&x2,&y2);
		switch (p->tipo&3) {
	      case 0:
			Pantalla.Line(x1,y1,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 1:
			xc=x2;yc=y2;	
			break;
		  case 2:
			Pantalla.SPlineC(x1,y1,xc,yc,x2,y2);
			x1=x2;y1=y2;
			break;
		  case 3:
			x1=x2;y1=y2;
			break;
			};
		}
	break;
};
Pantalla.Alpha(255);
}

void Trazo2D::drawEdit(Matrix &mat,Punto2D *pnts,WORD pt)
{
if (pt>=cntpuntos) pt=cntpuntos-1;
WORD i;
long x1,y1;
int d;
Punto2D *p=&pnts[puntos];
for (i=0;i<cntpuntos;i++,p++)
	{
	mat.transforma(p->x,p->y,&x1,&y1);
	if (i==pt)
		{ 
//		Pantalla.Circle(x1,y1,loop(4,1)+1);
		d=pingpong(4,1)+1;
		Pantalla.Box(x1-d,y1-d,x1+d,y1+d); 
		}
	else
		{ 
		Pantalla.Box(x1-2,y1-2,x1+2,y1+2); 
		}
	}
};

void Trazo2D::drawEditT(Matrix &mat,Punto2D *pnts)
{
long x1,y1,x2,y2;
getBox(mat,pnts,&x1,&y1,&x2,&y2);
WORD m=style1[loop(8,1)];//WORD m=rec2[loop(2,1)];
x1--;y1--;x2++;y2++;
Pantalla.FLine(x1,y1,x2,y1,m);
Pantalla.FLine(x2,y1+1,x2,y2-1,m);
Pantalla.FLine(x2,y2,x1,y2,m);
Pantalla.FLine(x1,y2-1,x1,y1+1,m);
};

void Trazo2D::getBox(Matrix &mat,Punto2D *pnts,long *x1,long *y1,long *x2,long *y2)
{
WORD i;
long x,y;
Punto2D *p=&pnts[puntos];
mat.transforma(p->x,p->y,x1,y1);
*x2=*x1;*y2=*y1;
p++;
for (i=1;i<cntpuntos;i++,p++)
	{
	mat.transforma(p->x,p->y,&x,&y);
	if (x<*x1) *x1=x;
	if (y<*y1) *y1=y;
	if (x>*x2) *x2=x;
	if (y>*y2) *y2=y;
	}
}

/*
void Trazo2D::BBox(Matrix &mat)
{
Punto2D *pdib=puntos;
long x,y;
mat.transforma(pdib->x,pdib->y,&x,&y);
x1=x2=(short)x;
y1=y2=(short)y;
pdib++;
for (short i=1;i<cntpuntos;i++,pdib++)
	{
	mat.transforma(pdib->x,pdib->y,&x,&y);
	if (x<x1) x1=(short)x;
	if (y<y1) y1=(short)y;
	if (x>x2) x2=(short)x;
	if (y>y2) y2=(short)y;
	}
}
*/
void Trazo2D::move(Punto2D *pnts,short x,short y)
{
Punto2D *pdib=&pnts[puntos];
for (short i=0;i<cntpuntos;i++,pdib++)
	{ pdib->x+=x;pdib->y+=y; }
}

void Trazo2D::changeNodo(Punto2D *pnts,WORD p) 
{ 
if (p<1 || p>=cntpuntos-1) return;
Punto2D *a=&pnts[puntos+p],*b=&pnts[puntos+p+1],*c=&pnts[puntos+p-1],*d=&pnts[puntos+p+2];
switch (a->tipo) {
case 0:
	a->tipo=1;
	b->tipo=2;
	if (d->tipo==2) d->tipo=0;
	break;
case 1:
	a->tipo=0;
	b->tipo=0;
	break;
case 2:
	c->tipo=0;
	a->tipo=0;
	break;
	}
};

void Trazo2D::changeNodoVisible(Punto2D *pnts,WORD p) 
{ 
if (p<1 || p>=cntpuntos-1) return;
Punto2D *a=&pnts[puntos+p];
switch (a->tipo) {
case 0:	a->tipo=3;break;
case 1:	break;
case 2:	break;
case 3:	a->tipo=0;break;
	}
};

//------------------------- TRAZO --------------------------------
void Linea::ini(short x,short y)
{
Punto2D *p=pnt;
p->x=x;p->y=y;cntp=1;
pp.x=x;pp.y=y;
}

void Linea::add(short x,short y)
{
Punto2D *a,*p=&pnt[cntp-1];
if (p->x==x && p->y==y) return;// no agregar puntos iguales
if (cntp<2) 
	{
	p++;p->x=x;p->y=y;cntp++;
	return; 
	}
a=p-1;
//int ang=angle(a->x,a->y,p->x,p->y,x,y);
int ang=recta2(a->x,a->y,p->x,p->y,x,y);
if (ang<10)
	{ 
	p->x=x;p->y=y;
	return; 
	}
p++;p->x=x;p->y=y;
if (cntp<PUNTOSPORLINEA) cntp++;  
}

void Linea::addPinta(short x,short y,short g)
{
if (pp.x==x && pp.y==y) return;// no agregar puntos iguales

add(pp.x-g,pp.y-g);
add(x+g,y+g);

pp.x=x;pp.y=y;
}
/*
void Linea::addBox()
{
if (cntp<1) return;
Punto2D *p=pnt;
int r;
int x1,y1,x=p->x,y=p->y;p++;
int x2,y2,x3,y3;
for (unsigned short i=1;i<cntp;i++,p++)
	{
	x1=p->x;y1=p->y;
	r=lines_intersect(x,y,x1,y1,x2,y2,x3,y3,&xr,&yr);
	if (r==DO_INTERSECT)
		{
		addintersect(i,j,xr,yr);
		}
	x=x1;y=y1;
	}
}
*/

void Linea::draw(void)
{
if (cntp<1) return;
Punto2D *p=pnt;
int x1,y1,x=p->x,y=p->y;p++;
for (unsigned short i=1;i<cntp;i++,p++)
	{
	x1=p->x;y1=p->y;
	Pantalla.Line(x,y,x1,y1);
	x=x1;y=y1;
	}
}

void Linea::drawPoli(void)
{
if (cntp<1) return;
Punto2D *p=pnt;
int xa,ya,x1,y1,x=p->x,y=p->y;p++;
xa=x;ya=y;
for (WORD i=1;i<cntp;i++,p++)
	{
	x1=p->x;y1=p->y;
	//Pantalla.Line(x,y,x1,y1);
	Pantalla.pSegmento(x,y,x1,y1);
	x=x1;y=y1;
	}
Pantalla.pSegmento(x,y,xa,ya);
Pantalla.drawPoli();
//Pantalla.Line(x,y,xa,ya);
}

void Linea::drawant(void)
{
if (cntp<1) return;
Punto2D *p=pnt;
int x1,y1,x=p->x,y=p->y;p++;
WORD m=style1[loop(8,1)];//WORD m=rec2[loop(2,1)];
Pantalla.XOR(true);
Pantalla.Color(0xffff);
for (unsigned short i=1;i<cntp;i++,p++)
	{
	x1=p->x;y1=p->y;
	Pantalla.FLine(x,y,x1,y1,m);
	x=x1;y=y1;
	}
Pantalla.XOR(false);
}

void Linea::move(short x,short y)
{
pnt[cntp-1].x=x;pnt[cntp-1].y=y;
}

void Linea::moveNodo(unsigned short e,short x,short y)
{
pnt[e].x=x;pnt[e].y=y;
}

void Linea::del(unsigned short nro)
{
if (nro<0 || nro>=cntp) return;
for (unsigned short i=nro+1;i<cntp;i++)
  pnt[i-1]=pnt[i];
cntp--;
}

void Linea::optimize(void)
{
short ang,mx,my,i;
char tipo;
Punto2D *a,*a2,*b,*plinea=pnt;
plinea->tipo=0;plinea++;
plinea->tipo=0;plinea++;
for (i=2;i<cntp;i++,plinea++)
	{
	tipo=0;
	a=plinea-2;b=plinea-1;
	ang=angle(a->x,a->y,b->x,b->y,plinea->x,plinea->y);
	if (a->tipo!=1)
		{
		if (ang<750) { b->tipo=1;tipo=2;}	// transforma a spline
		}
	else if (i>3)		// puede simplificar
		{
		a2=plinea-3;
		if (cospl(a2->x,a2->y,a->x,a->y,b->x,b->y,plinea->x,plinea->y,&mx,&my)<1)
			{
			a->x=mx;a->y=my;
			b->x=plinea->x;b->y=plinea->y;
			plinea--;
			del(i);
			continue;
			}
		}
	plinea->tipo=tipo;
	}
}

