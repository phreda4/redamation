//
// flib.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#include "flib.h"
#include "Aygshell.h"
#include "stgapi.h"
#include "graph.h"

HINSTANCE hInst;
HWND hWnd;

unsigned int basetime = 0;
int b1,b2,b3,b4,b5;
int bl,br,bu,bd;
int blp,brp,bup,bdp;
bool button1up,button2up,button3up,button4up,button5up;
bool buttonuup,buttondup,buttonlup,buttonrup;
unsigned short mxpos,mypos,mcxpos,mcypos;
bool mbutton,mcproc=false;

TCHAR wincls[11] = TEXT("Redamation");

// Custom button layout stuff
int scan_up, scan_left, scan_right, scan_down;
int scan_A, scan_B, scan_C, scan_D, scan_E, lastKey;

GXKeyList kl;// Hardware buttons

CSTGapiBuffer bufferScreen;

//------------------------------------------------------------------------------
void initscreen() 
{ 
screen.CreateMemoryBuffer();
SetPixelCopy();
SetClip(0,0,screen.disp.cxWidth-1,screen.disp.cyHeight-1);// init clip
}

#define MENU_HEIGHT 26
//------------------------------------------------------------------------------
void taskbar(bool show)
{
RECT rc;
GetWindowRect( hWnd, &rc );
HWND hWndTB=FindWindow(TEXT("HHTaskbar"),NULL);
if (show)
	{
	SHFullScreen( hWnd, SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON + SHFS_SHOWSTARTICON );
	ShowWindow( hWndTB, SW_SHOW );
	MoveWindow(hWnd,rc.left,rc.top + MENU_HEIGHT,rc.right,rc.bottom - 2 * MENU_HEIGHT,TRUE);
	}
else
	{
	SHFullScreen(hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON + SHFS_HIDESTARTICON );
	ShowWindow(hWndTB, SW_HIDE );
	MoveWindow(hWnd,rc.left,rc.top - MENU_HEIGHT,rc.right,rc.bottom + MENU_HEIGHT,TRUE);
	}
}
//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
short key=0;
switch (message) 
	{
/*	case WM_ENABLE: 
		break;
	case WM_ACTIVATE: 
		if (LOWORD(wParam) == WA_INACTIVE ) 
			{ 
			taskbar(true);// set pause flag;//	pause_emulation();
			} 
		else if (LOWORD(wParam) == WA_ACTIVE ) 
			{
			taskbar(false);// clear pause flag;//restart_emulation();
			}
		break;*/
	case WM_SETTINGCHANGE: 
		taskbar(false);
		break;
	case WM_LBUTTONDOWN:
		mxpos=LOWORD(lParam);
		mypos=HIWORD(lParam);
		mcproc=true;
		mbutton=true;
		break;
	case WM_LBUTTONUP:
		mbutton=false;
		break;
	case WM_MOUSEMOVE:
		mxpos=LOWORD(lParam);
		mypos=HIWORD(lParam);
		break;
	case WM_KEYUP:
		key = (short)wParam;
		if (key==scan_left) bl = 0;
		if (key==scan_right) br = 0;
		if (key==scan_up) bu = 0;
		if (key==scan_down) bd = 0;
		if (key==scan_A) b1 = 0;
		if (key==scan_B) b2 = 0;
		if (key==scan_C) b3 = 0;
		if (key==scan_D) b4 = 0;
		if (key==scan_E) b5 = 0;
		break;
	case WM_KEYDOWN:
		key = (short)wParam;
		if (key==scan_A) b1 = 1;
		if (key==scan_B) b2 = 1;
		if (key==scan_C) b3 = 1;
		if (key==scan_D) b4 = 1;
		if (key==scan_E) b5 = 1;
		if (key==scan_left) bl = blp = 1;
		if (key==scan_right) br = brp = 1;
		if (key==scan_up) bu = bup = 1;
		if (key==scan_down) bd = bdp = 1;
		lastKey = key;
		break;
	case WM_CREATE: 
		initscreen(); 
		break;  
	case WM_DESTROY: 
		PostQuitMessage(0); 
		break;  
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
return 0;
}
//------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
WNDCLASS wcls;
wcls.style			= 0;
wcls.lpfnWndProc	= (WNDPROC)WndProc;
wcls.cbClsExtra		= 0;
wcls.cbWndExtra		= 0;
wcls.hInstance		= hInstance;
wcls.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(1));
wcls.hCursor		= 0;
wcls.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
wcls.lpszMenuName	= 0;
wcls.lpszClassName	= wincls;
return RegisterClass(&wcls);
}
//------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hi, int nCmdShow)
{
hInst=hi;
hWnd=CreateWindowEx(WS_EX_TOPMOST,wincls,TEXT("Redamation"),WS_VISIBLE,0,0,240,320,NULL,NULL,hi,NULL);
if (!hWnd) return FALSE;
ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);
return TRUE;
}

//*****************************************************************************************
extern void __cdecl main(void);
//*****************************************************************************************
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
taskbar(false);
//resettimer();
if (!MyRegisterClass(hInstance)) return FALSE;
if (!InitInstance (hInstance, nCmdShow)) return FALSE;
GXOpenDisplay(hWnd,GX_FULLSCREEN);
GXOpenInput();
kl = GXGetDefaultKeys(GX_NORMALKEYS);
scan_up=kl.vkUp;
scan_down=kl.vkDown;
scan_left=kl.vkLeft;
scan_right=kl.vkRight;
scan_A=kl.vkA;scan_B=kl.vkB;scan_C=kl.vkC;scan_D=kl.vkStart;scan_E=0;
mbutton=false;
b1=b2=b3=b4=b5=0;
srand(clock());
//---------------------------------------------------------------------
main();
//---------------------------------------------------------------------
taskbar(true);
GXCloseInput();
GXCloseDisplay();
return 0;
}

// rutina de loop
//------------------------------------------------------------
void update()
{
MSG msg;
while (PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE ) == TRUE)
	{
	GetMessage( &msg, 0, 0, 0 );
	TranslateMessage( &msg );
	DispatchMessage( &msg );
	}
}

//------------------------------------------------------------
void redraw(void) 
{ 
void *pBuffer = GXBeginDraw();
bufferScreen.SetBuffer(pBuffer);
bufferScreen.BitBlt(&screen);
GXEndDraw();
}  

//-------------------------------------------------------
Menu *getMenu(Menu *m,unsigned short x,unsigned short y)
{
Menu *p=m;
while (p->t!=NULL)
	{
	if (x>=p->x1 && x<=p->x2 && y>=p->y1 && y<=p->y2)
		return p;
	p++;
	}
return NULL;
}

void drawMenu(Menu *m)
{
int x,y;
Menu *p=m;
SetRGB(0,255,0);
while (p->t!=NULL)
	{
	Box(p->x1,p->y1,p->x2,p->y2);
	x=((p->x1+p->x2)>>1)-((strlen(p->t)*7)>>1);
	y=((p->y1+p->y2)>>1)-4;
	writeTex(x,y,p->t);
	p++;
	}
}

char getMenuEx(Menu *m,unsigned short x,unsigned short y,unsigned short x1,unsigned short y1)
{
Menu *p=m;
char r=1;
while (p->t!=NULL)
	{
	if (x>=x1+p->x1 && x<=x1+p->x1+p->x2 && y>=y1+p->y1 && y<=y1+p->y1+p->y2)
		{
		if (p->t[0]>30)	{
			SetPixelXOR();SetColor(0x0ff0);
			BoxFill(x1+p->x1,y1+p->y1,x1+p->x1+p->x2,y1+p->y1+p->y2);
			redraw();
			while (pendown()) update();
			penpos(&x,&y);
			BoxFill(x1+p->x1,y1+p->y1,x1+p->x1+p->x2,y1+p->y1+p->y2);
			SetPixelCopy();
			redraw();
			if (!(x>=x1+p->x1 && x<=x1+p->x1+p->x2 && y>=y1+p->y1 && y<=y1+p->y1+p->y2)) r=0;
			}
		return r;
		}
	p++;r++;
	}
return 0;
}

void drawMenuEx(Menu *m,unsigned short xd,unsigned short yd)
{
short x,y,x1,y1,x2,y2;
Menu *p=m;
while (p->t!=NULL)
	{
	x1=xd+p->x1;y1=yd+p->y1;
	x2=xd+p->x1+p->x2;y2=yd+p->y1+p->y2;
	SetColor(0x79ef);BoxFill(x1,y1,x2,y2);
	SetColor(0xffff);HLine(x1,y1,x2);VLine(x1,y1,y2);
	SetColor(0x1863);HLine(x1,y2,x2);VLine(x2,y1,y2);
	if (p->t[0]>30) {
		x=((x1+x2)>>1)-((strlen(p->t)*7)>>1);y=((y1+y2)>>1)-4;
		SetColor(0x0);writeTexB(x,y,p->t);
		SetColor(0xffff);writeTex(x,y,p->t);
		}
	p++;
	}
}

//----------------------------------------------------
#define MAXSENS 256
Menu mint[MAXSENS];
int cntmint;

void clearSensor()
{
cntmint=0;
}

int getSensor(unsigned short x,unsigned short y)
{
Menu *p=mint;
while (p->t!=NULL)
	{
	if (x>=p->x1 && x<=p->x2 && y>=p->y1 && y<=p->y2)
		return (int)p->t;
	p++;
	}
return 0;
}

int getNSensor(unsigned short x,unsigned short y,unsigned short n)
{
unsigned short cn=0;
Menu *p=mint;
while (p->t!=NULL)
	{
	if (x>=p->x1 && x<=p->x2 && y>=p->y1 && y<=p->y2)
		{
		if (cn==n) return (int)p->t;
		cn++;
		}
	p++;
	}
return 0;

}

void addSensor(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2,int v)
{
if (cntmint>=MAXSENS) return;
mint[cntmint].x1=x1;
mint[cntmint].y1=y1;
mint[cntmint].x2=x2;
mint[cntmint].y2=y2;
mint[cntmint].t=(char*)v;
cntmint++;
mint[cntmint].t=NULL;
}

//---------------------------------------------------------------
const char teclas[]="QWERTYUIOP-+ASDFGHJKL*/^ZXCVBNM ;:<>";

void drawTeclado()
{
unsigned short i;
SetRGB(240,240,240);Box(5,239,235,295);
SetRGB(0,32,0);BoxFill(6,240,234,294);
int u=6,v=240;
for (i=0;i<strlen(teclas);i++)
	{
	SetColor(0x0);writeCarN(u+6,v+6,teclas[i]);
	SetRGB(0,255,0);writeCarN(u+5,v+5,teclas[i]);
	u+=19;if (u>220) { u=6;v+=18; }
	}
SetColor(0);
for (i=240;i<295;i+=18) HLine(6,i,234);
for (i=6;i<240;i+=19)	VLine(i,240,294);
}

char inputTeclado(int x,int y)
{
if (x<6 || x>234 || y<240 || y>294) return '#';// caracter de no tecla
x=x-6;y=y-240;
x=x/19;y=y/18;
return teclas[x+y*12];
}

void editaText(char *t,int c)
{
unsigned short x,y;
int p=strlen(t),cp=p;
char e;
DWORD baset=0;
drawTeclado();
SetColor(0);BoxFill(1,229,238,238);
SetColor(0xffff);Box(0,228,239,239);
writeTex(2,230,t);
redraw();
while (1) {
	update();
	if (click()) {
		penpos(&x,&y);
		e=inputTeclado(x,y);
		switch (e) {
		case '#':
				while (pendown()) update();
				return;
		case '<':if (cp>0) cp--;
				SetColor(0);BoxFill(1,229,238,238);
				SetColor(0xffff);writeTex(2,230,t);
				break;
		case '>':if (cp<p) cp++;
				SetColor(0);BoxFill(1,229,238,238);
				SetColor(0xffff);writeTex(2,230,t);
				break;
		default:
			if (cp<c) 
				{
				if (cp==p)
				{ t[p]=e;p++;t[p]=0;}
				else
				{ t[cp]=e;}
				cp++;
				SetColor(0);BoxFill(1,229,238,238);
				SetColor(0xffff);writeTex(2,230,t);
				redraw();
				}
			}
		}
	if (GetTickCount()-baset>80)
		{
		SetPixelXOR();SetColor(0xffff);
		BoxFill(2+cp*7,229,9+cp*7,238);
		SetPixelCopy();
		baset=GetTickCount();
		redraw();
		}
	}	
}


void drawMenu2(char *m,unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
char *p=m;
while (*p!=0) {
	switch (*p) {
	case '{':// corte vertical
		// para cada componente
	case '['://corte horizontal
		//para cada componenete
	default:
		return;
		}
	}
}