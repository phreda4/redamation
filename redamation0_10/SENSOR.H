#ifndef SENSOR_H
#define SENSOR_H

#include "winuser.h"
//#include "figura.h"
#include "stgapi.h"
#include "resource.h"

//---------------------------------------------------------------
class DlgColor {
public:
	BYTE r,g,b,a;
	char tipo;

	DlgColor() { r=g=b=a=0xff; }
	void setColor(Color &c,char t)
		{
		a=c.alpha;
		screen.getRGBColor(r,g,b,c.color);
		tipo=t;
		}
	void getColor(Color &c,char &t)
		{
		c.alpha=a;
		c.color=screen.GetNativeColor(RGB(r,g,b));
		t=tipo;
		}
	void draw();
	void bucle();
};

//---------------------------------------------------------------
class DlgFiles {
public:
	char nombre[150];

	DlgFiles() {}
	void draw();
	void bucle();
};

//---------------------------------------------------------------
class Animador {
private:
	CNativeBitmap* pMenuBit; 
// Animacion ani;
// char nombre[100];

// dialogos--------------------------------------------
	DlgColor dlgcolor;	// dialogo para elegir color
	DlgFiles dlgfiles;

// datos trazo actual----------------------------------
	NODOP pnt[1024];	// espacio para el trazo actual
	Trazo trActual;		// trazo actual

	unsigned short nroNodo;		// nodo Actual
	GradColor pact;		// gradiente actual

	unsigned short pntActual;		// punto actual

	unsigned short nroTrActual;	// nro trazo actual en figura

	Figura figActual;	// figura actual (conjunto de trazos) y estados

	//Escena escActual;	// conjunto de figuras, y linea de tiempo
	Vista vista;
public:
	void draw();	// dibujar pantalla
	void bucle();	// bucle principal
	
	void load();	// cargar animacion
	void save();	// grabar animacion
	void play();	// ejec
};


#endif