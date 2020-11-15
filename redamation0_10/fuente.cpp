//
//  Vesa.cc
//
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include "vesa.h"
#include "memman.h"

#define VESA640x480x8	0x101
#define VESA640x480x15	0x110
#define VESA640x480x16	0x111
#define VESA640x480x24	0x112

char fuente[]={
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x40,0x00,0x00, // !
  0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // "
  0x40,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00, //# es !
  0x10,0x38,0x50,0x50,0x38,0x14,0x14,0x38,0x10,0x00,0x00, //$
  0x21,0x52,0x24,0x04,0x08,0x10,0x12,0x25,0x42,0x00,0x00, //%
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //& *****
  0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //'
  0x10,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00, //(
  0x40,0x20,0x10,0x10,0x10,0x10,0x10,0x20,0x40,0x00,0x00, //)
  0x00,0x50,0x20,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //*
  0x00,0x00,0x10,0x10,0x7c,0x10,0x10,0x00,0x00,0x00,0x00, //+
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x00, //,
  0x00,0x00,0x00,0x00,0x7c,0x00,0x00,0x00,0x00,0x00,0x00, //-
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00, //.
  0x04,0x04,0x08,0x08,0x10,0x20,0x20,0x40,0x40,0x00,0x00, ///
  0x38,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00, //0
  0x20,0x60,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00, //1
  0x38,0x44,0x04,0x04,0x08,0x10,0x20,0x40,0x7c,0x00,0x00, //2
  0x38,0x44,0x04,0x04,0x38,0x04,0x04,0x44,0x38,0x00,0x00, //3
  0x0c,0x14,0x14,0x24,0x24,0x44,0x7e,0x04,0x04,0x00,0x00, //4
  0x7c,0x40,0x40,0x40,0x78,0x04,0x04,0x44,0x38,0x00,0x00, //5
  0x38,0x44,0x40,0x40,0x78,0x44,0x44,0x44,0x38,0x00,0x00, //6
  0x7c,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x20,0x00,0x00, //7
  0x38,0x44,0x44,0x44,0x38,0x44,0x44,0x44,0x38,0x00,0x00, //8
  0x38,0x44,0x44,0x44,0x3c,0x04,0x04,0x44,0x38,0x00,0x00, //9
  0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00, //:
  0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x40,0x80,0x00,  //;
  0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x00,0x00, //<
  0x00,0x00,0x00,0x7c,0x00,0x7c,0x00,0x00,0x00,0x00,0x00, //=
  0x40,0x20,0x10,0x08,0x04,0x08,0x10,0x20,0x40,0x00,0x00, //>
  0x38,0x44,0x04,0x08,0x10,0x10,0x10,0x00,0x10,0x00,0x00, //?
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // @ ******
  0x08,0x08,0x14,0x14,0x22,0x22,0x3e,0x41,0x41,0x00,0x00, // A
  0x78,0x44,0x44,0x44,0x78,0x44,0x44,0x44,0x78,0x00,0x00, // B
  0x38,0x44,0x40,0x40,0x40,0x40,0x40,0x44,0x38,0x00,0x00, // C
  0x70,0x48,0x44,0x44,0x44,0x44,0x44,0x48,0x70,0x00,0x00, // D
  0x7c,0x40,0x40,0x40,0x78,0x40,0x40,0x40,0x7c,0x00,0x00, // E
  0x7c,0x40,0x40,0x40,0x78,0x40,0x40,0x40,0x40,0x00,0x00, // F
  0x38,0x44,0x40,0x40,0x4c,0x44,0x44,0x44,0x38,0x00,0x00, // G
  0x44,0x44,0x44,0x44,0x7c,0x44,0x44,0x44,0x44,0x00,0x00, // H
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00, // I
  0x1c,0x08,0x08,0x08,0x08,0x08,0x48,0x48,0x30,0x00,0x00, // J
  0x44,0x48,0x50,0x60,0x40,0x60,0x50,0x48,0x44,0x00,0x00, // K
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x78,0x00,0x00, // L
  0x41,0x41,0x63,0x63,0x55,0x55,0x49,0x49,0x41,0x00,0x00, // M
  0x42,0x62,0x62,0x52,0x52,0x4a,0x46,0x46,0x42,0x00,0x00, // N
  0x38,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00, // O
  0x78,0x44,0x44,0x44,0x78,0x40,0x40,0x40,0x40,0x00,0x00, // P
  0x38,0x44,0x44,0x44,0x44,0x44,0x54,0x4b,0x3b,0x02,0x00, // Q
  0x78,0x44,0x44,0x44,0x78,0x44,0x44,0x44,0x44,0x00,0x00, // R
  0x38,0x44,0x40,0x40,0x38,0x04,0x04,0x44,0x38,0x00,0x00, // S
  0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00, // T
  0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00, // U
  0x44,0x44,0x44,0x28,0x28,0x28,0x10,0x10,0x10,0x00,0x00, // V
  0x41,0x41,0x41,0x49,0x49,0x55,0x55,0x22,0x22,0x00,0x00, // W
  0x44,0x44,0x28,0x28,0x10,0x28,0x28,0x44,0x44,0x00,0x00, // X
  0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x00,0x00, // Y
  0x7c,0x04,0x08,0x08,0x10,0x20,0x20,0x40,0x7c,0x00,0x00, // Z
  0x60,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x60,0x00,0x00, //[
  0x10,0x00,0x10,0x10,0x10,0x20,0x40,0x44,0x38,0x00,0x00, //\  es ?
  0x60,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x60,0x00,0x00, //]
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //^ *******
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //_ *******
  0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //` ******
  0x00,0x00,0x00,0x38,0x04,0x3c,0x44,0x44,0x3c,0x00,0x00, //a
  0x40,0x40,0x40,0x78,0x44,0x44,0x44,0x44,0x78,0x00,0x00, //b
  0x00,0x00,0x00,0x38,0x44,0x40,0x40,0x44,0x38,0x00,0x00, //c
  0x04,0x04,0x04,0x3c,0x44,0x44,0x44,0x44,0x3c,0x00,0x00, //d
  0x00,0x00,0x00,0x38,0x44,0x7c,0x40,0x44,0x38,0x00,0x00, //e
  0x20,0x40,0x40,0x60,0x40,0x40,0x40,0x40,0x40,0x00,0x00, //f
  0x00,0x00,0x00,0x3c,0x44,0x44,0x44,0x44,0x3c,0x04,0x38, //g
  0x40,0x40,0x40,0x78,0x44,0x44,0x44,0x44,0x44,0x00,0x00, //h
  0x00,0x40,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00, //i
  0x00,0x20,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x40, //j
  0x40,0x40,0x40,0x48,0x50,0x60,0x50,0x48,0x44,0x00,0x00, //k
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00, //l
  0x00,0x00,0x00,0x76,0x49,0x49,0x49,0x49,0x49,0x00,0x00, //m
  0x00,0x00,0x00,0x58,0x64,0x44,0x44,0x44,0x44,0x00,0x00, //n
  0x00,0x00,0x00,0x38,0x44,0x44,0x44,0x44,0x38,0x00,0x00, //o
  0x00,0x00,0x00,0x78,0x44,0x44,0x44,0x44,0x78,0x40,0x40, //p
  0x00,0x00,0x00,0x3c,0x44,0x44,0x44,0x44,0x3c,0x04,0x04, //q
  0x00,0x00,0x00,0x60,0x40,0x40,0x40,0x40,0x40,0x00,0x00, //r
  0x00,0x00,0x00,0x30,0x48,0x20,0x10,0x48,0x30,0x00,0x00, //s
  0x40,0x40,0x40,0x60,0x40,0x40,0x40,0x40,0x20,0x00,0x00, //t
  0x00,0x00,0x00,0x44,0x44,0x44,0x44,0x4c,0x34,0x00,0x00, //u
  0x00,0x00,0x00,0x44,0x44,0x28,0x28,0x10,0x10,0x00,0x00, //v
  0x00,0x00,0x00,0x49,0x49,0x55,0x55,0x22,0x22,0x00,0x00, //w
  0x00,0x00,0x00,0x48,0x48,0x30,0x30,0x48,0x48,0x00,0x00, //x
  0x00,0x00,0x00,0x44,0x44,0x44,0x44,0x44,0x3c,0x04,0x38, //y
  0x00,0x00,0x00,0x7c,0x08,0x10,0x20,0x40,0x7c,0x00,0x00, //z
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //{
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40, //|
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //}
  0x00,0x38,0x00,0x58,0x64,0x44,0x44,0x44,0x44,0x00,0x00, // es en~e
  };

char espacio[]={
  4,	//
  3,	//!
  5,	//"
  3,	//#
  7,	//$
  8,	//%
  7,	//&
  3,	//'
  4,	//(
  4,	//)
  4,	//*
  6,	//+
  3,	//,
  6,	//-
  3,	//.
  7,	///
  6,	//0
  3,	//1
  6,	//2
  6,	//3
  6,	//4
  6,	//5
  6,	//6
  6,	//7
  6,	//8
  6,	//9
  3,	//:
  3,	//;
  6,	//<
  6,	//=
  6,	//>
  6,	//?
  7,	//@
  8,	// A
  7,	// B
  6,	// C
  7,	// D
  7,	// E
  7,	// F
  7,	// G
  7,    // H
  3,    // I
  6,	// J
  7,	// K
  5,	// L
  8,	// M
  7,	// N
  7,	// O
  7,	// P
  7,	// Q
  7,	// R
  6,	// S
  6,	// T
  7,	// U
  7,	// V
  8,	// W
  7,	// X
  7,	// Y
  7,	// Z
  3,	//[
  6,	//\
  3,	//]
  4,	//^
  4,	//_
  4,	//`
  6,	//a
  6,	//b
  6,	//c
  6,	//d
  6,	//e
  3,	//f
  6,	//g
  6,	//h
  2,	//i
  3,	//j
  6,	//k
  2,	//l
  8,	//m
  6,	//n
  6,	//o
  6,	//p
  6,	//q
  3,	//r
  5,	//s
  3,	//t
  6,	//u
  6,	//v
  8,	//w
  6,	//x
  6,	//y
  6,	//z
  3,	//{
  3,	//|
  3,	//}
  6,	//~ es ene
  };

short grColor;
short x1c,y1c,x2c,y2c;
short grBancoAct;

inline void vesaBanco(short nro)
{
if (nro==grBancoAct) return;
grBancoAct=nro;
union REGS r;
r.x.ax=0x4f05;
r.x.bx=0;
r.x.dx=nro;
int86(0x10, &r, &r);
}

short vesaInic(short modo)
{
union REGS r;
r.x.ax=0x4f02;	// poner modo vesa
r.x.bx=modo;
int86(0x10,&r,&r);
if (r.x.ax!=0x004f) 
  return 0;
grBancoAct=0xff;
vesaBanco(0);
return 1;
}

void (*griPunto)(short,short);
short (*griColor)(short,short);
void (*griLineaH)(short,short,short);
void (*griLineaV)(short,short,short);

void griPunto8(short,short);
short griColor8(short,short);
void griLineaH8(short,short,short);
void griLineaV8(short,short,short);

void griPunto16(short,short);
short griColor16(short,short);
void griLineaH16(short,short,short);
void griLineaV16(short,short,short);

void grInic()
{
grClip(0,0,639,479);
if (vesaInic(VESA640x480x16))
  {
  griPunto=griPunto16;
  griColor=griColor16;
  griLineaH=griLineaH16; 
  griLineaV=griLineaV16; 
  return;
  }
if (vesaInic(VESA640x480x8))
  {
  griPunto=griPunto8;
  griColor=griColor8;
  griLineaH=griLineaH8; 
  griLineaV=griLineaV8; 
  return;
  }
}

void grFin()	// rutina me cago en VESA
{
union REGS r;
r.x.ax=0x3;
int86(0x10,&r,&r);
}

void grClip(short x1,short y1,short x2,short y2)
{
x1c=x1;y1c=y1;x2c=x2;y2c=y2;
}

void griPunto8(short x,short y)
{
long lugar=x+y*640;
vesaBanco(lugar>>16);
lugar&=0xffff;
lugar+=0xd0000000; // direccion de video
*(char *)lugar=grColor;
}

short griColor8(short x,short y)
{
long lugar=x+y*640;
vesaBanco(lugar>>16);
lugar&=0xffff;
lugar+=0xd0000000; // direccion de video
return *(char *)lugar;
}

void griLineaH8(short x1,short y,short x2)
{
long fuen,dest;
fuen=y*640;
dest=fuen+x2;
fuen+=x1;
char bf=fuen>>16;
char bd=dest>>16;
fuen&=0xffff;
fuen+=0xd0000000; // direccion de video
if (bf!=bd)
  {
  vesaBanco(bf);
  _RowSetB(fuen,grColor,0xd0010000-fuen);
  fuen=0xd0000000;
  dest&=0xffff;
  bf=bd;
  }
else
  dest=x2-x1+1;
vesaBanco(bf);
_RowSetB(fuen,grColor,dest);
}

void griLineaV8(short x,short y1,short y2)
{
long lugar=x+y1*640;
vesaBanco(lugar>>16);
lugar&=0xffff;
lugar+=0xd0000000; // direccion de video
for (short y=y1;y<y2;y++,lugar+=640)
  {
  if (lugar>0xd000ffff)
    {
    vesaBanco(grBancoAct+1);
    lugar-=0x10000;
    }
  *(char *)lugar=grColor;
  }
}

void griPunto16(short x,short y)
{
long lugar=(x<<1)+y*640*2;
vesaBanco(lugar>>16);
lugar&=0xffff;
lugar+=0xd0000000; // direccion de video
*(short *)lugar=grColor;
}

short griColor16(short x,short y)
{
long lugar=(x<<1)+y*640*2;
vesaBanco(lugar>>16);
lugar&=0xffff;
lugar+=0xd0000000; // direccion de video
return *(short *)lugar;
}

void griLineaH16(short x1,short y,short x2)
{
long fuen,dest;
fuen=y*640*2;
dest=fuen+(x2<<1);
fuen+=(x1<<1);
char bf=fuen>>16;
char bd=dest>>16;
fuen&=0xffff;
fuen+=0xd0000000; // direccion de video
if (bf!=bd)
  {
  vesaBanco(bf);
  _RowSetW(fuen,grColor,(0xd0010000-fuen)>>1);
  fuen=0xd0000000;
  dest&=0xffff;
  dest>>=1;
  bf=bd;
  }
else
  dest=x2-x1+1;
vesaBanco(bf);
_RowSetW(fuen,grColor,dest);
}

void griLineaV16(short x,short y1,short y2)
{
long lugar=(x<<1)+y1*640*2;
vesaBanco(lugar>>16);
lugar&=0xffff;
lugar+=0xd0000000; // direccion de video
for (short y=y1;y<=y2;y++,lugar+=640*2)
  {
  if (lugar>0xd000ffff)
    {
    vesaBanco(grBancoAct+1);
    lugar-=0x10000;
    }
  *(short *)lugar=grColor;
  }
}

inline void CLIPX(short &x1,short &x2)
{
if (x1<x1c) x1=x1c;
if (x2>x2c) x2=x2c;
}

inline void CLIPY(short &y1,short &y2)
{
if (y1<y1c) y1=y1c;
if (y2>y2c) y2=y2c;
}

void grRect(short x1,short y1,short x2,short y2)
{
if (x2<x1c||x1>x2c||y1>y2c||y2<y1c) return;
CLIPX(x1,x2);
CLIPY(y1,y2);
for (short y=y1;y<=y2;y++)
  (*griLineaH)(x1,y,x2);
}

void grPunto(short x,short y)
{
if (x<x1c||x>x2c||y<y1c||y>y2c) return;
(*griPunto)(x,y);
}

void grLineaH(short x1,short y,short x2)
{
if (x2<x1c||x1>x2c||y<y1c||y>y2c) return;
CLIPX(x1,x2);
(*griLineaH)(x1,y,x2);
}

void grLineaV(short x,short y1,short y2)
{
if (y1>y2c||y2<y1c||x<x1c||x>x2c) return;
CLIPY(y1,y2);
(*griLineaV)(x,y1,y2);
}

void grLinea(short x1,short y1,short x2,short y2)	// no debe usarse
{
short x,y,z,dz,i1,i2;
short dx=abs(x1-x2);
short dy=abs(y1-y2);
if (x1<x2)
  {
  x=x1;y=y1;
  if (y1>y2) z=-1; else z=1;
  }
else
  {
  x=x2;y=y2;
  if (y2>y1) z=-1; else z=1;
  };
if (dx>dy) i2=dx; else i2=dy;
(*griPunto)(x,y);
if (dx==0)	// linea vertical
  {
  for (i1=1;i1<=i2;i1++,y+=z)
    (*griPunto)(x,y);
  return;
  }
if (dy==0)	// linea horizontal
  {
  for (i1=1;i1<=i2;i1++,x++)
    (*griPunto)(x,y);
  return;
  }
dz=i2/2;
for (i1=1;i1<=i2;i1++)
  {
  if (dz<dx) { dz+=dy;x++; }
  if (dz>=dx) { dz-=dx;y+=z; }
  (*griPunto)(x,y);
  }
}

short grTamanio(short x1,short y1,short x2,short y2)
{
return 2+(x2-x1+1)*(y2-y1+1)*2;	// *1 es 8 *2 es 15/16 *3 es 24
}

void grPoner(short x,short y,char *p)	// 16
{
short i,j,largo,alto;
largo=*p++;
alto=*p++;
if (largo+x>639) largo=640-x;
if (alto+y>479) alto=480-y;
if (y<0) { p-=alto*y;alto+=y;y=0; } 
if (x<0) { p-=x;largo+=x;x=0; }
for (i=0;i<alto;i++,y++)
  for (j=0;j<largo;j++,p+=2)
    {
    grColor=*(short *)p;
    (*griPunto)(x+j,y);
    }
}

void grSacar(short x,short y,short x2,short y2,char *p)
{
char i,j;
char l=(x2-x+1);
char a=(y2-y+1);
*p++=l;
*p++=a;
for (i=0;i<a;i++,y++)
  for (j=0;j<l;j++,p+=2)
    *(short *)p=(*griColor)(x+j,y);
}

void grMarcoAfuera(short x1,short y1,short x2,short y2)
{
grColor=GR_BLANCO;
grLineaH(x1,y1,x2);
grLineaV(x1,y1,y2);
grColor=GR_GRISOSCURO;
grLineaH(x1,y2,x2);
grLineaV(x2,y1,y2);
}

void grMarcoAdentro(short x1,short y1,short x2,short y2)
{
grColor=GR_GRISOSCURO;
grLineaH(x1,y1,x2);
grLineaV(x1,y1,y2);
grColor=GR_BLANCO;
grLineaH(x1,y2,x2);
grLineaV(x2,y1,y2);
}

void grLimpiar()
{
grColor=GR_NEGRO;
for (short y=0;y<479;y++)
  (*griLineaH)(0,y,639);
}

const char bit[]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};

void grCaracter(short x,short y,char c)
{
char i,j,*car=&fuente[(c-32)*11];
char largo=espacio[c-32];
for (i=0;i<11;i++,car++,y++)
  for (j=0;j<largo;j++)
    if (*car&bit[j])
      grPunto(x+j,y);
}

const char bitd[]={0x80,0xc0,0x60,0x30,0x18,0xc,0x6,0x3,0x1};

void grCaracterN(short x,short y,char c)
{
char i,j,*car=&fuente[(c-32)*11];
char largo=espacio[c-32]+1;
for (i=0;i<11;i++,car++,y++)
  for (j=1;j<largo;j++)	// falta primera linea
    if (*car&bitd[j])
      grPunto(x+j,y);
}

void grTexto(short x,short y,char *n)
{
for (;*n!=0;n++)
  {
  grCaracter(x,y,*n);
  x+=espacio[*n-32];
  }
}

void grTextoN(short x,short y,char *n)
{
for (;*n!=0;n++)
  {
  grCaracterN(x,y,*n);
  x+=espacio[*n-32]+1;
  }
}

void grTextoA(short x,short y,char *n)
{
char largo=strlen(n);
for (signed char i=largo-1;i>=0;i--)
  {
  x-=espacio[n[i]-32];
  grCaracter(x,y,n[i]);
  }
}

void grTextoNA(short x,short y,char *n)
{
char largo=strlen(n);
for (signed char i=largo-1;i>=0;i--)
  {
  x-=espacio[n[i]-32];
  grCaracterN(x,y,n[i]);
  }
}

short grLargo(char *n)
{
short cnt=0;
for (;*n!=0;n++)
  cnt+=espacio[*n-32];
return cnt;
}

short grLargoN(char *n)
{
short cnt=0;
for (;*n!=0;n++)
  cnt+=espacio[*n-32]+1;
return cnt;
}

void strinsertar(char *n,char l,char t)
{
for (char i=strlen(n)+1;i>=l;i--)
  n[i+1]=n[i];
n[l]=t;
}

void strborrar(char *n,char l)
{
for (n+=l;*n!=0;n++)
  *n=*(n+1);
}

void grEditar(short x1,short y1,short x2,short y2,char l,char *n)
{
short xx,yy;
short lugarg;
char t,te,lugar,bb;
lugar=strlen(n);
lugarg=grLargoN(n)+x1;
do { rtPos(&xx,&yy,&bb); } while (bb!=0);
rtDesactivar();
grColor=GR_GRISCLARO;grRect(x1,y1,x2,y2);
grColor=GR_NEGRO;grTextoN(x1,y1,n);
do {
  grColor=GR_BLANCO;grLineaV(lugarg,y1,y2);
  rtActivar();
  do {
    rtPos(&xx,&yy,&bb);
  } while (bb==0&&!kbhit());
  if (kbhit())
    { t=getch();if (t==0) te=getch(); else te=0; }
  else
    { t=13;te=0; }
  rtDesactivar();
  grColor=GR_GRISCLARO;grLineaV(lugarg,y1,y2);
  if (t>=' '&&t<='~'&&strlen(n)<=l)
    {
    strinsertar(n,lugar,t);
    grColor=GR_GRISCLARO;grRect(lugarg,y1,x2,y2);
    grColor=GR_NEGRO;grTextoN(lugarg,y1,&n[lugar]);
    lugar++;
    lugarg+=espacio[t-32]+1;
    }
  if (t==8&&lugar>0)
    {
    lugar--;
    lugarg-=espacio[n[lugar]-32]+1;
    strborrar(n,lugar);
    grColor=GR_GRISCLARO;grRect(lugarg,y1,x2,y2);
    grColor=GR_NEGRO;grTextoN(lugarg,y1,&n[lugar]);
    }
  switch(te) {
    case 75:		// flecha atras
      if (lugar>0)
	{
	lugar--;
	lugarg-=espacio[n[lugar]-32]+1;
	}
      break;
    case 77:		// flecha adelante
      if (lugar<strlen(n))
	{
	lugarg+=espacio[n[lugar]-32]+1;
	lugar++;
	}
      break;
    case 83:		// delete
      if (lugar<strlen(n))
	{
	strborrar(n,lugar);
	grColor=GR_GRISCLARO;grRect(lugarg,y1,x2,y2);
	grColor=GR_NEGRO;grTextoN(lugarg,y1,&n[lugar]);
	}
      break;
    }
} while (t!=13);
rtActivar();
}

short colores[]={0,GR_AZUL,GR_VERDE,GR_CYAN,GR_ROJO,GR_VIOLETA,GR_MARRON,
         GR_GRISCLARO,GR_GRISOSCURO,0,0,0,0,0,GR_AMARILLO,0xffff};

void grIcono(short x,short y,char *ic,short xl,short yl)
{
char e=0;
short valor,xx=x;
xl+=x;yl+=y;
for(;y<yl;y++)
  for(x=xx;x<xl;x++)
    {
    if (e==0)
      valor=((*ic)>>4)&0xf;
    else
      { valor=(*ic)&0xf;ic++; }
    e^=1;
    if (valor!=0) { grColor=colores[valor];grPunto(x,y); }
    }
}

void grIcono2(short x,short y,char *ic,short xl,short yl)
{
short valor,xx=x;
xl>>=1;yl>>=1;
xl+=x;yl+=y;
for(;y<yl;y++,ic+=16)
  for(x=xx;x<xl;x++,ic++)
    {
    valor=(*ic)&0xf;
    if (valor!=0) { grColor=colores[valor];grPunto(x,y); }
    }
}

//---------------------------------------------------------------------------

Sprite::Sprite()
{
fondo=new char[grTamanio(0,0,32,32)];
}

Sprite::~Sprite()
{
desactivar();
delete fondo;
}

void Sprite::activar()
{
atr=1;
rtDesactivar();
grSacar(x,y,x+31,y+31,fondo);
grIcono(x,y,dibujo,32,32);
rtActivar();
}

void Sprite::desactivar()
{
atr=0;
rtDesactivar();
grPoner(x,y,fondo);
rtActivar();
}

void Sprite::mover(short xx,short yy)
{
if (x==xx&&y==yy) return;
if (atr==1)
  {
  rtDesactivar();
  grPoner(x,y,fondo);
  grSacar(xx,yy,xx+31,yy+31,fondo);
  grIcono(xx,yy,dibujo,32,32);
  rtActivar();
  }
x=xx;y=yy;
}

//---------------------------------------------------------------------------

char rtActivo;
char *rtFondo;
short *rtImagen;
short rtx,rty;

short rtFlecha[]={0x1000,0x0000, //    0,1,0,0,0,0,0,0,0,0,0,0,
                 0x6400,0x0000, //    1,2,1,0,0,0,0,0,0,0,0,0,	
                 0x6900,0x0000, //    1,2,2,1,0,0,0,0,0,0,0,0,
                 0x6A40,0x0000, //    1,2,2,2,1,0,0,0,0,0,0,0,
                 0x6A90,0x0000, //    1,2,2,2,2,1,0,0,0,0,0,0,
                 0x6AA4,0x0000, //    1,2,2,2,2,2,1,0,0,0,0,0,
                 0x6AA9,0x0000, //    1,2,2,2,2,2,2,1,0,0,0,0,
                 0x6AAA,0x4000, //    1,2,2,2,2,2,2,2,1,0,0,0,
                 0x6AAA,0x9000, //    1,2,2,2,2,2,2,2,2,1,0,0,
                 0x6AAA,0xA400, //    1,2,2,2,2,2,2,2,2,2,1,0,
                 0x6AAA,0xA900, //    1,2,2,2,2,2,2,2,2,2,2,1,
                 0x6A95,0x5400, //    1,2,2,2,2,1,1,1,1,1,1,0,
                 0x6A40,0x0000, //    1,2,2,2,1,0,0,0,0,0,0,0,
                 0x6900,0x0000, //    1,2,2,1,0,0,0,0,0,0,0,0,
                 0x6400,0x0000, //    1,2,1,0,0,0,0,0,0,0,0,0,
                 0x1000,0x0000};//    0,1,0,0,0,0,0,0,0,0,0,0,

void rtInic()
{
union REGS regs;
regs.x.ax=0x00;
int86(0x33,&regs,&regs);
if (!regs.x.ax) return;
// limites x e y
regs.x.dx=0;
regs.x.cx=639;
regs.x.ax=0x07;
int86(0x33,&regs,&regs);
regs.x.dx=0;
regs.x.cx=479;
regs.x.ax=0x08;
int86(0x33,&regs,&regs);
// Velocidad ?
regs.x.dx=32;
regs.x.ax=0x13;
int86(0x33,&regs,&regs);
// paso ?
regs.x.dx=8;
regs.x.cx=8;
regs.x.ax=0x0F;
int86(0x33,&regs,&regs);
rtImagen=rtFlecha;
rtActivo=0;
rtFondo=new char[grTamanio(0,0,16,16)];
rtx=rty=0;
short xx,yy;
char bb;
rtPos(&xx,&yy,&bb);
}

void rtFin()
{
rtDesactivar();
delete rtFondo;
}

short rtColor[]={0,GR_NEGRO,GR_BLANCO,GR_ROJO};

void rtPoner(short x,short y)
{
short *p=rtImagen;
short i,j;
short valor;
char col;
for (i=0;i<16;i++,y++)
  for (j=0;j<16;j++)
    {
    if ((j&0x7)==0) valor=*p++;
    col=(valor&0xc000)>>14;
    valor<<=2;
    if (col!=0) 
      { grColor=rtColor[col];(*griPunto)(x+j,y); }
    }
}

void rtActivar()
{
grSacar(rtx,rty,rtx+16,rty+16,rtFondo);
rtPoner(rtx,rty);
rtActivo=1;
}

void rtDesactivar()
{
grPoner(rtx,rty,rtFondo);
rtActivo=0;
}

void rtPos(short *x,short *y,char *b)
{
union REGS regs;
regs.x.ax=0x03;
int86(0x33,&regs,&regs);
*b=regs.h.bl;
*x=regs.x.cx;
*y=regs.x.dx;
if (*x!=rtx||*y!=rty)
  {
  if (rtActivo==1)
    { 
    grPoner(rtx,rty,rtFondo);
    grSacar(*x,*y,*x+16,*y+16,rtFondo);
    rtPoner(*x,*y);
    }
  rtx=*x;rty=*y;
  }
}

void rtAcomodar(short x1,short y1,short x2,short y2,short *xx,short *yy)
{
char bb;
rtPos(xx,yy,&bb);
if (x1+*xx<0) *xx-=(x1+*xx);
if (x2+*xx>639) *xx-=(x2+*xx)-639;
if (y1+*yy<0) *yy-=(y1+*yy);
if (y2+*yy>419) *yy-=(y2+*yy)-419;	// por la barra
}