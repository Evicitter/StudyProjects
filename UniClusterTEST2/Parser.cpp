//Last update time : 24.09.2013 02:54:10

//#include "stdafx.h"

#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//#include <iostream>

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

FILE* hWrite=NULL;

bool my_comparison( string p1, string p2)
{
 transform (p1.begin(),p1.end(), p1.begin(),toupper);
 transform (p2.begin(),p2.end(), p2.begin(),toupper);

 return p1 < p2;
}

void parseFile(const char* fname)
{
	clock_t clk = clock();

	if(hWrite==NULL)
	{
		if(fopen_s(&hWrite, "output.txt", "wt")!=0) { printf_s("File output.txt can't created\n"); return; }
		setvbuf(hWrite,NULL, _IOFBF, 32768);
	}

	//for out to cout
	printf_s("%s", fname);
	fprintf_s(hWrite,"%s", fname);
	//-------------------------------------

	FILE* ffile=NULL;
	if( fopen_s(&ffile, fname, "rt") != 0) { printf_s("file %s can't open for reading", fname); return; }
	setvbuf(ffile,NULL, _IOFBF, 8 << 20); //Выделяем 8МБ для кэш буффера файла
	
	const size_t maxchars = 256 << 10;
	char* cbuffer = (char*)_aligned_malloc(maxchars, 16);
	char* obuffer = (char*)_aligned_malloc(maxchars, 16);

	memset(cbuffer,0,maxchars);
	memset(obuffer,0,maxchars);

	vector<string> xstring;

	while(!feof(ffile))
	{
		fgets(cbuffer,maxchars,ffile);

		if(strncmp(cbuffer, "PUBLIC", 6) == 0)
		{
			register const char* find = strchr(cbuffer+6, '?');
			if(find!=NULL)
			{
				if( !isalpha( *(find+1) ) ) continue;

				register const char* bendvar = strchr(find+1, '@');
				if(bendvar!=NULL)
				{
					//проверить ещё один символ "собака"
					if( *(bendvar+1) != '@' ) continue;
					if( *(bendvar+2) != '3' ) continue;

					 strncpy_s(obuffer, maxchars, find+1, (bendvar-1-find)*sizeof(char));
					 //printf_s(" %s", obuffer);
					 //fprintf_s(hWrite," %s", obuffer);
					 //sprintf_s(obuffer, maxchars, "%s", );
					 xstring.push_back( string(obuffer) );
				}
			}
		}

		//Non safe clear buffer's
		*(unsigned int*)cbuffer = 0;
		*(unsigned int*)obuffer = 0;
	}

	//for(int i=0;
	std::stable_sort(xstring.begin(),xstring.end(), my_comparison);
	for(int i=0; i<xstring.size(); ++i)
	{
		printf_s(" %s", xstring[i].c_str());
		fprintf_s(hWrite," %s", xstring[i].c_str());
		xstring[i].clear();
	}
	xstring.clear();

	_aligned_free(obuffer);
	_aligned_free(cbuffer);
	fclose(ffile);

	clk = clock() - clk;

	printf_s("\t(%i mls)\n", clk);
	fprintf_s(hWrite, "\n");
}

#define has_null_byte(x) (((x) - 0x01010101) & ~(x) & 0x80808080)
#define has_null_byte2(x) (((x) - 0x0101010101010101ui64) & ~(x) & 0x8080808080808080ui64)

int _tmain(int argc, _TCHAR* argv[])
{
	printf_s("-------------- start parser ----------------\n");


	unsigned long long x;
	wscanf_s(L"%I64i", &x);
	x = has_null_byte2(x);
	wprintf_s(L"\nx = %0.4I64x\n", x);

	_wsystem(L"PAUSE");
	return 0;


	_finddata_t fd;
	intptr_t hFile;
	char fullname[256];

	hFile = _findfirst("asms\\*.asm", &fd);

	//int counter=0;
	if(hFile != -1)
	{
		do
		{
			strcpy_s(fullname, "asms\\");
			strcat_s(fullname, fd.name);
			parseFile( fullname );
			//if(counter++ >= 10) break;

		} while(_findnext(hFile, &fd) == 0);
	}
	_findclose(hFile);

	if(hWrite!=NULL) fclose(hWrite);

	getchar();

	return 0;
}
