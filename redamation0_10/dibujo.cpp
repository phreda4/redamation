#include "stdafx.h"
#include "flib.h"
//..................
#include "dibujo.h"
#include "poli.h"


int Dibujo::size()// tamanio en bytes
{
unsigned short cntp1=cntp,cntt1=cntt;
if (tipo==1) { cntp1-=marca;cntt1-=marcat; }
return sizeof(Cabecera)+
	sizeof(NodoDibujo)*(int)cntp1+sizeof(TipoTrazo)*(int)cntt1+sizeof(NodoCambio)*(int)cntc;
}

void Dibujo::setmem(char *p)// graba en memoria (ajusta si es del tipo 1)
{
unsigned short cntp1=cntp,cntt1=cntt;
if (tipo==1) // grabo solo las diferencias
	{ cntp1-=marca;cntt1-=marcat; }
else
	{ marca=0;marcat=0; }
Cabecera cab;
cab.tipo=tipo;
cab.tiempo=tiempo;
cab.cntp=cntp1;
cab.cntt=cntt1;
cab.cntc=cntc;
memcpy(p,&cab,sizeof(Cabecera));
p+=sizeof(Cabecera);
memcpy(p,&pnt[marca],sizeof(NodoDibujo)*cntp1);
p+=sizeof(NodoDibujo)*cntp1;
memcpy(p,&trz[marcat],sizeof(TipoTrazo)*cntt1);
p+=sizeof(TipoTrazo)*cntt1;
memcpy(p,cmb,sizeof(NodoCambio)*cntc);
}

void Dibujo::getmem(char *p)// carga de memoria (ojo tiene que estar en Dibujo el anterior)
{
Cabecera cab;
memcpy(&cab,p,sizeof(Cabecera));// mejor con cast pero no anda
p+=sizeof(Cabecera);
tipo=cab.tipo;
unsigned short cntp1,cntt1;
if (tipo==1) // cargo las diferencias
	{ marca=cntp1=cntp;marcat=cntt1=cntt; }
else
	{ marca=cntp1=marcat=cntt1=0;}
tiempo=cab.tiempo;
cntp=cab.cntp;
cntt=cab.cntt;
cntc=cab.cntc;
memcpy(&pnt[cntp1],p,sizeof(NodoDibujo)*cntp);
p+=sizeof(NodoDibujo)*cntp;cntp+=marca;
memcpy(&trz[cntt1],p,sizeof(TipoTrazo)*cntt);
p+=sizeof(TipoTrazo)*cntt;cntt+=marcat;
memcpy(cmb,p,sizeof(NodoCambio)*cntc);
}


void Dibujo::setSub()
{
tipo=1;marca=cntp;marcat=cntt;cntc=0;
};

unsigned short Dibujo::buscaCmb(char t,short nro)
{
NodoCambio *c=cmb;
for (unsigned short i=0;i<cntc;i++,c++)
	if (c->tipo==t && c->nro==nro) return i;
return cntc;
}

//====================================================================================
void Dibujo::draw()
{
if (cntp<1) return;
bool poli=false;
int x,y,mx,my,xa,ya;
long x1,y1,x2,y2;
unsigned short n=0;
NodoDibujo *p=pnt;
TipoTrazo *nt=trz;
Matrix m1,maux=mat;
for (unsigned short i=0;i<cntp;i++,p++)
	{
	maux.transforma(p->x,p->y,&x1,&y1);
	switch (p->tipo){
      case 0:
		if (poli)// no es el primero porque poli=false
			{
			pSegmento(xa,ya,x,y);//addSPlin(xa,ya,(xa+x)>>1,(ya+y)>>1,x,y);
			drawPoli();
			}
//		else
//		drawLine();
		switch (nt->tipo) {
		case 0:// linea
			SetColor(nt->color);
			poli=false;
			break;
		case 1:// poli solido
			pSolidColor(nt->color);	
			poli=true;
			break;
		//case 2: // poli alpha,lgrad,rgrad
			}
		//pSolidColor(trz[n].color);
		// hacer la matriz para cada trazo
		maux.traslate(nt->xt,nt->yt);
		m1.scale(nt->s,nt->s);
		maux.mult(m1);
		m1.rotate(nt->r);
		maux.mult(m1);
		maux.mult(mat);
		
		// avanzar el trazo
		n++;nt++;
		maux.transforma(p->x,p->y,&x1,&y1);
		x=x1;y=y1;
		xa=x;ya=y;
		break;
      case 1:
		if (poli)
			pSegmento(x,y,x1,y1);
		else
			Line(x,y,x1,y1);
		x=x1;y=y1;
        break;
      case 2:
		p++;
		if (p->tipo!=0 && i<cntp-1)
			{ i++;maux.transforma(p->x,p->y,&x2,&y2); }
		else
			{ x2=xa;y2=ya;p--; }
        mx=(x+x2)>>1;my=(y+y2)>>1;
		x1=mx+((x1-mx)<<1);y1=my+((y1-my)<<1);
		if (poli)
			pSPline(x,y,x1,y1,x2,y2);
		else
			SPlin(x,y,x1,y1,x2,y2);
		x=x2;y=y2;
        break;
	  case 3:// move
		x=x1;y=y1;
		break;
		}
	}
if (poli)
	{
	pSegmento(xa,ya,x,y);//addSPlin(xa,ya,(xa+x)>>1,(ya+y)>>1,x,y);
	drawPoli();
	}
//else
//	drawLine();
}
//====================================================================================

void Dibujo::drawEdit(unsigned short nt,unsigned short np)
{
if (cntp<1 || nt>=cntt) return;
long x,y,x1,y1,xa,ya;
unsigned short npu=0,icnt=trzD[nt].cnodo;
NodoDibujo *p=&pnt[trzD[nt].nnodo];
SetColor(0xffff);
SetPixelXOR();
clearSensor();

TipoTrazo *tt=&trz[nt];
Matrix m1,maux;
maux.traslate(tt->xt,tt->yt);
m1.scale(tt->s,tt->s);
maux.mult(m1);
m1.rotate(tt->r);
maux.mult(m1);
maux.mult(mat);

for (unsigned short i=0;i<icnt;i++,p++)
	{
	maux.transforma(p->x,p->y,&x1,&y1);
	switch (p->tipo)
		{
      case 0:
		xa=x1;ya=y1;
      case 1:
		Box(x1-4,y1-4,x1+4,y1+4);
        break;
      case 2:
		Circle(x1,y1,4);
		if (npu==np) Box(x1-2,y1-2,x1+2,y1+2);
		npu++;addSensor(x1-5,y1-5,x1+5,y1+5,npu);
		p++;i++;
		if (i<icnt)
			{ 
			maux.transforma(p->x,p->y,&x1,&y1);
			Box(x1-4,y1-4,x1+4,y1+4);
			}
		else
			{ x1=xa;y1=ya; }
        break;
		}
	x=x1;y=y1;
	if (npu==np) Circle(x,y,3);
	npu++;addSensor(x-5,y-5,x+5,y+5,npu);
	}
SetPixelCopy();
}

void Dibujo::drawBox(unsigned short nt)
{
short x1,y1,x2,y2;
if (cntp<1 || nt>=cntt) return;
TipoTrazoD *ptrzD=&trzD[nt];
x1=ptrzD->x1;y1=ptrzD->y1;x2=ptrzD->x2;y2=ptrzD->y2;
/*
TipoTrazo *tt=&trz[nt];
Matrix m1,maux;
maux.traslate(tt->xt,tt->yt);
m1.scale(tt->s,tt->s);
maux.mult(m1);
m1.rotate(tt->r);
maux=mat;
maux.mult(m1);
//maux.mult(mat);

maux.transforma(trzD->x1,ptrzD->y1,&x1,&y1);
maux.transforma(trzD->x2,ptrzD->y2,&x2,&y2);
*/
SetColor(0xffff);
SetPixelXOR();
BoxFill(x1-2,y1-2,x1+2,y1+2);
BoxFill(x2-2,y1-2,x2+2,y1+2);
BoxFill(x1-2,y2-2,x1+2,y2+2);
BoxFill(x2-2,y2-2,x2+2,y2+2);
Box(x1,y1,x2,y2);
SetPixelCopy();
}

void Dibujo::setSensor()
{
TipoTrazoD *ptrzD=trzD;
//TipoTrazo *nt=trz;
for (unsigned short i=0;i<cntt;i++,ptrzD++)
	{
	addSensor(ptrzD->x1,ptrzD->y1,ptrzD->x2,ptrzD->y2,i+1000);
	}
}


//---------------------------------------------------------------------

void Dibujo::addTrazo(Entrada *e,char t,WORD c)
{
if (e->trazo.cntp<1) return;
if (cntp+e->trazo.cntp>MAXPNTDIBUJO) return;

TipoTrazo *tra=&trz[cntt];
TipoTrazoD *trad=&trzD[cntt];
tra->inic(t,c);// inicia los movimientos tambien
trad->nnodo=cntp;
cntt++;
NodoLinea *p=e->trazo.pnt;
NodoDibujo *a2,*a,*b,*pd=&pnt[cntp];
short mx,my,ang;
long x1,y1;

imat.transforma(p->x,p->y,&x1,&y1);

pd->x=x1;pd->y=y1;pd->tipo=0;cntp++;
p++;pd++;
for (unsigned short i=1;i<e->trazo.cntp;i++,p++,pd++)
	{
	imat.transforma(p->x,p->y,&x1,&y1);
	if (i>2)
		{
		a=pd-2;b=pd-1;
		ang=angle(a->x,a->y,b->x,b->y,x1,y1);
		if (a->tipo!=2)		// puede transformar a curva
			{
			if (ang<750) b->tipo=2;	// transforma a spline
			}
		else if (i>3)		// puede simplificar
			{
			a2=pd-3;
			if (cospl(a2->x,a2->y,a->x,a->y,b->x,b->y,x1,y1,&mx,&my)<1)
				{
				a->x=mx;a->y=my;
				b->x=x1;b->y=y1;
				pd--;
				continue;
				}
			}
		}
	pd->x=x1;pd->y=y1;pd->tipo=1;
	cntp++;
	}
trad->cnodo=cntp-trad->nnodo;
}

void Dibujo::addCircle(short x,short y,short rx,short ry,char t,WORD c)
{
TipoTrazo *tra=&trz[cntt];
TipoTrazoD *trad=&trzD[cntt];
tra->inic(t,c);
trad->nnodo=cntp;
cntt++;
NodoDibujo *pd=&pnt[cntp];
short x1,y1;
short sen,cos;
long xr1,yr1;
char tipo=0;
for (short i=0;i<3600;i+=450)
	{
	sincos(i,sen,cos);
	x1=(x+(cos*rx)/0x4000);y1=(y+(sen*ry)/0x4000);
	imat.transforma(x1,y1,&xr1,&yr1);
	pd->x=xr1;pd->y=yr1;pd->tipo=tipo;
	if (tipo==2) tipo=1; else tipo=2;
	cntp++;pd++;
	}
trad->cnodo=cntp-trad->nnodo;
}

void Dibujo::addBox(short x1,short y1,short x2,short y2,char t,WORD c)
{
TipoTrazo *tra=&trz[cntt];
TipoTrazoD *trad=&trzD[cntt];
tra->inic(t,c);
trad->nnodo=cntp;
cntt++;
NodoDibujo *pd=&pnt[cntp];
long xc1,yc1;
imat.transforma(x1,y1,&xc1,&yc1);
pd->x=xc1;pd->y=yc1;pd->tipo=0;cntp++;pd++;
imat.transforma(x2,y1,&xc1,&yc1);
pd->x=xc1;pd->y=yc1;pd->tipo=1;cntp++;pd++;
imat.transforma(x2,y2,&xc1,&yc1);
pd->x=xc1;pd->y=yc1;pd->tipo=1;cntp++;pd++;
imat.transforma(x1,y2,&xc1,&yc1);
pd->x=xc1;pd->y=yc1;pd->tipo=1;cntp++;pd++;
imat.transforma(x1,y1,&xc1,&yc1);
pd->x=xc1;pd->y=yc1;pd->tipo=1;cntp++;pd++;
trad->cnodo=cntp-trad->nnodo;
}

//-----------------------------------------------------------------------------------

void Dibujo::moveTrazo(unsigned short nt,short xa,short ya,short x,short y)
{
if (nt>=cntt) return;
if (xa==x && ya==y) return;
long xt1,yt1,xt2,yt2;

TipoTrazo *tt=&trz[nt];

Matrix m1,maux;
maux.traslate(tt->xt,tt->yt);
m1.scale(tt->s,tt->s);
maux.mult(m1);
m1.rotate(tt->r);
maux.mult(m1);
maux.mult(mat);
//maux.mult(mat)
maux.invert(m1);

m1.transforma(xa,ya,&xt1,&yt1);
m1.transforma(x,y,&xt2,&yt2);
xt2-=xt1;yt2-=yt1;
/*
NodoDibujo *pnt1=&pnt[trzD[nt].nnodo];
for (unsigned short i=0;i<trzD[nt].cnodo;i++,pnt1++)
	{
	pnt1->x+=xt2;pnt1->y+=yt2;
	}
*/
tt->xt+=xt2;tt->yt+=yt2;
if (nt<marcat)
	{
	unsigned short cc=buscaCmb(1,nt);
	NodoCambio *c=&cmb[cc];
	if (cc==cntc) 
		{ 
		cntc++;
		xt1=yt1=0;
		}
	else
		{
		xt1=c->x;yt1=c->y;// como es incremental necesito guardar el anterior
		}
	c->tipo=1;
	c->nro=nt;
	c->x=xt1+xt2;
	c->y=yt1+yt2;
	}
regeneraBBox();
}


void Dibujo::moveNodo(unsigned short nt,unsigned short np,int x,int y)
{
long xt,yt;
unsigned short cntp1=trzD[nt].nnodo+np;
NodoDibujo *p=&pnt[cntp1];
TipoTrazo *tt=&trz[nt];
Matrix m1,maux;
maux.traslate(tt->xt,tt->yt);
m1.scale(tt->s,tt->s);
maux.mult(m1);
m1.rotate(tt->r);
maux.mult(m1);
maux.mult(mat);
maux.invert(m1);

m1.transforma(x,y,&xt,&yt);
p->x=xt;p->y=yt;
if (nt<marcat) 
  {
  unsigned short cc=buscaCmb(5,cntp1);
  if (cc==cntc) cntc++;
  NodoCambio *c=&cmb[cc];
  c->tipo=5;
  c->nro=cntp1;
  c->x=p->x;
  c->y=p->y;
  }
}

void Dibujo::delTrazo(unsigned short nt)// funciona mal
{
if (cntp<1) return;
if (nt>=cntt) return;
unsigned short cpnt=trzD[nt].cnodo;
unsigned short nnodo=trzD[nt].nnodo;
cntt--;
cntp-=cpnt;
if (nt==cntt) return;// si es el ultimo... ya esta
memmove(&pnt[nnodo],&pnt[trzD[nt+1].nnodo],sizeof(NodoDibujo)*(cntp-nnodo));
memmove(&trz[nt],&trz[nt+1],sizeof(TipoTrazo)*(cntt-nt));
memmove(&trzD[nt],&trzD[nt+1],sizeof(TipoTrazoD)*(cntt-nt));
regenera();
//for (unsigned short i=nt;i<cntt;i++)// correccion de los trazos siguientes
//  trzD[i].nnodo-=cpnt;
}

void Dibujo::delNodo(unsigned short nt,unsigned short np)
{
if (cntp<1) return;
if (nt>=cntt) return;
if (np>trzD[nt].cnodo) return;
cntp--;
NodoDibujo *p=&pnt[trzD[nt].nnodo+np];
memmove(p,p+1,sizeof(NodoDibujo)*(cntp-(trzD[nt].nnodo+np)));
trzD[nt].cnodo--;
for(unsigned short i=nt+1;i<cntt;i++)
  trzD[i].nnodo--;
if (np==0) 	p->tipo=0;
else if (np>1 && (p->tipo==0 || (p-1)->tipo==2)) (p-1)->tipo=1; 
}

void Dibujo::insNodo(unsigned short nt,unsigned short np)
{
if (cntp<1) return;
if (nt>=cntt) return;
if (np>trzD[nt].cnodo) return;
cntp++;
unsigned short nnodo=trzD[nt].nnodo+np;
NodoDibujo *p=&pnt[nnodo];
memmove(p+1,p,sizeof(NodoDibujo)*(cntp-nnodo));
trzD[nt].cnodo++;
for(unsigned short i=nt+1;i<cntt;i++)
  trzD[i].nnodo++;
if (p->tipo==0) (p+1)->tipo=1;
if (np<=trzD[nt].cnodo)
	{
	p++;
	p->x+=(p+1)->x;p->x>>=1;
	p->y+=(p+1)->y;p->y>>=1;
	}
}

//------------------------------------------------------------------------

void Dibujo::changeWin(Vis &v)
{
unsigned short cc;
//if (vista.xt!=v.xt || vista.yt!=v.yt)
	{
	cc=buscaCmb(6,0);
	if (cc==cntc) cntc++;
	NodoCambio *c=&cmb[cc];
	c->tipo=6;c->nro=0;
	c->x=v.xt;c->y=v.yt;
	}
//if (vista.angle!=v.angle || vista.sc!=v.sc)
	{
	cc=buscaCmb(7,0);
	if (cc==cntc) cntc++;
	NodoCambio *c=&cmb[cc];
	c->tipo=7;c->nro=0;
	c->x=v.angle;c->y=v.sc;
	}
}

void Dibujo::moveWin(short xz,short yz,short x,short y)
{
long xt1,yt1,xt2,yt2;
imat.transforma(xz,yz,&xt1,&yt1);
imat.transforma(x,y,&xt2,&yt2);
vista.move(xt2-xt1,yt2-yt1);
vista.makeMatrix(mat);
mat.invert(imat);
regeneraBBox();
}

void Dibujo::rsWin(short xz,short yz,short x,short y)
//d1=interpola((1<<(FRACMAT-8)),(1<<FRACMAT),y,320);
//d2=interpola(0,3600,x,240);
{
int d1=vista.sc+((y-yz)<<5);
if (d1<(1<<(FRACMAT-8))) d1=1<<(FRACMAT-8);
if (d1>(1<<FRACMAT)) d1=(1<<FRACMAT);
vista.scale(d1);
short d2=vista.angle+(xz-x);
vista.rota(d2);
vista.makeMatrix(mat);
mat.invert(imat);
regeneraBBox();
}

void Dibujo::changeLC(unsigned short nt,unsigned short np)
{
if (cntp<1) return;
if (nt>=cntt) return;
if (np==0 || np>=trzD[nt].cnodo) return;
NodoDibujo *p=&pnt[trzD[nt].nnodo+np];
if (p->tipo==0) return;
if (p->tipo==2)
	p->tipo=1;
else
	{
	p->tipo=2;
	if (np>0 && ((p-1)->tipo==2)) (p-1)->tipo=1;// pone recto en el anterior
	}
}

//----------------------------------------------------------------
// Cambios

void Dibujo::cargaCambios()
{
unsigned short i,j;
if (tipo==0) return;// un cuadro 0 no tiene cambios
NodoDibujo *p;
TipoTrazo *t;
NodoCambio *c=cmb;
for (i=0;i<cntc;i++,c++)
	switch (c->tipo) {
	case 1:// mueve trazos
/*
		p=&pnt[trzD[c->nro].nnodo];
		for (j=0;j<trzD[c->nro].cnodo;j++,p++)
			{ p->x+=c->x;p->y+=c->y; }
			*/
		t=&trz[c->nro];
		t->xt=c->x;t->yt=c->y;
		break;
	case 5:// mueve puntos
		p=&pnt[c->nro];
		p->x=c->x;p->y=c->y;
		break;
	case 6:// mueve ventana
		vista.xt=c->x;vista.yt=c->y;
		vista.makeMatrix(mat);
		mat.invert(imat);
		break;
	case 7:// rota y escala ventana
		vista.angle=c->x;vista.sc=c->y;
		vista.makeMatrix(mat);
		mat.invert(imat);
		break;
	}
}

void Dibujo::settiempo()
{
NodoCambio *nc=cmb;
NodoCambioD *ncD=cmbD;
NodoDibujo *pt;
TipoTrazo *nt;
for (unsigned short i=0;i<cntc;i++,nc++,ncD++)
	{
	switch (nc->tipo) {
	case 1:// mover trazo
		/*
		ncD->x=nc->x/tiempo;
		ncD->y=nc->y/tiempo;
		*/
		nt=&trz[nc->nro];
		ncD->x=nt->xt;
		ncD->y=nt->yt;
		break;
	case 5:// mover nodo
		pt=&pnt[nc->nro];
		ncD->x=pt->x;
		ncD->y=pt->y;
		break;
	case 6:// mueve ventana
		ncD->x=vista.xt;
		ncD->y=vista.yt;
		break;
	case 7:// rota y escala ventana
		ncD->x=vista.angle;
		ncD->y=vista.sc;
		break;
		}
	}
}

void Dibujo::settiempot(unsigned short t)
{
unsigned short i,j;
NodoCambio *nc=cmb;
NodoCambioD *ncD=cmbD;
NodoDibujo *pt;
TipoTrazo *nt;
for (i=0;i<cntc;i++,nc++,ncD++)
	{
	switch (nc->tipo) {
	case 1:// mover trazo
		/*
		pt=&pnt[trzD[nc->nro].nnodo];
		for (j=0;j<trzD[nc->nro].cnodo;j++,pt++)
			{ 
			pt->x+=ncD->x;
			pt->y+=ncD->y; 
			}
			*/
		nt=&trz[nc->nro];
		nt->xt=interpola(ncD->x,nc->x,t,tiempo);
		nt->yt=interpola(ncD->y,nc->y,t,tiempo);
		break;
	case 5:// mover nodo
		pt=&pnt[nc->nro];
		pt->x=interpola(ncD->x,nc->x,t,tiempo);
		pt->y=interpola(ncD->y,nc->y,t,tiempo);
		break;
	case 6:// mueve ventana
		vista.xt=interpola(ncD->x,nc->x,t,tiempo);
		vista.yt=interpola(ncD->y,nc->y,t,tiempo);
		vista.makeMatrix(mat);
		mat.invert(imat);
		break;
	case 7:// rota y escala ventana
		vista.angle=interpola(ncD->x,nc->x,t,tiempo);
		vista.sc=interpola(ncD->y,nc->y,t,tiempo);
		vista.makeMatrix(mat);
		mat.invert(imat);
		break;
		}
	}
}

//------------------------------------------------------------------
// Preparar los trazos para edicion
void Dibujo::regenera()
{
unsigned short n=0,cc;
NodoDibujo *p=pnt;
TipoTrazoD *ptrzD=trzD;
for (unsigned short i=0;i<cntp;i++,p++)
	{
	switch (p->tipo)
		{
      case 0:
		if (i>0) (ptrzD-1)->cnodo=cc;
		ptrzD->nnodo=i;
		n++;ptrzD++;
		cc=1;
		break;
	  default:
		cc++;
		}
	}
(ptrzD-1)->cnodo=cc;
}

void Dibujo::regeneraBBox()
{
short xm,ym,xmm,ymm;	
long x1,y1;
NodoDibujo *p=pnt;
TipoTrazo *nt=trz;
TipoTrazoD *ptrzD=trzD;
Matrix m1,maux;
for (unsigned short i=0;i<cntp;i++,p++)
	{
	maux.transforma(p->x,p->y,&x1,&y1);
//	x1=p->x;y1=p->y;
    if (p->tipo==0)
		{
		if (i>0)
			{
			(ptrzD-1)->x1=xm-1;
			(ptrzD-1)->y1=ym-1;
			(ptrzD-1)->x2=xmm+1;
			(ptrzD-1)->y2=ymm+1;
			(ptrzD-1)->xc=(xmm+xm)>>1;
			(ptrzD-1)->yc=(ymm+ym)>>1;
			}
		
		maux.traslate(nt->xt,nt->yt);
		m1.scale(nt->s,nt->s);
		maux.mult(m1);
		m1.rotate(nt->r);
		maux.mult(m1);
		maux.mult(mat);

		maux.transforma(p->x,p->y,&x1,&y1);
		
		//x1=p->x;y1=p->y;
		xm=xmm=x1;ym=ymm=y1;
		nt++;ptrzD++;
		}
	if (x1<xm) xm=x1;
	if (y1<ym) ym=y1;
	if (x1>xmm) xmm=x1;
	if (y1>ymm) ymm=y1;
	}
(ptrzD-1)->x1=xm-1;
(ptrzD-1)->y1=ym-1;
(ptrzD-1)->x2=xmm+1;
(ptrzD-1)->y2=ymm+1;
(ptrzD-1)->xc=(xmm+xm)>>1;
(ptrzD-1)->yc=(ymm+ym)>>1;
}

//-----------------------------------------------------------------
void Dibujo::clearSel()
{
//sel.cntpnt=0;
}

void Dibujo::setSel(short x1,short y1,short x2,short y2,Vis &vista)
{
	/*
sel.cntpnt=
sel.pnts[];
sel.xc=;
sel.yc=;
*/
}

void Dibujo::moveSel(short dx,short dy)// mueve los seleccionados
{
}

void Dibujo::rotaSel(short ang)
{
}

void Dibujo::scaleSel(short sx,short sy)
{
}

// falta cargar y grabar seleccion
