//Last update time : 06.12.2013 05:06:40

#include "tchar.h"
#include "stdio.h"

//#include <Windows.h>

#include "cfgFile.h"
#include "binFile.h"

#include "findCol.h"
#include "ColidPool.h"

#include <mpi.h>

#include "mympi_ext.h"

#define sabs(x) ((x)<0i16) ? -(x) : (x)

#ifndef MAKE8CC
#define MAKE8CC(a,b,c,d,e,f,g,h) ((__int64)h<<56) | ((__int64)g<<48) | ((__int64)f<<40) | ((__int64)e<<32) | ((__int64)d<<24) | ((__int64)c<<16) | ((__int64)b<<8) | ((__int64)a)
#endif

static bool bDeleteFile=true;
//static unsigned char* gBuffer[3]={NULL,NULL,NULL};	//global buffer
static unsigned char* gBuffer=NULL;
static bool MPI_MODE=false;
static char** cargv;

static wchar_t xpath[512]={0};
static wchar_t fullsymbol[512]={0};
static wchar_t workfullname[512]={0};
static wchar_t workname[512]={0};

//GLOBAL MPI ------------------
static time_t tstart;
static time_t tend;
static int omp_core=1;
static int mpi_rank=0;
static int mpi_size=1;
static int mpi_namelen;
//char mpi_processor_name[MPI_MAX_PROCESSOR_NAME]={0};
static char outprint[4096]={0};

static int* redindexCol=NULL;

#define TAG_STATUS 1000
#define COUNT_DISTRIBUTION_PORTION 1024
//-----------------------------

//TOOLS ---------------------------------------
//#define printx wprintf_s
void __fastcall printx(wchar_t* x, ...)
{
	//CharToOemW(x, outprint);
	va_list args;
    va_start(args, x);
    vwprintf(x, args);
    va_end(args);
	fflush( stdout );
}

bool __fastcall serverMPI(unsigned char* buffer, __int64 fmt, __int64 count, unsigned char size, short groups, unsigned __int64& er1, unsigned __int64& er2, unsigned __int64& er3)
{
	int status = 1;
	MPI_Request mpr[3];

	for(int i=1; i<mpi_size; ++i) MPI_Send(&status, 1, MPI_INT, i, TAG_STATUS, MPI_COMM_WORLD);
	for(int i=1; i<mpi_size; ++i)
	{
		MPI_Send(&fmt, 1, MPI_INT64_T, i, 10, MPI_COMM_WORLD);
		MPI_Send(&count, 1, MPI_INT64_T, i, 11, MPI_COMM_WORLD);
		MPI_Send(&size, 1, MPI_UNSIGNED_CHAR, i, 12, MPI_COMM_WORLD);
		MPI_Send(&groups, 1, MPI_SHORT, i, 13, MPI_COMM_WORLD);
	}
	for(int i=1; i<mpi_size; ++i) MPI_Send(buffer, (int)count * groups * size, MPI_BYTE, i, 14, MPI_COMM_WORLD);
	
	//----------------------------------------------------------
	MPI_Barrier(MPI_COMM_WORLD);

	//замер времени выполнения, для записи в статистику
	time(&tstart);

	//Ожидание для раздачи задач
	int label[2]={0};  //label[0]='status'    label[1]='state'

	int countworkend=mpi_size-1;
	__int64 indexinfo[2]={0,0}; //start index , endindex

	do
	{
		MPI_Status mst;
		MPI_Irecv(label, 2, MPI_INT, MPI_ANY_SOURCE, 112, MPI_COMM_WORLD, &mpr[0]); my_wait_status(&mpr[0],&mst);
		if(label[0]==0x880088aa) //клиент требует задачу
		{
			if(indexinfo[0] >= (count*groups))
			{
			 label[0]=0x880088bb; 
			 label[1]=1;			//завершаем клиента

			 MPI_Send(label, 2, MPI_INT, mst.MPI_SOURCE, 113, MPI_COMM_WORLD);
			 --countworkend;
			 if(countworkend==0) break;
			}
			else
			{
			 label[0]=0x880088bb; 
			 label[1]=0;			//даём клиенту задачу

			 //end offset
			 indexinfo[1] += COUNT_DISTRIBUTION_PORTION * groups;
			 if(indexinfo[1] > (count*groups))
			 {
				 indexinfo[1]=count*groups;
			 }

			 MPI_Send(label, 2, MPI_INT, mst.MPI_SOURCE, 113, MPI_COMM_WORLD);
			 MPI_Send(indexinfo, 2, MPI_INT64_T, mst.MPI_SOURCE, 114, MPI_COMM_WORLD);
			 
			 indexinfo[0] += COUNT_DISTRIBUTION_PORTION * groups; //start offset
			}
		}
	}while(true);
	//----------------------------------------------------------

	//wait workers
	int scnt=mpi_size-1;
	while(true)
	{
	 MPI_Irecv(&status, 1, MPI_INT, MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, &mpr[0]);		my_wait(&mpr[0]);
	 if(status == 0x10aa10aa) --scnt;
	 if(scnt == 0) break;
	}
	//-------------------------------------------------------------------------------
	unsigned __int64 ers[3]={0};
	unsigned __int64 rers[3]={0};
	if(redindexCol != NULL) { _aligned_free(redindexCol); redindexCol=NULL; }
	redindexCol = (int*)_aligned_malloc( count * groups * 4, 16 );
	int* indexCol = (int*)_aligned_malloc( count * groups * 4, 16 );

	memset(indexCol,0,count * groups * 4);
	memset(redindexCol,0,count * groups * 4);
	memset(root_countcalls,0,16 * sizeof(int));

	MPI_Reduce(indexCol, redindexCol, (int)count * groups, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(ers, rers, 3, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(count_calls, root_countcalls, 16, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	er1=rers[0];
	er2=rers[1];
	er3=rers[2];

	_aligned_free(indexCol);

	//замер времени выполнения, для записи в статистику
	time(&tend);

	bool bDeleteStatus=true;
	if(redindexCol != NULL)
	{
	 wchar_t resfilename[512];
	 memset(resfilename, 0, sizeof(resfilename));
	 wcscpy_s(resfilename, 512, xpath);
	 wcscat_s(resfilename, 512, L"Report1.txt");

	 FILE* htxt_result=NULL;
	 _wfopen_s(&htxt_result, resfilename, L"at, ccs=UTF-16LE");
	 if(htxt_result!=NULL)
	 {
		 int diffseconds=0;
		 tm ts,te;
		 localtime_s(&ts, &tstart);
		 localtime_s(&te, &tend);
		 diffseconds = (int)difftime(tend, tstart);
		 fwprintf_s(htxt_result, L"file(%s),fcsg(%0.16I64x,%I64i,%hhi,%hi) ", workname, fmt, count, size, groups);
		 fwprintf_s(htxt_result, L"start(day(%i) %i:%0.2i:%0.2i) end(day(%i) %i:%0.2i:%0.2i) ", ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec, te.tm_mday, te.tm_hour, te.tm_min, te.tm_sec);
		 fwprintf_s(htxt_result, L"elapsed(%i:%0.2i:%0.2i) ", diffseconds / 3600, (diffseconds/60) % 60, diffseconds % 60);		 
		 for(int i=1; i<mpi_size; ++i)
			 fwprintf_s(htxt_result, L" %S(%i) :", procnames + MPI_MAX_PROCESSOR_NAME * i, root_countcalls[i]);
		 fwprintf_s(htxt_result,L"\r\n");
		 
		 fwprintf_s(htxt_result,L"{\r\n");
		 if(groups>=1)
		 {
			getstat(buffer, count, groups, 0);
			fwprintf_s(htxt_result, L"\tср.арифм.= %lf\r\n", avg_ar);
			fwprintf_s(htxt_result, L"\tср.геом.= %lf\r\n", avg_ge);
			fwprintf_s(htxt_result, L"\tср.гарм.= %lf\r\n", avg_hr);
			fwprintf_s(htxt_result, L"\tср.квадр.= %lf\r\n", avg_sq);
			fwprintf_s(htxt_result, L"\tср.линейное.отклонение.= %lf\r\n", qlin);
			fwprintf_s(htxt_result, L"\tср.квадр.отклонение.= %lf\r\n", q);
			fwprintf_s(htxt_result, L"\tдисперсия.= %lf\r\n", qdisp);
			fwprintf_s(htxt_result, L"\tчастота бит=");
			for(int i=0; i<8; ++i) fwprintf_s(htxt_result,L" %I64u", bitbin[i]);
			fwprintf_s(htxt_result, L"\r\n\tчастота байт=");
			for(int i=0; i<256; ++i) fwprintf_s(htxt_result,L" %I64u", bytebin[i]);
			fwprintf_s(htxt_result, L"\r\n");
		 }
		 if(groups>=2)
		 {
			fwprintf_s(htxt_result,L"\t==================\r\n");
			getstat(buffer, count, groups, 1);
			fwprintf_s(htxt_result, L"\tср.арифм.= %lf\r\n", avg_ar);
			fwprintf_s(htxt_result, L"\tср.геом.= %lf\r\n", avg_ge);
			fwprintf_s(htxt_result, L"\tср.гарм.= %lf\r\n", avg_hr);
			fwprintf_s(htxt_result, L"\tср.квадр.= %lf\r\n", avg_sq);
			fwprintf_s(htxt_result, L"\tср.линейное.отклонение.= %lf\r\n", qlin);
			fwprintf_s(htxt_result, L"\tср.квадр.отклонение.= %lf\r\n", q);
			fwprintf_s(htxt_result, L"\tдисперсия.= %lf\r\n", qdisp);
			fwprintf_s(htxt_result, L"\tчастота бит=");
			for(int i=0; i<8; ++i) fwprintf_s(htxt_result,L" %I64u", bitbin[i]);
			fwprintf_s(htxt_result, L"\r\n\tчастота байт=");
			for(int i=0; i<256; ++i) fwprintf_s(htxt_result,L" %I64u", bytebin[i]);
			fwprintf_s(htxt_result, L"\r\n");
		 }
		 if(groups>=3)
		 {
			fwprintf_s(htxt_result,L"\t==================\r\n");
			getstat(buffer, count, groups, 2);
			fwprintf_s(htxt_result, L"\tср.арифм.= %lf\r\n", avg_ar);
			fwprintf_s(htxt_result, L"\tср.геом.= %lf\r\n", avg_ge);
			fwprintf_s(htxt_result, L"\tср.гарм.= %lf\r\n", avg_hr);
			fwprintf_s(htxt_result, L"\tср.квадр.= %lf\r\n", avg_sq);
			fwprintf_s(htxt_result, L"\tср.линейное.отклонение.= %lf\r\n", qlin);
			fwprintf_s(htxt_result, L"\tср.квадр.отклонение.= %lf\r\n", q);
			fwprintf_s(htxt_result, L"\tдисперсия.= %lf\r\n", qdisp);
			fwprintf_s(htxt_result, L"\tчастота бит=");
			for(int i=0; i<8; ++i) fwprintf_s(htxt_result,L" %I64u", bitbin[i]);
			fwprintf_s(htxt_result, L"\r\n\tчастота байт=");
			for(int i=0; i<256; ++i) fwprintf_s(htxt_result,L" %I64u", bytebin[i]);
			fwprintf_s(htxt_result, L"\r\n");
		 }
		 fwprintf_s(htxt_result,L"}\r\n");
	 }
	 
	 if(groups==3)
	 {
			for(int i=0; i<count*3;i+=3)
			{
				//if(redindexCol[i+0]!=0) ++er1;
				//if(redindexCol[i+1]!=0) ++er2;
				//if(redindexCol[i+2]!=0) ++er3;
				register int rr1=redindexCol[i+0]/3;
				register int rr2=redindexCol[i+1]/3;
				register int rr3=redindexCol[i+2]/3;
				if( (rr1 != rr2) || ( rr1 != rr3 ) )
				{
					printx(L"b(%hhi,%hhi,%hhi) i(%i) j(%i,%i,%i)\n", (rr1!=0), (rr2!=0) , (rr3!=0), i/3, rr1, rr2, rr3);
					if(htxt_result!=NULL) fwprintf_s(htxt_result, L"\tb(%hhi,%hhi,%hhi) i(%i) j(%i,%i,%i)\r\n", (rr1!=0), (rr2!=0) , (rr3!=0), i/3, rr1, rr2, rr3);
					bDeleteStatus=false;
				}
			}
			_aligned_free(redindexCol);
			redindexCol=NULL;
	 }
	 else if(groups==2)
	 {
			for(int i=0; i<count*2;i+=2)
			{
				//if(redindexCol[i+0]!=0) ++er1;
				//if(redindexCol[i+1]!=0) ++er2;
				//if(redindexCol[i+2]!=0) ++er3;
				register int rr1=redindexCol[i+0]/2;
				register int rr2=redindexCol[i+1]/2;
				if( (rr1 != rr2) )
				{
					printx(L"b(%hhi,%hhi) i(%i) j(%i,%i)\n", (rr1!=0), (rr2!=0) , i/2, rr1, rr2);
					if(htxt_result!=NULL) fwprintf_s(htxt_result, L"\tb(%hhi,%hhi) i(%i) j(%i,%i)\n", (rr1!=0), (rr2!=0) , i/2, rr1, rr2);
					bDeleteStatus=false;
				}
				//for debug
				//if( ((rr1!=0) || (rr2!=0)) && (htxt_result!=NULL)) fwprintf_s(htxt_result, L"\tb(%hhi,%hhi) i(%i) j(%i,%i)\n", (rr1!=0), (rr2!=0) , i/2, rr1, rr2);
			}
			_aligned_free(redindexCol);
			redindexCol=NULL;
	 }
	 if(htxt_result!=NULL) { fclose(htxt_result); htxt_result=NULL; }
	}

	return bDeleteStatus;
}

void clientMPI()
{
	//MPI_Status s;

		/*MPI_Status s;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
		int xcount;
		MPI_Get_count(&s, MPI_CHAR, &xcount);

		char *buf = new char[count];
		MPI_Recv(buf, count, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);*/
	__int64 fmt=0;
	__int64 count=0;
	unsigned char size=16;
	short groups=0;
	int status=0;

	do
	{
	 MPI_Request mpr[4];
	 
	 MPI_Irecv(&status, 1, MPI_INT, 0, TAG_STATUS, MPI_COMM_WORLD, &mpr[0]);
	 my_wait(&mpr[0]);

	 if(status == 0x77007700) break;

	 MPI_Irecv(&fmt, 1, MPI_INT64_T, 0, 10, MPI_COMM_WORLD, &mpr[0]);
	 MPI_Irecv(&count, 1, MPI_INT64_T, 0, 11, MPI_COMM_WORLD, &mpr[1]);
	 MPI_Irecv(&size, 1, MPI_UNSIGNED_CHAR, 0, 12, MPI_COMM_WORLD, &mpr[2]);
	 MPI_Irecv(&groups, 1, MPI_SHORT, 0, 13, MPI_COMM_WORLD, &mpr[3]);
	 my_wait_all(4,mpr);

	 //printx(L"client count(%I64i) groups(%hhi)\n", count, groups);
	 unsigned char* buf = (unsigned char*)_aligned_malloc( count * groups * size, 16 );
	 if(buf==NULL) { printx(L"буффер не был выделен\n"); MPI_Abort(MPI_COMM_WORLD,1111111111); return; }
	 
	  MPI_Irecv(buf, (int)count * groups * size, MPI_BYTE, 0, 14, MPI_COMM_WORLD, &mpr[0]); 	  my_wait(&mpr[0]);
	  
	  //WORK UNIT-------------------------------------------------------------------
	  int* indexCol = (int*)_aligned_malloc( count * groups * 4, 16 );
	  memset(indexCol, 0, count * groups * 4);

	  unsigned __int64 ers[3]={0};
	  //xbeginfc_cluster(buf, count, 16, groups, ers[0],ers[1],ers[2], mpi_rank, mpi_size, indexCol);

	  //req -----------------------------------
	  count_calls[mpi_rank]=0; //int countcalls=0;
	  MPI_Barrier(MPI_COMM_WORLD);
	  int label[2]={0};
	  do
	  {
		  label[0]=0x880088aa; //запрос на задачу у сервера
		  MPI_Send(label, 2, MPI_INT, 0, 112, MPI_COMM_WORLD);
		  MPI_Irecv(label, 2, MPI_INT, 0, 113, MPI_COMM_WORLD, &mpr[0]);	my_wait(&mpr[0]);
		  if(label[0] == 0x880088bb) //сервер отослал задачу
		  {
			  if(label[1]==1) //сервер отправил сообщение об окончании задач
			  {
				  break;
			  }
			  __int64 indexinfo[2];
			  MPI_Irecv(indexinfo, 2, MPI_INT64_T, 0, 114, MPI_COMM_WORLD, &mpr[0]);	my_wait(&mpr[0]);
			  xbeginfc_chunk_cluster(buf, fmt, count, 16, groups, ers[0], ers[1], ers[2], indexCol, indexinfo[0], indexinfo[1]);
			  ++count_calls[mpi_rank]; //++countcalls;
		  }
	  }while(true);
	  //---------------------------------------

	  //send workend
	  status=0x10aa10aa;
	  MPI_Send(&status, 1, MPI_INT, 0, 111, MPI_COMM_WORLD);
	  //-----------------------------------------------------

	  MPI_Reduce(indexCol, NULL, (int)count * groups, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	  MPI_Reduce(ers, NULL, 3, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
	  MPI_Reduce(count_calls, NULL, 16, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); //MPI_Gather(&countcalls, 1, MPI_INT, NULL, 1, MPI_INT, 0, MPI_COMM_WORLD);

	  _aligned_free(indexCol);
	  //----------------------------------------------------------------------------
	  _aligned_free(buf);
	}while(true);
}

int _tmain(int argc, _TCHAR* argv[])
{
	_wsetlocale(LC_ALL, L"RUS");
	SetPriorityClass( GetCurrentProcess(),  BELOW_NORMAL_PRIORITY_CLASS );

	//check mpi -------------------------------------------------------------------------------
	for(int i=1; i<argc; ++i) if( wcsstr(argv[i], L"-mpi" ) != NULL) { MPI_MODE=true; break; }
	//-----------------------------------------------------------------------------------------

	memset(xpath,0,sizeof(xpath));
	memset(fullsymbol,0,sizeof(fullsymbol));

	wchar_t* wp = wcsrchr(argv[0],L'\\');
	wcsncpy_s(xpath, 512, argv[0], (wp-argv[0])+1);
	wcscpy_s(fullsymbol, 512, xpath);
	wcscat_s(fullsymbol, 512, L"*.?bin");

	//wchar_t nbuf[MAX_PATH];
	//GetCurrentDirectoryW(MAX_PATH, nbuf);
	//wprintf_s(L"%s\n", nbuf);
	SetCurrentDirectoryW(xpath);
	//GetCurrentDirectoryW(MAX_PATH, nbuf);
	//wprintf_s(L"%s\n", nbuf);

	if( MPI_MODE )
	{
		cargv = (char**)_aligned_malloc( argc * sizeof(void**), 16 );
		for(int i=0; i<argc; ++i)
		{
			size_t cchar=0;
			char ptb[128];
			wcstombs_s( &cchar, ptb, argv[i], 128 );
			cargv[i] = (char*)_aligned_malloc( cchar, 16 );
			memset(cargv[i], 0, cchar);
			strcpy_s(cargv[i], cchar, ptb);
		}
		
		MPI_Init(&argc, &cargv);
		MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
		MPI_Get_processor_name(mpi_processor_name, &mpi_namelen);

		omp_core = omp_get_max_threads();

		send_recv_ServerConfiguration(mpi_rank, mpi_size, omp_core, mpi_processor_name);

		printx(L"MPI режим инициирован на (%s) ранг %i/%i ядер(%i)\n", mpi_processor_name, mpi_rank, mpi_size, omp_core);

		//Серверу приоритет процесса - выше среднего
		if( mpi_rank == 0 ) SetPriorityClass( GetCurrentProcess(),  ABOVE_NORMAL_PRIORITY_CLASS );
	}

	binFile Fbin;

	if(mpi_rank == 0)
	{
		//findfiles------------------------------------------------
		//cfgFile cfg;
		//cfg.openConfig(xpath);
		
		_wfinddata64_t fd;
		intptr_t iptr = _wfindfirsti64(fullsymbol, &fd);
		
	if( (iptr != 2) && (iptr != 22) && (iptr != -1) )
	{
		do
		{
		  memset(workfullname,0,sizeof(workfullname));
		  wcscpy_s(workfullname, 256, xpath);
		  wcscat_s(workfullname, 256, fd.name);

		  memset(workname,0,sizeof(workname));
		  wcscpy_s(workname, 512, fd.name);

		  if( Fbin.openFile( workfullname ) != 0 ) { printx(L"Файл \"%s\" не был открыт!!!\n", fd.name); continue; }
		  else printx(L"Проверка файла: \"%s\" (%I64i KB)\n", fd.name, fd.size >> 10);
		//--------------------------------------------------------------------------------------------------

		int recs = Fbin.checkRecords();
		printx(L"Всего записей в файле = %i\n", recs);

		clock_t clk=clock();
		const int maxfmt=2;
		__int64 fmtfind[maxfmt] /* HIDE */;
		unsigned char sizes[maxfmt]={16, 32};
		long long cntcol[maxfmt]={0i64};

		for(int i=0; i<maxfmt; ++i)
		{
			cntcol[i] = Fbin.GetCollide( fmtfind[i], sizes[i] );
			char nname[16];
			memset(nname, 0, 16);
			memcpy(nname, &fmtfind[i], 8);
			printx(L"\tfmt(%S) = errors(%I64i)\n", nname, cntcol[i]);
		}
		clk=clock()-clk;
		Fbin.closeFile();

		for(int i=1; i<maxfmt; ++i)	if(cntcol[0]!=cntcol[i]) bDeleteFile=false;

		 if( bDeleteFile )
			printx(L"Файл удалён = %i\tВремя=%i\n", _wunlink( workfullname ), clk);
		 else
			 bDeleteFile = true;
		} while( _wfindnexti64(iptr, &fd) == 0 );

		_findclose(iptr);
		}

		//Завершение клиентов
		//if(MPI_MODE) { int status=0x77007700; for(int i=1; i<mpi_size; ++i) MPI_Send(&status,1, MPI_INT, i, TAG_STATUS, MPI_COMM_WORLD); }
		//--------------------------------------------
	}

	if(MPI_MODE && (mpi_rank != 0))
		clientMPI();

	if( MPI_MODE )
	{
	//	MPI_Finalize();
		for(int i=0; i<argc; ++i)
		{
			if(cargv[i]!=NULL)
			{
				_aligned_free(cargv[i]);
				cargv[i]=NULL;
			}
		}
		_aligned_free(cargv);
	}
	return 0;
}