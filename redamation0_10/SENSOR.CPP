#include "stdafx.h"

#include "flib.h"
#include "sensor.h"
#include "graph.h"
#include "aygshell.h"

// Agregar nodo
// Borrar nodo
#define VACIO	0
#define	TRAZO	1
#define EDITA	2
#define ZOOM	3

//-----------------------------------------------
void Animador::bucle()
{
Menu *m;
//TCHAR texto[140];
int terminar=0;
int x=100,y;
int xz,yz;
int editar,estado=VACIO;
// seteos de inicio
trActual.tipo='l';
trActual.color.color=0xffff;
trActual.color.alpha=255;
trActual.pnt=pnt;
nroTrActual=-1;
// zoom
vista.zoom=1<<16;
vista.xt=vista.yt=0;
// cargar menu
HBITMAP hMenu=SHLoadImageFile(_T("menudibujo.png"));// /My doc /
//::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
pMenuBit=screen.CreateNativeBitmap(hMenu);
::DeleteObject(hMenu);
// dibujar
/*
			trActual.clear();
			trActual.ini(5,5,vista);
			trActual.add(10,11,vista);
			trActual.add(15,14,vista);
			trActual.add(20,21,vista);
			*/
draw();
// bucle principal
while (terminar==0)
	{
	update();
	if (click())
		{
		penpos(&x,&y);
		if (y<24) 
			{
			int xn=x/20;
			SetRGB(64,64,64);XBoxFill(xn*20,0,xn*20+19,23); // resaltar el boton
			redraw();
			while (pendown() && y<24 && x>=xn*20 && x<=xn*20+19) // esperar hasta que levante el lapiz
				{
				penpos(&x,&y);
				update();
				}
			if (y>23 || x<xn*20 || x>xn*20+19)
				{ 
				draw();
				continue; 
				}
			switch (x/20){
			case 0:
				terminar=1;
				break;
			case 6:// color
				dlgcolor.setColor(trActual.color,trActual.tipo);
				dlgcolor.draw();
				dlgcolor.bucle();
				dlgcolor.getColor(trActual.color,trActual.tipo);
				if (nroTrActual!=-1) figActual.set(nroTrActual,trActual);
				draw();
				break;
			case 9:// menu principal
				dlgfiles.draw();
				dlgfiles.bucle();
				draw();
				break;
			case 3:// grabar
				figActual.save("prueba.fig");
				draw();
				break;
			case 4:// grabar pol
/*				trActual.reduce();
				figActual.add(trActual);
				trActual.clear();
				estado=VACIO;*/
				figActual.load("prueba.fig");
				draw();
				break;
			case 5: // borrar pol actual
				trActual.clear();
				if (nroTrActual==-1) 
					{
					draw();
					break;
					}
				figActual.del(nroTrActual);
				nroTrActual=-1;
				estado=VACIO;
				draw();
				break;
			case 2: // proximo pol
				if (nroTrActual==-1 || nroTrActual>=figActual.cntpol)
					{
					figActual.add(trActual);
					nroTrActual=-1;
					}
				else
					{
					figActual.set(nroTrActual,trActual);
					nroTrActual++;
					}
				trActual.clear();
				if (nroTrActual==-1 || nroTrActual>=figActual.cntpol) nroTrActual=0;  // el primero si no hay ninguno
				figActual.get(nroTrActual,&trActual);
				estado=VACIO;
				draw();
				break;
			case 1: // anterior pol
				if (nroTrActual==-1) 
					{
					figActual.add(trActual);
					nroTrActual=figActual.cntpol-1;
					}
				else
					{
					figActual.set(nroTrActual,trActual);
					nroTrActual--;
					}
				trActual.clear();
				if (nroTrActual==-1) nroTrActual=figActual.cntpol-1;
				figActual.get(nroTrActual,&trActual);
				estado=VACIO;
				draw();
				break;
			case 7:// agregar nodo

				break;
			case 8:// borrar nodo
				trActual.del(nroNodo);
				figActual.set(nroTrActual,trActual);
				draw();
				break;
			case 10:// mover
				if (estado==ZOOM)
					estado=VACIO;
				else
					estado=ZOOM;
				nroTrActual=-1;
				trActual.clear(); // fijarse si grabar ???
				draw();
				break;
			case 11:// acercar o alejar
				if (y<12)
					{
					if (vista.zoom>1) 
						{
						vista.zoom>>=1;
						vista.xt+=120*vista.zoom;
						vista.yt+=160*vista.zoom;
						}
					}
				else
					if (vista.zoom<(1<<30)) 
						{
						vista.xt-=120*vista.zoom;
						vista.yt-=160*vista.zoom;
						vista.zoom<<=1;
						}
				draw();
				break;
				}
			}
		else
			{
			if (estado==ZOOM)
				{
				xz=x;yz=y;
				continue;
				}
			editar=getSensor(x,y);
			if (editar==0) // nuevo trazo
				{
				if (nroTrActual==-1)
					{
					figActual.add(trActual);
					nroTrActual=figActual.cntpnt-1;
					}
				else
					{
					figActual.set(nroTrActual,trActual);
					nroTrActual=-1;
					}
				trActual.clear();
				trActual.ini(x,y,vista);
				nroTrActual=-1;// agrega uno siempre
				estado=TRAZO;
				}
			else if (editar<5000) // mover nodo
				{
				editar--;
				trActual.drawEdit(vista,nroNodo);
				nroNodo=editar;
				trActual.drawEdit(vista,nroNodo);
				estado=EDITA;
				redraw();
				}
			else if (editar<10000) // cambia modo
				{
				trActual.drawEdit(vista,nroNodo);
//				trActual.setChange(editar-5000);
				trActual.drawEdit(vista,nroNodo);
				if (nroTrActual!=-1) figActual.set(nroTrActual,trActual);
				estado=VACIO;
				draw();
				}
			else if (editar==55555) // nodo con foco
				{
				editar=nroNodo;
				estado=EDITA;
				}
			//draw();
			}
		}
	else if (pendown())
		{
		penpos(&x,&y);
		if (estado==TRAZO)	// agregar un nodo
			{
//			trActual.drawDraw(vista);
			trActual.add(x,y,vista);
			trActual.drawDraw(vista);
			redraw();
			}
		else if (estado==EDITA) // mover un nodo
			{	
			trActual.drawEdit(vista,nroNodo);
			trActual.moveNodo(editar,x,y,vista);
			trActual.drawEdit(vista,nroNodo);
			redraw();
			}
		else if (estado==ZOOM)
			{
			vista.xt+=(xz-x)*vista.zoom;
			vista.yt+=(yz-y)*vista.zoom;
			xz=x;yz=y;
			draw();
			}
		}
	else
		{
		if (estado==TRAZO) 
			{
			if (trActual.cntp>1)
				{
				figActual.add(trActual);
				nroTrActual=figActual.cntpol-1;
				}
			else
				nroTrActual=-1;
			estado=VACIO;
			draw();
			}
		else if (estado==EDITA)
			{
			figActual.set(nroTrActual,trActual);
			estado=VACIO;
			draw();
			}
		}
	}
}

void Animador::draw()
{
ClrSrc(0);
figActual.draw(vista);
trActual.drawEdit(vista,nroNodo);
screen.BitBlt(0,0,240,24,pMenuBit);

//drawMenu(minicio);	
//SetColor(trActual.color.color);BoxFill(227,0,239,12);// color
//char tt[40];
//sprintf(tt,"pol:%i",nroTrActual);
//writeTex(150,2,tt);
redraw();
}

void Animador::load()	// cargar animacion
{
}

void Animador::save()	// grabar animacion
{
}

void Animador::play()	// ejec
{
}

//------------------------------------------------------
Menu mcolor[]={
//{227,0,239,12,"Salir"},
{0,0,47,12,"Salir"},// x,y,x,y,n
{20,30,148,40,"R"},
{20,40,148,50,"G"},
{20,50,148,60,"B"},
{20,60,148,70,"A"},
{160,40,220,50,"linea"},
{160,50,220,60,"poli"},
{20,80,70,90,"solido"},
{80,80,130,90,"g.lin"},
{140,80,190,90,"g.rad"},
{0,0,0,0,NULL}
};

void DlgColor::draw()
{
SetColor(0);BoxFill(0,0,239,180);
SetRGB(0,255,0);Box(0,0,239,180);
SetRGB(255,0,0);BoxFill(20,30,20+(r>>1),40);
SetRGB(0,255,0);BoxFill(20,40,20+(g>>1),50);
SetRGB(0,0,255);BoxFill(20,50,20+(b>>1),60);
SetColor(0xffff);BoxFill(20,60,20+(a>>1),70);
SetRGB(r,g,b);BoxFill(214,1,238,25);
SetRGB(255,255,255);
if (tipo=='l')
	BoxFill(160,40,220,50);
else
	BoxFill(160,50,220,60);
drawMenu(mcolor);
redraw();
}

void DlgColor::bucle()
{
Menu *m;
int terminar=0;
int x,y,c=-1;
while (terminar==0)
	{
	update();
	if (click())
		{
		penpos(&x,&y);
		m=getMenu(mcolor,x,y);
		if (m!=NULL)
			{ 
			x-=m->x1;
			switch (m->t[0]){
			case 'S':terminar=1;break;
			case 'R':c=1;r=x<<1;draw();break;
			case 'G':c=2;g=x<<1;draw();break;
			case 'B':c=3;b=x<<1;draw();break;
			case 'A':c=4;a=x<<1;draw();break;
			case 'l':tipo='l';draw();break;
			case 'p':tipo='p';draw();break;
				}
			}
		else
			c=0;
		}
	else if (pendown())
		{
		penpos(&x,&y);
		if (m!=NULL)	x-=m->x1;
		if (x>0 && x<128)
			switch (c) {
		case 1:	r=x<<1;draw();break;
		case 2:	g=x<<1;draw();break;
		case 3:	b=x<<1;draw();break;
		case 4:	a=x<<1;draw();break;
			}
		}
	}
}

//---------------------------------------------------
void DlgFiles::draw()
{
SetRGB(0,0,255);
BoxFill(0,0,240,320);
SetColor(0xffff);
redraw();
char msg[100];

WIN32_FIND_DATA FileData;   // Data structure describes the file found
HANDLE hSearch;             // Search handle returned by FindFirstFile
BOOL bFinished = FALSE;
// Start searching for .txt files in the root directory.
hSearch = FindFirstFile (TEXT("\\*.fig"), &FileData);
if (hSearch == INVALID_HANDLE_VALUE)
	{
	writeTex(0,0,"No files found.");
	return;
	}
int x=0;
while (!bFinished)
	{
	sprintf(msg,"%lS",FileData.cFileName);
	writeTex(0,x,msg);
	x+=7;
    if (!FindNextFile(hSearch, &FileData))
		{
		bFinished = TRUE;
		if (GetLastError () == ERROR_NO_MORE_FILES)
			{
			writeTex(0,x,"Found all of the files.");
			}
		else
			{
			writeTex(0,x,"Unable to find next file.");
			}
		}
	}
// Close the search handle.
if (!FindClose (hSearch))
	{
	writeTex(0,0,"Unable to close search handle.");
	}
redraw();
}

void DlgFiles::bucle()
{
redraw();
while (!pendown()) update();
while (pendown()) update();
while (!pendown()) update();
}
