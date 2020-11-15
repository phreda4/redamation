#include "stdafx.h"
#include "flib.h"
//..................
#include "entrada.h"

//------------------------------------
bool Entrada::bucle()
{
unsigned short x,y;
if (pendown())
	{
	penpos(&x,&y);
	if (!dibujando)
		{ 
		trazo.ini(x,y);
		dibujando=true;
		return false;
		}
	else
		{
		SetColor(0xffff);
		trazo.addDraw(x,y);
		redraw();
		return false;
		}
	}
if (dibujando==true)
	{
	dibujando=false;
	return true;
	}
return false;
}
//------------------------------------
bool EntradaBox::bucle()
{
unsigned short x,y;
if (pendown())
	{
	penpos(&x,&y);
	if (!dibujando)
		{ 
		x1=x;y1=y;x2=x;y2=y;
		dibujando=true;
		return false;
		}
	else
		{
		SetColor(0xffff);
		SetPixelXOR();
		Box(x1,y1,x2,y2);
		x2=x;y2=y;
		Box(x1,y1,x2,y2);
		SetPixelCopy();
		redraw();
		return false;
		}
	}
if (dibujando==true)
	{
	dibujando=false;
	return true;
	}
return false;
}
//------------------------------------
bool EntradaCir::bucle()
{
unsigned short x,y;
if (pendown())
	{
	penpos(&x,&y);
	if (!dibujando)
		{ 
		xc=x;yc=y;rx=0;ry=0;
		SetColor(0xffff);
		SetPixelXOR();
		Ellipse(xc,yc,rx,ry);
		SetPixelCopy();
		dibujando=true;
		return false;
		}
	else
		{
		SetColor(0xffff);
		SetPixelXOR();
		Ellipse(xc,yc,rx,ry);
		rx=abs(x-xc);ry=abs(y-yc);
		Ellipse(xc,yc,rx,ry);
		SetPixelCopy();
		redraw();
		return false;
		}
	}
if (dibujando==true)
	{
	dibujando=false;
	return true;
	}
return false;
}
