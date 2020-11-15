#include "stdafx.h"
#include "libro.h"

void Libro::create(void)
{
};

void Libro::destroy(void)
{
};

void Libro::clear(void)
{
cntmem=0;// memoria ocupada
cnthojas=0;
for (int i=0;i<MAXHOJAS;i++)
	{
	hojas[i].largo=0;
	}
};

void Libro::get(short n,Dibujo &d)
{
if (n<0 || n>=MAXHOJAS) return;// no poner donde no se puede
if (hojas[n].largo==0) 
	{
	d.clear();
	return;
	}
d.memget(hojas[n].memoria);
};

void Libro::put(short n,Dibujo &d)
{
if (n<0 || n>=MAXHOJAS) return;// no poner donde no se puede
int tamanio=d.memsize();
while (n>cnthojas) 
	{
	hojas[cnthojas].memoria=&mem[cntmem];
	hojas[cnthojas].largo=0;
	cnthojas++;
	//cntmem+=tamanio;
	};
if (n==cnthojas)// se agrega uno
	{
	cnthojas++;
	hojas[n].memoria=&mem[cntmem];
	cntmem+=tamanio;
	}
else if (n<cnthojas)// correr los otros cuadros
	{
	int dif=tamanio-hojas[n].largo;//sizein(cuadros[n]);//
	int resto=0;
	for (int i=n+1;i<cnthojas;i++)
		resto+=hojas[i].largo;
	if (dif!=0  &&  n+1<cnthojas)
		{
		char *pp=hojas[n+1].memoria;
		memmove(pp+dif,pp,resto);
		for (unsigned short i=n+1;i<cnthojas;i++)
			hojas[i].memoria+=dif;
		}
	cntmem+=dif;
	}
d.memput(hojas[n].memoria);
hojas[n].largo=tamanio;
};