#include "stdafx.h"
#include "gui.h"
#include "guiext.h"
#include "charlist.h"

//----------------------------------------------
/*
GuiList _guilista[]={
{GUIBTN,0,136,20,185,39,"#00ACancel"},
{GUIBTN,1,190,20,231,39,"#0A0Ok"},
{GUIBTN,2,8,20,39,39,"<"},
{GUIBTN,3,100,20,131,39,">"},
{GUIZNA,4,0,44,233,63},
{GUIZNA,5,0,64,233,83},
{GUIZNA,6,0,84,233,103},
{GUIZNA,7,0,104,233,123},
{GUIZNA,8,0,124,233,143},
{GUIZNA,9,0,144,233,163},
{GUIZNA,10,0,164,233,183},
{GUIZNA,11,0,184,233,203},
{GUIZNA,12,0,204,233,223},
{GUIZNA,13,0,224,233,243},
{GUIZNA,14,0,244,233,263},
{GUIZNA,15,0,264,233,283},

{GUIEND}
};

int _cntlista,_actlista,_cntpag,_actpag,_actual;
char *_lista;

void _drawguilist(void)
{
	
char linea[200],nombre[100];
sprintf(linea,"%d/%d",_actpag+1,_cntpag+1);
Pantalla.Color(0xffff);Pantalla.FillBox(40,20,99,39);
Pantalla.Color(0);
Pantalla.TextVC(70,30,linea);
ColorRGBA c1,c2,c3;
c1.set(200,200,200,255);
c2.set(255,255,255,255);
c3.set(0,0,40,255);
Pantalla.Color(0);
int i,aux;
for (i=0;i<12;i++)
	{
	aux=i+(_actpag*12);
	if (aux==_actual)	Pantalla.Color(c3);
	else if ((i&1)==0) Pantalla.Color(c1); 
	else Pantalla.Color(c2);
	Pantalla.FillBox(5,44+20*i,233,63+20*i);
	if (aux<_cntlista)
		{
		if (aux==_actual)
			Pantalla.Color(0xffff);
		else
			Pantalla.Color(0);
		ListGetCopyC(_lista,aux,linea,'|');
		ListGetCopyC(linea,1,nombre,'^');
		Pantalla.TextV(8,49+20*i,nombre);
		}
	}

}

bool gui_lista(int *valor,char *lista,char *titulo)
{
_lista=lista;
IceScr();
Pantalla.Color(0xffff);
Pantalla.Boton(4,4,235,288);
Pantalla.Color(0);
Pantalla.TextVC(120,12,titulo);

_cntlista=ListLenC(lista,'|');
if (*valor>_cntlista) *valor=-1;
_actual=*valor;
_cntpag=_cntlista/12;
_actpag=0;

WORD evento;
bool fin=false;
bool salida=false;
int aux;
while (!fin) {
	evento=gui.run(_guilista,NULL,_drawguilist);
	switch (evento) {
	case 0:
		salida=false;fin=true;
		break;
	case 1:// ok
		salida=true;fin=true;
		break;
	case 2://>
		if (_actpag>0) _actpag--;
		break;
	case 3://<
		if (_actpag<_cntpag) _actpag++;
		break;
	default:
		aux=evento-4;
		if ((aux+(_actpag*12))<_cntlista) _actual=aux+(_actpag*12);
		break;
		}
	}
*valor=_actual;
return salida;
}


//------------------------------------------------
GuiList _guisino[]={
{GUIBTN,1,60,140,110,170,"#00ASi"},
{GUIBTN,2,130,140,180,170,"#0A0No"},
{GUIEND}
};

bool gui_sino(char *titulo,char *pregunta)
{
WORD evento;
IceScr();
Pantalla.ColorRGB(128,128,128);
Pantalla.Boton(40,80,199,190);
Pantalla.Color(0xffff);
Pantalla.TextVC(120,100,titulo);
Pantalla.TextVC(120,120,pregunta);

evento=gui.run(_guisino);
if (evento==1) 
	return true;
else
	return false;
}

//-------------------------------------------------
GuiList numpadstatic[]={
{GUIBTN,7,00,00,29,19,"7"},
{GUIBTN,8,30,00,59,19,"8"},
{GUIBTN,9,60,00,89,19,"9"},
{GUIBTN,4,00,20,29,39,"4"},
{GUIBTN,5,30,20,59,39,"5"},
{GUIBTN,6,60,20,89,39,"6"},
{GUIBTN,1,00,40,29,59,"1"},
{GUIBTN,2,30,40,59,59,"2"},
{GUIBTN,3,60,40,89,59,"3"},
{GUIBTN,0,00,60,59,79,"0"},
{GUIZNA,10,60,60,89,79},
{GUIBTN,11,90,00,119,19,"#00AC"},
{GUIBTN,12,90,20,119,39,"#A00B"},
{GUIBTN,13,90,40,119,79,"#0A0OK"},
{GUIEND},
{GUIBTN,10,60,60,89,79,"-"},// con menos
{GUIBTN,10,60,60,89,79,","}// con punto
};

GuiList numpad[40];

char gui_numero[100];

bool gui_numpad(char *valor,int cmax,int x,int y,void (*dm)(void))// sin signo
{
int i;
for (i=0;i<14;i++)
	{
	numpad[i]=numpadstatic[i];
	numpad[i].x1+=x;
	numpad[i].x2+=x;
	numpad[i].y1+=y;
	numpad[i].y2+=y;
	}
numpad[14]=numpadstatic[14];
WORD evento;
char c;
strcpy(gui_numero,valor);
int cnt=strlen(gui_numero);
bool fin=false,salida=false;
while (!fin)
	{
	evento=gui.run(numpad,NULL,dm);
	switch (evento) {
	case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
		if (cnt<cmax) {
			c='0'+evento;
			if (!strcmp(gui_numero,"0")) strcpy(gui_numero,"");
			strncat(gui_numero,&c,1);
			cnt++;
			}
		break;
	case 10:// signo menos
		break;
	case 12:// limpiar memoria
		strcpy(gui_numero,"");
		cnt=0;
		break;
	case 13:// ok
		strcpy(valor,gui_numero);
		fin=true;salida=true;
		break;
	case 11:
		*valor=-1;
		fin=true;salida=false;// cancelar
		break;
		};
	}
return salida;
}

bool gui_numpadd(int *valor,int cmax,int x,int y,void (*dm)(void))// con decimal
{
int i;
for (i=0;i<14;i++)
	{
	numpad[i]=numpadstatic[i];
	numpad[i].x1+=x;
	numpad[i].x2+=x;
	numpad[i].y1+=y;
	numpad[i].y2+=y;
	}
numpad[10].n=numpadstatic[16].n;
numpad[14]=numpadstatic[14];
WORD evento;
char c;
sprintf(gui_numero,"%d",*valor);
int cnt=strlen(gui_numero);
bool fin=false,salida=false;
while (!fin)
	{
	evento=gui.run(numpad,NULL,dm);
	switch (evento) {
	case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
		if (cnt<cmax) {
			c='0'+evento;
			if (!strcmp(gui_numero,"0")) strcpy(gui_numero,"");
			strncat(gui_numero,&c,1);
			cnt++;
			}
		break;
	case 10:// signo punto
		if (cnt<cmax) {
			c='.';
			strncat(gui_numero,&c,1);
			}
		break;
	case 12:// limpiar memoria
		strcpy(gui_numero,"");
		cnt=0;
		break;
	case 13:// ok
		// saco punto
		// retorno posicion
		sscanf(gui_numero,"%d",valor);
		fin=true;salida=true;
		break;
	case 11:
		*valor=-1;
		fin=true;salida=false;// cancelar
		break;
		};
	}
return salida;
}

bool gui_numpads(int *valor,int cmax,int x,int y,void (*dm)(void))// con signo
{
int i;
for (i=0;i<14;i++)
	{
	numpad[i]=numpadstatic[i];
	numpad[i].x1+=x;
	numpad[i].x2+=x;
	numpad[i].y1+=y;
	numpad[i].y2+=y;
	}
numpad[10].n=numpadstatic[15].n;
numpad[14]=numpadstatic[14];
WORD evento;
char c;
char aux[10];
sprintf(gui_numero,"%d",*valor);
//strcpy(gui_numero,"");
int cnt=strlen(gui_numero);
bool fin=false,salida=false;
while (!fin)
	{
	evento=gui.run(numpad,NULL,dm);
	switch (evento) {
	case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
		if (cnt<cmax) {
			c='0'+evento;
			if (!strcmp(gui_numero,"0")) strcpy(gui_numero,"");
			strncat(gui_numero,&c,1);
			cnt++;
			}
		break;
	case 10:// signo menos
		if (gui_numero[0]=='-')
			strcpy(gui_numero,gui_numero+1);
		else
			{
			strcpy(aux,gui_numero);
			gui_numero[0]='-';
			gui_numero[1]=0;
			strcat(gui_numero,aux);
			}
		break;
	case 12:// limpiar memoria
		strcpy(gui_numero,"");
		cnt=0;
		break;
	case 13:// ok
		sscanf(gui_numero,"%d",valor);
		fin=true;salida=true;
		break;
	case 11:
		*valor=-1;
		fin=true;salida=false;// cancelar
		break;
		};
	}
return salida;
}

bool gui_numpadex(int *valor,int cmax,int x,int y,void (*dm)(void),GuiList *gl)// con extension
{
int i;
for (i=0;i<14;i++)
	{
	numpad[i]=numpadstatic[i];
	numpad[i].x1+=x;
	numpad[i].x2+=x;
	numpad[i].y1+=y;
	numpad[i].y2+=y;
	}
GuiList *g1=gl;
for (i=14;g1->tipo!=GUIEND;g1++,i++)
	{
	numpad[i]=*g1;
	numpad[i].x1+=x;
	numpad[i].x2+=x;
	numpad[i].y1+=y;
	numpad[i].y2+=y;
	}
numpad[i]=numpadstatic[14];
WORD evento;
char c;
sprintf(gui_numero,"%d",*valor);
//strcpy(gui_numero,"");
int cnt=strlen(gui_numero);
bool fin=false,salida=false;
while (!fin)
	{
	evento=gui.run(numpad,NULL,dm);
	switch (evento) {
	case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
		if (cnt<cmax) {
			c='0'+evento;
			if (!strcmp(gui_numero,"0")) strcpy(gui_numero,"");
			strncat(gui_numero,&c,1);
			cnt++;
			}
		break;
	case 10:// signo menos
		break;
	case 12:// limpiar memoria
		strcpy(gui_numero,"");
		cnt=0;
		break;
	case 13:// ok
		sscanf(gui_numero,"%d",valor);
		fin=true;salida=true;
		break;
	case 11:
		*valor=-1;
		fin=true;salida=false;// cancelar
		break;
		};
	}
return salida;
}
*/
//----------------------------------------------------------------------------
GuiList _teclado[]={
{GUIBTN,1, 0,200, 9,216,{(int)"1"}},
{GUIBTN,2,10,200,19,216,{(int)"2"}},
{GUIBTN,3,20,200,29,216,{(int)"3"}},
{GUIBTN,4,30,200,39,216,{(int)"4"}},
{GUIBTN,5,40,200,49,216,{(int)"5"}},
{GUIBTN,6,50,200,59,216,{(int)"6"}},
{GUIBTN,7,60,200,69,216,{(int)"7"}},
{GUIBTN,8,70,200,79,216,{(int)"8"}},
{GUIBTN,9,80,200,89,216,{(int)"9"}},
{GUIBTN,0,90,200,99,216,{(int)"0"}},
{GUIEND}
};

void _gui_texto_draw(void)
{
}

bool gui_texto(char *texto,char cnt,int x1,int y1,int x2,int y2)
{
WORD evento;
bool fin=false;
while (!fin)
	{
	evento=gui.run(_teclado,NULL,_gui_texto_draw);
	};
return true;
}
