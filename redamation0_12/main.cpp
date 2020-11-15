#include "stdafx.h"
/////////////////////////////////////////////////////////
//
// MAIN.CPP
//
// Redamation source-Abril 2005
/////////////////////////////////////////////////////////
#include "redalib.h"
#include "graficos.h"
#include "gui.h"
#include "guiext.h"
#include "charlist.h"
#include "dibujo.h"
#include "libro.h"
#include "escena.h"
#include "main.h"

//-- variables 
int menumain=0,botonbuscar=0,menudibujos=0,menuescena=0,botonera=0;
//int comboaccion=0;
int botedita=0;

Dibujo dibujo;
Libro libro;
Escena escena;
Vista vista;
Matrix mat,imat;

short nrocolor=0;

int cnthojas=0,edithoja;
int tiempoa;
int cntobj,editobj;

ColorRGBA coloractual;
int colorh,colors,colorl;// extension de color
ColorRGBA colores[32];// 16 fijos y 16 elegidos

//------------------------------------------------------------------
//---------------------	Buscar Archivo
char archivos[8192];
short cntarchivos;

char carpetas[8192];
short cntcarpetas;

char carpetap[512];
short paginaarc,paginacar;
short selecionarc,selecioncar;
int nro;

//------------------- Archivo de inicio
void readINIFILE()
{
char *n="\\reda\\redamation.ini";
FILE *stream;
if((stream=fopen(n,"r"))==NULL) return;
fgets(carpetap,512,stream);
if(fclose(stream)) return;
}

//---------------------------------------------------------
void _drawarchivos(GuiList *gl){}
void _drawcarpetas(GuiList *gl){}
void _drawpaginas(GuiList *gl){}

void cargararchivos(void)
{
char path[1024];
strcpy(path,carpetap);
strcat(path,"*.*");// tipo de archivo
getTFiles(path,archivos,carpetas);
cntarchivos=ListLenC(archivos,'|');
cntcarpetas=ListLenC(carpetas,'|');
paginaarc=paginacar=0;
selecionarc=selecioncar=0;
}
//---------------------------------------------------------
void opcbuscar(WORD e)// operaciones de buscar
{
switch (e) {
case 2:
	switch (botonbuscar) {
	case 0:// nuevo archivo
		break;
	case 1:// nuevo Clon
		break;
	case 2:// borrar archivo
		break;
		};
	break;
case 10:// archivos
	selecionarc=nro+paginaarc;//depagina;
	// elegir un archivo
	break;
case 20:// paginas de archivos
	if (nro*10<cntarchivos)
		paginaarc=nro*10;//depagina*10;
	break;
case 30:// carpetas
	selecioncar=nro+paginacar;//depagina;
	char nombre[1024];
	ListGetCopyC(carpetas,selecioncar,nombre,'|');
	if (*nombre!=0) 
		{
		carpetap[strlen(carpetap)-1]=0;// saco el 
		strcat(carpetap,nombre);
		strcat(carpetap,"\\");
		cargararchivos();
		}
	break;
case 40:// paginas de carpetas
	if (nro*10<cntcarpetas)
		paginacar=nro*10;//depagina*10;
	break;
case 50:// cambiar carpeta
	if (strcmp(carpetap,"\\"))
		{
		char *p=&carpetap[strlen(carpetap)-2];
		while (*p!='\\') p--;
		*p=0;
		strcat(carpetap,"\\");
		cargararchivos();
		}
	break;
	};
}

void drawarchivo(int x1,int y1,int x2,int y2,char *n,bool s=false)
{
if (*n=='\\')
	Pantalla.ColorRGB(0,0,(s)?180:80);
else
	Pantalla.ColorRGB(0,(s)?180:80,0);
Pantalla.FillBoxR(x1,y1,x2,y2,2);
Pantalla.Color(0xffff);
Pantalla.TextV(x1+((s)?2:4),y1+1,n);
};

void drawbuscar(void)
{
int i,j;
char tt[80];
Pantalla.Color(0xffff);
Pantalla.ClrScr();
Pantalla.Color(0);
Pantalla.TextVG(2,4,"RedaMation [0.12]");
Pantalla.TextVGC(120,310,"email:pablo@comtron.com.ar");
Pantalla.ColorRGB(0, 80,0);Pantalla.FillBox(0,33,239,214);// archivos
Pantalla.ColorRGB(0,0, 80);Pantalla.FillBoxR(0,20,239,33,1);// path
Pantalla.ColorRGB(0,0,180);Pantalla.FillBoxR(0,214,239,300,1);// carpetas
Pantalla.Color(0xffff);
Pantalla.TextVG(2,22,carpetap);
int x=0,y=52;// dibujar archivos
for (i=0;i<cntarchivos;i++)
	{
	ListGetCopyC(archivos,i,tt,'|');
	if (i>=paginaarc && i<paginaarc+10)
		{
		drawarchivo(x+1,y,x+117,y+14,tt,(selecionarc==i));
		y+=16;
		};
	}
if (cntarchivos>10)// paginas de archivos
	{
	x=cntarchivos/10;
	int ya=42;//205;
	for (i=1;i<=x+1;i++)
		{
		sprintf(tt,"%d",i);
		y=i*14-6;
		if (i-1==paginaarc/10){
			Pantalla.ColorRGB(128,255,128);j=2;
		}else{
			Pantalla.ColorRGB(0,255,0);j=-2;
			}
		Pantalla.FillCircle(y,ya,6);
		Pantalla.Color(0xffff);Pantalla.FillCircle(y+j,ya+j,2);
		Pantalla.Color(0);Pantalla.TextVC(y,ya+1,tt);
		}
	}
int xd=0,yd=230;//216;
for (i=0;i<cntcarpetas;i++)
	{
	ListGetCopyC(carpetas,i,tt,'|');
	if (i>=paginacar && i<paginacar+10)
		{
		drawarchivo(xd+1,yd,xd+117,yd+12,tt,false);//(selecioncar==cntd));//
		xd+=120;if (xd>120) { xd=0;yd+=14; }
		};
	}
if (cntcarpetas>10) // paginas de carpetas
	{
	x=cntcarpetas/10;
	yd=222;//293;
	for (i=1;i<=x+1;i++)
		{
		sprintf(tt,"%d",i);
		y=i*14-6;
		if (i-1==paginacar/10){
			Pantalla.ColorRGB(128,128,255);j=2;
		}else{
			Pantalla.ColorRGB(0,0,255);j=-2;
			}
		Pantalla.FillCircle(y,yd,6);
		Pantalla.ColorRGB(200,200,255);Pantalla.FillCircle(y+j,yd+j,2);
		Pantalla.Color(0);Pantalla.TextVC(y,yd+1,tt);
		}
	}
// dibujo
//Pantalla.Color(0xffff);
Pantalla.ColorRGB(0,180,0);
Pantalla.FillBoxR(116,51,236,211,1);
}

//--------------------------------------------  Mundo
GuiList listaBuscar[]={
//{GUIBTNSH,2	,0,0,60,20		,{0,180,20,20,(int)&botonbuscar}},// botonera
{GUIZNAE,50	,1,21,238,35},
{GUIMAT	,20	,0,33,239,49	,{(int)&nro,1,15,(int)_drawpaginas}},
{GUIMAT	,10	,0,50,239,214	,{(int)&nro,10,1,(int)_drawarchivos}},
{GUIMAT	,40	,0,214,239,230	,{(int)&nro,1,15,(int)_drawpaginas}},
{GUIMAT	,30	,0,231,239,300	,{(int)&nro,5,2,(int)_drawcarpetas}},
{GUIEND}
};

//------------------------------------------------------------------
//-------------------- Dibujar dibujo
WORD coloresarco[240];

//------------------ Dialogo de color
void _drawcolor(GuiList *gl)
{
Pantalla.Color(coloractual);
Pantalla.FillBox(gl->x1,gl->y1,gl->x2,gl->y2);
Pantalla.Alpha(255);
Pantalla.Color(0);
Pantalla.Box(gl->x1,gl->y1,gl->x2,gl->y2);
}

char tipocolor;

void _drawgrilla(GuiList *gl)
{
int r,g,b;
int i,j;
for (i=0;i<120;i++)
	for (j=0;j<120;j++)
		{
		HSL_to_RGB(colorh,i*2,j*2,&r,&g,&b);
		Pantalla.ColorRGB(r,g,b);
		Pantalla.SetPixel(i+gl->x1,j+gl->y1);
		}
i=(colors>>1)+gl->x1;
j=(colorl>>1)+gl->y1;
Pantalla.Color(0xffff);
Pantalla.XOR(true);
Pantalla.HLine(gl->x1,j,gl->x2-1);
Pantalla.VLine(i,gl->y1,gl->y2-1);
Pantalla.XOR(false);
};

void _drawslide(GuiList *gl)
{
int i;//,r,g,b;
for (i=0;i<240;i++)
	{
	//HSL_to_RGB(i,240,120,&r,&g,&b);
	//Pantalla.ColorRGB(r,g,b);
	Pantalla.Color(coloresarco[i]);
	Pantalla.HLine(gl->x1+1,i+gl->y1,gl->x2-1);
	}
Pantalla.Color(0);
i=colorh+gl->y1;
Pantalla.Box(gl->x1,i-1,gl->x2,i+1);
};

int nrog,nros,nroa;

void _drawalpha(GuiList *gl)
{
Pantalla.Color(0);
Pantalla.Box(gl->x1+1,gl->y1,gl->x2-1,gl->y2);
int y=gl->y1+nroa;
Pantalla.Box(gl->x1,y-1,gl->x2,y+1);
};

//---------------------
GuiList dialogcolor[]={
{GUIMAT,0,60,30,60+120,30+120,{(int)&nrog,120,120,(int)_drawgrilla}},
{GUIMAT,0,200,30,220,30+240,{(int)&nros,240,1,(int)_drawslide}},
{GUIMAT,0,20,30,40,30+255,{(int)&nroa,255,1,(int)_drawalpha}},
{GUIZNAE,9999,0,0,240,320},
{GUIEND}
};

static char hexd[]="0123456789ABCDEF";

void drawcolor(void)
{
Pantalla.ColorRGB(200,200,200);
Pantalla.Boton(0,20,239,300);
char snum[10];
snum[0]=hexd[(coloractual.r>>4)&0xf];
snum[1]=hexd[coloractual.r&0xf];
snum[2]=hexd[(coloractual.g>>4)&0xf];
snum[3]=hexd[coloractual.g&0xf];
snum[4]=hexd[(coloractual.b>>4)&0xf];
snum[5]=hexd[coloractual.b&0xf];
snum[6]=0;

Pantalla.Color(0);
Pantalla.TextVC(120,180,snum);
colorh=nros;
colors=(nrog%120)*2;
colorl=(nrog/120)*2;

int r,g,b;
HSL_to_RGB(colorh,colors,colorl,&r,&g,&b);
coloractual.set(r,g,b,nroa);//255
Pantalla.Color(0xffff);
Pantalla.FillBox(60,190,180,220);
Pantalla.Color(coloractual);
Pantalla.FillBox(181,1,198,18);
Pantalla.FillBox(60,190,180,220);
Pantalla.Alpha(255);
}
//---------------------------- Boton de hoja
void _drawhoja(GuiList *gl)
{
char tt[8];
//guiIndex *d=(guiIndex*)gl->DATA;//	d->cnt
sprintf(tt,"%c",'A'+edithoja);
Pantalla.ColorRGB(200,200,0);
Pantalla.DBotonD(gl->x1,gl->y1,gl->x2,gl->y2,tt);
}

void drawhoja(void)
{
//libro.get(edithoja,dibujo);
//dibujo.draw(mat);
}

//---------------------------------------------------------
GuiList dialoghoja[]={
{GUIINDEXD	,80	,0,20,19,300,{(int)&cnthojas,(int)&edithoja}},
//{GUIMAT	,0	,0,40,19,256+40	,{(int)&edithoja,64,1,(int)_drawhoja}},
{GUIZNAE,9999,0,0,240,320},
{GUIEND}
};

//---------------------------------------------------------
int editpoli,editnodo;

void _drawseltr(GuiList *gl)
{
if (editpoli!=-1) dibujo.drawTrazo(mat,editpoli);
gui_redibujar=true;// necesita redibujo completo
}

void _calcseltr(GuiList *gl,WORD x,WORD y)
{
if (gl->estado==0) // toca el lapiz
	{
	editpoli=getISensor(x,y);
	}
else if (editpoli!=-1) 
	{//gsensor->move(x-gui.x1,y-gui.y1);
	long x1,y1,x2,y2;
	imat.transforma(x,y,&x1,&y1);
	imat.transforma(gui.x1,gui.y1,&x2,&y2);
	if (x1!=x2 || y1!=y2) 
		{
		dibujo.moveTrazo(editpoli,(short)(x1-x2),(short)(y1-y2));
		gui.x1=x;gui.y1=y;
		dibujo.addSensorObj(mat);// que modifique nroobj solamente
		}
	};
}

//---------------------------- Seleccion de nodos
void _drawselnodo(GuiList *gl)
{
if (editnodo!=-1) dibujo.drawNodo(mat,editpoli,editnodo);
gui_redibujar=true;// necesita redibujo completo
};

void _calcselnodo(GuiList *gl,WORD x,WORD y)
{
if (gl->estado==0) // toca el lapiz
	{
	int nro=getISensor(x,y);
	if (nro!=-1) editnodo=nro;
	}
else if (editpoli!=-1 && editnodo!=-1)// mueve el lapiz
	{
	long x1,y1;
	imat.transforma(x,y,&x1,&y1);
	dibujo.moveNodo(editpoli,editnodo,(short)x1,(short)y1);
	dibujo.addSensorPnt(mat,editpoli);
	}
}

//------------------------------------------------
//----------------------------------------- Dibujo
void drawdibujo(void)
{
Pantalla.Color(0xffff);Pantalla.ClrScr();
dibujo.draw(mat);
/*
char tt[20];
sprintf(tt,"c:%d p:%d",cntobj,editpoli);
Pantalla.Color(0);Pantalla.TextVC(160,10,tt);
*/
};

short menuamodod[]={1,2,4,6,0,0};

void opcdibujo(WORD e)
{
short i;
switch (e) {
case 10:// menu dibujos
	gui.modo=menuamodod[menudibujos];
	if (menudibujos>3 && editpoli!=-1)
		{
		coloractual.setC(dibujo.trazos[editpoli].color);
		RGB_to_HSL(coloractual.r,coloractual.g,coloractual.b,&colorh,&colors,&colorl);
		nros=colorh;
		nrog=(colors/2)+(colorl*60);
		nroa=coloractual.a;
		}
	switch(menudibujos) {
	case 4:	dibujo.addSensorObj(mat);break;
	case 5:	dibujo.addSensorPnt(mat,editpoli);break;
	};
	break;
case 11:// seleccion de color
	i=gui.modo;
	gui.modo=0;
	gui.run(dialogcolor,NULL,drawcolor);
	gui.color=coloractual;
	if (menudibujos>3 && editpoli!=-1)
		{
		dibujo.trazos[editpoli].color=coloractual;
		}
	gui.modo=i;
	break;
case 12://nro de pagina
	libro.put(edithoja,dibujo);
	i=gui.modo;
	gui.modo=0;
	gui.run(dialoghoja,NULL,drawhoja);
	gui.modo=i;
	libro.get(edithoja,dibujo);
	gui.modo=menuamodod[menudibujos];
	switch(menudibujos) {
	case 4:	dibujo.addSensorObj(mat);break;
	case 5:	dibujo.addSensorPnt(mat,editpoli);break;
	};
	break;
case 20:
	coloractual=dibujo.trazos[editpoli].color;
	RGB_to_HSL(coloractual.r,coloractual.g,coloractual.b,&colorh,&colors,&colorl);
	nros=colorh;
	nrog=(colors/2)+(colorl*60);
	nroa=coloractual.a;
	break;
case 40:// botones de hoja
	switch(botonera) {
	case 0:// crear hoja
		libro.put(edithoja,dibujo);
		edithoja=cnthojas;
		libro.get(edithoja,dibujo);
		if (cnthojas<63) cnthojas++;
		break;
	case 1:// borrar hoja actual
		break;
	case 2:// herramientas
		break;
	};
	gui.modo=menuamodod[menudibujos];
	switch(menudibujos) {
	case 4:dibujo.addSensorObj(mat);break;
	case 5:dibujo.addSensorPnt(mat,editpoli);break;
	};
	break;
case 50:// botones de trazo
	switch(botonera) {
	case 0:// borrar 
		dibujo.delTrazo(editpoli);
		if (editpoli>=dibujo.cntt) editpoli=dibujo.cntt-1;
		dibujo.addSensorObj(mat);
		break;
	case 1:// clonar
		dibujo.addClon(editpoli);
		editpoli=dibujo.cntt-1;	
		dibujo.addSensorObj(mat);
		break;
	case 2:// abajo
		dibujo.bajaTrazo(editpoli);
		editpoli++;if (editpoli>=dibujo.cntt) editpoli=dibujo.cntt-1;
		dibujo.addSensorObj(mat);
		break;
	case 3:// arriba
		dibujo.subeTrazo(editpoli);
		editpoli--;if (editpoli<0) editpoli=0;
		dibujo.addSensorObj(mat);
		break;
	case 4:// cambio
		dibujo.tipoTrazo(editpoli);
		break;
		};
	break;
case 60:// boton de punto
	switch(botonera) {
	case 0:// agregar
		dibujo.insNodo(editpoli,editnodo);
		dibujo.addSensorPnt(mat,editpoli);
		break;
	case 1:// borrar
		dibujo.delNodo(editpoli,editnodo);
		if (editnodo>=dibujo.trazos[editpoli].cntpuntos) editnodo=dibujo.trazos[editpoli].cntpuntos-1;
		dibujo.addSensorPnt(mat,editpoli);
		break;
	case 2:// curva
		dibujo.changeNodo(editpoli,editnodo);
		break;
	case 3:// invisible
		dibujo.changeNodoVisible(editpoli,editnodo);
		break;
	case 4:// anterior
		editpoli--;
		if (editpoli<0) editpoli=dibujo.cntt-1;
		dibujo.addSensorPnt(mat,editpoli);
		break;
	case 5:// siguiente
		editpoli++;
		if (editpoli>=dibujo.cntt) editpoli=0;
		dibujo.addSensorPnt(mat,editpoli);
		break;
/*
case 3:// pone en nuevo grupo
dibujo.trazos[editpoli].puntos[editnodo].tipo&=0x3;
dibujo.trazos[editpoli].puntos[editnodo].tipo|=(grupo&0x3f)<<2;
break;
*/
	};
	break;
case GUITRZ:// entro algun trazo
	switch (menudibujos) {
	case 0:
		dibujo.addLinea(imat,gui.trazo,2,coloractual,0);
		dibujo.unirUltimo();
		break;
	case 1:dibujo.addLinea(imat,gui.trazo,0,coloractual,0);
		// puede unir ultimo aqui tambien
		break;
	case 2:dibujo.addRect(imat,gui.x1,gui.y1,gui.x2,gui.y2,coloractual,0);
		break;
	case 3:dibujo.addEllipse(imat,gui.x1,gui.y1,abs(gui.x1-gui.x2),abs(gui.y1-gui.y2),coloractual,0);
		break;
		};
	break;
	};
}

//--------------------------------------------  Dibujar
GuiList listaCrea[]={
//{GUIBTNSH,40	,20,0,80,20	,{80,80,20,20,(int)&botonera}},
{GUIEND}
};

//------------------------------------------------
GuiList listaEditaO[]={// Selecciona trazo
{GUIBTNSH,50	,50,0,150,20	,{0,180,20,20,(int)&botonera}},
{GUIZNAI,20		,0,0,240,320	,{(int)_drawseltr,(int)_calcseltr}},
{GUIEND}
};

//------------------------------------------------
GuiList listaEditaP[]={// Selecciona punto
{GUIBTNSH,60	,50,0,170,20	,{0,220,20,20,(int)&botonera}},
{GUIZNAI,0		,0,0,240,320	,{(int)_drawselnodo,(int)_calcselnodo}},
{GUIEND}
};
//------------------------------------------------
GuiList listaDibujar[]={// Editar dibujos
{GUIZNAI	,12	,0,0,19,19		,{(int)_drawhoja}},
{GUIBTNSH	,40	,20,0,50,20		,{0,100,15,20,(int)&botonera}},// botones de hoja
{GUICOMBOV	,10	,200,0,220,20	,{40,0,20,20,(int)&menudibujos,6}},
{GUIZNAI	,11	,180,0,199,19	,{(int)_drawcolor}},
{GUICASE	,0	,0,0,0,0		,{(int)&menudibujos,0,(int)listaCrea}},
{GUICASE	,0	,0,0,0,0		,{(int)&menudibujos,1,(int)listaCrea}},
{GUICASE	,0	,0,0,0,0		,{(int)&menudibujos,2,(int)listaCrea}},
{GUICASE	,0	,0,0,0,0		,{(int)&menudibujos,3,(int)listaCrea}},
{GUICASE	,0	,0,0,0,0		,{(int)&menudibujos,4,(int)listaEditaO}},
{GUICASE	,0	,0,0,0,0		,{(int)&menudibujos,5,(int)listaEditaP}},
{GUIEND}
};

//------------------------------------------------
//---------------------------------------- Escena
void _drawtiempo(GuiList *gl)
{
Pantalla.Color(0);
int xc=(gl->x1+gl->x2)>>1;
Pantalla.VLine(xc,gl->y1,gl->y2);
Pantalla.FillCircle(xc,gl->y1,3);
Pantalla.FillCircle(xc,gl->y2,3);
Pantalla.ColorRGB(0,0,255);
Pantalla.FillCircle(xc,gl->y1+tiempoa,3);//(gl->y2-gl->y1)/200
// mover la escena al tiempo a

}

void _drawselobj(GuiList *gl)
{
if (editobj!=-1) 
	{
	escena.drawSelect(editobj);
	gui_redibujar=true;// necesita redibujo completo
	}
};

void _calcselobj(GuiList *gl,WORD x,WORD y)
{
if (gl->estado==0) // toca el lapiz
	{
	int nro=getISensor(x,y);
	if (nro!=-1) editobj=nro;
	}
else if (editobj!=-1) 
	{//gsensor->move(x-gui.x1,y-gui.y1);
	long x1,y1,x2,y2;
	Elemento *e=&escena.elementos[editobj];
	e->imat.transforma(x,y,&x1,&y1);
	e->imat.transforma(gui.x1,gui.y1,&x2,&y2);
	if (x1!=x2 || y1!=y2) 
		{
		escena.move(editobj,(short)(x1-x2),(short)(y1-y2));
		gui.x1=x;gui.y1=y;
		escena.addSensorO();//mat);// que modifique nroobj solamente
		}
	};
};

//----------------- configuracion de objeto
int nroescala,nrorotar;

void _drawescala(GuiList *gl)
{
Pantalla.Color(0);
int x=(gl->x1+gl->x2)>>1;//,y=(gl->y1+gl->y2)>>1;
Pantalla.Line(x,gl->y1,gl->x2-5,gl->y2);
Pantalla.Line(x,gl->y1,gl->x1+5,gl->y2);
Pantalla.HLine(gl->x1+5,gl->y2,gl->x2-5);
int y=gl->y1+nroescala-4;
Pantalla.ColorRGB(0,0,200);
Pantalla.DBoton(gl->x1,y,gl->x2,y+8,"");
if (editobj!=-1) escena.escala(editobj,nroescala*MAXZOOM/280);
};

void _drawrotar(GuiList *gl)
{
Pantalla.Color(0);
int y=(gl->y1+gl->y2)>>1;
Pantalla.Line(gl->x1,y,gl->x2,y);
Pantalla.ColorRGB(0,0,255);
int x=gl->x1+nrorotar-4;
Pantalla.DBoton(x,gl->y1,x+8,gl->y2,"");
if (editobj!=-1) escena.rotar(editobj,(nrorotar-120)*3600/120);
};


GuiList dialogconf[]={
//{GUIINDEXD	,80	,0,20,19,300	,{&cnthojas,&edithoja}},
//{GUIMAT	,0	,0,40,19,256+40,{(int)&edithoja,64,1,(int)_drawhoja}},
{GUIMAT	,0	,210,20,229,300,{(int)&nroescala,280,1,(int)_drawescala}},
{GUIMAT	,0	,0,300,239,319,{(int )&nrorotar,1,239,(int)_drawrotar}},
{GUIZNAE,9999,0,0,240,320},
{GUIEND}
};

void drawconf(void)
{
Pantalla.Color(0xffff);
Pantalla.ClrScr();
escena.draw();
char tt[40];
if (editobj!=-1)
	{
	Elemento *e=&escena.elementos[editobj];
	sprintf(tt,"x:%d y:%d a:%d s:%d",e->vista.xt,e->vista.yt,e->vista.angle,e->vista.sc);
	Pantalla.Color(0);
	Pantalla.TextV(0,0,tt);
	}
};

void ventana_agregarobjeto(void)// menu pi
{
char tipo=0;


switch (tipo) {
case 0:
	escena.addObj(edithoja);
	break;
case 1:
	escena.addTxt("");
	break;
case 2:
	escena.addNro(0);
	break;
case 3:
	escena.addVar();
	break;
case 4:
	escena.addTab();
	}
editobj=cntobj;
cntobj++;
}

//------------------------------ Escena
GuiList listaEscena[];

void _drawseleccion(void)
{
if (editobj!=-1) 
	{
	escena.drawSelect(editobj);
	gui_redibujar=true;// necesita redibujo completo
	}
/*
int i,j;
int alto=300;
int x=0,y=306;
char t[40];
alto-=14;
for (i=0;i<escena.cntelementos;i++)
	{
	if (x==0)
		{
		Pantalla.Color(0);
		Pantalla.FillBox(x,y,x+240,y+14);
		}
	sprintf(t,"%d",i);
	j=i*255/escena.cntelementos;
	Pantalla.ColorRGB(0,j,255-j);
	if (i==editobj)
		Pantalla.DBoton(x-1,y-1,x+22,y+14,t);
	else
		Pantalla.DBoton(x,y,x+20,y+12,t);
	x+=20;if (x>=240) 
		{ 
		x=0;y-=14;alto-=14; 

		}
	//escena.elementos[i].padre
	}

if (x==0) alto+=14;
Pantalla.Color(0);
Pantalla.FillBox(0,alto,240,alto+20);

listaEscena[0].y1=alto;
listaEscena[0].y2=alto+19;
*/
}

void drawescena(void)
{
Pantalla.Color(0xffff);
Pantalla.ClrScr();
escena.sett(tiempoa);
escena.draw();
escena.drawKey(editobj);// dibujar marcado
// dibujo unidades


int i,j;
int alto=300;
int x=0,y=306;
char t[40];
alto-=14;
for (i=0;i<escena.cntelementos;i++)
	{
	if (x==0)
		{
		Pantalla.Color(0);
		Pantalla.FillBox(x,y,x+240,y+13);
		}
	sprintf(t,"%d",i);
	j=i*255/escena.cntelementos;
	if (i==editobj)
		{
		Pantalla.ColorRGB(255,0,j);
		Pantalla.DBotonD(x,y-1,x+20,y+14,t);
		}
	else
		{
		Pantalla.ColorRGB(0,j,255-j);
		Pantalla.DBoton(x,y,x+20,y+12,t);
		}
	x+=20;if (x>=240) 
		{ 
		x=0;y-=14;alto-=14; 

		}
	//escena.elementos[i].padre
	}

if (x==0) alto+=14;
Pantalla.Color(0);
Pantalla.FillBox(0,alto,240,alto+19);

listaEscena[0].y1=alto;
listaEscena[0].y2=alto+19;

}

void opcescena(WORD e)
{
short i;
switch (e) {
case 12:// nro de hoja
	i=gui.modo;
	gui.modo=0;
	gui.run(dialoghoja,NULL,drawhoja);
	gui.modo=i;
	if (editobj!=-1) 
		{
		escena.elementos[editobj].hoja=edithoja;
		escena.addSensorO();
		}
	break;
case 30:// mover un objeto con el tiempo
	break;
case 50:// botones de objetos
	if (libro.cnthojas==0) break;// si no hay hojas.. no funciona
	switch (botonera) {
	case 0:	// nuevo
		ventana_agregarobjeto();
		break;
	case 1:// editar
		break;
	case 2:// entrar
		break;
	case 3:// salir
		break;
	case 4:// borrar
		cntobj--;
		break;
		}
	break;
case 70:// botones
	if (libro.cnthojas==0) break;// si no hay hojas.. no funciona
	switch (botonera) {
	case 0:
		escena.play();
		break;
	case 1:// agregar keyframe
		if (editobj!=-1)
			{
			Elemento *e=&escena.elementos[editobj];
			escena.addKeyF(editobj,tiempoa,e->vista);
			}
		break;
	case 2:// borrar keyframe
		escena.delKeyF(editobj,tiempoa);
		break;
	case 3:{// propiedades de los objetos		
		Elemento *e=&escena.elementos[editobj];
		// rotacion,escala,color,alpha ,posicion (numerica),blur,
		nroescala=e->vista.sc*280/MAXZOOM;
		nrorotar=e->vista.angle*120/3600;
		//..
		//..

		i=gui.modo;
		gui.modo=0;
		gui.run(dialogconf,NULL,drawconf);
		gui.modo=i;
//		if (editobj!=-1)
//			escena.addKeyF(editobj,tiempoa,e->vista);
		}break;

	};
	escena.addSensorO();
	break;
case 80:// cambiar elementos
	escena.addSensorO();
	break;
case 90:// mover tiempo
//	escena.sett(tiempoa);
	escena.addSensorO();
	break;
	};
}

int tipo_objeto;


//------------------------------------------------ Escena
GuiList listaEscena[]={
{GUIBTNSH	,50 ,0,280,100,280+19	,{0,260,20,20,(int)&botonera}},
//{GUIZNAI	,12	,0,0,19,19		,{(int)_drawhoja}},
{GUICOMBOV	,40	,200,0,220,20	,{80,0,20,20,(int)&tipo_objeto,5}},
//{GUIBTNSH	,70	,20,0,100,20	,{0,140,20,20,(int)&botonera}},
//{GUIBTNSH	,50 ,0,300,29,319	,{0,100,15,20,(int)&botonera}},



//{GUIINDEX	,80	,30,300,239,319	,{(int)&cntobj,(int)&editobj}},// objetos
//{GUIMAT		,90	,220,30,239,290	,{(int)&tiempoa,200,1,(int)_drawtiempo}},


//{GUIZNAI	,30,0,0,240,320		,{(int)_drawselobj,(int)_calcselobj}},
{GUIZNAI	,30,0,0,240,320		,{(int)_drawseleccion,(int)_calcselobj}},
{GUIEND}
};

//-----------------------------------------------------
//--------------------------------------- Configuracion
GuiList listaConf[]={// Configurar
{GUIEND}
};

void drawconfig(void)
{
Pantalla.Color(0xff);
Pantalla.ClrScr();
}

void opcconfig(WORD e)
{
}

//-------------------------------------------------------
//---------------------------------------- Menu Principal
GuiList inicio[]={
{GUICOMBOV	,100,220,0,240,20	,{0,0,20,20,(int)&menumain,5}},
{GUICASE	,0	,0,0,0,0		,{(int)&menumain,0,(int)listaDibujar}},
{GUICASE	,0	,0,0,0,0		,{(int)&menumain,1,(int)listaEscena}},
{GUICASE	,0	,0,0,0,0		,{(int)&menumain,2,(int)listaBuscar}},
{GUICASE	,0	,0,0,0,0		,{(int)&menumain,3,(int)listaConf}},
{GUIEND}
};
//-------------------------------------------------------------
const BYTE rp[]={0,128,192,255,192,255,128,255,0,0,128,255,0,0,0,0};
const BYTE gp[]={0,128,192,255,0,0,0,0,128,255,128,255,0,0,128,255};
const BYTE bp[]={0,128,192,255,0,0,128,255,0,0,0,0,128,255,128,255};
//-------------------------------------------------------------
// Principal
//-------------------------------------------------------------
void main(void)
{
//---------------- Pantalla de Inicio
Pantalla.Color(0x0);
Pantalla.ClrScr();
Pantalla.Color(0xffff);
Pantalla.TextVC2(120,160,"RedaMation");
Pantalla.redraw();
// arcoiris
short i;
int r,g,b;
for (i=0;i<240;i++)
	{
	HSL_to_RGB(i,240,120,&r,&g,&b);
	Pantalla.ColorRGB(r,g,b);
	coloresarco[i]=Pantalla.color.directo;
	}
//---------------- comienzo de programa
dibujo.create(2048,512);
dibujo.clear();
libro.create();
libro.clear();
escena.create(&libro);
escena.clear();

gui.skin=Pantalla.loadBitmap("\\reda\\skin1.png");

menumain=0;

cnthojas=1;
edithoja=0;

editpoli=0;
editnodo=0;
// colores

for (i=0;i<16;i++)
  colores[i].set(rp[i],gp[i],bp[i],255);
nrocolor=0;
coloractual=colores[nrocolor];
RGB_to_HSL(coloractual.r,coloractual.g,coloractual.b,&colorh,&colors,&colorl);
nros=colorh;
nrog=(colors/2)+(colorl*60);
nroa=255;

gui.color=coloractual;
strcpy(carpetap,"\\");
readINIFILE();

vista.clear();
vista.makeMatrix(mat);
mat.invert(imat);

cargararchivos();
void (*funcdibujo)(void);
void (*funcopcion)(WORD e);
funcdibujo=drawdibujo;
funcopcion=opcdibujo;
bool fin=false;
WORD evento;
gui.modo=1;
while (!fin)
	{
	evento=gui.run(inicio,NULL,funcdibujo);
	switch (evento) {
	case 100:// Menu Principal
		if (edithoja!=-1)
			libro.put(edithoja,dibujo);
		switch (menumain) {
		case 0:// dibujo
			funcdibujo=drawdibujo;
			funcopcion=opcdibujo;
			gui.modo=menuamodod[menudibujos];
			libro.get(edithoja,dibujo);
			switch(menudibujos) {
			case 4:	dibujo.addSensorObj(mat);break;
			case 5:	dibujo.addSensorPnt(mat,editpoli);break;
			};
			break;
		case 1://  Escena
			funcdibujo=drawescena;
			funcopcion=opcescena;
			gui.modo=0;
			switch (menuescena) {
			case 0://
				escena.rew();
				escena.addSensorO();
				break;
			case 1://
				break;
				};
			break;
		case 2:// buscar archivo
			funcdibujo=drawbuscar;
			funcopcion=opcbuscar;
			gui.modo=0;
			break;
		case 3:// propiedades
			funcdibujo=drawconfig;
			funcopcion=opcconfig;
			gui.modo=0;
			break;
		case 4:// salir
			fin=true;
			break;
			};
		break;
	default:
		funcopcion(evento);
		break;
		};
	};
delete(gui.skin);

escena.destroy();
libro.destroy();
dibujo.destroy();
};