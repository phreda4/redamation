#include "stdafx.h"
#include "aygshell.h"
#include "redalib.h"
#include "graficos.h"

const TCHAR wincls[5] = TEXT("lib");

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

int scan_up, scan_left, scan_right, scan_down;
int scan_A, scan_B, scan_C, scan_D, scan_E, lastKey;

GXKeyList kl;				// Hardware buttons

#define MENU_HEIGHT 26

//------------------------------------------------------------------------------
void taskbar(bool show)
{
RECT rc;
GetWindowRect(hWnd,&rc);
HWND hWndTB=FindWindow(TEXT("HHTaskbar"),NULL);
if (show)
	{
	SHFullScreen(hWnd,SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON + SHFS_SHOWSTARTICON);
	ShowWindow(hWndTB,SW_SHOW );
	MoveWindow(hWnd,rc.left,rc.top + MENU_HEIGHT,rc.right,rc.bottom - 2 * MENU_HEIGHT,TRUE);
	}
else
	{
	SHFullScreen(hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON + SHFS_HIDESTARTICON);
	ShowWindow(hWndTB, SW_HIDE );
	MoveWindow(hWnd,rc.left,rc.top - MENU_HEIGHT,rc.right,rc.bottom + MENU_HEIGHT,TRUE);
	}
}

// rutina de loop
//------------------------------------------------------------
void update()
{
MSG msg;
while (PeekMessage(&msg,0,0,0,PM_NOREMOVE)==TRUE)
	{
	GetMessage(&msg,0,0,0);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
}


//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
short key=0;
switch (message) 
	{
	case WM_MOUSEMOVE:
		mxpos=LOWORD(lParam);
		mypos=HIWORD(lParam);
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
/*
	case WM_CREATE: 
		initscreen(); 
		break;  
*/
    case WM_DESTROY:
        GXCloseInput();
        GXCloseDisplay();
		taskbar(true);
        PostQuitMessage(0);
        break;
    case WM_KILLFOCUS:
        GXSuspend();
        break;
    case WM_SETFOCUS:
        GXResume();
        break;
	case WM_SETTINGCHANGE: 
		taskbar(false);
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
wcls.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
wcls.lpszMenuName	= 0;
wcls.lpszClassName	= wincls;
return RegisterClass(&wcls);
}
//------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hi,int nCmdShow)
{
hInst=hi;
hWnd=CreateWindowEx(WS_EX_TOPMOST,wincls,wincls,WS_VISIBLE,0,0,240,320,NULL,NULL,hi,NULL);
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
if (!MyRegisterClass(hInstance)) return FALSE;
if (!InitInstance(hInstance, nCmdShow)) return FALSE;
GXOpenDisplay(hWnd,GX_FULLSCREEN);
GXOpenInput();
kl=GXGetDefaultKeys(GX_NORMALKEYS);

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
GXCloseInput();
GXCloseDisplay();
taskbar(true);
DestroyWindow(hWnd);
return 0;
}
