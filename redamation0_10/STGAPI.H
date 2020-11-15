#ifndef STGAPI_H
#define STGAPI_H

#include "gx.h"

/* lookup table for scaling 5 bit colors up to 8 bits */
/*
int _rgb_scale_5[32] =
{
   0,   8,   16,  24,  32,  41,  49,  57,
   65,  74,  82,  90,  98,  106, 115, 123,
   131, 139, 148, 156, 164, 172, 180, 189,
   197, 205, 213, 222, 230, 238, 246, 255
};


/* lookup table for scaling 6 bit colors up to 8 bits */
/*
int _rgb_scale_6[64] =
{
   0,   4,   8,   12,  16,  20,  24,  28,
   32,  36,  40,  44,  48,  52,  56,  60,
   64,  68,  72,  76,  80,  85,  89,  93,
   97,  101, 105, 109, 113, 117, 121, 125,
   129, 133, 137, 141, 145, 149, 153, 157,
   161, 165, 170, 174, 178, 182, 186, 190,
   194, 198, 202, 206, 210, 214, 218, 222,
   226, 230, 234, 238, 242, 246, 250, 255
};*/

/////////////////////////////////////////////////////////////
// CNativeBitmap class represents a bitmap in the native format
class CNativeBitmap {
friend class CSTGapiBuffer;
public:
	int GetWidth()		{ return m_size.cx;}
	int GetHeight()		{ return m_size.cx;	}
	// default constructor
	CNativeBitmap()		{ m_pBits = 0;memset(&m_size, 0, sizeof(SIZE));	}
	~CNativeBitmap()	{ delete m_pBits; }
	SIZE GetBitmapSize() const	{ return m_size; }
protected:
	WORD* m_pBits;		// native bits
	SIZE  m_size;		// bitmap size
};

// forward declaration
struct BGRColor;

/*
//* given r,g,b on [0 ... 1],
//* return (h,s,l) on [0 ... 1] 
void RGB_to_HSL	(int r,int g,int b,int *h,int *s,int *l)
{
int v,m,vm,r2,g2,b2;
v=MAX(r,g);
v=MAX(v,b);
m=MIN(r,g);
m=MIN(m,b);
*l=(m+v)/(2<<16);
if (*l<=0) return;
*s=vm=v-m;
if (*s>0) 
	{
	*s/=(*l<=(1<<15))?(v+m):((2<<16)-v-m);
    } 
else
	return;
r2=(v-r)/vm;
g2=(v-g)/vm;
b2=(v-b)/vm;
if (r==v)
	*h=(g==m?(5<<16)+b2:(1<<16)-g2);
else if (g==v)
	*h=(b==m?(1<<16)+r2:(3<<16)-b2);
else
	*h=(r==m?(3<<16)+g2:(5<<16)-r2);
*h/=(6<<16);
}

//    * given h,s,l on [0..1],
//     * return r,g,b on [0..1]
void HSL_to_RGB(int h,int sl,int l,int *r,int *g,int *b)
{
int v;
v=(l<=(1<<15))?(l*((1<<16)+sl)):(l+sl-l*sl);
if (v<=0) 
	{
	*r=*g=*b=0;
    } 
else 
	{
	int m,sv,sextant;
	int fract,vsf,mid1,mid2;
	m=l+l-v;
	sv=(v-m)/v;
	h*=(6<<16);
	sextant=h;	
	fract=h-sextant;
	vsf=v*sv*fract;
	mid1=m+vsf;
	mid2=v-vsf;
	switch (sextant) {
		case 0: *r=v;*g=mid1;*b=m; break;
		case 1: *r=mid2;*g=v;*b=m; break;
		case 2: *r=m;*g=v;*b=mid1; break;
		case 3: *r=m;*g=mid2;*b=v; break;
		case 4: *r=mid1;*g=m;*b=v; break;
		case 5: *r=v;*g=m;*b=mid2; break;
		}
    }
}
*/
class CSTGapiBuffer {
public:
	WORD* Buffer;	// pointer to the display buffer
	int BufferStartOffset;	// zero-based offset of the buffer start in words
	int BufferEndOffset;	// zero-based offset of the buffer end in words
	int XPitch;	// x-pitch in words
	int YPitch;	// y-pitch in words
	GXDisplayProperties disp;	// display properties

	CSTGapiBuffer(void* pBuffer):Buffer((WORD*)pBuffer),m_pCurrentPos(NULL),BufferStartOffset(0),BufferEndOffset(0),m_pMemoryBuffer(NULL)
	{ InitDisplayProps(); }
	CSTGapiBuffer():Buffer(NULL),m_pCurrentPos(NULL),BufferStartOffset(0),BufferEndOffset(0),m_pMemoryBuffer(NULL)
	{ InitDisplayProps();}
	~CSTGapiBuffer()
	{ delete m_pMemoryBuffer; }

	inline void SetBuffer(void *pBuffer)
	{ Buffer = (WORD*)pBuffer; }

	BOOL CreateMemoryBuffer();	// Creates a memory based gapi buffer

	void BitBlt(const CSTGapiBuffer *pMemoryBuffer);	// Transfers pixels from a one buffer to another

	inline void BitBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, int nSrcX = 0, int nSrcY = 0)
	{ InternalBlt(nDestX, nDestY, nWidth, nHeight, pNativeBitmap, nSrcX, nSrcY, 0xFFFF); }
	inline void TransparentBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, DWORD dwTransparentNativeColor, int nSrcX = 0, int nSrcY = 0)
	{ InternalBlt(nDestX, nDestY, nWidth, nHeight, pNativeBitmap, nSrcX, nSrcY, dwTransparentNativeColor); }
	void MaskedBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, const CNativeBitmap* pNativeMask, int nSrcX = 0, int nSrcY = 0);

	CNativeBitmap* CreateNativeBitmap(HBITMAP hBitmap);
	///////////////////////////////////////////////////////
	// Pixel access	
	inline void SetPixel(DWORD X,DWORD Y,WORD dwNativeColor)
	{
	if (X>=disp.cxWidth || Y>=disp.cyHeight) return;
	WORD *dwOffset=Buffer+X*XPitch+Y*YPitch;
	*dwOffset=dwNativeColor;
	}
	inline void SetPixelF(DWORD X,DWORD Y,WORD dwNativeColor)
	{
	WORD *dwOffset=Buffer+X*XPitch+Y*YPitch;
	*dwOffset=dwNativeColor;
	}
	inline void SetPixelX(DWORD X,DWORD Y,WORD dwNativeColor)
	{
	if (X>=disp.cxWidth || Y>=disp.cyHeight) return;
	WORD *dwOffset=Buffer+X*XPitch+Y*YPitch;
	*dwOffset=*dwOffset^dwNativeColor;
	}
	///////////////////////////////////////////////////////
	// High perfomance drawing
	inline void SetPos(DWORD dwX, DWORD dwY)
	{ m_pCurrentPos = Buffer + XPitch * dwX + YPitch * dwY;}
	inline void IncXPos()
	{ m_pCurrentPos += XPitch;}
	inline void IncYPos()
	{ m_pCurrentPos += YPitch;}
	inline void DecXPos()
	{ m_pCurrentPos -= XPitch;}
	inline void DecYPos()
	{ m_pCurrentPos -= YPitch;}
	inline void SetPixel(DWORD dwNativeColor)
	{ *m_pCurrentPos = (WORD) dwNativeColor;}
	inline void SetPixelX(DWORD dwNativeColor)
	{ *m_pCurrentPos ^= (WORD) dwNativeColor;}
	inline SIZE GetDisplaySize()
	{ SIZE sz = { disp.cxWidth, disp.cyHeight };return sz;}
	inline DWORD GetBufferSize();	// RET: display buffer size in bytes

	// COLOR
	inline WORD Get16bppColor(COLORREF crColor)
	{
	return (WORD)((GetRValue(crColor)>>3)<<RedD|(GetGValue(crColor)>>3)<<GreenD|(GetBValue(crColor)>>3));
	}

	inline WORD GetNativeColor(COLORREF clrColor)
	{ return Get16bppColor(clrColor);}

	/* alpha = 0 : select c1, alpha = 255 select c2 */
	inline WORD mix_alpha(WORD c1,WORD c2, int alpha)
	{
	long r1,r2,r;long g1,g2,g;long b1,b2,b;
	r1 = c1 & RED_MASK;r2 = c2 & RED_MASK;
	r = (((r2-r1)*alpha + r1 * 256) >> 8) & RED_MASK;
	g1 = c1 & GREEN_MASK;g2 = c2 & GREEN_MASK;
	g = (((g2-g1)*alpha + g1 * 256) >> 8) & GREEN_MASK;
	b1 = c1 & BLUE_MASK;b2 = c2 & BLUE_MASK;
	b = (((b2-b1)*alpha + b1 * 256) >> 8) & BLUE_MASK;
	return (WORD)(r|g|b);
	}
	inline WORD setRGBColor(BYTE r,BYTE g,BYTE b)
	{
	return (WORD)(((r>>3)<<RedD)&RED_MASK|((g>>3)<<GreenD)&GREEN_MASK|((b>>3)&BLUE_MASK));
	}
	inline void getRGBColor(BYTE &r,BYTE &g,BYTE &b,WORD c)
	{
	r=((c&RED_MASK)>>RedD)<<3;
	g=((c&GREEN_MASK)>>GreenD)<<3;
	b=(c&BLUE_MASK)<<3;
	}
protected:
	void InitDisplayProps();// inializes internal stuctures
	static BGRColor* GetDIBits(HBITMAP hBitmap, SIZE& size, HBITMAP& hResultBitmap);// GetDIBits emulation (Windows CE does not support GetDIBits)
	WORD* CreateNativeBitmapFromBGRBits(int nWidth, int nHeight, const BGRColor* pDIBits);// Creates a native bitmap from DIB bits
	BOOL InternalBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, int nSrcX, int nSrcY, DWORD dwTransparentNativeColor);

	/////////////////////////////////
	// member variables
	void* m_pMemoryBuffer;	// pointer to the memory buffer
	WORD* m_pCurrentPos;	// current position (used for high perfomance drawing)
	int RedD,GreenD;
	unsigned int RED_MASK,GREEN_MASK,BLUE_MASK;
};

//************************************
extern CSTGapiBuffer screen;

#endif