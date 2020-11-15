#ifndef MAT_H
#define MAT_H

#define interpola(a,b,t1,t2) a*t1/t2+b*(t2-t1)/t2

extern unsigned short _sqrt_table[256];

inline int sqrtc(long n)
{
if (n<1) return 0;
else if (n<(1<<8)+1)	return (int)(_sqrt_table[n]>>12);
else if (n<(1<<16)+1)	return (int)(((long)0xFF83*_sqrt_table[n>>8])>>24);
else return 255; 
// else if (n<(1<<24)+1)return (int)(((long)0xFF83*0xFF83*_sqrt_table[n>>16])>>24);
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

inline int dist(int x1,int y1,int x2,int y2)
{
long dx=x1-x2,dy=y1-y2;
return sqrtc(dx*dx+dy*dy);
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

// matriz de transformacion 2d

#define FRACMAT 14
 
struct Matrix {
	long a,b,c,d;
	long tx,ty;

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
	sincos(an,sen,cos);///0x4000 a >>8 => >>6
	lsen=(long)sen;lcos=(long)cos;
	//lsen<<=1;lcos<<=1;
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

#endif