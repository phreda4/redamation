#ifndef POLI_H
#define POLI_H

void clearPoli(unsigned short x1,unsigned short y1,unsigned short w,unsigned short h);

void pSolidColor(WORD color);

void pSegmento(int x1,int y1,int x2,int y2);
void pSPline(int x1,int y1,int x2,int y2,int x3,int y3);

void drawPoli();
void drawLine();

#endif
