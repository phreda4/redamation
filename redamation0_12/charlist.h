#ifndef CHARLIST_H
#define CHARLIST_H

// manejo de listas
int ListLenC(char *e,char s);
inline int ListLen(char *e) { return ListLenC(e,','); };

char *ListGetAtC(char *e,int n,char s);
inline char *ListGetAt(char *e,int n) { return ListGetAtC(e,n,','); };

void ListGetCopyC(char *e,int n,char *t,char s);
void ListGetConcatC(char *e,int n,char *t,char s);
inline void ListGetCopy(char *e,int n,char *t) { ListGetCopyC(e,n,t,','); };

int ListInd(char *e,char *v,char s);

int loadfile(char *n,char *f);// cargar archivo n en f

// manejo de archivos
void fileAttr(char *n,char *s,char *t);
bool existFile(char *n);
int getCntFiles(char *path);
void getFiles(char *path,char *b);
void getTFiles(char *path,char *b,char *c);
void getNFiles(char *path,int c,int n,char *b);// cantidad por grupo, grupo
int getCntDir(char *path);
void getNDir(char *path,int c,int n,char *b);// cantidad por grupo, grupo

// manejo de fecha y hora
void copydatenow(char *n);
void copytimenow(char *n);
#endif