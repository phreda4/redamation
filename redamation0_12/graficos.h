#ifndef GRAFICOS_H
#define GRAFICOS_H

#include "mat.h"
#include "redalib.h"

#define SIZESCREEN 240*320*2

struct BufferGrafico;
extern BufferGrafico Pantalla;

void RGB_to_HSL(int r,int g,int b,int *h,int *s,int *l);
void HSL_to_RGB(int h,int s,int l,int *r,int *g,int *b);

//-------COLOR
const int _rgb_scale_4[16] =
{0,20,41,57,74, 90, 106, 123,139, 156, 172, 189,205, 222, 238, 255};

const int _rgb_scale_5[32] =
{0,   8,   16,  24,  32,  41,  49,  57, 65,  74,  82,  90,  98,  106, 115, 123,
 131, 139, 148, 156, 164, 172, 180, 189,197, 205, 213, 222, 230, 238, 246, 255};

const int _rgb_scale_6[64] =
{0,   4,   8,   12,  16,  20,  24,  28,32,  36,  40,  44,  48,  52,  56,  60,
 64,  68,  72,  76,  80,  85,  89,  93,97,  101, 105, 109, 113, 117, 121, 125,
 129, 133, 137, 141, 145, 149, 153, 157,161, 165, 170, 174, 178, 182, 186, 190,
 194, 198, 202, 206, 210, 214, 218, 222,226, 230, 234, 238, 242, 246, 250, 255};

const WORD style1[]={0x0f0f,0x1e1e,0x3c3c,0x7878,0xf0f0,0xe1e1,0xc3c3,0x8787};

struct ColorRGBA {
	byte r,g,b,a;
	WORD directo;

	void set(byte r1,byte g1,byte b1,byte a1) { r=r1;g=g1;b=b1;a=a1;directo=get565(); };
	WORD get565() { WORD r1=r;r1<<=8;WORD g1=g;g1<<=3;return (WORD)(r1&0xf800|g1&0x7e0|(b>>3)&0x1f); };
	WORD get555() { WORD r1=r;r1<<=8;WORD g1=g;g1<<=2;return (WORD)(r1&0xfc00|g1&0x3e0|(b>>3)&0x1f); };
	void mix(ColorRGBA &c,byte alpha) 
	{
	r=((int)(c.r-r)*alpha+(r<<8))>>8;
	g=((int)(c.g-g)*alpha+(g<<8))>>8;
	b=((int)(c.b-b)*alpha+(b<<8))>>8;
	directo=get565();
	};
	bool equal(ColorRGBA &c2) { return (r==c2.r && g==c2.g && b==c2.b && a==c2.a); }
	void setC(ColorRGBA &c) {r=c.r;g=c.g;b=c.b;a=c.a;directo=c.directo;};
};

extern ColorRGBA colornegro,colorblanco;

//-------BIPMAP
struct CNativeBitmap {
	int GetWidth()		{ return m_size.cx;}
	int GetHeight()		{ return m_size.cx;	}
	CNativeBitmap()		{ m_pBits = 0;memset(&m_size, 0, sizeof(SIZE));	}
	~CNativeBitmap()	{ delete m_pBits; }
	SIZE GetBitmapSize() const	{ return m_size; }
	WORD* m_pBits;		// native bits
	SIZE  m_size;		// bitmap size
};

struct BGRColor
{
	BGRColor() {}
	BGRColor(byte R, byte G, byte B) : m_R(R), m_G(G), m_B(B) {}
	byte m_B;byte m_G;byte m_R;
};

struct DIBINFO:public BITMAPINFO
{
RGBQUAD	 arColors[255];    // Color table info - adds an extra 255 entries to palette
operator LPBITMAPINFO()          { return (LPBITMAPINFO) this; }
operator LPBITMAPINFOHEADER()    { return &bmiHeader;          }
RGBQUAD* ColorTable()            { return bmiColors;           }
};

extern void (*_spixel)(BufferGrafico *bg);
extern void (*_spixela)(BufferGrafico *bg,byte a);

void _spixeld(BufferGrafico *bg);// dibujo color solido
void _spixelda(BufferGrafico *bg,byte a);
void _spixelaa(BufferGrafico *bg);// dibujo solido con alpha
void _spixelaaa(BufferGrafico *bg,byte a);
void _spixelx(BufferGrafico *bg);// xor
void _spixelxa(BufferGrafico *bg,byte a);

//:::::::::::::::::
struct Segmento {
	short ymax;// donde corta
	long x,dx;// inicio de x, avance de x
	struct Segmento *next,*nextValid;
};

#define NB_SEGMENT_MAX	512 // cada segmento es una linea
#define SEGFRAC	8			// precision de los poligonos
#define MASFRAC	0xff	
#define MAXLINES 321 // 1+ alto de la pantalla

//----Buffer grafico
struct BufferGrafico {
	WORD* Buffer;			// pointer to the display buffer
	int BufferStartOffset;	// zero-based offset of the buffer start in words
	int BufferEndOffset;	// zero-based offset of the buffer end in words
	int XPitch,YPitch;	// x,y-pitch in words
	GXDisplayProperties disp;	// display properties
	ColorRGBA color;
	void* m_pMemoryBuffer;	// pointer to the memory buffer
	WORD* m_pCurrentPos;	// current position (used for high perfomance drawing)
	byte alpha;
	WORD RedD,GreenD;
	WORD RED_MASK,GREEN_MASK,BLUE_MASK;
	short hp,wp;// subventana
	short limymin,limymax;
	Segmento *linact;
	Segmento *lines[MAXLINES];
	Segmento seg[NB_SEGMENT_MAX];

// funciones
	BufferGrafico():BufferStartOffset(0),BufferEndOffset(0)	{ InitDisplayProps();}
	~BufferGrafico() { delete m_pMemoryBuffer; }
	inline void BitBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, int nSrcX = 0, int nSrcY = 0)
	{ InternalBlt(nDestX, nDestY, nWidth, nHeight, pNativeBitmap, nSrcX, nSrcY, 0xFFFF); }
	inline void TransparentBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, DWORD dwTransparentNativeColor, int nSrcX = 0, int nSrcY = 0)
	{ InternalBlt(nDestX, nDestY, nWidth, nHeight, pNativeBitmap, nSrcX, nSrcY, dwTransparentNativeColor); }
	void MaskedBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, const CNativeBitmap* pNativeMask, int nSrcX = 0, int nSrcY = 0);

	CNativeBitmap* CreateNativeBitmap(HBITMAP hBitmap);
	CNativeBitmap* loadBitmap(char *filename);

	inline SIZE GetDisplaySize()	{ SIZE sz = { disp.cxWidth, disp.cyHeight };return sz;}
	inline WORD Get16bppColor(COLORREF crColor)	{ return (WORD)((GetRValue(crColor)>>3)<<RedD|(GetGValue(crColor)>>3)<<GreenD|(GetBValue(crColor)>>3));	}
	inline WORD GetNativeColor(COLORREF clrColor){ return Get16bppColor(clrColor);}
	inline WORD mix_alpha(WORD c1,WORD c2,byte alpha)
	{
	int r1,r2,r,g1,g2,g,b1,b2,b;
	r1=c1&RED_MASK;r2=c2&RED_MASK;
	r=(((r2-r1)*alpha+(r1<<8))>>8)&RED_MASK;
	g1=c1&GREEN_MASK;g2=c2&GREEN_MASK;
	g=(((g2-g1)*alpha+(g1<<8))>>8)&GREEN_MASK;
	b1=c1&BLUE_MASK;b2=c2&BLUE_MASK;
	b=(((b2-b1)*alpha+(b1<<8))>>8)&BLUE_MASK;
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
// dibujo de pixel
	inline void SetPos(DWORD dwX, DWORD dwY){ m_pCurrentPos=Buffer+XPitch*dwX+YPitch*dwY;}
	inline void IncXPos()					{ m_pCurrentPos+=XPitch;}
	inline void IncYPos()					{ m_pCurrentPos+=YPitch;}
	inline void DecXPos()					{ m_pCurrentPos-=XPitch;}
	inline void DecYPos()					{ m_pCurrentPos-=YPitch;}
	inline void XPos(int i)					{m_pCurrentPos+=XPitch*i;};
	inline void YPos(int i)					{m_pCurrentPos+=YPitch*i;};
	inline void SPixelX(void)				{ *m_pCurrentPos^=color.directo; }
	inline void SPixel(void)				{_spixel(this);}
	inline void SPixelA(byte a)				{_spixela(this,a);}
	inline void XOR(bool s)
	{ if (s) {::_spixel=::_spixelx;::_spixela=::_spixelxa;}	else Alpha(alpha);}
	inline void Alpha(byte a)
	{alpha=a;if (alpha==255) {::_spixel=::_spixeld;::_spixela=::_spixelda;} else{::_spixel=::_spixelaa;::_spixela=::_spixelaaa;}}
// color
	inline void Color(WORD c) { color.directo=c; };
	inline void Color(ColorRGBA &c) { color=c;Alpha(c.a); };
	inline void ColorRGB(byte r,byte g,byte b) 	{ color.set(r,g,b,255); }
//	inline void ColorRGBA(byte r,byte g,byte b,byte a)  { color.set(r,g,b,a); }
// sin recorte
	void ClrScr(void);
	void AALine(WORD x1,WORD y1,WORD x2,WORD y2);
	bool ClipLine(int *X1,int *Y1,int *X2,int *Y2);
// con recorte
	void VLine(short x1,short y1,short y2);
	void HLine(short x1,short y1,short x2);
	void HLineD(int x1,int y1,int x2,ColorRGBA &c1,ColorRGBA &c2);
	void VLineD(int x1,int y1,int y2,ColorRGBA &c1,ColorRGBA &c2);
	inline void SetPixelF(DWORD X,DWORD Y)
	{
	SetPos(X,Y);SPixel();	
	}
	inline void SetPixel(DWORD X,DWORD Y)
	{
	if (X>=disp.cxWidth || Y>=disp.cyHeight) return;
	SetPos(X,Y);SPixel();	
	}
	inline void SetPixelA(DWORD X,DWORD Y,byte a)
	{ 
	if (X>=disp.cxWidth || Y>=disp.cyHeight) return;
	SetPos(X,Y);SPixelA(a);	
	}
	inline void SetPixelX(DWORD X,DWORD Y)
	{
	if (X>=disp.cxWidth || Y>=disp.cyHeight) return;
	SetPos(X,Y);SPixelX();	
	}
	void SPline(int x1,int y1,int x2,int y2,int x3,int y3)
	{
	SPlineIter(x1<<4,y1<<4,x2<<4,y2<<4,x3<<4,y3<<4);
	}
	void SPlineC(int x1,int y1,int x2,int y2,int x3,int y3)
	{
	int mx,my;
	mx=(x1+x3)>>1;my=(y1+y3)>>1;
	x2=mx+((x2-mx)<<1);y2=my+((y2-my)<<1);
	SPlineIter(x1<<4,y1<<4,x2<<4,y2<<4,x3<<4,y3<<4);
	}
	void Line(int x1,int y1,int x2,int y2);
	void FLine(int x1,int y1,int x2,int y2,WORD t);
	void Box(int x1,int y1,int x2,int y2);
	void BoxR(int x1,int y1,int x2,int y2,byte r);
	void FillBox(int x1,int y1,int x2,int y2);
	void FillBoxR(int x1,int y1,int x2,int y2,byte r);
	void Circle(int x,int y,int r);
	void Ellipse(int x,int y,int rx,int ry);
	void FillCircle(int x,int y,int r);
	void Ellipse(int x,int y,int rx,int ry,bool fill);

	// figuras mas complejas
	void DBoton(int x1,int y1,int x2,int y2,char *t);
	void DBotonD(int x1,int y1,int x2,int y2,char *t);
/*
	void wChar(short x,short y,char c);
	void wChar6(short x,short y,char c);
	void wCharB(short x,short y,char c);
	void Text(short x,short y,char *n);
	void Text6(short x,short y,char *n);
	void TextB(short x,short y,char *n);
	void TextSize(char *n,short *x,short *y);
	void TextC(short x,short y,char *n);
	void TextBC(short x,short y,char *n);
*/
	void TextList(short x1,short y1,short x2,short y2,char *l,short c);

	char wCharV(short x,short y,unsigned char c);
	char wCharVB(short x,short y,unsigned char c);
	char wCharVG(short x,short y,unsigned char c);

	void TextSizeV(char *n,short *x,short *y);
	void TextV(short x,short y,char *n);
	void TextVG(short x,short y,char *n);
	void TextVB(short x,short y,char *n);
	void TextVC(short x,short y,char *n);
	void TextVBC(short x,short y,char *n);
	void TextVGC(short x,short y,char *n);
	
	char wCharV2(short x,short y,unsigned char c);
	char wCharVB2(short x,short y,unsigned char c);
	void TextSizeV2(char *n,short *x,short *y);
	void TextV2(short x,short y,char *n);
	void TextVB2(short x,short y,char *n);
	void TextVC2(short x,short y,char *n);
	void TextVBC2(short x,short y,char *n);


	void Boton(int x1,int y1,int x2,int y2);
	void BotonD(int x1,int y1,int x2,int y2);
	void BotonP(int x1,int y1,int x2,int y2);
// privadas
	void SPlineIter(long x1,long y1,long x2,long y2,long x3,long y3);
// poligono
	void pSPline(int x1,int y1,int x2,int y2,int x3,int y3)
	{pSPlineIter(x1<<4,y1<<4,x2<<4,y2<<4,x3<<4,y3<<4);}
	void pSPlineC(int x1,int y1,int x2,int y2,int x3,int y3)
	{
	int mx=(x1+x3)>>1;int my=(y1+y3)>>1;x2=mx+((x2-mx)<<1);y2=my+((y2-my)<<1);
	pSPlineIter(x1<<4,y1<<4,x2<<4,y2<<4,x3<<4,y3<<4);
	}
	void pSegmento(int x1,int y1,int x2,int y2);
	void drawPoli(void);	
	void drawLines(void);// lineas con grosor
// shape (multicolor, multigrosor, linea
	void drawShape(void);
// privadas
	inline Segmento *BufferGrafico::allocS()
	{
	Segmento *s;
	if ((linact-seg)>=NB_SEGMENT_MAX) return NULL;
	s=linact++;
	return s;
	}
	Segmento *avanzaS(Segmento *curSegs,int y);
	Segmento *nuevoS(Segmento *curSegs,Segmento *newSegs);
	void Flinea(int y,int x1,int x2,int x3,int x4);
	void pSPlineIter(long x1,long y1,long x2,long y2,long x3,long y3);
// refrescar
	void redraw();

// para hacer triple buffer
	int sizeScreen();
	void toBuff(void *p);
	void fromBuff(void *p);

protected:
	void InitDisplayProps();// inializes internal stuctures
	static BGRColor* GetDIBits(HBITMAP hBitmap, SIZE& size, HBITMAP& hResultBitmap);// GetDIBits emulation (Windows CE does not support GetDIBits)
	WORD* CreateNativeBitmapFromBGRBits(int nWidth, int nHeight, const BGRColor* pDIBits);// Creates a native bitmap from DIB bits
	BOOL InternalBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, int nSrcX, int nSrcY, DWORD dwTransparentNativeColor);
};

void superdraw(short x1,short y1,short x2,short y2,char *t);

#endif