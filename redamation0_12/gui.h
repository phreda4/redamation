/////////////////////////////////////////////////////////
//
// GUI.H
//
// Redamation source
/////////////////////////////////////////////////////////
#ifndef GUI_H
#define GUI_H

#include "graficos.h"
#include "punto.h"

//-----------------------------------------------------
extern bool gui_redibujar;// flag de redibujo
//-----------------------------------------------------
enum {
	GUIBTN=0,
	GUIBTNSRV,
	GUIBTNSRH,
	GUIBTNSV,
	GUIBTNSH,
	GUICOMBOV,
	GUICOMBOH,
	GUIZNAI,
	GUIZNAE,
	GUIMAT,
	GUIINDEX,
	GUIINDEXD,
	GUIDIALOG=0xfa,// dialogo por boton
	GUICASE=0xfb,// case por variable
	GUITRZ=0xfd,// zona de entrada de trazo	
	GUIEND=0xfe// fin de formulario
	};

struct GuiList;

union parametro {
	int i;
	int *p;
	char *c;
	short *s;
	GuiList *gl;
	void (*_f0)(void);
	void (*_f1)(GuiList *gl);
	void (*_f2)(GuiList *gl,short x,short y);
};

struct ficha {
	parametro a,b,c,d,e,f;
};

//-----------------------------------------------------
struct GuiList {
	BYTE tipo;	// tipo de elemento
	short id;	// identificacion
	WORD x1,y1,x2,y2;// coordenadas
	ficha DATA;// seis parametros variables
	char estado;
	void draw(CNativeBitmap *skin);
	void calc(short x,short y);
	inline bool in(short x,short y) { return (x>=x1 && y>=y1 && x<=x2 && y<=y2); };
};

//-----------------------------------------------------
struct EveList {
	int value;
	void (*_accion)(void);
	int accumulator;
	inline void ini(void) {accumulator=0;};
};
//-----------------------------------------------------
struct Gui {
	Linea trazo;
	GuiList *_lgui;
	EveList *_leve;
	char buffer[SIZESCREEN];// tamanio buffer

	short modo;
	bool estadotrazo;
	ColorRGBA color;
	CNativeBitmap *skin;

	GuiList *gactual;// objeto actual
	WORD x1,y1,x2,y2;

	void (*_drawmain)(void);

	GuiList *hit(WORD x,WORD y,GuiList *g);
	GuiList *hit(WORD x,WORD y);

	void draw(GuiList *g);
	void draw(void);

	void ini(GuiList *g);
	
	WORD run(GuiList *gl,EveList *el=NULL,void (*_dm)(void)=NULL);
//	WORD dialog(GuiList *gl,EveList *el=NULL,void (*_dm)(void)=NULL,void (*opc)(WORD e));
};
//-----------------------------------------------------
extern Gui gui;
//-----------------------------------------------------
struct GuiSensor {
	short x1,y1,x2,y2;
	int t;
	inline bool in(short x,short y) { return (x>=x1 && y>=y1 && x<=x2 && y<=y2); };
	inline void move(short x,short y) {x1+=x;y1+=y;x2+=x;y2+=y;};
};
//-----------------------------------------------------
void clearSensor();
void addSensor(short x1,short y1,short x2,short y2,int v);
GuiSensor *getSensor(short x,short y);
int getISensor(short x,short y);

void IceScr(void);

#endif