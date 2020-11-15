/////////////////////////////////////////////////////////
//
// GUI.CPP
//
// Redamation source
/////////////////////////////////////////////////////////
#include "stdafx.h"
#include "redalib.h"
#include "graficos.h"
#include "gui.h"
#include "charlist.h"

// variable externa
Gui gui;
//-------------------------------------------------------
void GuiList::draw(CNativeBitmap *skin)
{
switch(tipo) {
case GUIBTN:// boton grafico
	{
	if (estado==0)
		Pantalla.TransparentBlt(x1,y1,x2-x1,y2-y1,skin,0xffff,DATA.a.i,DATA.b.i); 
	else
		Pantalla.TransparentBlt(x1,y1,x2-x1,y2-y1,skin,0xffff,DATA.c.i,DATA.d.i);
	}
	break;
case GUIBTNSRV:
	{
	Pantalla.TransparentBlt(x1,y1,x2-x1,y2-y1,skin,0xffff,DATA.a.i,DATA.b.i); 
	int y=(*(DATA.e.p))*DATA.d.i;
	Pantalla.TransparentBlt(x1,y1+y,DATA.c.i,DATA.d.i,skin,0xffff,DATA.a.i+DATA.c.i,DATA.b.i+y);
	}
	break;
case GUIBTNSRH:
	{
	Pantalla.TransparentBlt(x1,y1,x2-x1,y2-y1,skin,0xffff,DATA.a.i,DATA.b.i); 
	int x=(*(DATA.e.p))*DATA.c.i;
	Pantalla.TransparentBlt(x1+x,y1,DATA.c.i,DATA.d.i,skin,0xffff,DATA.a.i+x,DATA.b.i+DATA.d.i);
	}
	break;
case GUIBTNSV:
	{
	Pantalla.TransparentBlt(x1,y1,x2-x1,y2-y1,skin,0xffff,DATA.a.i,DATA.b.i); 
	if (estado!=0)
		{
		int y=(*(DATA.e.p))*DATA.d.i;
		Pantalla.TransparentBlt(x1,y1+y,DATA.c.i,DATA.d.i,skin,0xffff,DATA.a.i+DATA.c.i,DATA.b.i+y);
		}
	}
	break;
case GUIBTNSH:
	{
	Pantalla.TransparentBlt(x1,y1,x2-x1,y2-y1,skin,0xffff,DATA.a.i,DATA.b.i); 
	if (estado!=0)
		{
		int x=(*(DATA.e.p))*DATA.c.i;
		Pantalla.TransparentBlt(x1+x,y1,DATA.c.i,DATA.d.i,skin,0xffff,DATA.a.i+x,DATA.b.i+DATA.d.i);
		}
	}
	break;
case GUICOMBOV:// combo
	{
	int y=(*(DATA.e.p))*DATA.d.i;
	Pantalla.TransparentBlt(x1,y1,DATA.c.i,DATA.d.i,skin,0xffff,DATA.a.i+DATA.c.i,DATA.b.i+y);
	}
	break;
case GUICOMBOH:
	{
	int x=(*(DATA.e.p))*DATA.c.i;
	Pantalla.TransparentBlt(x1,y1,DATA.c.i,DATA.d.i,skin,0xffff,DATA.a.i+x,DATA.b.i+DATA.d.i);
	}
	break;
case GUIZNAI:
	{
	DATA.a._f1(this);
//	d->draw(this);
	}
	break;
case GUIMAT:
	{
	if (DATA.d._f1!=NULL) DATA.d._f1(this);
	//if (d->draw!=NULL) d->draw(this);
	}
	break;
case GUIINDEX:
	{
	int x,y,i;
	char tt[8];
//	int cntvisible=(x2-x1)/14;
	y=(y1+y2)>>1;
	x=x2-7;
	Pantalla.Color(0);
	Pantalla.HLine(x1,y,x2);
	for (i=0;i<*(DATA.a.p);i++)// cntvisible
		{
		sprintf(tt,"%d",i+1);
		if (i==*(DATA.b.p))
			{
			Pantalla.ColorRGB(200,200,0);
			Pantalla.DBotonD(x-7,y1,x+7,y2,tt);
			}
		else
			{
			Pantalla.ColorRGB(0,200,0);
			Pantalla.DBoton(x-7,y1,x+7,y2,tt);
			}
		x-=14;
		}
	}
	break;
case GUIINDEXD:
	{
	int x,y,i;
	char tt[8];
	//	int cntvisible=(x2-x1)/14;
	x=(x1+x2)>>1;
	y=y1+6;
	Pantalla.Color(0);
	Pantalla.VLine(x,y1,x2);
//	ColorRGBA c1,c2=color;c1=c2;c1.mix(colorblanco,128);
	for (i=0;i<*(DATA.a.p);i++)// cntvisible
		{
		sprintf(tt,"%c",'A'+i);
		if (i==*(DATA.b.p))
			{
			Pantalla.ColorRGB(200,200,0);
			Pantalla.DBotonD(x1,y-6,x2,y+6,tt);
			}
		else
			{
			Pantalla.ColorRGB(0,200,0);
			Pantalla.DBoton(x1,y-6,x2,y+6,tt);
			}
		y+=12;
		}
	}
	break;
	};
}
//-------------------------------------------------------
void GuiList::calc(short hitx,short hity)
{
switch (tipo) {
case GUIBTNSRV:
case GUIBTNSV:
	{
	*(DATA.e.p)=(hity-y1)/DATA.d.i;
	if (*(DATA.e.p)*DATA.d.i>=y2) (*(DATA.e.p))--;
	}
	break;
case GUIBTNSRH:
case GUIBTNSH:
	{
	*(DATA.e.p)=(hitx-x1)/DATA.c.i;
	if (*(DATA.e.p)*DATA.c.i>=x2) (*(DATA.e.p))--;
	}
	break;
case GUIZNAI:
	{
	if (DATA.b._f2!=NULL) DATA.b._f2(this,hitx,hity);
	//if (d->calc!=NULL)	d->calc(this,hitx,hity);
	}
	break;
case GUIMAT:
	{
	int h=(x2-x1)/DATA.c.i;
	int v=(y2-y1)/DATA.b.i;
	int a=(hitx-x1)/h;if (a==DATA.c.i) a--;
	int b=(hity-y1)/v;if (b==DATA.b.i) b--;
	*(DATA.a.p)=a+(DATA.c.i)*b;
	}
	break;
case GUIINDEX:
	{
	int lugar=(hitx-x1)/14;//	int max=(x2-x1)/14;
	*(DATA.b.p)=16-lugar;
	}
	break;
case GUIINDEXD:
	{
	int lugar=(hity-y1)/12;
	*(DATA.b.p)=lugar;
	}
	break;
	};
};
//-------------------------------------------------------
GuiList guicombo[3];
//-------------------------------------------------------
void execombov(GuiList *g)
{
short x1,y1,x2,y2;
x1=g->x1;y1=g->y1;
x2=g->x2;y2=g->y2;
if (y1<160)	
	{
	y2=y1+(g->DATA.f.i*(y2-y1));
//	if (y2>320) { a=y2-319;y1-=a;y2-=a;};
	}
else
	{
	y1=y2-(g->DATA.f.i*(y2-y1));
	//y1=240;
//	if (y1<0) { a=-y1;y1+=a;y2+=a;};
	}
GuiList *gl=guicombo;
gl->tipo=GUIBTNSRV;
gl->x1=x1;gl->y1=y1;gl->x2=x2;gl->y2=y2;
gl->id=9999;gl->DATA=g->DATA;
gl++;
gl->tipo=GUIZNAE;
gl->x1=0;gl->y1=0;gl->x2=239;gl->y2=319;
gl->id=9998;
gl++;
gl->tipo=GUIEND;
bool salir=false;
//Gui combo;
//while (!salir) {
	gui.run(guicombo);
//	};
}
//-------------------------------------------------------
void execomboh(GuiList *g)
{
short x1,y1,x2,y2;
x1=g->x1;y1=g->y1;
x2=g->x2;y2=g->y2;
if (x1<160)	
	{
	x2=x1+(g->DATA.f.i*(x2-x1));
//	if (y2>320) { a=y2-319;y1-=a;y2-=a;};
	}
else
	{
	x1=x2-(g->DATA.f.i*(x2-x1));
	//y1=240;
//	if (y1<0) { a=-y1;y1+=a;y2+=a;};
	}
GuiList *gl=guicombo;
gl->tipo=GUIBTNSRH;
gl->x1=x1;gl->y1=y1;gl->x2=x2;gl->y2=y2;
gl->id=9999;gl->DATA=g->DATA;
gl++;
gl->tipo=GUIZNAE;
gl->x1=0;gl->y1=0;gl->x2=239;gl->y2=319;
gl->id=9998;
gl++;
gl->tipo=GUIEND;
bool salir=false;
//Gui combo;
//while (!salir) {
	gui.run(guicombo);
//	};
}
//-------------------------------------------------------
void execombod(GuiList *g)
{
short x1,y1,x2,y2;
x1=g->x1;y1=g->y1;
x2=g->x2;y2=g->y2;
if (y1<160)	
	{
	y2=y1+(g->DATA.f.i*(y2-y1));
//	if (y2>320) { a=y2-319;y1-=a;y2-=a;};
	}
else
	{
	y1=y2-(g->DATA.f.i*(y2-y1));
	//y1=240;
//	if (y1<0) { a=-y1;y1+=a;y2+=a;};
	}
GuiList *gl=guicombo;
gl->tipo=GUIBTNSRV;
gl->x1=x1;gl->y1=y1;gl->x2=x2;gl->y2=y2;
gl->id=9999;gl->DATA=g->DATA;
gl++;
gl->tipo=GUIZNAE;
gl->x1=0;gl->y1=0;gl->x2=239;gl->y2=319;
gl->id=9998;
gl++;
gl->tipo=GUIEND;
bool salir=false;
//Gui combo;
//while (!salir) {
	gui.run(guicombo);
//	};
}

//------------------------------------------------------- GUI
void Gui::draw(GuiList *g)
{
while (g->tipo!=GUIEND) {
	if (g->tipo==GUICASE)
		{
		if (*(g->DATA.a.p)==(g->DATA.b.i)) draw((g->DATA.c.gl));
		}
	else g->draw(skin);
	g++;
	}
}

void Gui::draw(void)
{
if (estadotrazo) {
	Pantalla.toBuff(buffer);
	Pantalla.Color(color);
	switch (modo) {
		case 1:	trazo.draw();break;
		case 2:	trazo.drawPoli();break;
		case 3: Pantalla.Line(x1,y1,x2,y2);break;
		case 4:	Pantalla.Box(x1,y1,x2,y2);break;
		case 5:	Pantalla.Circle(x1,y1,dist(x1,y1,x2,y2));break;
		case 6: Pantalla.Ellipse(x1,y1,abs(x1-x2),abs(y1-y2));break;
		}
	}
else {
	if (_drawmain!=NULL) _drawmain();
	GuiList *g1=_lgui;
	while (g1->tipo!=GUIEND) {
		if (g1->tipo==GUICASE)
			{
			if (*(g1->DATA.a.p)==(g1->DATA.b.i)) draw((g1->DATA.c.gl));
			}
		else g1->draw(skin);
		g1++;
		};
	}
Pantalla.redraw();
}
//-------------------------------------------------------
GuiList *Gui::hit(WORD x,WORD y,GuiList *g)
{
GuiList *g2;
while (g->tipo!=GUIEND) {
	if (g->tipo==GUICASE) 
		{
		if (*(g->DATA.a.p)==(g->DATA.b.i)) {
			g2=hit(x,y,g->DATA.c.gl);
			if (g2!=NULL) return g2;
			};
		}
	else if (g->in(x,y)) return g;
	g++;
	}
return NULL;
};
//-------------------------------------------------------
GuiList *Gui::hit(WORD x,WORD y)
{
GuiList *g2,*g1=_lgui;
while (g1->tipo!=GUIEND) {
	if (g1->tipo==GUICASE)
		{
		if (*(g1->DATA.a.p)==(g1->DATA.b.i)) {
			g2=hit(x,y,g1->DATA.c.gl);
			if (g2!=NULL) return g2;
			};
		};
	if (g1->in(x,y)) return g1;
	g1++;
	}
return NULL;
};
//-------------------------------------------------------
void Gui::ini(GuiList *g)
{
while (g->tipo!=GUIEND) {
	if (g->tipo==GUICASE)
		ini(g->DATA.c.gl);
	else
		g->estado=0;
	g++;
	};
}
//-------------------------------------------------------
bool gui_redibujar;

WORD Gui::run(GuiList *gl,EveList *el,void (*_dm)(void))
{
_drawmain=_dm;
_lgui=gl;
_leve=el;
// inicia los componentes
if (_lgui==NULL) return 0xffff;
ini(_lgui);
// inicia los timer
EveList *e1=_leve;
if (e1!=NULL)
	{
	while (e1->value!=0) {
		e1->ini();
		e1++;
		};
	}
//int valor,ahora;
//DWORD FPSLastTime=GetTickCount();
WORD cod;
bool salir=false,dibujando=false;
gui_redibujar=false;
draw();// dibuja
estadotrazo=false;
WORD x,y;
gactual=NULL;
while (!salir) {
	update();
	if (click())
		{
		dibujando=true;
		penpos(&x,&y);
		x1=x2=x;y1=y2=y;
		gactual=hit(x,y);
		if (gactual==NULL) { 
			estadotrazo=true;
			Pantalla.fromBuff(buffer);
			trazo.ini(x,y);
			}
		else
			{// apreto algo
			gactual->calc(x,y);
			gactual->estado=1;
			gui_redibujar=true;
			}
		}
	else if (pendown())
		{
		penpos(&x,&y);
		if (x!=x2 || y!=y2)
			{
			x2=x;y2=y;
			if (gactual==NULL) {
				if (modo>0) {
					trazo.add(x,y);
					gui_redibujar=true;
					};
				}
			else if (gactual->in(x,y))
				{
				gactual->calc(x,y);// move
				gui_redibujar=true;
				}
			}
		}
	else if (dibujando)
		{
		dibujando=false;
		if (gactual==NULL) {
			estadotrazo=false;
			if (modo>0) {
				salir=true;
				cod=GUITRZ;
				}
		} else {
			if (gactual->in(x,y)) {// ejecuto zona
				cod=gactual->id;
				if (cod!=0) salir=true;
				switch (gactual->tipo) {
				case GUICOMBOV:
					execombov(gactual);
					break;
				case GUICOMBOH:
					execomboh(gactual);
					break;
				case GUIDIALOG:
					break;
					};
				_drawmain=_dm;
				_lgui=gl;
				_leve=el;
				};
			gactual->estado=0;
			}
		gui_redibujar=true;
		}
	
/*
	ahora=GetTickCount();// control de tiempo
	valor=ahora-FPSLastTime;
	if (valor<0) valor=GetTickCount();
	FPSLastTime=ahora;
	if (e1!=NULL)
		{
		while (e1->value!=0) {
			e1->accumulator+=valor;
			if (e1->accumulator>e1->valor)	{
				e1->accumulator=0;
				e1->_accion();
				redibujar=true;
				}
			e1++;
			};
		}
*/
	// dibujar si hace falta
	if (gui_redibujar) {
		gui_redibujar=false;
		draw();
		};
	};
return cod;
};

/*
WORD dialog(GuiList *gl,EveList *el=NULL,void (*_dm)(void)=NULL,void (*opc)(WORD e))
{
bool fin=false;
WORD evento;
gui.modo=1;
while (!fin)
	{
	evento=run(gl,el,_dm);
	opc(evento);
	}

};
*/
//-------------------------------------------------------
void IceScr(void)
{
ColorRGBA c;
c.set(0,0,255,100);
Pantalla.Color(c);
Pantalla.FillBox(0,0,239,319);
c.set(255,255,255,255);
Pantalla.Color(c);
};
/////////////////////////////////////////////////////////
//
// SENSOR
//
/////////////////////////////////////////////////////////
#define MAXSENS 1024

GuiSensor mint[MAXSENS];
int cntmint,nromint;
//-------------------------------------------------------
void clearSensor()
{
nromint=cntmint=0;
mint->t=-1;
}
//-------------------------------------------------------
GuiSensor *getSensor(short x,short y)
{
GuiSensor *p=mint;
while (p->t!=-1)
	{
	if (p->in(x,y))	return p;
	p++;
	}
return NULL;
}
//-------------------------------------------------------
int getISensor(short x,short y)
{
int cnt=0;
GuiSensor *g=mint;
int nro=-1;
while  (g->t!=-1)
	{
	if (g->in(x,y))
		{
		if (nro==-1) nro=g->t;
		if (cnt==nromint) 
			{
			nromint++;
			return g->t;
			}
		cnt++;
		}
	g++;
	}
nromint=(nro==-1)?0:1; 
return nro;
}
//-------------------------------------------------------
void addSensor(short x1,short y1,short x2,short y2,int v)
{
if (cntmint>=MAXSENS) return;
mint[cntmint].x1=x1;mint[cntmint].y1=y1;
mint[cntmint].x2=x2;mint[cntmint].y2=y2;
mint[cntmint].t=v;
cntmint++;
mint[cntmint].t=-1;
}