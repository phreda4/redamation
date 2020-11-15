//
//
#include "stdafx.h"
#include "stgapi.h"

CSTGapiBuffer screen;

/////////////////////////////////////////////////////////////////////////////
// BGRColor structure encapsulates color information about one point. Color
// order is Blue, Green, Red (not RGB).
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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
CNativeBitmap* CSTGapiBuffer::CreateNativeBitmap(HBITMAP hBitmap)
{
if (hBitmap == NULL) return NULL;
CNativeBitmap* pBitmap = new CNativeBitmap(); // caller is responsible to delete
if (pBitmap == NULL) return NULL;
HBITMAP hResultBitmap;
BGRColor* pDIBits=GetDIBits(hBitmap, pBitmap->m_size, hResultBitmap);
pBitmap->m_pBits=CreateNativeBitmapFromBGRBits(pBitmap->m_size.cx, pBitmap->m_size.cy, pDIBits);
::DeleteObject(hResultBitmap);
return pBitmap;
}

BOOL CSTGapiBuffer::InternalBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, int nSrcX, int nSrcY, DWORD dwTranspNativeColor)
{
// check for incorrect parameters
if (Buffer==NULL || pNativeBitmap==NULL || pNativeBitmap->m_pBits==NULL || nWidth<=0 || nHeight<=0 || nSrcX<0 || nSrcY<0)
	return FALSE;
// check if the bitmap is large enough
if (nSrcX+nWidth>pNativeBitmap->GetBitmapSize().cx || nSrcY+nHeight>pNativeBitmap->GetBitmapSize().cy)
	return FALSE;
// check if there is nothing to draw
if (nDestX>(int) disp.cxWidth || nDestY>(int) disp.cyHeight)
	return TRUE;
// check boundaries and crop if necessary
if (nDestX < 0)
	{
	nWidth += nDestX;
	nSrcX -= nDestX;
	nDestX = 0;
	}
if (nDestY < 0)
	{
	nHeight += nDestY;
	nSrcY -= nDestY;
	nDestY = 0;
	}
if (nWidth > (int)disp.cxWidth - nDestX)
	{
	nWidth=disp.cxWidth - nDestX;
	}
if (nHeight>(int)disp.cyHeight - nDestY)
	{
	nHeight=disp.cyHeight - nDestY;
	}
// transfer bits
WORD* pdestline = Buffer;
pdestline += nDestX * XPitch + nDestY * YPitch;
WORD* psrcline = (WORD*) pNativeBitmap->m_pBits;
psrcline += nSrcX + nSrcY * pNativeBitmap->m_size.cx;
// transparent color should not decrease perfomance
// if it is not specified
if (dwTranspNativeColor == 0xffff)
	{
	for (int y=0;y<nHeight;y++)
		{
		WORD* pdest = pdestline;
		WORD* psrc = psrcline;
		for (int x=0;x<nWidth;x++)
			{
			*pdest = *psrc;
			pdest += XPitch;
			psrc++;
			}
		pdestline += YPitch;
		psrcline += pNativeBitmap->m_size.cx;
		}
	}
else
	{
	for (int y=0;y<nHeight;y++)
		{
		WORD* pdest = pdestline;
		WORD* psrc = psrcline;
		for (int x=0;x<nWidth;x++)
			{
			if (*psrc!=dwTranspNativeColor)
				{
				*pdest=*psrc;
				}
			pdest +=XPitch;
			psrc++;
			}
		pdestline +=YPitch;
		psrcline += pNativeBitmap->m_size.cx;
		}
	}
return true;
}


void CSTGapiBuffer::MaskedBlt(int nDestX, int nDestY, int nWidth, int nHeight, const CNativeBitmap* pNativeBitmap, const CNativeBitmap* pNativeMask, int nSrcX /*=0*/, int nSrcY /*=0*/)
{
// check for incorrect parameters
if (Buffer==NULL || pNativeBitmap==NULL || pNativeBitmap->m_pBits==NULL || nWidth<=0 || nHeight<=0 || nSrcX<0 || nSrcY<0)
	return;
// the mask should have the same size as the bitmap
if (pNativeBitmap->GetBitmapSize().cx != pNativeMask->GetBitmapSize().cx || pNativeBitmap->GetBitmapSize().cy != pNativeMask->GetBitmapSize().cy)
	return;
// check if the bitmap is large enough
if (nSrcX + nWidth > pNativeBitmap->GetBitmapSize().cx || nSrcY + nHeight > pNativeBitmap->GetBitmapSize().cy)
	return;
// check if there is nothing to draw
if (nDestX > (int)disp.cxWidth || nDestY > (int)disp.cyHeight)
	return;
// check boundaries and crop if necessary
if (nDestX < 0)
	{
	nWidth += nDestX;
	nSrcX -= nDestX;
	nDestX = 0;
	}
if (nDestY < 0)
	{
	nHeight += nDestY;
	nSrcY -= nDestY;
	nDestY = 0;
	}
if (nWidth > (int) disp.cxWidth - nDestX)
	{
	nWidth = disp.cxWidth - nDestX;
	}
if (nHeight > (int) disp.cyHeight - nDestY)
	{
	nHeight = disp.cyHeight - nDestY;
	}
// transfer bits
WORD* pDestLine =Buffer;
pDestLine += nDestX *XPitch + nDestY *YPitch;
WORD* pSrcLine = (WORD*) pNativeBitmap->m_pBits;
pSrcLine += nSrcX + nSrcY * pNativeBitmap->m_size.cx;
WORD* pMaskLine = (WORD*) pNativeMask->m_pBits;
pMaskLine += nSrcX + nSrcY * pNativeMask->m_size.cx;
for (int y = 0; y < nHeight; y++)
	{
	WORD* pDest = pDestLine;
	WORD* pSrc = pSrcLine;
	WORD* pMask = pMaskLine;
	for (int x = 0; x < nWidth; x++)
		{
		if (*pMask > 0)	*pDest = *pSrc;
		pDest +=XPitch;
		pSrc++;
		pMask++;
		}
	pDestLine +=YPitch;
	pSrcLine += pNativeBitmap->m_size.cx;
	pMaskLine += pNativeMask->m_size.cx;
	}
}


WORD* CSTGapiBuffer::CreateNativeBitmapFromBGRBits(int nWidth, int nHeight,const BGRColor *pDIBits)
{
if (pDIBits == NULL || nWidth < 1 || nHeight < 1)
	return NULL;
int nCorrectedWidth = ((nWidth + 3) / 4 ) * 4;
WORD* pNativeBuffer = new WORD[nWidth*nHeight];
WORD* pDest = pNativeBuffer;
for (int y = 0; y < nHeight; y++)
	{
	const BGRColor* pSrc = pDIBits + nCorrectedWidth*(nHeight-y-1);
	for (int x = 0; x < nWidth; x++)
		{
		*pDest = Get16bppColor(RGB(pSrc->m_R, pSrc->m_G, pSrc->m_B));
		pDest++;
		pSrc++;
		}
	}
return pNativeBuffer;
}


BGRColor* CSTGapiBuffer::GetDIBits(HBITMAP hBitmap, SIZE& size, HBITMAP& hResultBitmap)
{
// Getting bimap size
BITMAP bm;
GetObject(hBitmap, sizeof(BITMAP), &bm);
// Creating new bitmap and receive pointer to it's bits.
HBITMAP hTargetBitmap;
void *pBuffer;
int nCorrectedWidth = ((bm.bmWidth + 3) / 4 ) * 4;
// Initilize DIBINFO structure
DIBINFO  dibInfo;
dibInfo.bmiHeader.biBitCount = 24;
dibInfo.bmiHeader.biClrImportant = 0;
dibInfo.bmiHeader.biClrUsed = 0;
dibInfo.bmiHeader.biCompression = 0;
dibInfo.bmiHeader.biHeight = bm.bmHeight;
dibInfo.bmiHeader.biPlanes = 1;
dibInfo.bmiHeader.biSize = 40;
dibInfo.bmiHeader.biSizeImage = nCorrectedWidth*bm.bmHeight*3;
dibInfo.bmiHeader.biWidth = nCorrectedWidth;
dibInfo.bmiHeader.biXPelsPerMeter = 3780;
dibInfo.bmiHeader.biYPelsPerMeter = 3780;
dibInfo.bmiColors[0].rgbBlue = 0;
dibInfo.bmiColors[0].rgbGreen = 0;
dibInfo.bmiColors[0].rgbRed = 0;
dibInfo.bmiColors[0].rgbReserved = 0;
// Create bitmap and receive pointer to points into pBuffer
HDC hDC = ::GetDC(NULL);
ASSERT(hDC);
hTargetBitmap = ::CreateDIBSection(hDC,(const BITMAPINFO*)dibInfo,DIB_RGB_COLORS,(void**)&pBuffer,NULL,0);
::ReleaseDC(NULL, hDC);
// Copy source bitmap into the target bitmap.
// Create 2 device contexts 
HDC hMemDc = ::CreateCompatibleDC(NULL);
HDC hTargetDc = ::CreateCompatibleDC(NULL);
// Select source bitmap into one DC, target into another
HBITMAP hOldBitmap1 = (HBITMAP) ::SelectObject(hMemDc, hBitmap);
HBITMAP hOldBitmap2 = (HBITMAP) ::SelectObject(hTargetDc, hTargetBitmap);
// Copy source bitmap into the target one
::BitBlt(hTargetDc, 0, 0, bm.bmWidth, bm.bmHeight, hMemDc, 0, 0, SRCCOPY);
// Restore device contexts
::SelectObject(hMemDc, hOldBitmap1);
::SelectObject(hTargetDc, hOldBitmap2);
::DeleteDC(hMemDc);
::DeleteDC(hTargetDc);
	// Here we can bitmap bits: pBuffer. Note:
	// 1. pBuffer contains 3 bytes per point
	// 2. Lines ane from the bottom to the top!
	// 3. Points in the line are from the left to the right
	// 4. Bytes in one point are BGR (blue, green, red) not RGB
	// 5. pBuffer will be automatically deleted 
	//    when delete hTargetBitmap
hResultBitmap = hTargetBitmap;
size.cx = bm.bmWidth;
size.cy = bm.bmHeight;
return (BGRColor*) pBuffer;
}

void CSTGapiBuffer::InitDisplayProps()
{
disp=GXGetDisplayProperties();
XPitch =disp.cbxPitch >> 1;
YPitch =disp.cbyPitch >> 1;
if (XPitch < 0)
	BufferStartOffset =XPitch * (disp.cxWidth - 1);
else
	BufferEndOffset =XPitch * (disp.cxWidth - 1);
if (YPitch < 0)
	BufferStartOffset +=YPitch * (disp.cyHeight - 1);
else
	BufferEndOffset +=YPitch * (disp.cyHeight - 1);
if (disp.ffFormat | kfDirect565) 
	{ 
	RedD=11;GreenD=6;
	RED_MASK=0xF800;
	GREEN_MASK=0x07E0;
	BLUE_MASK=0x001F;
	}
else if (disp.ffFormat | kfDirect555) 
	{ 
	RedD=10;GreenD=5;
	RED_MASK=0xFC00;
	GREEN_MASK=0x03E0;
	BLUE_MASK=0x001F;
	}
}


BOOL CSTGapiBuffer::CreateMemoryBuffer()
{
if (Buffer != NULL || disp.cBPP != 16) return FALSE;
m_pMemoryBuffer = new WORD[BufferEndOffset - BufferStartOffset + 1];
if (m_pMemoryBuffer == NULL) return FALSE;
Buffer = (WORD*) m_pMemoryBuffer - BufferStartOffset;
return TRUE;
}


void CSTGapiBuffer::BitBlt(const CSTGapiBuffer *pMemoryBuffer)
{
//if (m_dispProps.cBPP != 16 || pMemoryBuffer->m_dispProps.cBPP != 16) return FALSE;
//if (!pMemoryBuffer->m_pMemoryBuffer) return FALSE;
memcpy(Buffer+BufferStartOffset,pMemoryBuffer->m_pMemoryBuffer,(BufferEndOffset-BufferStartOffset+1)*2);
}



