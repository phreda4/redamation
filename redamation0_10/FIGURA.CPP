//
//  Trazo y Figura
//
#include "stdafx.h"
#include "flib.h"
#include "figura.h"
#include "graph.h"

//------------------------- TRAZO --------------------------------
void Trazo::ini(int x,int y,Vista &v)
{
cntp=0;
NODOP *p=&pnt[cntp++];
p->x=v.setX(x);
p->y=v.setY(y);
p->bx=p->by=-1;
}

void Trazo::add(int x,int y,Vista &v)
{
long mx,my;
int x1=v.setX(x),y1=v.setY(y);
NODOP *p,*a;
if (pnt[cntp-1].x==x1 && pnt[cntp-1].y==y1) return;
if (cntp<2)
	{
	p=&pnt[cntp];
	p->x=x1;p->y=y1;p->bx=-1;p->by=-1;
	if (cntp<1024) cntp++;  
	}
else
	{
	a=&pnt[cntp-2];
	p=&pnt[cntp-1];
	if (a->bx==-1)// no es curva
		{
		int ang=angle(a->x,a->y,p->x,p->y,x1,y1);
		if (ang<70) // es una recta
			{ p->x=x1;p->y=y1; }
/*	    else if (ang<450)// es una curva
			{
            mx=(a->x+x1)>>1;my=(a->y+y1)>>1;
			a->bx=mx+((p->x-mx)<<1);a->by=my+((p->y-my)<<1);
			p->x=x1;p->y=y1;
			}*/
		else
			{
			p=&pnt[cntp];
			p->x=x1;p->y=y1;p->bx=-1;p->by=-1;
			if (cntp<1024) cntp++;  
	      	}
		}
	else  // es curva.. simplifica ?
		{
		if (cocurva(a->x,a->y,a->bx,a->by,p->x,p->y,x1,y1,&mx,&my)<2) // angulo de igualdad
			{
			a->bx=mx;a->by=my;
      		p->x=x1;p->y=y1;
			}
		else
			{
			p=&pnt[cntp];
			p->x=x1;p->y=y1;p->bx=-1;p->by=-1;
			if (cntp<1024) cntp++;  
	      	}
		}
	}
}

void Trazo::move(int x,int y,Vista &v)
{
pnt[cntp-1].x=v.setX(x);
pnt[cntp-1].y=v.setY(y);
}

void Trazo::moveNodo(int e,int x,int y,Vista &v)
{
pnt[e].x=v.setX(x);
pnt[e].y=v.setY(y);
}

void Trazo::drawDraw(Vista &v)
{
if (cntp<1) return;

SetColor(0xffff);
NODOP *a,*p=pnt;
int x1,y1,x=v.getX(p->x),y=v.getY(p->y);a=p;p++;
for (int i=1;i<cntp-1;i++,p++)
	{
	x1=v.getX(p->x);y1=v.getY(p->y);
	if (a->bx==-1)
		Line(x,y,x1,y1);
	else
		SPlin(x,y,v.getX(a->bx),v.getY(a->by),x1,y1);
	x=x1;y=y1;a=p;
	}
}

void Trazo::drawEdit(Vista &v,int nro) 
{
if (cntp<1) return;
NODOP *a,*p=pnt;
int x=v.getX(p->x),y=v.getY(p->y);a=p;p++;
int x1,y1;
clearSensor();
SetColor(0xffff);
XBox(x-3,y-3,x+3,y+3);
addSensor(x-3,y-3,x+3,y+3,1);
for (int i=1;i<cntp-1;i++,p++)
	{
	x1=v.getX(p->x);y1=v.getY(p->y);
	if (a->bx==-1)
		Line(x,y,x1,y1);
	else
		SPlin(x,y,v.getX(a->bx),v.getY(a->by),x1,y1);
	x=x1;y=y1;a=p;
	Box(x-3,y-3,x+3,y+3);
	addSensor(x-3,y-3,x+3,y+3,i+1);
	}

/*
for (int i=1;i<cntp;i++,p++)
	{
	x1=v.getX(p->x);y1=v.getY(p->y);
	if (sp>0) sp--;
	else 
		{
		if (p->tipo==2)
			{
//		SPline(x,y,p->x,p->y,p[1].x,p[1].y,p[2].x,p[2].y);
			cx=(v.getX(p[1].x)+x1)>>1;cy=(v.getY(p[1].y)+y1)>>1;
			SetRGB(255,0,0);XCircle(cx,cy,3);//SetColor(0xffff);
			addSensor(cx-3,cy-3,cx+3,cy+3,5000+i);
			sp=2;
			}
		else
			{
//		XLine(x,y,p->x,p->y);	
			if (pnt[i-1].tipo==1 && i<cntp-1)
				{
				cx=(x+x1)>>1;cy=(y+y1)>>1;
				SetRGB(255,0,255);XCircle(cx,cy,3);//SetColor(0xffff);
				addSensor(cx-3,cy-3,cx+3,cy+3,4999+i);
				}
			}
		}
	x=x1;y=y1;
	if (sp>0) SetRGB(2550,255,0); else SetRGB(0,255,0);
	XBox(x-3,y-3,x+3,y+3);//	XVLine(x,y-3,y+3);XHLine(x-3,y,x+3);
	SetColor(0xffff);
	addSensor(x-3,y-3,x+3,y+3,1+i);
	}
p=&pnt[cntp-1];
if (nro<0 || nro>=cntp) return;
cx=v.getX(pnt[nro].x);cy=v.getY(pnt[nro].y);
SetColor(0xffff);
XCircle(cx,cy,6);
*/
//if (tipo!='l') // si es poligono
	//{
//	SetRGB(0,0,255);XLine(pnt[0].x,pnt[0].y,p->x,p->y);
	//}
}

void Trazo::drawLast()
{
SetColor(0xffff);
NODOP *p=&pnt[cntp-2];
int x=p->x,y=p->y;p++;
for (int i=0;i<1;i++,p++)
	{
	Line(x,y,p->x,p->y);
	x=p->x;y=p->y;
	}
}

void Trazo::del(int nro)
{
if (nro<0 || nro>=cntp) return;
for (int i=nro+1;i<cntp;i++)
  pnt[i-1]=pnt[i];
cntp--;
}

//------------------------- FIGURA --------------------------------
//void Figura::addColor()
//{}

void Figura::add(Trazo &tr)
{
if (tr.cntp<2) return;
if (cntpnt+tr.cntp>MAXPNTFIG) return; // no entra
pol[cntpol]=tr;
pol[cntpol].pnt=&pnt[cntpnt];
cntpol++;
NODOP *pos=tr.pnt;
NODOP *pnts=&pnt[cntpnt];
for (int i=0;i<tr.cntp;i++,pos++,pnts++,cntpnt++)
	*pnts=*pos;
}

void Figura::set(short n,Trazo &tr)
{
if (n<0 || n>=cntpol) return;
// hacer lugar
if (tr.cntp!=pol[n].cntp) // hay que hacer lugar
	{
	}
// copiar;
pol[n].cntp=tr.cntp;
pol[n].tipo=tr.tipo;
pol[n].color=tr.color;
NODOP *pos=pol[n].pnt;
for (int i=0;i<tr.cntp;i++,pos++)
	*pos=tr.pnt[i];
}

void Figura::get(short n,Trazo *tr)
{
if (n>=cntpol) return;
tr->cntp=pol[n].cntp;
tr->tipo=pol[n].tipo;
tr->color=pol[n].color;
NODOP *p=pol[n].pnt;
for (int i=0;i<pol[n].cntp;i++,p++)
    tr->pnt[i]=*p;
}

void Figura::del(short n)
{
/*
for (NODOP *p=pol[n].pnt;p<ultimo
	{
	for (int i=pol[n].pnt
	for (int i=n+1;i<cntpol;i++)
		pol[i-1]=po[i];
	}*/
if (n==cntpol-1) 
	cntpol--;
}

void Figura::draw(Vista &v)
{
Trazo *p=pol;
NODOP *pa,*pp;
int sp;

PatronColor pc;
//SetLinearPat(pc);
SetRadialPat(pc,a,b,c,d,tx<<14,ty<<14);

int x,y,x1,y1;//,x2,y2;
for(int i=0;i<cntpol;i++,p++)	// cada dibujo
	{
	SetColor(p->color.color);
	if (p->tipo=='l')
		{
		//SetColor(p->color);
		pp=p->pnt;sp=0;
		x=v.getX(pp->x);y=v.getY(pp->y);pa=pp;pp++;
		for (int j=1;j<p->cntp-1;j++,pp++)
			{
			x1=v.getX(pp->x);y1=v.getY(pp->y);
			if (pa->bx==-1)
				Line(x,y,x1,y1);
			else
				SPlin(x,y,v.getX(pa->bx),v.getY(pa->by),x1,y1);
			x=x1;y=y1;pa=pp;
			}
		}
	else
		{
		pp=p->pnt;sp=0;
		x=v.getX(pp->x);y=v.getY(pp->y);pa=pp;pp++;
		for (int j=1;i<p->cntp-1;j++,pp++)
			{
			x1=v.getX(pp->x);y1=v.getY(pp->y);
			if (pa->bx==-1)
				addSegment(x,y,x1,y1);
			else
				addSPlin(x,y,v.getX(pa->bx),v.getY(pa->by),x1,y1);
			x=x1;y=y1;pa=pp;
			}
		SetSolidColor(p->color);
//		SetLinearColor(&pc,a,b,tx,ty);
//		SetRadialColor(&pc);
		drawPolygon();
		}
	}
}

void Figura::save(char *t)
{
HANDLE hFile;
DWORD Bytes;

TCHAR szName[255];
wsprintf(szName,_T("%hs"),t);// de char a charw

hFile=CreateFile(szName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
if (hFile==INVALID_HANDLE_VALUE)  return;

WriteFile(hFile,&cntpnt,sizeof(cntpnt),&Bytes,NULL);
WriteFile(hFile,pnt,sizeof(NODOP)*cntpnt,&Bytes,NULL);
WriteFile(hFile,&cntpol,sizeof(cntpol),&Bytes,NULL);
for (int i=0;i<cntpol;i++)
	{
	WriteFile(hFile,pol,sizeof(Trazo)*cntpol,&Bytes,NULL);
	}

CloseHandle(hFile);
}

void Figura::load(char *t)
{
HANDLE hFile;
DWORD Bytes;
TCHAR szName[255];
wsprintf(szName,_T("%hs"),t);// de char a charw

hFile=CreateFile(szName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
if (hFile == INVALID_HANDLE_VALUE)	return;

ReadFile(hFile,&cntpnt,sizeof(cntpnt),&Bytes,NULL);
ReadFile(hFile,pnt,sizeof(NODOP)*cntpnt,&Bytes,NULL);
ReadFile(hFile,&cntpol,sizeof(cntpol),&Bytes,NULL);
ReadFile(hFile,pol,sizeof(Trazo)*cntpol,&Bytes,NULL);

CloseHandle(hFile);
}

/*
void Figura::draws(char *buffer)
{
char *p=buffer;
while (*p!=0) {
	switch (*p) {
//--------------------------- dibujar
	case 'l':// lineas
		break;
	case 'p':// poligono
		break;
	case 'q':// poligono radial
		break;
	case 'r':// poligono lineal
		break;
//--------------------------- anima dibujo
	case 'a':// animacion
		break;
//--------------------------- sensor evento
	case 's':// define sensor
		break;
	case 'b':// borrar sensor
		break;

		}
	}
}
*/