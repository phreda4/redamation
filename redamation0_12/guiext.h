#ifndef GUIEX_H
#define GUIEX_H


// teclado
/*
extern char gui_numero[100];
bool gui_numpad(char *valor,int cmax,int x,int y,void (*dm)(void));// numero
bool gui_numpadd(int *valor,int cmax,int x,int y,void (*dm)(void));// con decimal
bool gui_numpads(int *valor,int cmax,int x,int y,void (*dm)(void));// con signo
bool gui_numpadex(int *valor,int cmax,int x,int y,void (*dm)(void),GuiList *gl);// con extension

// eleccion de lista
bool gui_lista(int *valor,char *lista,char *titulo);
// eleccion de db
// si no
bool gui_sino(char *titulo,char *pregunta);
// void gui_mensaje(char *titulo,char *mensaje);


  */
bool gui_texto(char *texto,char cnt,int x1,int y1,int x2,int y2);

#endif