#include "stdafx.h"
#include "graph.h"
//:::::::::::::::::: clip
Rect clip_rect;
//::::::::::::::::::pixel y color
WORD fColor;
Color pixel;
PatronColor *pc;

void (*_fillLine)(int,int,int);	// funcion de llenado
void (*_putpixel)(WORD *pp);

//*******************************************
// CLIP
//*******************************************
void SetClip(int x1,int y1,int x2,int y2)
{
clip_rect.xmin=x1;clip_rect.xmax=x2;
clip_rect.ymin=y1;clip_rect.ymax=y2;
}
//*******************************************
bool clip(int &y,int &start,int &end)
{
int xmin,xend;
if (y < clip_rect.ymin || y >= clip_rect.ymax) return true;
if (end <= start) return true;
xmin = clip_rect.xmin;
xend = clip_rect.xmax;
if (end <= xmin || start >= xend) return true;
if (start < xmin) start = xmin;
if (end > xend) end = xend;
return false;
}
//*******************************************
// PIXEL y COLOR
//*******************************************
void _fillFlatLine(int y,int start,int end)
{
register int n;
if (clip(y,start,end)) return;
WORD *point=screen.Buffer + screen.YPitch*y +screen.XPitch*start;
n = end-start;				
while (n--) 
	{
	*point=pixel.color;
	point++;			
	}
}
//*******************************************
void _fillAlphaLine(int y,int start,int end)
{
register int n;
if (clip(y,start,end)) return;
WORD *point=screen.Buffer +screen.YPitch*y +screen.XPitch*start;
n = end-start;				
while (n--) 
	{ 
	*point=screen.mix_alpha(*point, pixel.color, pixel.alpha);
	point++;			
	}
}
//*******************************************
void _fillLGLine(int y,int start,int end)
{
long dr,r,v,r2;
register int n;
Matrix *m = &pc->imat;
if (clip(y,start,end)) return;
n = end-start;
r = (long) (m->a * start + m->b * y + m->tx);
dr = (long) (m->a);
Color *cp,*ramp=pc->ramp;
WORD *point=(WORD*)screen.Buffer+screen.YPitch*y+screen.XPitch*start;
r2 = r + n * dr;
while (n--) 
	{
	v = r>>16;
	v&=0xff;
	cp = &ramp[v];
	*point=screen.mix_alpha(*point,cp->color, cp->alpha);
	point++;
	r += dr;
	}
}
//*******************************************
void _fillRGLine(int y,int start,int end)
{
long X,dx,r,Y,dy;
int dist2;
Matrix *m=&pc->imat;
if (clip(y,start,end)) return;
register int n=end-start;
X=(long)(m->a * start + m->b * y + m->tx);
Y=(long)(m->c * start + m->d * y + m->ty);
dx=(long)(m->a);
dy=(long)(m->c);
Color *cp,*ramp=pc->ramp;
WORD *point=(WORD*)screen.Buffer+screen.YPitch*y+screen.XPitch*start;
while (n--) 
	{
	dist2 = ((X>>16)*(X>>16))+((Y>>16)*(Y>>16));
	r=sqrtc(dist2);
	cp = &ramp[r];
	*point=screen.mix_alpha(*point, cp->color, cp->alpha);
	point++;
	X+=dx;						
	Y+=dy;						
	}		
}
//*******************************************
void SetLinearPat(PatronColor &p,long a1,long b1,long tx1,long ty1)
{
int i;
for (i=0;i<256;i++)
	{
	p.ramp[i].color=0xff00+i;
	p.ramp[i].alpha=255;
	}
p.imat.a=a1;
p.imat.b=b1;
//p.imat.c=c1;
//p.imat.d=d1;
p.imat.tx=tx1;
p.imat.ty=ty1;
/*
Matrix mat;
int  n,r,l;
long red, green, blue, alpha;
long dRed, dGreen, dBlue, dAlpha;
long min,max;
Matrix *m;
mat = *(matrix) * f->matrix;
// Compute inverted matrix
f->gradient.imat = mat.invert();
///* renormalize the matrix 
m=&f->gradient.imat;
//if (f->type == f_LinearGradient) {
	m->a = m->a * FRAC * (1/128.0) * 65536.0;
	m->b = m->b * FRAC * (1/128.0) * 65536.0;
	m->tx = (long) ((m->tx + 16384) * (1/128.0) * 65536.0);
} else {
	m->a = m->a * FRAC * (1/64.0) * 65536.0;
	m->b = m->b * FRAC * (1/64.0) * 65536.0;
	m->c = m->c * FRAC * (1/64.0) * 65536.0;
	m->d = m->d * FRAC * (1/64.0) * 65536.0;
	m->tx = (long) (m->tx * (1/64.0) * 65536.0);
	m->ty = (long) (m->ty * (1/64.0) * 65536.0);
}
// Reset translation in inverted matrix
//f->gradient.has_alpha = 0;
// Build a 256 color ramp
// Store min and max
min = f->gradient.ratio[0];
max = f->gradient.ratio[f->gradient.nbGradients-1];
for(r=0;r<f->gradient.nbGradients-1;r++)
	{
	Color start,end;
	l = f->gradient.ratio[r+1]-f->gradient.ratio[r];
	if (l == 0) continue;
	if (cxform) {
		start = cxform->getColor(f->gradient.color[r]);
		end   = cxform->getColor(f->gradient.color[r+1]);
	} else {
		start = f->gradient.color[r];
		end   = f->gradient.color[r+1];
	}
	if (start.alpha != ALPHA_OPAQUE || end.alpha != ALPHA_OPAQUE) {
		f->gradient.has_alpha = 1;
		}
	dRed   = end.red - start.red;
	dGreen = end.green - start.green;
	dBlue  = end.blue - start.blue;
	dAlpha = end.alpha - start.alpha;
	dRed   = (dRed<<16)/l;
	dGreen = (dGreen<<16)/l;
	dBlue  = (dBlue<<16)/l;
	dAlpha  = (dAlpha<<16)/l;
	red   = start.red <<16;
	green = start.green <<16;
	blue  = start.blue <<16;
	alpha  = start.alpha <<16;
	for (n=f->gradient.ratio[r]; n<=f->gradient.ratio[r+1]; n++) 
		{
		f->gradient.ramp[n].red = red>>16;
		f->gradient.ramp[n].green = green>>16;
		f->gradient.ramp[n].blue = blue>>16;
		f->gradient.ramp[n].alpha = alpha>>16;
		f->gradient.ramp[n].pixel = gd->allocColor(f->gradient.ramp[n]);
		red += dRed;
		green += dGreen;
		blue += dBlue;
		alpha += dAlpha;
		}
	}
for(n=0; n<min; n++) {
	f->gradient.ramp[n].pixel = f->gradient.ramp[min].pixel;
	f->gradient.ramp[n].alpha = f->gradient.ramp[min].alpha;
	}
for(n=max; n<256; n++) {
	f->gradient.ramp[n].pixel = f->gradient.ramp[max].pixel;
	f->gradient.ramp[n].alpha = f->gradient.ramp[max].alpha;
	}
*/
}
//*******************************************
void SetRadialPat(PatronColor &p,long a1,long b1,long c1,long d1,long tx1,long ty1)
{
int i;
for (i=0;i<256;i++)
	{
	p.ramp[i].color=0xff00+i;
	p.ramp[i].alpha=255;
	}
p.imat.a=a1;
p.imat.b=b1;
p.imat.c=c1;
p.imat.d=d1;
p.imat.tx=tx1;
p.imat.ty=ty1;
}
//******************************************
// SET COLOR y PIXEL
//*******************************************
void _pixelCopy(WORD *pp)
{
*pp=fColor;
}
//*******************************************
void _pixelXOR(WORD *pp)
{
*pp=*pp^fColor;
}
//*******************************************

//*******************************************
void SetPixelCopy()
{
_putpixel=_pixelCopy;
}
//*******************************************
void SetPixelXOR()
{
_putpixel=_pixelXOR;
}
//*******************************************
void SetSolidColor(WORD c)
{
pixel.color=c;
pixel.alpha=255;
_fillLine=_fillFlatLine;
}
//*******************************************
void SetSolidAColor(Color &c)
{
pixel=c;
if (pixel.alpha==255)
	_fillLine=_fillFlatLine;
else
	_fillLine=_fillAlphaLine;
}
//*******************************************
void SetLinearColor(PatronColor *p)
{
pc=p;
_fillLine=_fillLGLine;
}
//*******************************************
void SetRadialColor(PatronColor *p)
{
pc=p;
_fillLine=_fillRGLine;
}
//*******************************************
//void SetBitmapColor();
//*******************************************
// DIBUJO
//*******************************************
void ClrSrc(WORD color)
{
register WORD *pp=screen.Buffer;
for (register int i=0;i<=screen.BufferEndOffset;i++,pp++)
  *pp=color;
}
//*******************************************
void Line(int x1,int y1,int x2,int y2)
{
int n,dx,dy;
register int a;
if (y1 > y2 || (y1 == y2 && x1 > x2)) 
	{long tmp;tmp=x1;x1=x2;x2=tmp;tmp=y1;y1=y2;y2=tmp;}
if (y1 < clip_rect.ymin && y1 != y2) 
	{
	x1 += (x2-x1)*(clip_rect.ymin-y1)/(y2-y1);
	y1 = clip_rect.ymin;
    }
if (y2 > clip_rect.ymax && y1 != y2) 
	{
	x2 -= (x2-x1)*(y2-clip_rect.ymax)/(y2-y1);
	y2 = clip_rect.ymax;
    }
if (x1 < x2) 
	{
	if (x1 < clip_rect.xmin && x1 != x2) 
		{
		y1 += (y2-y1)*(clip_rect.xmin-x1)/(x2-x1);
		x1 = clip_rect.xmin;
	    }
    if (x2 > clip_rect.xmax && x1 != x2) 
		{
		y2 -= (y2-y1)*(x2-clip_rect.xmax)/(x2-x1);
		x2 = clip_rect.xmax;
	    }
    }
if (x1 > x2) 
	{
	if (x2 < clip_rect.xmin && x2 != x1) 
		{
		y2 -= (y2-y1)*(clip_rect.xmin-x2)/(x1-x2);
		x2 = clip_rect.xmin;
	    }
    if (x1 > clip_rect.xmax && x2 != x1) 
		{
		y1 += (y2-y1)*(x1-clip_rect.xmax)/(x1-x2);
		x1 = clip_rect.xmax;
	    }
    }
if (x1 == x2 && y1 == y2) return;
if (x1 < clip_rect.xmin || x2 < clip_rect.xmin) return;
if (y1 < clip_rect.ymin || y2 < clip_rect.ymin) return;
if (x1 > clip_rect.xmax || x2 > clip_rect.xmax) return;
if (y1 > clip_rect.ymax || y2 > clip_rect.ymax) return;
register WORD *pp=screen.Buffer+screen.XPitch*x1+screen.YPitch*y1;
dx=x2-x1;dy=y2-y1;
//**********************
/* macros */
#define PUTPIXEL()  {_putpixel(pp);}
#define DRAWLINE(dx,dy,inc_1,inc_2) \
  n=dx;a=2*dy-dx;\
  dy=2*dy;dx=2*dx-dy;\
  do {\
    PUTPIXEL();\
	if (a>0) { pp+=(inc_1); a-=dx; }\
	else { pp+=(inc_2); a+=dy; }\
  } while (--n >= 0);
/* fin macro */
//**********************
int yp1=screen.YPitch,yp2=screen.YPitch+1,yp3=screen.YPitch-1;
if (dx == 0 && dy == 0) 
	{
    PUTPIXEL();
	} 
else if (dx > 0) 
	{
    if (dx >= dy) 
		{DRAWLINE(dx, dy,yp2,1);} 
	else 
		{DRAWLINE(dy, dx,yp2,yp1);}
	} 
else 
	{
    dx = -dx;
    if (dx >= dy) 
		{DRAWLINE(dx, dy,yp3, -1);} 
	else 
		{DRAWLINE(dy, dx,yp3,yp1);}
	}
#undef DRAWLINE
#undef PUTPIXEL
}
//*******************************************
void VLine(int x1,int y1,int y2)
{
if (y1>y2) { int tmp;tmp=y1;y1=y2;y2=tmp; }
if (x1<clip_rect.xmin || x1>clip_rect.xmax) return;
if (y2<clip_rect.ymin || y1>clip_rect.ymax) return;
if (y1<clip_rect.ymin) y1=clip_rect.ymin;
if (y2>clip_rect.ymax) y2=clip_rect.ymax;
int cnt=y2-y1+1;
register WORD *pp=(WORD*)screen.Buffer+screen.XPitch*x1+screen.YPitch*y1;
for (register int i=0;i<cnt;i++,pp+=screen.YPitch)
	_putpixel(pp);
}
//*******************************************
void HLine(int x1,int y1,int x2)
{
if (x1>x2) { int tmp;tmp=x1;x1=x2;x2=tmp; }
if (y1<clip_rect.ymin || y1>clip_rect.ymax) return;
if (x2<clip_rect.xmin || x1>clip_rect.xmax) return;
if (x1<clip_rect.xmin) x1=clip_rect.xmin;
if (x2>clip_rect.xmax) x2=clip_rect.xmax;
int cnt=x2-x1+1;
register WORD *pp=screen.Buffer+screen.XPitch*x1+screen.YPitch*y1;
for (register int i=0;i<cnt;i++,pp++)
	_putpixel(pp);
}
//*******************************************
void HLineD(int x1,int y1,int x2,WORD c1,WORD c2)
{
int cnt=x2-x1+1;
BYTE r1,g1,b1,r2,g2,b2;
screen.getRGBColor(r1,g1,b1,c1);
screen.getRGBColor(r2,g2,b2,c2);
int r=r1,g=g1,b=b1;
r<<=8;g<<=8;b<<=8;
int dr=r2-r1,dg=g2-g1,db=b2-b1;
dr<<=8;dg<<=8;db<<=8;
dr/=cnt;dg/=cnt;db/=cnt;
register WORD *pp=screen.Buffer+screen.XPitch*x1+screen.YPitch*y1;
for (register int i=0;i<cnt;i++,pp++)
	{
	SetRGB(r>>8,g>>8,b>>8);
	r+=dr;g+=dg;b+=db;
	_putpixel(pp);
	}

}
//*******************************************
//*******************************************
void Box(int x1,int y1,int x2,int y2)
{
VLine(x1,y1,y2);VLine(x2,y1,y2);HLine(x1,y1,x2);HLine(x1,y2,x2);
}
//*******************************************
void BoxFill(int x1,int y1,int x2,int y2)
{
for (int y=y1;y<y2+1;y++)
	HLine(x1,y,x2);
}
//----------------------------------
inline void _SetPixel(int X,int Y)
{
if (X<0 || Y<0 || X>=clip_rect.xmax || Y>=clip_rect.ymax) return;
register WORD *dwOffset=screen.Buffer+X*screen.XPitch+Y*screen.YPitch;
_putpixel(dwOffset);
}
//*******************************************
void Circle(int x,int y,int r)
{
int cx=0,cy=r;
int df=1-r; 
int d_e=3;
int d_se=-2*r+5;
do {
	_SetPixel(x+cx, y+cy);
    if (cx)	_SetPixel(x-cx,y+cy);
    if (cy)	_SetPixel(x+cx,y-cy);
    if ((cx) && (cy)) _SetPixel(x-cx,y-cy);
    if (cx!=cy) 
		{
		_SetPixel(x+cy,y+cx);
		if (cx) _SetPixel(x+cy,y-cx);
		if (cy) _SetPixel(x-cy,y+cx);
		if (cx && cy) _SetPixel(x-cy,y-cx);
		}
    if (df<0)  
		{
		df+=d_e;d_e +=2;d_se+=2;
		}
	else 
		{ 
		df+=d_se;
		d_e+=2;
		d_se+=4;
		cy--;
		} 
	cx++; 
} while (cx<=cy);
}
//*******************************************
void Ellipse(int x, int y, int rx, int ry)
{
int ix,iy;
int h,i,j,k;
int oh,oi,oj,ok;
if (rx<1) rx=1; 
if (ry<1) ry=1;
h=i=j=k=0xFFFF;
if (rx>ry) 
	{
	ix=0;iy=rx*64;
    do {
	 oh=h;oi=i;oj=j;ok=k;
	 h=(ix+32)>>6;i=(iy+32)>>6;
	 j=(h*ry)/rx;k=(i*ry)/rx;
	 if (((h!=oh)||(k!=ok))&&(h<oi)) 
		{
		_SetPixel(x+h,y+k); 
	    if (h) _SetPixel(x-h,y+k);
	    if (k) 
			{
			_SetPixel(x+h,y-k); 
		    if (h) _SetPixel(x-h,y-k);
			}
		}
	 if (((i!=oi)||(j!=oj))&&(h<i)) 
		{
	    _SetPixel(x+i,y+j); 
	    if (i) _SetPixel(x-i,y+j);
	    if (j) 
			{
			_SetPixel(x+i,y-j); 
			if (i) _SetPixel(x-i,y-j);
			}
		}
	 ix=ix+iy/rx;iy=iy-ix/rx;
     } while (i > h);
	} 
else 
	{
	ix=0;iy=ry*64;
    do {
	 oh=h;oi=i;oj=j;ok=k;
	 h=(ix+32)>>6;i=(iy+32)>>6;
	 j=(h*rx)/ry;k=(i*rx)/ry;
	 if (((j!=oj)||(i!=oi)) && (h<i)) 
		{
	    _SetPixel(x+j,y+i); 
	    if (j) _SetPixel(x-j,y+i);
	    if (i) 
			{
			_SetPixel(x+j,y-i); 
			if (j) _SetPixel(x-j,y-i);
			}
		}
	 if (((k != ok) || (h != oh)) && (h < oi)) 
		{
	    _SetPixel(x+k,y+h); 
	    if (k) _SetPixel(x-k,y+h);
	    if (h) 
			{
			_SetPixel(x+k,y-h); 
			if (k) _SetPixel(x-k,y-h);
			}
		}
	 ix=ix+iy/ry;iy=iy-ix/ry;
	} while(i > h);
	}
}
//***************
void EllipseFill(int x, int y, int rx, int ry)
{
int ix, iy;
int a, b, c, d;
int da, db, dc, dd;
int na, nb, nc, nd;
int sx, sy, dx, dy;
sx = x-rx-1;sy = y-ry-1;dx = x+rx+1;dy = y+ry+1;
if ((sx>=clip_rect.xmax) || (sy>=clip_rect.ymax) || (dx<clip_rect.xmin) || (dy<clip_rect.ymin))
	 return;
if (rx<1) rx = 1;
if (ry<1) ry = 1;
if (rx>ry) 
	{
	dc = -1;dd = 0xFFFF;
	ix = 0;iy = rx * 64;
	na = 0;nb = (iy + 32) >> 6;
	nc = 0;nd = (nb * ry) / rx;
	do {
	 a = na;b = nb;c = nc;d = nd;
	 ix = ix + (iy / rx);iy = iy - (ix / rx);
	 na = (ix + 32) >> 6; nb = (iy + 32) >> 6;
	 nc = (na * ry) / rx; nd = (nb * ry) / rx;
	 if ((c > dc) && (c < dd)) 
		{
		_fillLine(x-b,y+c,x+b);
	    if (c) _fillLine(x-b,y-c,x+b);
	    dc = c;
		}
	 if ((d < dd) && (d > dc)) 
		{ 
	    _fillLine(x-a, y+d, x+a);
	    _fillLine(x-a, y-d, x+a);
	    dd = d;
		}
      } while(b > a);
	} 
else 
	{
	da = -1;db = 0xFFFF;
	ix = 0;iy = ry * 64; 
	na = 0;nb = (iy + 32) >> 6;
	nc = 0;nd = (nb * rx) / ry;
    do {
	 a = na;b = nb;c = nc;d = nd; 
	 ix = ix + (iy / ry);iy = iy - (ix / ry);
	 na = (ix + 32) >> 6;nb = (iy + 32) >> 6;
	 nc = (na * rx) / ry;nd = (nb * rx) / ry;
	 if ((a > da) && (a < db)) 
		{
	    _fillLine(x-d, y+a, x+d); 
	    if (a) _fillLine(x-d, y-a, x+d);
	    da = a;
		}

	 if ((b < db) && (b > da)) 
		{ 
	    _fillLine(x-c, y+b, x+c);
	    _fillLine(x-c, y-b, x+c);
	    db = b;
		}
      } while(b > a);
   }
}
//---------------------------------------------- calculo de curva de bezier
void SPl(long x1,long y1,long x2,long y2,long x3,long y3,long x4,long y4)
{
if (recta4(x1,y1,x2,y2,x3,y3,x4,y4)<8000)
	{
	Line(x1>>4,y1>>4,x4>>4,y4>>4);
	return;
	}
long hx=(x2+x3)>>1,hy=(y2+y3)>>1;
long l1x=(x1+x2)>>1,l1y=(y1+y2)>>1;
long l2x=(l1x+hx)>>1,l2y=(l1y+hy)>>1;
long r1x=(x3+x4)>>1,r1y=(y3+y4)>>1;
long r2x=(r1x+hx)>>1,r2y=(r1y+hy)>>1;
long xs=(l2x+r2x)>>1,ys=(l2y+r2y)>>1;
SPl(x1,y1,l1x,l1y,l2x,l2y,xs,ys);
SPl(xs,ys,r2x,r2y,r1x,r1y,x4,y4);
}
//*********************************************
void SPline(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
SPl(x1<<4,y1<<4,x2<<4,y2<<4,x3<<4,y3<<4,x4<<4,y4<<4);
}
//--------------------------------------------- bezier de 1 punto
void SPli(long x1,long y1,long x2,long y2,long x3,long y3)
{
if (recta(x1,y1,x2,y2,x3,y3)<8000)
	{
	Line(x1>>4,y1>>4,x3>>4,y3>>4);
	return;
	}
long x11=(x1+x2)>>1,y11=(y1+y2)>>1;
long x21=(x2+x3)>>1,y21=(y2+y3)>>1;
long x22=(x11+x21)>>1,y22=(y11+y21)>>1;
SPli(x1,y1,x11,y11,x22,y22);
SPli(x22,y22,x21,y21,x3,y3);
}
//**********************************************
void SPlin(int x1,int y1,int x2,int y2,int x3,int y3)
{
SPli(x1<<4,y1<<4,x2<<4,y2<<4,x3<<4,y3<<4);
}
//***********************************
// TEXTO
//***********************************
const unsigned char fuente[]={
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x10,0x10,0x10,0x0,0x10,0x0,
0x0,0x24,0x24,0x0,0x0,0x0,0x0,0x0,0x0,0x24,0x7e,0x24,0x24,0x7e,0x24,0x0,
0x0,0x8,0x3e,0x28,0x3e,0xa,0x3e,0x8,0x0,0x62,0x64,0x8,0x10,0x26,0x46,0x0,
0x0,0x10,0x28,0x10,0x2a,0x44,0x3a,0x0,0x0,0x8,0x10,0x0,0x0,0x0,0x0,0x0,
0x0,0x4,0x8,0x8,0x8,0x8,0x4,0x0,0x0,0x20,0x10,0x10,0x10,0x10,0x20,0x0,
0x0,0x0,0x14,0x8,0x3e,0x8,0x14,0x0,0x0,0x0,0x8,0x8,0x3e,0x8,0x8,0x0,
0x0,0x0,0x0,0x0,0x0,0x8,0x8,0x10,0x0,0x0,0x0,0x0,0x3e,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x18,0x18,0x0,0x0,0x0,0x2,0x4,0x8,0x10,0x20,0x0,
0x0,0x3c,0x46,0x4a,0x52,0x62,0x3c,0x0,0x0,0x18,0x28,0x8,0x8,0x8,0x3e,0x0,
0x0,0x3c,0x42,0x2,0x3c,0x40,0x7e,0x0,0x0,0x3c,0x42,0xc,0x2,0x42,0x3c,0x0,
0x0,0x8,0x18,0x28,0x48,0x7e,0x8,0x0,0x0,0x7e,0x40,0x7c,0x2,0x42,0x3c,0x0,
0x0,0x3c,0x40,0x7c,0x42,0x42,0x3c,0x0,0x0,0x7e,0x2,0x4,0x8,0x10,0x10,0x0,
0x0,0x3c,0x42,0x3c,0x42,0x42,0x3c,0x0,0x0,0x3c,0x42,0x42,0x3e,0x2,0x3c,0x0,
0x0,0x0,0x0,0x10,0x0,0x0,0x10,0x0,0x0,0x0,0x10,0x0,0x0,0x10,0x10,0x20,
0x0,0x0,0x4,0x8,0x10,0x8,0x4,0x0,0x0,0x0,0x0,0x3e,0x0,0x3e,0x0,0x0,
0x0,0x0,0x10,0x8,0x4,0x8,0x10,0x0,0x0,0x3c,0x42,0x4,0x8,0x0,0x8,0x0,
0x0,0x3c,0x4a,0x56,0x5e,0x40,0x3c,0x0,0x0,0x3c,0x42,0x42,0x7e,0x42,0x42,0x0,
0x0,0x7c,0x42,0x7c,0x42,0x42,0x7c,0x0,0x0,0x3c,0x42,0x40,0x40,0x42,0x3c,0x0,
0x0,0x78,0x44,0x42,0x42,0x44,0x78,0x0,0x0,0x7e,0x40,0x7c,0x40,0x40,0x7e,0x0,
0x0,0x7e,0x40,0x7c,0x40,0x40,0x40,0x0,0x0,0x3c,0x42,0x40,0x4e,0x42,0x3c,0x0,
0x0,0x42,0x42,0x7e,0x42,0x42,0x42,0x0,0x0,0x3e,0x8,0x8,0x8,0x8,0x3e,0x0,
0x0,0x2,0x2,0x2,0x42,0x42,0x3c,0x0,0x0,0x44,0x48,0x70,0x48,0x44,0x42,0x0,
0x0,0x40,0x40,0x40,0x40,0x40,0x7e,0x0,0x0,0x42,0x66,0x5a,0x42,0x42,0x42,0x0,
0x0,0x42,0x62,0x52,0x4a,0x46,0x42,0x0,0x0,0x3c,0x42,0x42,0x42,0x42,0x3c,0x0,
0x0,0x7c,0x42,0x42,0x7c,0x40,0x40,0x0,0x0,0x3c,0x42,0x42,0x52,0x4a,0x3c,0x0,
0x0,0x7c,0x42,0x42,0x7c,0x44,0x42,0x0,0x0,0x3c,0x40,0x3c,0x2,0x42,0x3c,0x0,
0x0,0xfe,0x10,0x10,0x10,0x10,0x10,0x0,0x0,0x42,0x42,0x42,0x42,0x42,0x3c,0x0,
0x0,0x42,0x42,0x42,0x42,0x24,0x18,0x0,0x0,0x42,0x42,0x42,0x42,0x5a,0x24,0x0,
0x0,0x42,0x24,0x18,0x18,0x24,0x42,0x0,0x0,0x82,0x44,0x28,0x10,0x10,0x10,0x0,
0x0,0x7e,0x4,0x8,0x10,0x20,0x7e,0x0,0x0,0xe,0x8,0x8,0x8,0x8,0xe,0x0,
0x0,0x0,0x40,0x20,0x10,0x8,0x4,0x0,0x0,0x70,0x10,0x10,0x10,0x10,0x70,0x0,
0x0,0x10,0x38,0x54,0x10,0x10,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,
0x0,0x1c,0x22,0x78,0x20,0x20,0x7e,0x0,0x0,0x0,0x38,0x4,0x3c,0x44,0x3c,0x0,
0x0,0x20,0x20,0x3c,0x22,0x22,0x3c,0x0,0x0,0x0,0x1c,0x20,0x20,0x20,0x1c,0x0,
0x0,0x4,0x4,0x3c,0x44,0x44,0x3c,0x0,0x0,0x0,0x38,0x44,0x78,0x40,0x3c,0x0,
0x0,0xc,0x10,0x18,0x10,0x10,0x10,0x0,0x0,0x0,0x3c,0x44,0x44,0x3c,0x4,0x38,
0x0,0x40,0x40,0x78,0x44,0x44,0x44,0x0,0x0,0x10,0x0,0x30,0x10,0x10,0x38,0x0,
0x0,0x4,0x0,0x4,0x4,0x4,0x24,0x18,0x0,0x20,0x28,0x30,0x30,0x28,0x24,0x0,
0x0,0x10,0x10,0x10,0x10,0x10,0xc,0x0,0x0,0x0,0x68,0x54,0x54,0x54,0x54,0x0,
0x0,0x0,0x78,0x44,0x44,0x44,0x44,0x0,0x0,0x0,0x38,0x44,0x44,0x44,0x38,0x0,
0x0,0x0,0x78,0x44,0x44,0x78,0x40,0x40,0x0,0x0,0x3c,0x44,0x44,0x3c,0x4,0x6,
0x0,0x0,0x1c,0x20,0x20,0x20,0x20,0x0,0x0,0x0,0x38,0x40,0x38,0x4,0x78,0x0,
0x0,0x10,0x38,0x10,0x10,0x10,0xc,0x0,0x0,0x0,0x44,0x44,0x44,0x44,0x38,0x0,
0x0,0x0,0x44,0x44,0x28,0x28,0x10,0x0,0x0,0x0,0x44,0x54,0x54,0x54,0x28,0x0,
0x0,0x0,0x44,0x28,0x10,0x28,0x44,0x0,0x0,0x0,0x44,0x44,0x44,0x3c,0x4,0x38,
0x0,0x0,0x7c,0x8,0x10,0x20,0x7c,0x0,0x0,0xe,0x8,0x30,0x8,0x8,0xe,0x0,
0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x70,0x10,0xc,0x10,0x10,0x70,0x0,
0x0,0x14,0x28,0x0,0x0,0x0,0x0,0x0,0x3c,0x42,0x99,0xa1,0xa1,0x99,0x42,0x3c,
};

const unsigned char fuente2[]={
0,0,0,0,0,0,0,0,56,56,56,56,56,0,56,56,
108,108,0,0,0,0,0,0,0,108,254,254,108,254,254,108,
48,126,224,124,14,254,252,48,98,230,204,24,48,102,206,140,
120,220,252,120,250,222,252,118,28,28,56,0,0,0,0,0,
14,28,28,28,28,28,28,14,112,56,56,56,56,56,56,112,
0,0,0,230,124,56,124,206,0,0,28,28,127,127,28,28,
0,0,0,0,0,28,28,56,0,0,0,0,124,124,0,0,
0,0,0,0,0,0,56,56,28,28,56,56,112,112,224,224,
124,254,238,238,238,254,254,124,56,120,248,56,56,254,254,254,
252,254,14,60,112,254,254,254,252,254,14,60,14,254,254,252,
238,238,238,254,254,14,14,14,254,254,224,252,14,254,254,252,
124,252,224,252,238,254,254,124,252,254,14,14,28,28,56,56,
124,254,238,124,238,254,254,124,124,254,238,126,14,254,254,252,
0,0,28,28,0,28,28,28,0,0,28,28,0,28,28,56,
6,14,28,56,56,28,14,6,0,0,124,124,0,124,124,124,
112,56,28,14,14,28,56,112,124,254,206,28,56,0,56,56,
124,198,190,182,190,182,200,126,124,254,238,254,238,238,238,238,
252,254,206,252,206,254,254,252,124,254,238,224,238,254,254,124,
252,254,238,238,238,254,254,252,254,254,224,248,224,254,254,254,
126,254,224,248,224,224,224,224,126,254,224,238,238,254,254,124,
238,238,238,254,238,238,238,238,254,254,56,56,56,254,254,254,
254,254,14,14,238,254,254,124,238,238,252,248,252,238,238,238,
224,224,224,224,224,254,254,126,130,198,238,254,254,238,238,238,
206,238,254,254,254,254,238,230,124,254,238,238,238,254,254,124,
252,254,238,238,252,224,224,224,124,254,238,238,254,254,252,118,
252,254,238,238,252,238,238,238,126,254,224,124,14,254,254,252,
254,254,56,56,56,56,56,56,238,238,238,238,238,254,254,124,
238,238,238,238,238,238,124,56,238,238,238,254,254,238,198,130,
238,238,124,56,124,238,238,238,238,238,124,124,56,56,112,112,
254,254,28,56,112,254,254,254,124,124,112,112,112,124,124,124,
112,112,56,56,28,28,14,14,124,124,28,28,28,124,124,124,
56,124,238,198,0,0,0,0,0,0,0,0,0,254,254,254,
56,56,28,0,0,0,0,0,0,124,254,238,254,238,238,238,
0,252,254,206,252,206,254,252,0,124,254,238,224,238,254,124,
0,252,254,238,238,238,254,252,0,254,254,224,248,224,254,254,
0,126,254,224,248,224,224,224,0,126,254,224,238,238,254,124,
0,238,238,238,254,238,238,238,0,254,254,56,56,56,254,254,
0,254,254,14,14,238,254,124,0,238,238,252,248,252,238,238,
0,224,224,224,224,224,254,126,0,130,198,238,254,254,238,238,
0,206,238,254,254,254,238,230,0,124,254,238,238,238,254,124,
0,252,254,238,238,252,224,224,0,124,254,238,238,254,252,118,
0,252,254,238,238,252,238,238,0,126,254,224,124,14,254,252,
0,254,254,56,56,56,56,56,0,238,238,238,238,238,254,124,
0,238,238,238,238,238,124,56,0,238,238,238,254,238,198,130,
0,238,238,124,56,124,238,238,0,238,238,124,124,56,56,112,
0,254,254,28,56,112,254,254,60,124,112,112,112,124,124,60,
56,56,56,0,56,56,56,56,120,124,28,28,28,124,124,120,
236,254,118,0,0,0,0,0,0,16,56,124,254,254,254,254};

const unsigned char bit[]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};
//*********************************
void writeCar(short x,short y,char c)
{
const unsigned char *car=&fuente[(c-32)*8];
char i,j;
for (i=0;i<8;i++,car++,y++)
  for (j=0;j<7;j++)
    if (*car&bit[j]) _SetPixel(x+j,y);
//    else putpixel(x+j,y,0);
}
//*********************************
void writeCarN(short x,short y,char c)
{
const unsigned char *car=&fuente2[(c-32)*8];
char i,j;
for (i=0;i<8;i++,car++,y++)
  for (j=0;j<8;j++)	// falta primera linea
    if (*car&bit[j])//||*car&bit[j-1])
      _SetPixel(x+j,y);
//    else putpixel(x+j,y,0);
}
//*********************************
void writeCarB(short x,short y,char c)
{
const unsigned char *car=&fuente[(c-32)*8];
char i,j;
for (i=0;i<8;i++,car++,y++)
  for (j=0;j<7;j++)
	  if (*car&bit[j]) 
		{
		 _SetPixel(x+j-1,y-1);_SetPixel(x+j-1,y);_SetPixel(x+j-1,y+1);
		 _SetPixel(x+j,y-1);_SetPixel(x+j,y);_SetPixel(x+j,y+1);
		 _SetPixel(x+j+1,y-1);_SetPixel(x+j+1,y);_SetPixel(x+j+1,y+1);
		}
}

//*********************************
void writeTex(short x,short y,char *n)
{
for (;*n!=0;n++)
  {
  writeCar(x,y,*n);x+=7;
  }
}
//*********************************
void writeTexB(short x,short y,char *n)
{
for (;*n!=0;n++)
  {
  writeCarB(x,y,*n);x+=7;
  }
}
//*********************************
void writeTexN(short x,short y,char *n)
{
for (;*n!=0;n++)
  {
  writeCarN(x,y,*n);x+=8;
  }
}
//*********************************
void writeTexA(short x,short y,char *n)
{
char largo=strlen(n);
for (signed char i=largo-1;i>=0;i--)
  {
  x-=7;writeCar(x,y,n[i]);
  }
}
//*********************************
void writeTexNA(short x,short y,char *n)
{
char largo=strlen(n);
for (signed char i=largo-1;i>=0;i--)
  {
  x-=8;writeCarN(x,y,n[i]);
  }
}
//*********************************
short TexLargo(char *n)
{
short cnt=0;
for (;*n!=0;n++) cnt+=7;
return cnt;
}
//*********************************
short TexLargoN(char *n)
{
short cnt=0;
for (;*n!=0;n++) cnt+=8;
return cnt;
}
//*********************************
short TexLargo3(char *n)
{
short cnt=0;
for (;*n!=0;n++) cnt+=5;
return cnt;
}
