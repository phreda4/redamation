#include "stdafx.h"
//..........................
#include "poli.h"

#include "graph.h"
#include "math.h"
#include "flib.h"

#define NB_SEGMENT_MAX	512 // cada segmento es una linea

#define SEGFRAC	8
#define MASFRAC	0xff

//:::::::::::::::::
struct Segmento {
	short ymax;// donde corta
	long x,dx;// inicio de x, fin de x, avance de x

	struct Segmento *next,*nextValid;
};

short xp,yp,wp,hp,limymin,limymax;
Segmento *linact;
Segmento *lines[320];
Segmento seg[NB_SEGMENT_MAX];

WORD pColor;

//------------------------------------------
void clearPoli(unsigned short x1,unsigned short y1,unsigned short w,unsigned short h)
{
xp=x1;yp=y1;wp=w;hp=h;
memset(lines,0,hp*sizeof(Segmento *));
linact=seg;
limymin=hp;limymax=-1;
}

void pSolidColor(WORD color)
{
pColor=color;
}

inline Segmento *allocS()
{
Segmento *s;
if ((linact-seg)>=NB_SEGMENT_MAX) return NULL;
s=linact++;
return s;
}

//------------------------------------------
void pSegmento(int x1,int y1,int x2,int y2)// 0000.0
{
Segmento *s;
int dx,x,ymin,ymax;
int temp;
if (y1==y2) return;
if (y1<y2) 
	{
    ymin=y1;ymax=y2;
	} 
else 
	{
    ymin=y2;ymax=y1;
	temp=x2;x2=x1;x1=temp;
    }
if (ymax<0 || ymin>hp) return;
x=x1<<SEGFRAC;
dx=((x2-x1)<<SEGFRAC)/(ymax-ymin);// que pasa a veces ???
if (ymin<0) { x+=dx*(-ymin);ymin=0; }
if (ymax>hp) ymax=hp;
s=allocS();
if (s==NULL) return;
s->next=s->nextValid=0;
s->ymax=ymax;
s->x=x;
s->dx=dx;
if (ymin<limymin) limymin=ymin;
if (ymax>limymax) limymax=ymax;
if (lines[ymin]==NULL) 
	lines[ymin]=s;
else 
	{
    Segmento *act,*prev;
    prev=0;
    for(act=lines[ymin];act;prev=act,act=act->next) 
		{
        if ((act->x+act->dx)>(s->x+s->dx)) 
			{
            if (prev) 
				{
                prev->next=s;
                s->next=act;
                } 
			else 
				{
                s->next=lines[ymin];
                lines[ymin]=s;
                }
            break;
            }
        }
	if (act==0) 
		{
        prev->next=s;
        s->next=act;
        }
    }
}

void pSPli(long x1,long y1,long x2,long y2,long x3,long y3)
{
if (recta(x1,y1,x2,y2,x3,y3)<1024)
	{
	pSegmento(x1>>4,y1>>4,x3>>4,y3>>4);
	return;
	}
long x11=(x1+x2)>>1,y11=(y1+y2)>>1;
long x21=(x2+x3)>>1,y21=(y2+y3)>>1;
long x22=(x11+x21)>>1,y22=(y11+y21)>>1;
pSPli(x1,y1,x11,y11,x22,y22);
pSPli(x22,y22,x21,y21,x3,y3);
}

void pSPline(int x1,int y1,int x2,int y2,int x3,int y3)
{
pSPli(x1<<4,y1<<4,x2<<4,y2<<4,x3<<4,y3<<4);
}

//------------------------------------------
Segmento *avanzaS(Segmento *curSegs,int y)
{
Segmento *s,*prev,*ant;
s=curSegs;
prev=ant=0;
while(s)
    {
    if (y>=s->ymax) // == para evitar superpuestos
		{
        if (prev)         // Remove this segment, no more valid
			prev->nextValid=s->nextValid;
		else 
			curSegs=s->nextValid;
        s=s->nextValid;
        } 
	else 
		{
		s->x+=s->dx;
		if (prev!=0 && s->x<prev->x) // se invirtio el orden
			{
			if (ant==0) curSegs=s;// es el primero
			else ant->nextValid=s;// intercambiar prev y seg
			prev->nextValid=s->nextValid;
			s->nextValid=prev;ant=s;s=prev->nextValid;
			}
		else  // sigue igual
			{
			ant=prev;prev=s;s=s->nextValid;
			}
        }
    }
return curSegs;
}

//------------------------------------------
Segmento *nuevoS(Segmento *curSegs,Segmento *newSegs)
{
Segmento *s,*se,*prev;
s=curSegs;
prev=0;
for (se=newSegs;se;se=se->next)// Check for new segments
    {
	if (curSegs==0)// Place it at the correct position according to X
		{
        curSegs=se;
        se->nextValid=0;
        } 
	else 
		{
        for(;s;prev=s,s=s->nextValid)
            {
            if (s->x>se->x)
				{
                if (prev)  // Insert before s
					{
                    se->nextValid=s;
                    prev->nextValid=se;
                    } 
				else 
					{
                    se->nextValid=curSegs;
                    curSegs=se;
                    }
                break;
                }
            }
		if (s==0) // Append at the end
			{
            prev->nextValid=se;
            se->nextValid=0;
            }
        }
    s=se;
    }
return curSegs;
}


//****************************************************************
// Poligonos con antialiasing
//****************************************************************
void Flinea(int y,int x1,int x2,int x3,int x4)
{
if (x1<0) x1=0;
if (x2<0) x2=0;
if (x2>x3) { x2=x3;if (x1>x2) x1=x2; }
//if (x2>x3) { int t=x2;x2=x3;x3=t;}

int ex1=x1>>SEGFRAC,ex2=x2>>SEGFRAC;
int ex3=x3>>SEGFRAC,ex4=x4>>SEGFRAC;
int fx1=x1&MASFRAC,fx2=x2&MASFRAC;
int fx3=x3&MASFRAC,fx4=x4&MASFRAC;

if (ex4<0 || ex1>wp) return;

register int n;
WORD *point=screen.Buffer+screen.YPitch*y+screen.XPitch*ex1;
// 1er degrade
if (ex1==ex2)
	{
	if (ex2!=ex3) 
		{
		*point=screen.mix_alpha(*point,pColor,255-((fx1+fx2)>>1));
		point++;
		}
	else// ex3 esta en el mismo pixel que ex2
		{
		int f1=(fx1+fx2)>>1,f2=(fx3+fx4)>>1;
		if (ex3==ex4) // estan todos en el mismo pixel
			{
			*point=screen.mix_alpha(*point,pColor,(f1>f2)?f1-f2:f2-f1);
			return;
			}
		else //x4 esta mas adelante
			{
			*point=screen.mix_alpha(*point,pColor,(f1>f2)?f1:f2);
			}
		}
	}
else
	{
	n=ex2-ex1;
	int alpha=0;// el comienzo puede no ser 0
	int da=((255)<<8)/n;
	if (ex2>wp) 
		{
		alpha+=da*(ex2-wp);
		ex2=wp;
		n=ex2-ex1;
		}
	n++;
	while (n--) 
		{ 
		*point=screen.mix_alpha(*point,pColor,alpha>>8);
		point++;
		alpha+=da;
		}
	}
// relleno
if (ex3>wp)
	{
	n=wp-ex2;
	ex3=wp+1;
	}
else
	n=ex3-ex2-1;
if (n<0) n=0;
while (n--)
	{
	*point=pColor;
	point++;			
	}
// 2do degrade
if (ex3>wp) return;
if (ex3==ex4)
	{
	*point=screen.mix_alpha(*point,pColor,(fx3+fx4)>>1);
	}
else
	{
	n=ex4-ex3;
	int alpha=255<<8;
	int da=((0-255)<<8)/n;
	if (ex4>wp)
		{
		alpha+=da*(ex4-wp);
		n=wp-ex3;
		}
	n++;
	while (n--) 
		{ 
		*point=screen.mix_alpha(*point,pColor,alpha>>8);
		point++;
		alpha+=da;
		}
	}
}

//------------------------------------------
void drawPoli()
{
int x1,x2,x3,x4,t;
Segmento *curSegs,*s;
if (limymax==-1) return;
curSegs=NULL;
for(unsigned short y=limymin;y<limymax;y++) 
	{
    curSegs=avanzaS(curSegs,y);
    curSegs=nuevoS(curSegs,lines[y]);
	for(s=curSegs;s && s->nextValid;s=s->nextValid)
	    {
		if (s->nextValid->x<0) { s=s->nextValid;continue; } // anteriores a la pantalla
	    if (s->x>(wp<<SEGFRAC)) break; //siguientes de la pantalla
		x1=s->x;x2=s->x+s->dx;
		if (s->dx<0) 		{t=x1;x1=x2;x2=t;}
		s=s->nextValid;
		x3=s->x;x4=s->x+s->dx;
		if (s->dx<0) 		{t=x3;x3=x4;x4=t;}	
		Flinea(y,x1,x2,x3,x4);
	    }
    }
memset(&lines[limymin],0,(limymax-limymin+1)*sizeof(Segmento *));
limymax=-1;
limymin=hp+1;
linact=seg;
}

//****************************************************************
// poligonos sin antialiasing
//****************************************************************
void linePoliF(int y,int start,int end)
{
if (start>wp) return;
if (start<0) start=0;
if (start>=end) return;
if (end>wp) end=wp;
register int n=end-start;
WORD *point=screen.Buffer+screen.YPitch*y+screen.XPitch*start;
while (n--) 
	{ 
	*point=pColor;
	point++;			
	}
}

//------------------------------------------
void drawPoliF()
{
int x1,x2;
Segmento *curSegs,*s;
if (limymax==-1) return;
curSegs=NULL;
for(unsigned short y=limymin;y<limymax;y++) 
	{
    curSegs=avanzaS(curSegs,y);
    curSegs=nuevoS(curSegs,lines[y]);
	for(s=curSegs;s && s->nextValid;s=s->nextValid)
	    {
		if (s->nextValid->x<0) { s=s->nextValid;continue; } // anteriores a la pantalla
	    if (s->x>(wp<<SEGFRAC)) break; //siguientes de la pantalla
		x1=(s->dx<0)?s->x+s->dx:s->x;
		s=s->nextValid;
		x2=(s->dx<0)?s->x:s->x+s->dx;
		linePoliF(y,x1>>SEGFRAC,x2>>SEGFRAC);
	    }
    }
memset(&lines[limymin],0,(limymax-limymin+1)*sizeof(Segmento *));
limymax=-1;
limymin=hp+1;
linact=seg;
}

//****************************************************************
// dibujo de lineas de poligono NO POR AHORA (faltan horizontales)
//****************************************************************
void lineLine(int y,int x1,int x2)
{
if (x1<0) x1=0;
if (x2>(wp<<SEGFRAC)) x2=wp<<SEGFRAC;
if (x2<x1) return;

int ex1=x1>>SEGFRAC,ex2=x2>>SEGFRAC;
int fx1=x1&MASFRAC,fx2=x2&MASFRAC;

register int n=ex2-ex1+1;
WORD *point=screen.Buffer+screen.YPitch*y+screen.XPitch*ex1;
while (n--) 
	{ 
	*point=pColor;
	point++;			
	}
}

//------------------------------------------
void drawLine()
{
int x1,x2,t;
Segmento *curSegs,*s;
if (limymax==-1) return;
curSegs=NULL;
for(unsigned short y=limymin;y<limymax;y++) 
	{
    curSegs=avanzaS(curSegs,y);
    curSegs=nuevoS(curSegs,lines[y]);
	for(s=curSegs;s;s=s->nextValid)
	    {
		if (s->x<0) continue; // anteriores a la pantalla
	    if (s->x>(wp<<SEGFRAC)) break; //siguientes de la pantalla
		x1=s->x;x2=s->x+s->dx;
		if (s->dx<0) 		
			{t=x1;x1=x2;x2=t;}

		lineLine(y,x1,x2);
	    }
    }
memset(&lines[limymin],0,(limymax-limymin+1)*sizeof(Segmento *));
limymax=-1;
limymin=hp+1;
linact=seg;
}

/*
void FLineA(int y,int start,int end,WORD color,BYTE alpha)
{
if (start>wp) return;
if (start<0) start=0;
if (start>=end) return;
if (end>wp) end=wp;
register int n=end-start;
WORD *point=screen.Buffer +screen.YPitch*y +screen.XPitch*start;
while (n--) 
	{ 
	*point=screen.mix_alpha(*point,color,alpha);
	point++;			
	}
}

void FLineD(int y,int start,int end,WORD color1,WORD color2)
{
if (start>wp) return;
if (start<0) start=0;
if (start>=end) return;
if (end>wp) end=wp;
register int n=end-start;		
int alpha=0;
int da=((255)<<8)/n;
WORD *point=screen.Buffer +screen.YPitch*y +screen.XPitch*start;
while (n--) 
	{ 
	*point=screen.mix_alpha(color1,color2,alpha>>8);
	point++;
	alpha+=da;
	}
}

void FPointA(int y,int x,WORD color,BYTE a)
{
if (x<0 || x>wp) return;
WORD *point=screen.Buffer +screen.YPitch*y +screen.XPitch*x;
*point=screen.mix_alpha(*point,color,a);
}

void FLineAD(int y,int start,int end,WORD color,BYTE a1,BYTE a2)
{
if (start>wp) return;
if (start<0) start=0;
if (end>wp) end=wp;
if (start>=end) return;
register int n=end-start;		
int alpha=a1<<8;
int da=((a2-a1)<<8)/n;
WORD *point=screen.Buffer +screen.YPitch*y +screen.XPitch*start;
while (n--) 
	{ 
	*point=screen.mix_alpha(*point,color,alpha>>8);
	point++;
	alpha+=da;
	}
}

*/