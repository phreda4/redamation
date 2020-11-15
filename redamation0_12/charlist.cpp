#include "stdafx.h"
#include "charlist.h"
#include <malloc.h>

int ListLenC(char *e,char s)
{
if (*e==0) return 0;
int cnt=1;
while (*e!=0) {
	if (*e==s) cnt++;
	e++;
	}
return cnt;
}

char *ListGetAtC(char *e,int n,char s)
{
int cnt=0;
if (n==0) return e;
while (*e!=0) {
	if (*e==s) 
		{
		cnt++;
		if (n==cnt) return (e+1);
		}
	e++;
	}
return NULL;
}

void ListGetCopyC(char *e,int n,char *t,char s)
{
char *p=ListGetAtC(e,n,s);
if (p==NULL) {
	*t=0;return;
	}	
while (*p!=0 && *p!=s) {
	*t=*p;
	t++;p++;
	}
*t=0;
}

void ListGetConcatC(char *e,int n,char *t,char s)
{
char *p=ListGetAtC(e,n,s);
if (p==NULL) return;
char *ta=&t[strlen(t)];
while (*p!=0 && *p!=s) {
	*ta=*p;
	ta++;p++;
	}
*ta=0;

}

int ListInd(char *e,char *v,char s)
{
int cnt=0;
char *p=strstr(e,v);
if (p==NULL) return -1;
while (p!=e) {
	if (*p==s) cnt++;
	p--;
	};
return cnt;
}
// optimizadas
/*
char _separator;
char *_puntero;
int _cantidad;

void ListFirst(char *e,char *t,char s)
{
_separator=s;
_puntero=e;
_cantidad=0;
if (*_puntero==0) {
	*t=0;
	_cantidad=-1;
	return;
	}
ListNext(t);
}

void ListNext(char *t)
{
while (*_puntero!=0 && *_puntero!=_separator) {
	*t=*_puntero;
	t++;_puntero++;
	}
*t=0;
_cantidad++;
}
*/
// manejo de archivos
int getCntFiles(char *path)
{
WIN32_FIND_DATA FileData;   // Data structure describes the file found
HANDLE hSearch;             // Search handle returned by FindFirstFile
BOOL bFinished = FALSE;
TCHAR szPath[255];
wsprintf(szPath,_T("%hs"),path);// de char a charw
hSearch=FindFirstFile(szPath,&FileData);
int cntarchivos=0;
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	while (!bFinished)
		{
//		sprintf(n,"%lS",FileData.cFileName);
		if (FileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY) cntarchivos++;
	    if (!FindNextFile(hSearch, &FileData)) bFinished = TRUE;
		}
	FindClose (hSearch);
	}
return cntarchivos;
}

bool existFile(char *n)
{
TCHAR szPath[255];
WIN32_FIND_DATA FileData;
HANDLE hSearch;
wsprintf(szPath,_T("%hs"),n);
hSearch=FindFirstFile(szPath,&FileData);
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	FindClose(hSearch);
	return true;
	}
return false;
}

void fileAttr(char *n,char *s,char *t)
{
TCHAR szPath[255];
WIN32_FIND_DATA FileData;
SYSTEMTIME lpSystemTime;
HANDLE hSearch;
wsprintf(szPath,_T("%hs"),n);
hSearch=FindFirstFile(szPath,&FileData);
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	long tam=(FileData.nFileSizeHigh*MAXDWORD)+FileData.nFileSizeLow;
	if (tam<1024)	sprintf(s,"%i B",tam);
	else if (tam<1024*1024) sprintf(s,"%i K",tam/1024);
	else sprintf(s,"%i M",tam/(1024*1024));
	if (FileTimeToSystemTime(&FileData.ftCreationTime,&lpSystemTime)) 
		sprintf(t,"%i/%i/%i",lpSystemTime.wDay,lpSystemTime.wMonth,lpSystemTime.wYear);
	else
		*t=0;
	FindClose(hSearch);
	}
}

void getNFiles(char *path,int c,int n,char *b)
{
TCHAR szPath[255];
char name[255];
WIN32_FIND_DATA FileData;   // Data structure describes the file found
HANDLE hSearch;             // Search handle returned by FindFirstFile
BOOL bFinished = FALSE;
wsprintf(szPath,_T("%hs"),path);// de char a charw
hSearch=FindFirstFile(szPath,&FileData);
int nroarchivos=0;
//*b=0;
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	while (!bFinished)
		{
		if (FileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY) 
			{
			if (nroarchivos>=c*n && nroarchivos<c*(n+1))
				{ 
				sprintf(name,"%lS",FileData.cFileName);
				strcat(b,name);strcat(b,"|"); 
				}
			nroarchivos++;
			}
	    if (!FindNextFile(hSearch,&FileData)) bFinished = TRUE;
		}
	FindClose(hSearch);
	}
if (strcmp(b,"")) b[strlen(b)-1]=0;
}

void getFiles(char *path,char *b)
{
TCHAR szPath[255];
char name[255];
WIN32_FIND_DATA FileData;   // Data structure describes the file found
HANDLE hSearch;             // Search handle returned by FindFirstFile
BOOL bFinished = FALSE;
wsprintf(szPath,_T("%hs"),path);// de char a charw
hSearch=FindFirstFile(szPath,&FileData);
int nroarchivos=0;
*b=0;
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	while (!bFinished)
		{
		if (FileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY) 
			{
			sprintf(name,"%lS",FileData.cFileName);
			strcat(b,name);strcat(b,"|"); 
			nroarchivos++;
			}
	    if (!FindNextFile(hSearch,&FileData)) bFinished = TRUE;
		}
	FindClose(hSearch);
	}
if (strcmp(b,"")) b[strlen(b)-1]=0;
}

void getTFiles(char *path,char *b,char *c)
{
TCHAR szPath[255];
char name[255];
WIN32_FIND_DATA FileData;   // Data structure describes the file found
HANDLE hSearch;             // Search handle returned by FindFirstFile
BOOL bFinished = FALSE;
wsprintf(szPath,_T("%hs"),path);// de char a charw
hSearch=FindFirstFile(szPath,&FileData);
*b=0;
*c=0;
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	while (!bFinished)
		{
		if (FileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY) 
			{
			sprintf(name,"\\%lS",FileData.cFileName);
			strcat(c,name);strcat(c,"|"); 
			}
		else if (FileData.dwFileAttributes==FILE_ATTRIBUTE_ARCHIVE) 
			{
			sprintf(name,"%lS",FileData.cFileName);
			strcat(b,name);strcat(b,"|"); 
			}
	    if (!FindNextFile(hSearch,&FileData)) bFinished = TRUE;
		}
	FindClose(hSearch);
	}
//\\Storage Card
if (strcmp(b,"")) b[strlen(b)-1]=0;
if (strcmp(c,"")) c[strlen(c)-1]=0;
}

int getCntDir(char *path)
{
WIN32_FIND_DATA FileData;   // Data structure describes the file found
HANDLE hSearch;             // Search handle returned by FindFirstFile
BOOL bFinished = FALSE;
TCHAR szPath[255];
wsprintf(szPath,_T("%hs"),path);// de char a charw
hSearch=FindFirstFile(szPath,&FileData);
int cntarchivos=0;
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	while (!bFinished)
		{
		if (FileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY) cntarchivos++;
	    if (!FindNextFile(hSearch,&FileData)) bFinished=TRUE;
		}
	FindClose(hSearch);
	}
return cntarchivos;
}

void getNDir(char *path,int c,int n,char *b)
{
TCHAR szPath[255];
char name[255];
WIN32_FIND_DATA FileData;   // Data structure describes the file found
HANDLE hSearch;             // Search handle returned by FindFirstFile
BOOL bFinished = FALSE;
wsprintf(szPath,_T("%hs"),path);// de char a charw
hSearch=FindFirstFile(szPath,&FileData);
int nroarchivos=0;
//*b=0;
if (hSearch!=INVALID_HANDLE_VALUE)
	{
	while (!bFinished)
		{
		if (FileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY) 
			{
			if (nroarchivos>=c*n && nroarchivos<c*(n+1))
				{
				sprintf(name,"%lS",FileData.cFileName);
				strcat(b,name);strcat(b,"|");
				}
			nroarchivos++;
			}
	    if (!FindNextFile(hSearch,&FileData)) bFinished = TRUE;
		}
	FindClose(hSearch);
	}
if (strcmp(b,"")) b[strlen(b)-1]=0;
}

int loadfile(char *n,char *f)// cargar archivo n en f
{
FILE *stream;
int numread;
// falta saber si esta y tamanio
if((stream=fopen(n,"r"))==NULL) return -1;
numread=fread(f,sizeof(char),8192,stream);
f[numread]=0;
if(fclose(stream)) return -1;
return numread;
}

void copydatenow(char *n)
{
SYSTEMTIME st;
GetLocalTime(&st);
sprintf(n,"%d/%02d/%4d",st.wDay,st.wMonth,st.wYear);
}

void copytimenow(char *n)
{
SYSTEMTIME st;
GetLocalTime(&st);
sprintf(n,"%2d:%02d",st.wHour,st.wMinute);
}

void FindingStorageCards (void)
{
TCHAR szMsg[100];       // String to store the error message
HANDLE hSearch;         // Search handle returned by FindFirstFile
WIN32_FIND_DATA  fd;    // Data structure describes the file found
BOOL bFinished = FALSE; // Flag to indicate whether the loop is done
TCHAR *szFname = TEXT("\\Storage Card*");
                          // Name that matches all storage cards

  // Ensure that Storage Card exists.
hSearch = FindFirstFile(szFname, &fd);
if (hSearch == INVALID_HANDLE_VALUE)
  {
  wsprintf(szMsg, TEXT("No storage card found."));
  return;
  }
do {
    // Test whether the file is really on a storage card, and
    // not just a directory in the root directory.
    // It must have both the directory attribute and
    // the temporary attribute.

    if (  (fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
       && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
    {
      wsprintf (szMsg,
                TEXT("%s is a storage card."), fd.cFileName);
    }
    else
    {
      wsprintf (szMsg,
                TEXT("%s is not a storage card."), fd.cFileName);
    }

    if (!FindNextFile (hSearch, &fd))
    {
      bFinished = TRUE;
      if (GetLastError () != ERROR_NO_MORE_FILES)
      {
        wsprintf (szMsg,
                  TEXT("Error trying to find files matching \"%s\"."),
                  szFname);
      }
    }
  }
  while (!bFinished);

  FindClose (hSearch);  // Close the search handle.

} // End of FindingStorageCards example code

