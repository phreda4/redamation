#ifndef MAT_H
#define MAT_H

/* return values */
#define DONT_INTERSECT 0
#define DO_INTERSECT   1
#define COLLINEAR      2

/* The SAME_SIGNS macro assumes arithmetic where the exclusive-or    */
/* operation will work on sign bits.  This works for twos-complement,*/
/* and most other machine arithmetic.                                */
#define SAME_SIGNS(a,b) (((long)((unsigned long)a^(unsigned long) b))>=0)
//#define interpola(a,b,t1,t2) ((t2>t1)?(a*t1/t2+b*(t2-t1)/t2):(a*t1/t2+b*(t1-t2)/t2))
// intervalo 0..t2
#define interpola(a,b,t1,t2) (a*t1/t2+b*(t2-t1)/t2)
//#define interpolate(v1,v2,mix) (v1*mix+v2*(1-mix))
 

#define iabs(a) ((a<0)?-a:a)

extern unsigned short _sqrt_table[256];

inline int sqrtc(long n)
{
if (n<1) return 0;
else if (n<(1<<8)+1)	return (int)(_sqrt_table[n]>>12);
else if (n<(1<<16)+1)	return (int)(((long)0xFF83*_sqrt_table[n>>8])>>24);
else return 255; 
// else if (n<(1<<24)+1)return (int)(((long)0xFF83*0xFF83*_sqrt_table[n>>16])>>24);
}

inline int azar(int min,int max) 
{ 
return (rand()*(max-min)/RAND_MAX)+min;
}

extern short _sincos[];

inline void sincos(short ang,short &sin,short &cos)
{
ang=(ang>3600)?ang-3600:(ang<0)?ang+3600:ang;
if (ang<=1800)
  {
  if (ang<=900)
    {
    cos=_sincos[ang];
    sin=_sincos[900-ang];
    }
  else 
    {
    ang-=900;
    cos=-_sincos[900-ang];
    sin=_sincos[ang];
    }
  }
else
  {
  if (ang<=2700)
    {
    ang-=1800;
    cos=-_sincos[ang];
    sin=-_sincos[900-ang];
    }
  else
    {
    ang-=2700;
    cos=_sincos[900-ang];
    sin=-_sincos[ang];
    }
  }
}

inline int distfull(int x1,int y1,int x2,int y2)
{
long dx=x1-x2,dy=y1-y2;
return sqrtc(dx*dx+dy*dy);
}

inline int distance(int dx,int dy)// fast
{
int min,max;
if (dx<0) dx=-dx;
if (dy<0) dy=-dy;
if (dx<dy) { min=dx;max=dy; } else { min=dy;max=dx; }
// coefficients equivalent to ( 123/128 * max ) and ( 51/128 * min )
return (((max<<8)+(max<<3)-(max<<4)-(max<<1)+(min<<7)-(min<<5)+(min<<3)-(min<<1))>> 8);
}

inline int dist(int x1,int y1,int x2,int y2)
{
return distance(x1-x2,y1-y2);
}

inline long recta(int x1,int y1,int x2,int y2,int x3,int y3)
{
long dx1=x2-x1,dy1=y2-y1;
long dx2=x3-x1,dy2=y3-y1;
return abs(dx1*dy2-dx2*dy1);
}

inline int recta2(int x1,int y1,int x2,int y2,int x3,int y3)
{
long dx1=x2-x1,dy1=y2-y1;
long dx2=x3-x1,dy2=y3-y1;
if (abs(dx1)>abs(dx2) || abs(dy1)>abs(dy2))
	return 1000;
return abs(dx1*dy2-dx2*dy1);
}

inline long recta4(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
long dx1=x2-x1,dy1=y2-y1;
long dx2=x3-x1,dy2=y3-y1;
long dx3=x4-x1,dy3=y4-y1;
return abs(dx1*dy2-dx2*dy1)+abs(dx1*dy3-dx3*dy1);
}

inline short teta(long x1,long y1,long x2,long y2)// angulo del vector
{
long dx=x2-x1,ax=abs(dx);
long dy=y2-y1,ay=abs(dy);
if (ax+ay==0) return 0;
long t=(dy*900)/(ax+ay);
if (dx<0) t=(2*900)-t; else if (dy<0) t=(4*900)+t;
return (short)t;
}

inline short angle(long x1,long y1,long x2,long y2,long x3,long y3) // angulo entre vectores
{
return abs(teta(x1,y1,x2,y2)-teta(x2,y2,x3,y3));
}

inline int cocurva(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,long *rx,long *ry)
{
int ax=(x1+x2)>>1,ay=(y1+y2)>>1;
int b1x=(x2+x3)>>1,b1y=(y2+y3)>>1;
int b2x=(ax+b1x)>>1,b2y=(ay+b1y)>>1;
int b3x=(b1x+b2x)>>1,b3y=(b1y+b2y)>>1;
int b4x=(b1x+x3)>>1,b4y=(b1y+y3)>>1;
int b5x=(b3x+b4x)>>1,b5y=(b3y+b4y)>>1;
int m1x=(x4+x1)>>1,m1y=(y4+y1)>>1;
*rx=m1x+(b5x-m1x)*2;*ry=m1y+(b5y-m1y)*2;
return angle(x1,y1,x2,y2,*rx,*ry);
}

// x2 y2 no es punto de control, es punto en la curva
inline short cospl(long x1,long y1,long x2,long y2,long x3,long y3,long x4,long y4,short *rx,short *ry)
{
int m1x=(x3+x1)>>1,m1y=(y3+y1)>>1;
int ax=m1x+(x2-m1x)*2,ay=m1y+(y2-m1y)*2;
int m2x=(x4+x1)>>1,m2y=(y4+y1)>>1;
int b1x=(ax+x3)>>1,b1y=(ay+y3)>>1;
int b2x=(x2+b1x)>>1,b2y=(y2+b1y)>>1;
int b3x=(x3+b1x)>>1,b3y=(y3+b1y)>>1;
*rx=(b1x+b2x)>>1;*ry=(b1y+b2y)>>1;
int bx=m2x+(*rx-m2x)*2,by=m2y+(*ry-m2y)*2;
return angle(x1,y1,ax,ay,bx,by);
}

int lines_intersect(long x1,long y1,long x2,long y2,long x3,long y3,long x4,long y4,long *x,long *y) ;

// matriz de transformacion 2d

#define FRACMAT 14
 
struct Matrix {
	long a,b,c,d,tx,ty;

//	Matrix() { clear();}
	void clear()
	{
	a=1<<FRACMAT;d=1<<FRACMAT;
	b=c=0;tx=ty=0;
	}
	void invert(Matrix &mat)
	{
	long det;
	det=(a*d-b*c)>>FRACMAT;
	if (det==0) return;
	mat.a=(d<<FRACMAT)/det;
	mat.b=-(b<<FRACMAT)/det;
	mat.c=-(c<<FRACMAT)/det;
	mat.d=(a<<FRACMAT)/det;
	mat.tx=-((mat.a*tx+mat.b*ty)>>FRACMAT);
	mat.ty=-((mat.c*tx+mat.d*ty)>>FRACMAT);
	}
	inline void traslate(long x,long y)
	{ a=1<<FRACMAT;b=0;c=0;d=1<<FRACMAT;tx=x;ty=y; }
	inline void scale(long x,long y)
	{ a=x;b=0;c=0;d=y;tx=ty=0; }

	void rotate(short an)
	{
	short sen,cos;
	long lsen,lcos;
	sincos(an,sen,cos);///0x4000 a >>8 => 
	lsen=(long)sen;lcos=(long)cos;//lsen<<=1;lcos<<=1;//	lsen>>=6;lcos>>=6;
	a=lcos;b=-lsen;c=lsen;d=lcos;
	tx=ty=0;
	}

	void transforma(long x, long y,long *xr,long *yr)
	{ 
/*
	*xr=((x*a)>>FRACMAT)+((y*b)>>FRACMAT)+tx;
	*yr=((x*c)>>FRACMAT)+((y*d)>>FRACMAT)+ty;
*/	
	*xr=((x*a+y*b)>>FRACMAT)+tx;
	*yr=((x*c+y*d)>>FRACMAT)+ty;
	}

	void mult(const Matrix m)
	{     
	Matrix mat;
/*
	mat.a=((a*m.a)>>FRACMAT)+((b*m.c)>>FRACMAT);
	mat.b=((a*m.b)>>FRACMAT)+((b*m.d)>>FRACMAT);
	mat.c=((c*m.a)>>FRACMAT)+((d*m.c)>>FRACMAT);
	mat.d=((c*m.b)>>FRACMAT)+((d*m.d)>>FRACMAT);
*/
	mat.a=(a*m.a+b*m.c)>>FRACMAT;
	mat.b=(a*m.b+b*m.d)>>FRACMAT;
	mat.c=(c*m.a+d*m.c)>>FRACMAT;
	mat.d=(c*m.b+d*m.d)>>FRACMAT;
	transforma(m.tx,m.ty,&mat.tx,&mat.ty);
	a=mat.a;b=mat.b;c=mat.c;d=mat.d;tx=mat.tx;ty=mat.ty;
	}
};

// 1<<4
#define MINZOOM 1
#define MAXZOOM (1<<FRACMAT)

struct Vista {
	short angle,xt,yt,sc;

	void clear()				{ angle=0;sc=MAXZOOM/10;xt=0;yt=0; }
	void position(short x,short y)	{ xt=x;yt=y; }
	void move(short x,short y)		{ xt+=x;yt+=y; }
	void scale(short s)			{ sc=s; }
	void rscale(short s)		{ sc+=s; }
	void rota(short a)			{ angle=a; }
	void rrota(short a)			{ angle+=a; }

	void makeMatrix(Matrix &m)
	{
	Matrix m1;
	m.traslate(120,160);
	m1.scale(sc,sc);m.mult(m1);
	m1.rotate(angle);m.mult(m1);
	m1.traslate(xt,yt);m.mult(m1);
	};
	void makeMatrix(Matrix &m,Matrix &mi)
	{
	Matrix m1;
	m.traslate(120,160);
	m1.scale(sc,sc);m.mult(m1);
	m1.rotate(angle);m.mult(m1);
	m1.traslate(xt,yt);m.mult(m1);
	m.invert(mi);
	};

};

#endif