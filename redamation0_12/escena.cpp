#include "stdafx.h"
#include "escena.h"
#include "gui.h"

void Escena::create(Libro *l)
{
libro=l;
vista.clear();
vista.makeMatrix(mat,imat);
//mat.invert(imat);
};

void Escena::destroy(void)
{
};

void Escena::clear(void) 
{
cntcuadros=cntelementos=0;
};

void Escena::addObj(short nro)
{
if (cntelementos>=64) return;
Elemento *e=&elementos[cntelementos];
e->hoja=nro;
e->vista.clear();
e->vista.move(cntelementos*10,cntelementos*10);
e->vista.makeMatrix(e->mat,e->imat);
cntelementos++;
};

void Escena::addTxt(char *n)
{
};

void Escena::addNro(int nro)
{
};

void Escena::addVar(void)
{
};

void Escena::addTab(void)
{
};

void Escena::move(short n,short x,short y)
{
Elemento *e=&elementos[n];
e->vista.move(x,y);
e->adjust();
};

void Escena::rotar(short n,short a)
{
Elemento *e=&elementos[n];
e->vista.rota(a);
e->adjust();
};

void Escena::escala(short n,short s)
{
Elemento *e=&elementos[n];
e->vista.scale(s);
e->adjust();
};

void Escena::addSensorO(void)//Matrix &mat)
{
int i;
char *mem;
Elemento *e=elementos;
Dibujo d;
clearSensor();
for(i=0;i<cntelementos;i++,e++)
	{
	mem=libro->get(e->hoja);
	if (mem!=NULL)
		{
		d.memset(mem);//d.memget(libro->get(e->hoja));
		d.BBox(e->mat);
		addSensor(d.x1,d.y1,d.x2,d.y2,i);
		// sensores de escala, rotacion y propiedades
		}
	}
}

void Escena::drawSelect(short n)
{
if (n>=cntelementos) return;
char t[5];
int x1,y1,x2,y2;
sprintf(t,"%d",n+1);
Dibujo d;
char *mem;
Elemento *e=&elementos[n];
mem=libro->get(e->hoja);
if (mem!=NULL)
	{
	d.memset(mem);//d.memget(libro->get(e->hoja));
	d.BBox(e->mat);
	Pantalla.Color(0xffff);
	Pantalla.XOR(true);
	WORD m=style1[loop(8,1)];//WORD m=rec2[loop(2,1)];
	x1=d.x1;y1=d.y1;x2=d.x2;y2=d.y2;
	x1--;y1--;x2++;y2++;
	Pantalla.FLine(x1,y1,x2,y1,m);
	Pantalla.FLine(x2,y1+1,x2,y2-1,m);
	Pantalla.FLine(x2,y2,x1,y2,m);
	Pantalla.FLine(x1,y2-1,x1,y1+1,m);
	Pantalla.XOR(false);
//	Pantalla.Box(d.x1,d.y1,d.x2,d.y2);
	Pantalla.ColorRGB(200,200,0);
	x1=(x1+x2)>>1;y1=(y1+y2)>>1;
	Pantalla.DBoton(x1-7,y1-6,x1+7,y1+6,t);
	};
};

#define UNIDIB 0
#define UNITEX 1
#define UNINUM 2
#define UNIVAR 3
#define UNITAB 4

void Escena::draw(void)
{
int i;
char *mem;
Elemento *e=elementos;
Dibujo d;
for(i=0;i<cntelementos;i++,e++)
	{
	switch (e->tipo) {
	case UNIDIB:// dibujo
		mem=libro->get(e->hoja);
		if (mem!=NULL) 
			{
			d.memset(mem);
			d.drawu(e->mat);
			}
		break;
	case UNITEX:
		// obtengo memoria del texto
		// dibujo texto
		break;
	case UNINUM:
		// obtengo constante
		break;
	case UNIVAR:
		// obtengo variable
		// obtengo formato de salida
		// dibujo
		break;
	case UNITAB:
		// dibujo cada uno.. luego grupo
		break;
		};
	}
};

void Escena::play(void)
{
DWORD baset=0,delay=1000/21;// fps
unsigned short tiempo=0;
bool termina=false;

while (!termina) {
	update();
	if (GetTickCount()-baset>delay) 
		{
		baset=GetTickCount();
		Pantalla.Color(0xffff);
		Pantalla.ClrScr();
		sett(tiempo);
		draw();
//		Pantalla.Color(0);
//		Pantalla.HLine(210,tiempo,218);
		Pantalla.redraw();

		tiempo+=1;if (tiempo>200) tiempo=0;
		}
	if (click()) termina=true;
	}
while (pendown()) update();
}

void Escena::moveWin(short xz,short yz,short x,short y)
{
/*
long xt1,yt1,xt2,yt2;
imat.transforma(xz,yz,&xt1,&yt1);
imat.transforma(x,y,&xt2,&yt2);
vista.move((short)(xt2-xt1),(short)(yt2-yt1));
vista.makeMatrix(mat);
mat.invert(imat);
*/
}

void Escena::rsWin(short x,short y)
{
/*
int d1=y;//d1=interpola((1<<(FRACMAT-8)),(1<<FRACMAT),y,320);
if (d1<MINZOOM) d1=MINZOOM;
if (d1>MAXZOOM) d1=MAXZOOM;
vista.scale(d1);
vista.rota(x);
vista.makeMatrix(mat);
mat.invert(imat);
*/
}
//-------------------------------------------------------------------------------------

void Escena::rew(void)// edita siempre en rew
{
int i;
char *mem;
Elemento *e=elementos;
Dibujo d;
for(i=0;i<cntelementos;i++,e++)
	{
	e->adjust();
	mem=libro->get(e->hoja);
	if (mem!=NULL)
		{
		d.memset(mem);
		d.drawu(e->mat);
		}
	}
}

void Escena::drawKey(short nro)
{
if (nro==-1) return;
Cuadros *c=cuadros;
Pantalla.Color(0);
for (short i=0;i<cntcuadros;i++,c++)
	{
	if (c->nro==nro)// los del numero
		{
		int y=30+c->tiempo;
		Pantalla.HLine(224,y,234);
		}
	}

}

void Escena::addKeyF(short nro,WORD t,Vista &d)
{
// buscar si reemplaza 1
Cuadros *pc,*c=cuadros;
pc=&cuadros[cntcuadros];
bool esta=false;
for (short i=0;i<cntcuadros;i++,c++)
	{
	if (c->tiempo==t && c->nro==nro)// reemplazar en el mismo 
		{
		pc=c;esta=true;
		break;
		}
	}
if (!esta) cntcuadros++;
pc->nro=nro;pc->tiempo=t;
pc->x=d.xt;
pc->y=d.yt;
pc->angulo=d.angle;
pc->escala=d.sc;
// ordenar por tiempo

}

void Escena::delKeyF(short nro,WORD t)
{
Cuadros *pc,*c=cuadros;
pc=&cuadros[cntcuadros];
bool esta=false;
for (short i=0;i<cntcuadros;i++,c++)
	{
	if (c->tiempo==t && c->nro==nro)// reemplazar en el mismo 
		{
		pc=c;esta=true;
		cntcuadros--;
		for (short j=i;j<cntcuadros;j++,c++)
			*c=*(c+1);
		break;
		}
	}
}

// mix=0..a
// mix=1<<16..b
#define interpolaf(a,b,mix) ((b*mix+a*((1<<16)-mix))>>16)

int escena_sett=-1;
void Escena::sett(int t)// ubica en tiempo t con busqueda total
{
if (escena_sett==t) return;
escena_sett=t;
//char tt[20];

if (cntcuadros==0) return;
int i;
Elemento *e=elementos;
for (i=0;i<cntelementos;i++,e++)
	{
	e->menorc=e->mayorc=-1;
	e->menort=0;e->mayort=1000;
	}
/*
Pantalla.Color(0);
sprintf(tt,"e:%d c:%d",cntelementos,cntcuadros);
Pantalla.TextV(0,300,tt);
*/
Cuadros *c=cuadros;
for (i=0;i<cntcuadros;i++,c++)// busqueda por fuerza bruta
	{
	e=&elementos[c->nro];
	if (c->tiempo<=t) // es menor
		{
		if (e->menort<=c->tiempo)
			{e->menort=c->tiempo;e->menorc=i;}
		}
	else// se paso el tiempo
		{
		if (e->mayort>c->tiempo)
			{e->mayort=c->tiempo;e->mayorc=i;}
		}
//	sprintf(tt,"%d) %d:%d",i,c->tiempo,c->nro);
//	Pantalla.TextV(0,i*14+14,tt);
	}
Cuadros *c1,*c2;
int delta;
e=elementos;
for (i=0;i<cntelementos;i++,e++)
	{
	if (e->menorc==-1)
		{
		if (e->mayorc==-1)	continue;
		e->menorc=e->mayorc;
		}
	else if (e->mayorc==-1)
		{
		e->mayorc=e->menorc;
		}
//	sprintf(tt,"%d) %d:%d",i,e->menorc,e->mayorc);
//	Pantalla.TextV(100,i*14+14,tt);
	c1=&cuadros[e->menorc];
	c2=&cuadros[e->mayorc];
	delta=((t-e->menort)<<16)/(e->mayort-e->menort);
	e->vista.xt=interpolaf(c1->x,c2->x,delta);
	e->vista.yt=interpolaf(c1->y,c2->y,delta);
	e->vista.angle=interpolaf(c1->angulo,c2->angulo,delta);
	e->vista.sc=interpolaf(c1->escala,c2->escala,delta);
	e->adjust();
//		sprintf(tt,"%d %d:%d",delta,c1->x,c2->x);
//		Pantalla.TextV(100,i*14+14,tt);
	}
tiempoactual=t;
}

void Escena::setto(int t)// ubica en tiempo t optimizada
{
}