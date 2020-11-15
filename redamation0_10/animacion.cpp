#include "stdafx.h"
#include "flib.h"
//..................
#include "animacion.h"

void Animacion::cargar(char *n)
{
HANDLE hFile;
DWORD Bytes;
TCHAR szName[255];
wsprintf(szName,_T("\\reda\\%hs.rdm"),n);// de char a charw
hFile=CreateFile(szName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
if (hFile == INVALID_HANDLE_VALUE)	return;
ReadFile(hFile,&ccuadros,sizeof(ccuadros),&Bytes,NULL);
ReadFile(hFile,&cmem,sizeof(cmem),&Bytes,NULL);
ReadFile(hFile,mem,cmem,&Bytes,NULL);
ReadFile(hFile,&vista,sizeof(vista),&Bytes,NULL);
ReadFile(hFile,&bColor,sizeof(bColor),&Bytes,NULL);
recostruye();
CloseHandle(hFile);
}

void Animacion::grabar(char *n)
{
HANDLE hFile;
DWORD Bytes;
TCHAR szName[255];
wsprintf(szName,_T("\\reda\\%hs.rdm"),n);// de char a charw
hFile=CreateFile(szName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
if (hFile==INVALID_HANDLE_VALUE)  return;
WriteFile(hFile,&ccuadros,sizeof(ccuadros),&Bytes,NULL);
WriteFile(hFile,&cmem,sizeof(cmem),&Bytes,NULL);
WriteFile(hFile,mem,cmem,&Bytes,NULL);
WriteFile(hFile,&vista,sizeof(vista),&Bytes,NULL);
WriteFile(hFile,&bColor,sizeof(bColor),&Bytes,NULL);
CloseHandle(hFile);
}

char Animacion::tipoin(char *p)
{
return *(char*)p;
}

int Animacion::sizein(char *p)
{
Cabecera cab;
memcpy(&cab,p,sizeof(Cabecera));
return sizeof(Cabecera)+
		sizeof(NodoDibujo)*cab.cntp+sizeof(TipoTrazo)*cab.cntt+sizeof(NodoCambio)*cab.cntc;
}

void Animacion::recostruye()
{
char *p=mem;
for (unsigned short i=0;i<ccuadros;i++)
	{
	cuadros[i]=p;
	scuadros[i]=sizein(p);
	p+=sizein(p);
	}
}

void Animacion::getDibujo(Dibujo *d,unsigned short n)
{
if (n>=ccuadros) 
	{
//	d->clear();
	return;
	}
unsigned short n1=n;
while (tipoin(cuadros[n1])==1) n1--;
d->getmem(cuadros[n1]);// cargo el tipo=0
d->setVista(vista);
while (n1<n)// vuelve al que se pidio
	{
	n1++;
	d->getmem(cuadros[n1]);
	//d->regenera();
	d->cargaCambios();
	}
d->regenera();// para edicion
d->regeneraBBox();
}

void Animacion::setDibujo(Dibujo *d,unsigned short n)
{
int tamanio=d->size();
if (n==ccuadros)// se agrega uno
	{
	ccuadros++;

	cuadros[n]=&mem[cmem];
	cmem+=tamanio;
	}
else// correr los otros cuadros
	{
	int dif=tamanio-scuadros[n];//sizein(cuadros[n]);//
	if (dif!=0  &&  n+1<ccuadros)
		{
		char *pp=cuadros[n+1];
		memmove(pp+dif,pp,cmem-(pp-mem));// falta restar algo
		for (unsigned short i=n+1;i<ccuadros;i++)
			cuadros[i]+=dif;
		}
	cmem+=dif;
	}
d->setmem(cuadros[n]);
scuadros[n]=tamanio;
}

void Animacion::trim()
{
unsigned short c=ccuadros-1;
while (c>0 && sizein(cuadros[c])==sizeof(char)+sizeof(unsigned short)*4) 
	{ ccuadros--;c--; }
}


void Animacion::play()
{
Dibujo dib;
dib.setVista(vista);

DWORD baset=0,delay=1000/fps;
unsigned short c=0,ctiempo=1;
bool termina=false;

while (!termina) {
	update();
	if (GetTickCount()-baset>delay) 
		{
		baset=GetTickCount();
		if (ctiempo==1)
			{
			if (c>=ccuadros) 
				{ c=0;dib.setVista(vista); }
			dib.getmem(cuadros[c]);
			dib.regenera();
			dib.regeneraBBox();
			ctiempo=dib.tiempo;
			c++;
			if (dib.tipo==1)
				{//seteo a primero
				dib.settiempo();
				dib.settiempot(ctiempo);
				}
			ClrSrc(bColor);
			dib.draw();
			redraw();
			}
		else
			{
			ctiempo--;
			if (dib.tipo==1)
				{// cambio a ctiempo
				dib.settiempot(ctiempo);
				ClrSrc(bColor);
				dib.draw();
				redraw();
				}
			}
		}
	if (click()) termina=true;
	}
while (pendown()) update();
}
