/////////////////////////////////////////////////////////
//
// DIBUJO.CPP
//
// Redamation source
/////////////////////////////////////////////////////////
#include "stdafx.h"
#include "dibujo.h"
#include "gui.h"

//-------Dibujo
void Dibujo::unirUltimo(void)
{
if (cntt<2) return;
Trazo2D *a1=&trazos[cntt-2],*a2=&trazos[cntt-1];
if (a1->tipo!=a2->tipo || (!a1->color.equal(a2->color))) return;
Punto2D *p=&pnts[a1->puntos+a1->cntpuntos];
p->tipo=3;// falta grupo
a1->cntpuntos+=a2->cntpuntos;
cntt--;
}

void Dibujo::addLinea(Matrix &imat,Linea &l,char t,ColorRGBA &c,short g)
{
if (l.cntp<2) return;// no agrega un punto
//l.optimize();
Trazo2D *actual=&trazos[cntt];	
actual->tipo=t;
actual->color=c;
actual->puntos=cntp;
actual->cntpuntos=0; 
byte gr=(g&0x3f)<<2;
long x1,y1;
short tx,ty;
Punto2D *plinea=l.pnt,*pdibujo=&pnts[actual->puntos];
for (WORD i=0;i<l.cntp;i++,plinea++)
	{
	imat.transforma(plinea->x,plinea->y,&x1,&y1);
	tx=(short)x1;ty=(short)y1;
	if (x1==(long)tx && y1==(long)ty)
		{
		pdibujo->x=tx;pdibujo->y=ty;pdibujo->tipo=plinea->tipo|gr;
		pdibujo++;
		cntp++;
		actual->cntpuntos++;
		}
	}
cntt++;
}
/*
void Dibujo::addCircle(short x,short y,short r,ColorRGBA &c,short g)
{
Trazo2D *actual=&trazos[cntt];
actual->tipo=4;
actual->color=c;
actual->puntos=&pnts[cntp];
actual->cntpuntos=2;
byte gr=(g&0x3f)<<2;
Punto2D *pdibujo=actual->puntos;
pdibujo->x=x;pdibujo->y=y;pdibujo->tipo=0|gr;
pdibujo++;cntp++;
pdibujo->x=r;pdibujo->y=r;pdibujo->tipo=0|gr;
cntp++;
cntt++;
}
*/
void Dibujo::addEllipse(Matrix &imat,short x,short y,short xr,short yr,ColorRGBA &c,short g)
{
Trazo2D *actual=&trazos[cntt];
actual->tipo=0;
actual->color=c;
actual->puntos=cntp;
actual->cntpuntos=0;
byte gr=(g&0x3f)<<2;
Punto2D *pdibujo=&pnts[actual->puntos];
short sen,cos;
long xr1,yr1,x1,y1;
long xa,ya;
imat.transforma(x,y,&xa,&ya);
//imat.transforma(yr,xr,&xr1,&yr1);
xr1=(xr*imat.a)/0x4000;
yr1=(yr*imat.d)/0x4000;
char tipo=0;
for (short i=0;i<3600;i+=450)
	{
	sincos(i,sen,cos);
	/*
	x1=(cos*xr)/0x4000;y1=(sen*yr)/0x4000;
	imat.transforma(x+x1,y+y1,&xr1,&yr1);
	pdibujo->x=(short)xr1;pdibujo->y=(short)yr1;pdibujo->tipo=tipo|gr;
	*/
	//imat.transforma(x1,y1,&xr1,&yr1);
	x1=xa+(xr1*cos)/0x4000;y1=ya+(yr1*sen)/0x4000;
	pdibujo->x=(short)x1;pdibujo->y=(short)y1;pdibujo->tipo=tipo|gr;

	pdibujo++;cntp++;
	actual->cntpuntos++;
	if (tipo==1) tipo=2; else tipo=1;
	}
cntt++;
}

void Dibujo::addRect(Matrix &imat,short x1,short y1,short x2,short y2,ColorRGBA &c,short g)
{
Trazo2D *actual=&trazos[cntt];
actual->tipo=0;// poligono
actual->color=c;
actual->puntos=cntp;
actual->cntpuntos=4;
byte gr=(g&0x3f)<<2;
Punto2D *pdibujo=&pnts[actual->puntos];
long xr1,yr1;
imat.transforma(x1,y1,&xr1,&yr1);
pdibujo->x=(short)xr1;pdibujo->y=(short)yr1;pdibujo->tipo=0|gr;
pdibujo++;cntp++;
imat.transforma(x2,y1,&xr1,&yr1);
pdibujo->x=(short)xr1;pdibujo->y=(short)yr1;pdibujo->tipo=0|gr;
pdibujo++;cntp++;
imat.transforma(x2,y2,&xr1,&yr1);
pdibujo->x=(short)xr1;pdibujo->y=(short)yr1;pdibujo->tipo=0|gr;
pdibujo++;cntp++;
imat.transforma(x1,y2,&xr1,&yr1);
pdibujo->x=(short)xr1;pdibujo->y=(short)yr1;pdibujo->tipo=0|gr;
cntp++;
cntt++;
}

void Dibujo::addClon(WORD t)
{
if (cntt==0) return;
Trazo2D *actual=&trazos[cntt],*origen=&trazos[t];
*actual=*origen;
actual->puntos=cntp;
memcpy(&pnts[actual->puntos],&pnts[origen->puntos],sizeof(Punto2D)*origen->cntpuntos);
cntp+=origen->cntpuntos;
cntt++;
WORD i;
Punto2D *p=&pnts[actual->puntos];
for (i=0;i<actual->cntpuntos;i++,p++)
	{
	p->x+=10;
	p->y+=10;
	}
}

//---- edicion
void Dibujo::drawTrazo(Matrix &mat,WORD t)
{
if (t>=cntt) if (cntt>0) t=cntt-1; else return;
Pantalla.XOR(true);
Pantalla.Color(0xffff);
trazos[t].drawEditT(mat,pnts);
Pantalla.XOR(false);
}

void Dibujo::drawNodo(Matrix &mat,WORD t,WORD p)
{
if (t>=cntt) if (cntt>0) t=cntt-1; else return;
Pantalla.XOR(true);
Pantalla.Color(0xffff);
trazos[t].drawEdit(mat,pnts,p);
Pantalla.XOR(false);
}

//---- manejo
void Dibujo::delTrazo(WORD t)
{
if (cntt==0 || t>=cntt) return; // no valido
cntt--;
int borrados=trazos[t].cntpuntos;
cntp-=borrados;
//if (t==cntt) return;
int i,atras=0;
for (i=0;i<t;i++)
	atras+=trazos[i].cntpuntos;
int adelante=cntp-atras;
memmove(&pnts[trazos[t].puntos],&pnts[trazos[t+1].puntos],adelante*sizeof(Punto2D));//muevo los puntos
for (i=t;i<cntt;i++)
	{
	trazos[i]=trazos[i+1];
	trazos[i].puntos-=borrados;
	}
}

void Dibujo::SwapPnts(WORD t)
{
Punto2D *temp;
WORD cnt1=trazos[t].cntpuntos;
WORD cnt2=trazos[t+1].cntpuntos;
Punto2D *p1=&pnts[trazos[t].puntos];
Punto2D *p2=&pnts[trazos[t+1].puntos];
WORD cntp=trazos[t].puntos+cnt2;
Punto2D *paux=&pnts[cntp];// cambio
if (cnt1>cnt2)
	{
	temp=new Punto2D[cnt2];
	memcpy(temp,p2,sizeof(Punto2D)*cnt2);
	memmove(paux,p1,sizeof(Punto2D)*cnt1);
	memcpy(p1,temp,sizeof(Punto2D)*cnt2);
	delete [] temp;
	}
else
	{
	temp=new Punto2D[cnt1];
	memcpy(temp,p1,sizeof(Punto2D)*cnt1);
	memmove(p1,p2,sizeof(Punto2D)*cnt2);
	memcpy(paux,temp,sizeof(Punto2D)*cnt1);
	delete [] temp;
	}
Trazo2D aux=trazos[t];trazos[t]=trazos[t+1];trazos[t+1]=aux;
trazos[t].puntos=trazos[t+1].puntos;
trazos[t+1].puntos=cntp;
}

void Dibujo::subeTrazo(WORD t)
{
if (cntt==0 || t==0) return; // no baja el primero
SwapPnts(t-1);
}

void Dibujo::bajaTrazo(WORD t)
{
if (cntt==0 || t>=cntt-1) return;// no sube  el ultimo
SwapPnts(t);
}

void Dibujo::tipoTrazo(WORD t)
{
if (t>=cntt) return;
switch (trazos[t].tipo) {
case 0:
	trazos[t].tipo=2;
	break;
case 2:
	trazos[t].tipo=0;
	break;
	};
}

//----------------- NODO
void Dibujo::delNodo(WORD t,WORD p)
{
if (cntt==0) return;
if (t>=cntt) if (cntt>0) t=cntt-1; else return;
Trazo2D *tr=&trazos[t];
if (tr->cntpuntos<3)
	{
	delTrazo(t);
	return;
	}
tr->cntpuntos--;
WORD i,s=tr->cntpuntos-p;
for (i=t+1;i<cntt;i++)
	{
	trazos[i].puntos--;
	s+=trazos[i].cntpuntos;
	}
cntp--;
Punto2D *pp=&pnts[tr->puntos+p];
memmove(pp,pp+1,sizeof(Punto2D)*s);
pp->tipo=0;
if (p>0) {
	if ((pp-1)->tipo==1) pp->tipo=2;
//	if ((pp-1)->tipo==2) pp->tipo=0;
	}
pnts[tr->puntos].tipo=0;
if (pnts[tr->puntos+1].tipo==2) pnts[tr->puntos+1].tipo=0;
}

void Dibujo::insNodo(WORD t,WORD p)
{
if (cntt==0) return;
Trazo2D *tr=&trazos[t];
tr->cntpuntos++;
WORD i,s=tr->cntpuntos-p;
for (i=t+1;i<cntt;i++)
	{
	trazos[i].puntos++;
	s+=trazos[i].cntpuntos;
	}
cntp++;
Punto2D *pp=&pnts[tr->puntos+p];
memmove(pp+1,pp,sizeof(Punto2D)*s);
pp->tipo=0;
if (p!=tr->cntpuntos-2) 
	{
	pp++;
	pp->x+=(pp+1)->x;pp->x>>=1;
	pp->y+=(pp+1)->y;pp->y>>=1;
	}
}

void Dibujo::BBox(Matrix &mat)
{
long x,y;
Punto2D *p=pnts;
mat.transforma(p->x,p->y,&x,&y);
x1=x2=x;
y1=y2=y;
p++;
for (int i=1;i<cntp;i++,p++)
	{
	mat.transforma(p->x,p->y,&x,&y);
	if (x<x1) x1=x;
	if (x>x2) x2=x;
	if (y<y1) y1=y;
	if (y>y2) y2=y;
	}
}

void Dibujo::drawu(Matrix &mat)
{
WORD i;
Trazo2D *actual=trazos;
for (i=0;i<cntt;i++,actual++)
	{ 
	actual->drawu(mat,pnts); 
	}
}

void Dibujo::draw(Matrix &mat)
{
WORD i;
Trazo2D *actual=trazos;
for (i=0;i<cntt;i++,actual++)
	{ 
	actual->draw(mat,grupos,pnts); 
	}
}

// agregar sensores
void Dibujo::addSensorObj(Matrix &mat)
{
clearSensor();
Trazo2D *tr=&trazos[cntt-1];
for (short i=cntt-1;i>=0;i--,tr--)
	{
	long x1,y1,x2,y2;
	tr->getBox(mat,pnts,&x1,&y1,&x2,&y2);
	addSensor(x1-1,y1-1,x2+1,y2+1,i);
	}
}

void Dibujo::addSensorPnt(Matrix &mat,short ntr)
{
clearSensor();
Trazo2D *tr=&trazos[ntr];
Punto2D *p=&pnts[tr->puntos];
long x1,y1;
for (short i=0;i<tr->cntpuntos;i++,p++)
	{
	mat.transforma(p->x,p->y,&x1,&y1);
	addSensor(x1-3,y1-3,x1+4,y1+4,i);
	}
}

void Dibujo::makegroup(void)
{
WORD i;
Grupo *gr=grupos;
for (i=0;i<64;i++,gr++)
	gr->xc=gr->yc=gr->x=gr->y=gr->alto=gr->ancho=gr->cntp=0;
Punto2D *pnt=pnts;
for (i=0;i<cntp;i++,pnt++)
	{
	gr=&grupos[pnt->tipo>>2];
	gr->cntp++;
	gr->xc+=pnt->x;
	gr->yc+=pnt->y;
	}
gr=grupos;
for (i=0;i<64;i++,gr++)
	if (gr->cntp>0)
		{
		gr->xc=gr->xc/gr->cntp;
		gr->yc=gr->yc/gr->cntp;
		}
gr->x=0;
gr->y=0;
}

void Dibujo::drawg(Matrix &mat)
{
WORD i;
long x1,y1;
char t[5];
Grupo *gr=grupos;
Pantalla.XOR(true);
Pantalla.Color(0xffff);
for (i=0;i<64;i++,gr++)
	if (gr->cntp>0)
		{
		mat.transforma(gr->xc+gr->x,gr->yc+gr->y,&x1,&y1);
		Pantalla.Circle(x1,y1,7);
		sprintf(t,"%d",i);
		Pantalla.TextVC((short)x1,(short)y1,t);
		}
Pantalla.XOR(false);
Pantalla.Color(0);
}

void Dibujo::moveg(Matrix &imat,byte g,short dx,short dy)
{
long x1,y1;
imat.transforma(dx,dy,&x1,&y1);
Grupo *gr=&grupos[g];
gr->x=x1-gr->xc;// relativo al centro
gr->y=y1-gr->yc;
}

// cargar y grabar de memoria
int Dibujo::memsize(void)
{
return	sizeof(cntp)+cntp*sizeof(Punto2D)+
		sizeof(cntt)+cntt*sizeof(Trazo2D);
}

char *Dibujo::memput(char *l)
{
memcpy(l,&cntp,sizeof(cntp));			l+=sizeof(cntp);
memcpy(l,pnts,sizeof(Punto2D)*cntp);	l+=sizeof(Punto2D)*cntp;
memcpy(l,&cntt,sizeof(cntt));			l+=sizeof(cntt);
memcpy(l,trazos,sizeof(Trazo2D)*cntt);	l+=sizeof(Trazo2D)*cntt;
return l;
}

char *Dibujo::memget(char *l)
{
memcpy(&cntp,l,sizeof(cntp));			l+=sizeof(cntp);
memcpy(pnts,l,sizeof(Punto2D)*cntp);	l+=sizeof(Punto2D)*cntp;
memcpy(&cntt,l,sizeof(cntt));			l+=sizeof(cntt);
memcpy(trazos,l,sizeof(Trazo2D)*cntt);	l+=sizeof(Trazo2D)*cntt;
return l;
}

void Dibujo::memset(char *l)// copiar por punteros
{
memcpy(&cntp,l,sizeof(cntp));	l+=sizeof(cntp);
pnts=(Punto2D*)l;				l+=sizeof(Punto2D)*cntp;
memcpy(&cntt,l,sizeof(cntt));	l+=sizeof(cntt);
trazos=(Trazo2D*)l;				l+=sizeof(Trazo2D)*cntt;
}