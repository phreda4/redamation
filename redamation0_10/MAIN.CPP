//
//  principal
//
#include "stdafx.h"
#include "aygshell.h"

#include "flib.h"
#include "entrada.h"
#include "dibujo.h"
#include "animacion.h"
#include "poli.h"

//.......................... Variables
//------------------------------------------------------

Entrada entrada;
EntradaBox entradabox;
EntradaCir entradacir;

Animacion ani;// animacion en memoria
Dibujo dibujo;// dibujo actual
/*
Vis vista;// vista de edicon
Matrix mvista,mivista;
*/

BYTE r,g,b,a;
WORD poliColor=0xffff;
char poliTipo=0;

unsigned short cursor;
char nombreDibujo[35];
bool anterior;
char archivos[64][35]; // 64 archivos de 35 chars
unsigned short cntarchivos,pagina;
unsigned short nrotrazo,nropnt;

//............................Constantes
//------------------------------------------------------
char *titulo="REDAmation 0.9";
const BYTE rp[]={0,0xff,0xff,0xff,0xff,0x7f,0x00,0x00,0x00,0x7f,0x00,0x00,0x7f,0xff,0xff,0xff};
const BYTE gp[]={0,0x00,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7f,0x00,0x00,0x00,0x7f,0xff};
const BYTE bp[]={0,0x00,0x00,0x00,0x7f,0x00,0x00,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
CNativeBitmap *pMenu1,*pMenut;

//***************************************************************
//.............................. elegir color
//------------------------------------------------------
Menu mcol[]={
{209,0,29,15,"(X)"},
{179,0,29,15,"C"},
{12,20,16,260,"\1"},
{42,20,16,260,"\1"},
{72,20,16,260,"\1"},

{140,40,50,20,"Poli"},
{140,60,50,20,"Linea"},
{0,0,0,0,NULL}
};

void drawColor()
{
ClrSrc(0);
for (short i=0;i<16;i++)
	{
	SetRGB(rp[i],gp[i],bp[i]);
	BoxFill(100,20+i*15,120,35+i*15);
	}
drawMenuEx(mcol,0,0);
SetRGB(r,g,b);
poliColor=fColor;
BoxFill(239-54,3,239-36,12);

SetColor(0xf800);
if (poliTipo==1)
	Box(140,40,190,60);
else
	Box(140,60,190,80);
BoxFill(11,20+(255-r),29,25+(255-r));
BoxFill(41,20+(255-g),59,25+(255-g));
BoxFill(71,20+(255-b),89,25+(255-b));
redraw();
}

void elijeColor()
{
screen.getRGBColor(r,g,b,poliColor);
char opcion;
unsigned short x,y;
drawColor();
while (1)
	{
	update();
	if (click())
		{
		penpos(&x,&y);
		opcion=getMenuEx(mcol,x,y,0,0);
		if (opcion>0) {
			switch(opcion) {
			case 1: return;
			case 3:r=255-(y-20);drawColor();break;
			case 4:g=255-(y-20);drawColor();break;
			case 5:b=255-(y-20);drawColor();break;
			case 6:
				poliTipo=1;
				drawColor();
				break;
			case 7:
				poliTipo=0;
				drawColor();
				break;
				}
			continue;
			}
		if (x>=100 && x<=120 && y>=20 && y<=20+16*15)
			{
			opcion=(y-20)/15;
			r=rp[opcion];g=gp[opcion];b=bp[opcion];
			drawColor();
			}
		}
	}
}

//***************************************************************
//.............................. modo dibujo
//------------------------------------------------------

Menu msis[]={
{0,0,30,14,"E"},// editar
{31,0,30,14,"V"},// vista
{62,0,30,14,"D"},// dibujar
{93,0,30,14,"S"},//salir
{0,0,0,0,NULL}
};

Menu mtime[]={
{ 0, 0,15,15,"|<"},
{17, 0,22,15,"<"},
{40, 0,22,15,">"},
{63, 0,15,15,">|"},
{0,16,78,12,"s"},
{98,0,16,28,"P"},// play
{80,0,15,14,"c"},//color
{80,15,15,13,"p"},// propiedades
{0,0,0,0,NULL}
};

Menu mdibujar[]={
{0,0,30,14,"F"},
{31,0,30,14,"P"},
{62,0,30,14,"B"},
{93,0,30,14,"C"},
{0,0,0,0,NULL}
};

Menu meditar[]={
{0,0,30,14,"F"},// nodo 
{31,0,30,14,"P"},// trazo
{62,0,15,14,"B"},
{78,0,14,14,"S"},
{93,0,15,14,"O"},
{109,0,14,14,"X"},
{0,0,0,0,NULL}
};

Menu mevista[]={
{0,0,30,14,"F"},
{31,0,30,14,"P"},
{62,0,15,14,"B"},
{78,0,14,14,"S"},
{93,0,15,14,"O"},
{109,0,14,14,"X"},
{0,0,0,0,NULL}
};

char modo=0;
char modod=0;
char modoe=0;
char modoz=0;

void drawDibujo()
{
ClrSrc(ani.bColor);
dibujo.draw();
screen.BitBlt(0,0,240,30,pMenu1);// dibuja menu
clearSensor();
switch (modo) {
case 0:// Dibujar trazo
	SetColor(0xffff);Box(117+(modod*31),17,145+(modod*31),28);
	break;
case 1:// Editar Nodos
	if (modoe==1)
		{
		dibujo.setSensor();
		dibujo.drawBox(nrotrazo);
		}
	else
		{
		dibujo.drawEdit(nrotrazo,nropnt);
		dibujo.setSensor();
		}
	screen.TransparentBlt(116,15,124,15,pMenut,0xffff,0,0);
	SetColor(0xffff);Box(117+(modoe*30),17,145+(modoe*30),28);
	break;
case 2:// Vista
	screen.TransparentBlt(116,15,124,15,pMenut,0xffff,0,15);
	SetColor(0xffff);Box(117+(modoz*30),17,145+(modoz*30),28);
	break;
	}
char tt[9];
sprintf(tt,"%i",cursor+1);
int l=28+(12-(TexLargo(tt)/2));
SetColor(0x0);writeTexB(l,4,tt);
SetColor(0xffff);writeTex(l,4,tt);
SetColor(poliColor);BoxFill(81,2,94,13);// color trazo
SetColor(ani.bColor);BoxFill(81,16,94,28);// color fondo
redraw();
}

// para editar color				elijeColor();
void dibujar()
{
modo=modod=modoe=modoz=0;
//DWORD baset=0,delay=3000;// 3 seg
poliColor=0xffff;
poliTipo=0;
cursor=0;
ani.getDibujo(&dibujo,cursor); // cargo el primer cuadro
unsigned short x,y;
unsigned short elecc;
int xz,yz;
int xa,ya,elije,elijeo;
char opcion;
bool esclick;

//long xt1,yt1;
//mivista.transforma(120,160,&xt1,&yt1);
//vista.center(xt1,yt1);
//vista.scale(mzoom);
//vista.rota(mangle);
//vista.position(xt1,yt1);
//vista.makeMatrix(mvista);
//mvista.invert(mivista);
dibujo.setVista(ani.vista);
//vista=ani.vista;
//vista.makeMatrix(mvista);
//mvista.invert(mivista);
drawDibujo();
while (1)
	{
	update();
	esclick=false;
	if (click())
		{
		esclick=true;
		penpos(&x,&y);xz=x;yz=y;
		opcion=getMenuEx(msis,x,y,116,0);//-------------------Menu Sistema
		if (opcion>0)
			{
			switch(opcion) {
			case 3:	modo=0;break;// dibujar
			case 1:	modo=1;elecc=-1;break;// editar
			case 2:	modo=2;break;// vista
			case 4:	ani.setDibujo(&dibujo,cursor);return;// salir
			}
			drawDibujo();
			continue;
			}
		opcion=getMenuEx(mtime,x,y,0,0);//--------------------Menu Tiempo
		if (opcion>0)
			{
			switch (opcion) {
			case 1:// primer cuadro
				ani.setDibujo(&dibujo,cursor);
				cursor=0;
				ani.getDibujo(&dibujo,cursor);
				break;
			case 2:// anterior
				if (cursor==0) break;
				ani.setDibujo(&dibujo,cursor);
				cursor--;
				ani.getDibujo(&dibujo,cursor);
				break;
			case 3:// siguiente
				if (cursor>ani.ccuadros) break;
				ani.setDibujo(&dibujo,cursor);
				cursor++;
				if (cursor<ani.ccuadros)// optimizar pra cuadro en memoria
					ani.getDibujo(&dibujo,cursor);
				else
					dibujo.setSub();
				break;
			case 4:// ultimo
				ani.setDibujo(&dibujo,cursor);
				cursor=ani.ccuadros-1;
				ani.getDibujo(&dibujo,cursor);
				break;
			case 6:// play
				ani.setDibujo(&dibujo,cursor);
				ani.trim();
				ani.play();
				ani.getDibujo(&dibujo,cursor);
				break;
			case 7:// color
				if (modo==1) 
					{
					poliColor=dibujo.trz[nrotrazo].color;
					poliTipo=dibujo.trz[nrotrazo].tipo;
					}
				elijeColor();
				if (modo==1) 
					{
					dibujo.trz[nrotrazo].color=poliColor;
					dibujo.trz[nrotrazo].tipo=poliTipo;
					}
				break;
			case 8:// color de fondo
				WORD pc=poliColor;
				char pt=poliTipo;
				poliColor=ani.bColor;
				elijeColor();
				ani.bColor=poliColor;
				poliColor=pc;
				poliTipo=pt;
				break;
				}
			drawDibujo();
			continue;
			}
		switch (modo) {//------------------------------------Menu Variable
		case 0:// dibujar
			opcion=getMenuEx(mdibujar,x,y,116,15);
			if (opcion>0) {
				modod=opcion-1;
				drawDibujo();
				continue;
				}
			break;
		case 1:// editar
			opcion=getMenuEx(meditar,x,y,116,15);
			if (opcion>0) 
				{
				switch(opcion) {
				case 1:// nodos
				case 2:// trazos
					modoe=opcion-1;
					break;
				case 4://curva
					// if (modoe==0) ... else 
					if (!dibujo.esmarca(nrotrazo))
						dibujo.changeLC(nrotrazo,nropnt);
					break;
				case 5:// nuevo
					if (!dibujo.esmarca(nrotrazo))
						dibujo.insNodo(nrotrazo,nropnt);
					break;
				case 6:// borrar
					if (!dibujo.esmarca(nrotrazo))
						dibujo.delNodo(nrotrazo,nropnt);
					if (nropnt>0) nropnt--;
					break;
					};
				drawDibujo();
				continue;
				}
			break;
		case 2:// vista
			opcion=getMenuEx(mevista,x,y,116,15);
			if (opcion>0)
				{
				switch(opcion) {
				case 1:// mover
				case 2:// scala&rotacion
					modoz=opcion-1;
					break;
				case 3:// poner en animacion
					if (cursor==0)
						ani.vista=dibujo.vista;
					else
						dibujo.changeWin(dibujo.vista);
					break;
				case 4:// sacar de animacion
					dibujo.setVista(ani.vista);
					break;
					};
				drawDibujo();
				continue;
				}
			break;
		};
		}
	//=============================SOBRE PANEL==============================================
	switch (modo) {
	case 0:	// dibujar
		switch (modod) {
		case 0:
			if (entrada.bucle()) // agrega trazos a dibujo
				{
				dibujo.addTrazo(&entrada,poliTipo,poliColor);
				drawDibujo();
				entrada.clear();
				}
			break;
		case 1:
			if (entrada.bucle()) // agrega trazos a dibujo
				{
				drawDibujo();
				entrada.clear();
				}
			break;
		case 2:
			if (entradabox.bucle())
				{
				dibujo.addBox(entradabox.x1,entradabox.y1,entradabox.x2,entradabox.y2,poliTipo,poliColor);
				drawDibujo();
				}	
			break;
		case 3:
			if (entradacir.bucle())
				{
				dibujo.addCircle(entradacir.xc,entradacir.yc,entradacir.rx,entradacir.ry,poliTipo,poliColor);
				drawDibujo();
				}	
			break;
		};
		break;
	case 1:// editar
		if (esclick)
			{
			penpos(&x,&y);
			elije=getSensor(x,y);
			if (elije>999)
				{
				if (elecc==-1)// elejir el primero
					{
					elecc=1;
					}
				else
					{
					elijeo=getNSensor(x,y,elecc);
					if (elijeo==0) 
						elecc=1;
					else	
						{
						elecc++;
						elije=elijeo;
						}
					}
				if (elije>999 && elije<dibujo.cntt+1000)
					{
					nrotrazo=elije-1000;
					drawDibujo();
					}
				}
			}
		switch (modoe) {
			case 0:// punto
				elije=getSensor(x,y);
				if (elije>0)
					{
					if (nrotrazo>=dibujo.cntt) continue;
					nropnt=elije-1;
					if (nropnt>=dibujo.trzD[nrotrazo].cnodo) continue;
					xa=x;ya=y;
					while (pendown()) 
						{
						update();
						penpos(&x,&y);
						if (xa!=x || ya!=y)
							{
							xa=x;ya=y;			
							dibujo.moveNodo(nrotrazo,nropnt,x,y);
							drawDibujo();
							}
						}
					drawDibujo();
					}	
				break;
			case 1:// trazo
				if (nrotrazo>=dibujo.cntt) continue;
				xa=x;ya=y;
				while (pendown())
					{
					update();
					penpos(&x,&y);
					if (xa!=x || ya!=y)
						{
						dibujo.moveTrazo(nrotrazo,xa,ya,x,y);
						xa=x;ya=y;
						drawDibujo();
						}
					}
				drawDibujo();
				break;
				};
			
		break;
	case 2:// vista
		switch (modoz) {
		case 0:// mover
			if (pendown())
				{
				penpos(&x,&y);
				dibujo.moveWin(xz,yz,x,y);
				xz=x;yz=y;
				drawDibujo();
				}
			break;
		case 1:// escala
			if (pendown())
				{
				penpos(&x,&y);
				dibujo.rsWin(xz,yz,x,y);
				xz=x;yz=y;
				drawDibujo();
				}
			break;
			};
		};
	//=========================================================================
/*
	if (GetTickCount()-baset>delay) // cada 3 seg .???
		{
		baset=GetTickCount();
		}
*/
	}
}

//***************************************************************
//.........................Menu principal
//------------------------------------------------------
Menu mprin[]={
{  2, 0,56,28,"O"},
{ 66, 0,52,20,"<<"},
{120, 0,52,20,">>"},
{181, 0,56,28,"X"},
//{66,20,107,10,"--"},
{0,0,0,0,NULL}
};

void drawPrincipal()
{
unsigned short j;
//ClrSrc(0x0000);
//SetRGB(0,32,0);BoxFill(0,0,239,10);
for (j=0;j<320;j++)
	HLineD(0,j,239,0x001f,0xffff);
j=TexLargoN(titulo);
SetColor(0x000);writeTexB(120-(j/2),10,titulo);
SetColor(0xffff);writeTex(120-(j/2),10,titulo);
clearSensor();
unsigned short desde=pagina*10;
for (unsigned short i=0;i<10;i++)
	{
//	SetRGB(0,128,0);BoxFill(10,26+i*25,228,51+i*25);
	if (strcmp(archivos[desde+i],""))
		{
		for (j=26;j<51;j++)
			HLineD(10,j+i*25,228,0xffff,0x07e0);
		for (j=28;j<50;j++)
			HLineD(204,j+i*25,225,0xf800,0x3800);
		SetColor(0);Box(10,26+i*25,228,51+i*25);
		SetColor(0x0);
		writeTexB(20,36+i*25,archivos[desde+i]);
		writeTexB(211,35+i*25,"X");
		SetColor(0xffff);
		writeTex(20,36+i*25,archivos[desde+i]);
		writeTex(211,35+i*25,"X");
		addSensor(10,26+i*25,200,51+i*25,i+1);
		addSensor(204,26+i*25,225,51+i*25,i+1000);
		}
	}
SetColor(0x0);
drawMenuEx(mprin,0,319-30);
redraw();
}

void cargaArchivos()
{
char n[255];
WIN32_FIND_DATA FileData;   // Data structure describes the file found
HANDLE hSearch;             // Search handle returned by FindFirstFile
BOOL bFinished = FALSE;
hSearch=FindFirstFile(TEXT("\\reda\\*.rdm"), &FileData);
cntarchivos=0;
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	while (!bFinished)
		{
		sprintf(n,"%lS",FileData.cFileName);
		*strchr(n,'.')=0;
		strcpy(archivos[cntarchivos],n);
		cntarchivos++;
	    if (!FindNextFile(hSearch, &FileData)) bFinished = TRUE;
		}
	FindClose (hSearch);
	}
archivos[cntarchivos][0]=0;
}

void editarAnimacion()
{
dibujar();
ani.grabar(nombreDibujo);
cargaArchivos();
drawPrincipal();
}

void principal()
{
cargaArchivos();
pagina=0;
drawPrincipal();
unsigned short x,y;
char opcion;
int n;
while (1)
	{
	update();
	if (click())
		{
		penpos(&x,&y);
		opcion=getMenuEx(mprin,x,y,0,319-30);
		if (opcion>0)
			{
			switch(opcion) {
			case 1:
				strcpy(nombreDibujo,"");
				editaText(nombreDibujo,30);
				if (strcmp(nombreDibujo,""))
					{
					dibujo.clear();
					ani.clear(25);// 25 cuadros por segundo
					ani.cargar(nombreDibujo);// si existe lo carga
					editarAnimacion();
					}
				drawPrincipal();
				break;
			case 2:
				if (pagina==0) break; 
				pagina--;
				drawPrincipal();
				break;
			case 3:
				if (pagina*10+10>cntarchivos) break;
				pagina++;
				drawPrincipal();
				break;
			case 4:
				return;
/*			case 5:
				prueba();
				drawPrincipal();
				break;*/
			}
			continue;
			}
		n=getSensor(x,y);// seleccionado
		if (n>0 && n<999)
			{
			while (pendown()) update();
			strcpy(nombreDibujo,archivos[pagina*10+(n-1)]);
			if (strcmp(nombreDibujo,""))
				{
				dibujo.clear();
				ani.clear(25);
				ani.cargar(nombreDibujo);
				editarAnimacion();
				}
			}
		else if (n>999)
			{
			while (pendown()) update();
			n-=1000;
			strcpy(nombreDibujo,archivos[pagina*10+n]);
			TCHAR szName[255];
			wsprintf(szName,_T("\\reda\\%hs.rdm"),nombreDibujo);// de char a charw
			DeleteFile(szName);
			cargaArchivos();
			drawPrincipal();
			}
		}
	}
}
//..........................................principal
//----------------------------------------------------------------
void main(void)
{
HBITMAP hMenu=SHLoadImageFile(_T("\\reda\\menu1.png"));
pMenu1=screen.CreateNativeBitmap(hMenu);
HBITMAP hMenut=SHLoadImageFile(_T("\\reda\\menut.png"));
pMenut=screen.CreateNativeBitmap(hMenut);
clearinput();

clearPoli(0,0,239,319);// donde dibuja los poligonos
SetClip(0,0,239,319);
entrada.clear();
dibujo.clear();
principal();
}
//----------------------------------------------------------------