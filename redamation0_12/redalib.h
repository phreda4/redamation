#ifndef REDALIB_H
#define REDALIB_H

#include "gx.h"
#include "windef.h"

//
// Flujo del programa
//
extern HINSTANCE hInst;
extern HWND hWnd;

extern unsigned int basetime;
extern int b1, b2, b3, b4, b5;
extern int bl, br, bu, bd;
extern int blp, brp, bup, bdp;
extern bool button1up, button2up, button3up, button4up, button5up;
extern bool buttonuup, buttondup, buttonlup, buttonrup;

extern unsigned short mxpos, mypos, mcxpos, mcypos;
extern bool mbutton,mcproc;

//------------------------------------------------------------
void update(void);

// rutinas de tiempo
inline void resettimer(void)
{
basetime = GetTickCount();
}

inline int clock(void)
{
return GetTickCount();
}

inline int msecs(void)
{
return GetTickCount() - basetime;
}

inline int loop(int max,int frames)
{
return ((GetTickCount()>>6)/frames)%max;
}

inline int pingpong(int max,int frames)
{
int aux=((GetTickCount()>>6)/frames)%(2*max);
return ((int)aux>max)?2*max-aux:aux;
}

inline bool buttonA(void)
{
bool retval = (b1 != 0);
return retval;
}

inline bool buttonB(void)
{
bool retval = (b2 != 0);
return retval;
}

inline bool buttonC(void)
{
bool retval = (b3 != 0);
return retval;
}

inline bool buttonD(void)
{
bool retval = (b4 != 0);
return retval;
}

inline bool buttonE(void)
{
bool retval = (b5 != 0);
return retval;
}

inline bool leftpressed(void)
{
bool retval = (blp != 0);
blp = 0;
return retval;
}

inline bool rightpressed(void)
{
bool retval = (brp != 0);
brp = 0;
return retval;
}

inline bool uppressed(void)
{
bool retval = (bup != 0);
bup = 0;
return retval;
}

inline bool downpressed(void)
{
bool retval = (bdp != 0);
bdp = 0;
return retval;
}

inline bool buttonleft(void)
{
bool retval = (bl != 0);
return retval;
}

inline bool buttonright(void)
{
bool retval = (br != 0);
return retval;
}

inline bool buttonup(void)
{
bool retval = (bu != 0);
return retval;
}

inline bool buttondown(void)
{
bool retval = (bd != 0);
return retval;
}

inline void clearinput(void)
{
blp=brp=bup=bdp=b1=b2=b3=b4=b5=bl=bu=br=bd=0;
mcproc=false;
}

inline bool pressed(bool clear)
{
bool retval = false;
if (buttonA() || buttonB() || buttonC() || buttonD() || buttonE()) retval = true;
if (!clear) if ((blp != 0) || (brp != 0) || (bup != 0) || (bdp != 0)) retval = true;
if (clear) if (uppressed() || downpressed() || leftpressed() || rightpressed()) retval = true;
return retval;
}

inline void penpos(unsigned short *x,unsigned short *y)
{
*x=mxpos;*y=mypos;
}

inline bool pendown(void)
{
return mbutton;
}

inline bool click(void)
{
if (!mcproc) return mcproc;
mcproc=false;
return true;
}


#endif
