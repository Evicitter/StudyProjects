//Last update time : 22.02.2012 00:01:12

//for WINDOWS
//C:\Program Files\MPICH2\bin\mpiexec -hosts 3 Corei5 2 AcerN SAMSUNGPC Z:\mpi\tfilterIND.exe -mpi

#include <stdbool.h>
#include <mpi.h>
#include <omp.h>
//#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>

//specs ------------------------------
#include <mm_malloc.h>
#include <emmintrin.h>
//------------------------------------

//compileflag = -fopenmp
//compileflag = -fomp

//commands ----------------
// mpicc -O3 -ffast-math -mfpmath=sse -std=gnu99 -fopenmp -msse3 -D_LARGEFILE64_SOURCE tfilterIND.c -L. -lgomp -Wl,-rpath,.
// mpicc -O3 -ffast-math -mfpmath=sse -std=gnu99 -fopenmp -msse3 -D_LARGEFILE64_SOURCE tfilterIND.c
//mpiexec -n 4 --host tusur-master,node1,node2,node3 ./a.out _33pin_scv.res
//mpiexec -n 6 --host tusur-master,node1,node2,node3,node4,node5 ./a.out _33pin_scv.res
//objdump -d -M intel-mnemonic
//-----------------------------------------------------------

//global variables --------------------------------------
static double FDelElapsedsecStart;
static double FDelElapsedsecEnd;
static double FValidElapsedsecStart;
static double FValidElapsedsecEnd;
static unsigned long long FDelElapsedsec;
static unsigned long long FDelElapsedmin;
static unsigned long long FDelElapsedhour;
static unsigned long long FValidElapsedsec;
static unsigned long long FValidElapsedmin;
static unsigned long long FValidElapsedhour;

#define prefdel FDelElapsedsecStart = omp_get_wtick()
#define postdel 				{	FDelElapsedsecEnd = omp_get_wtick(); \
									FDelElapsedsec = (unsigned long long)(FDelElapsedsecStart - FDelElapsedsecEnd); \
									FDelElapsedhour = FDelElapsedsec / 3600; \
									FDelElapsedmin = (FDelElapsedsec / 60) % 60; \
									FDelElapsedsec = FDelElapsedsec % 60; }

#define prefval FValidElapsedsecStart = omp_get_wtick()
#define postval 				{	FValidElapsedsecEnd = omp_get_wtick(); \
									FValidElapsedsec = (unsigned long long)(FDelElapsedsecEnd - FValidElapsedsecStart); \
									FValidElapsedhour = FValidElapsedsec / 3600; \
									FValidElapsedmin = (FValidElapsedsec / 60) % 60; \
									FValidElapsedsec = FValidElapsedsec % 60; }
//-------------------------------------------------------------------------------------------------------

//redirect intrinsics ------------------------------------------------------------------------------------------------------------------------
static __inline__ __attribute__((always_inline)) void __stosb(unsigned char *dest, unsigned char data, size_t count)
{
	   __asm__ __volatile__ (
	    "cld ; rep ; stosb "
	    : "=D" (dest), "=c" (count)
	    : "a"(data), "0"(dest), "1" (count)
	  );
}
//--------------------------------------------------------------------------------------------------------------------------------------------

//mpicc -fopenmp
//gcc -fomp

static size_t A32deleteZero( void* ar, size_t size, char* bmap, size_t bmcount )
{
prefdel;
	int* mtest = (int*)ar;
	size_t offset=0;
	for( size_t i=0; i<size; ++i)
	{
		if( bmap[i + offset] )
		{
			++offset;
			--i;
			--size;
			continue;
		}
		mtest[i] = mtest[i+offset];
	}
postdel;
	return size;
}

static size_t A64deleteZero( void* ar, size_t size, char* bmap, size_t bmcount )
{
prefdel;
	long long* mtest = (long long*)ar;
	size_t offset=0;
	for(size_t i=0; i<size; ++i)
	{
		if( bmap[i + offset] )
		{
			++offset;
			--i;
			--size;
			continue;
		}
		mtest[i] = mtest[i+offset];
	}
postdel;
	return size;
}

static size_t A128deleteZero( void* ar, size_t size, char* bmap, size_t bmcount )
{
prefdel;
	__m128i* mtest = (__m128i*)ar;
	size_t offset=0;
	for(size_t i=0; i<size; ++i)
	{
		if( bmap[i + offset] )
		{
			++offset;
			--i;
			--size;
			continue;
		}
		mtest[i] = mtest[i+offset];
	}
postdel;
	return size;
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

static void get_validate_map32( int* rHash, size_t startpos, size_t maxcount, char* bm, int mthreads, int mprocs, int crank )
{
prefval;
	#pragma omp parallel for schedule(dynamic,16)
	for(int i=crank*8; i<(int)(maxcount); i += mprocs*8)
	//for(int i=(int)startpos; i<(int)(maxcount-7); i+=8)
	{
		for(int p=0; p<8; ++p) 
			for(int t=p+1; t<8; ++t)
			{
				if(rHash[i+p] == rHash[i+t]) { bm[i+p]=1; bm[i+t]=1;  }
			}

			register __m128i HH1 = _mm_load_si128( (__m128i*)(rHash + i) );
			register __m128i HH2 = _mm_load_si128( (__m128i*)(rHash + i + 4) );

			//register int mm_j;
			register __m128i mm_j;
			for( int j=i+8; j<maxcount; ++j )
			{
				//mm_j = rHash[j];
				mm_j = _mm_set1_epi32( rHash[j] );

#define setx32(y) *(int*)(bm + i) |= (y)
				switch( _mm_movemask_ps( _mm_castsi128_ps( _mm_cmpeq_epi32(mm_j,HH1) ) ) )
				{
				case 0: break;
				case 1: setx32(0x00000001);		bm[j]=1; 				break;
				case 2: setx32(0x00000100);		bm[j]=1;				break;
				case 3: setx32(0x00000101);		bm[j]=1;				break;
				case 4: setx32(0x00010000);		bm[j]=1;				break;
				case 5:	setx32(0x00010001);		bm[j]=1;				break;
				case 6: setx32(0x00010100);		bm[j]=1;				break;
				case 7: setx32(0x00010101);		bm[j]=1;				break;
				case 8: setx32(0x01000000);		bm[j]=1;				break;
				case 9: setx32(0x01000001);		bm[j]=1;				break;
				case 10: setx32(0x01000100);	bm[j]=1;				break;
				case 11: setx32(0x01000101);	bm[j]=1;				break;
				case 12: setx32(0x01010000);	bm[j]=1;				break;
				case 13: setx32(0x01010001);	bm[j]=1;				break;
				case 14: setx32(0x01010100);	bm[j]=1;				break;
				case 15: setx32(0x01010101);	bm[j]=1;				break;
				}
#undef setx32
#define setx32(y) *(int*)(bm + i + 4) |= (y)
				switch( _mm_movemask_ps( _mm_castsi128_ps( _mm_cmpeq_epi32(mm_j,HH2) ) ) )
				{
				case 0: break;
				case 1: setx32(0x00000001);		bm[j]=1; 				break;
				case 2: setx32(0x00000100);		bm[j]=1;				break;
				case 3: setx32(0x00000101);		bm[j]=1;				break;
				case 4: setx32(0x00010000);		bm[j]=1;				break;
				case 5:	setx32(0x00010001);		bm[j]=1;				break;
				case 6: setx32(0x00010100);		bm[j]=1;				break;
				case 7: setx32(0x00010101);		bm[j]=1;				break;
				case 8: setx32(0x01000000);		bm[j]=1;				break;
				case 9: setx32(0x01000001);		bm[j]=1;				break;
				case 10: setx32(0x01000100);	bm[j]=1;				break;
				case 11: setx32(0x01000101);	bm[j]=1;				break;
				case 12: setx32(0x01010000);	bm[j]=1;				break;
				case 13: setx32(0x01010001);	bm[j]=1;				break;
				case 14: setx32(0x01010100);	bm[j]=1;				break;
				case 15: setx32(0x01010101);	bm[j]=1;				break;
				}
#undef setx32
			}
	}
postval;
}


#define USE_SSE41 0
static void get_validate_map64( long long* rHash, size_t startpos, size_t maxcount, char* bm, int mthreads, int mprocs, int crank )
{
prefval;
#pragma omp parallel for schedule(dynamic,16)
	//for(int i=(int)startpos; i<(int)(maxcount-7); i+=8)
	for(int i=crank*8; i<(int)(maxcount); i += mprocs*8)
	{
		for(int p=0; p<8; ++p) 
			for(int t=p+1; t<8; ++t)
			{
				if(rHash[i+p] == rHash[i+t]) { bm[i+p]=1; bm[i+t]=1;  }
			}

			register __m128i HH1L = _mm_load_si128( (__m128i*)(rHash + i    ) );
			register __m128i HH1H = _mm_load_si128( (__m128i*)(rHash + i + 2) );
			register __m128i HH2L = _mm_load_si128( (__m128i*)(rHash + i + 4) );
			register __m128i HH2H = _mm_load_si128( (__m128i*)(rHash + i + 6) );

			//register int mm_j;
			register __m128i mm_j;
			for( int j=i+8; j<maxcount; ++j )
			{
				mm_j = _mm_set1_epi64x( rHash[j] );
#define setx32(y) *(int*)(bm + i) |= (y)
				register int mmmask;
#if USE_SSE41
				mmmask = _mm_movemask_pd( _mm_castsi128_pd( _mm_cmpeq_epi64(mm_j,HH1L) ) );
				mmmask = (_mm_movemask_pd( _mm_castsi128_pd( _mm_cmpeq_epi64(mm_j,HH1H) ) ) << 2) | mmmask;
#else
				register __m128i xmtmp = _mm_cmpeq_epi32(mm_j,HH1L);
				mmmask = _mm_movemask_pd( _mm_castsi128_pd( _mm_and_si128( xmtmp, _mm_slli_si128(xmtmp, 4) ) ) );
				xmtmp = _mm_cmpeq_epi32(mm_j,HH1H);
				mmmask = (_mm_movemask_pd( _mm_castsi128_pd( _mm_and_si128( xmtmp, _mm_slli_si128(xmtmp, 4)) ) ) << 2) | mmmask;
#endif
				switch( mmmask )
				{
				case 0: break;
				case 1: setx32(0x00000001);		bm[j]=1; 				break;
				case 2: setx32(0x00000100);		bm[j]=1;				break;
				case 3: setx32(0x00000101);		bm[j]=1;				break;
				case 4: setx32(0x00010000);		bm[j]=1;				break;
				case 5:	setx32(0x00010001);		bm[j]=1;				break;
				case 6: setx32(0x00010100);		bm[j]=1;				break;
				case 7: setx32(0x00010101);		bm[j]=1;				break;
				case 8: setx32(0x01000000);		bm[j]=1;				break;
				case 9: setx32(0x01000001);		bm[j]=1;				break;
				case 10: setx32(0x01000100);	bm[j]=1;				break;
				case 11: setx32(0x01000101);	bm[j]=1;				break;
				case 12: setx32(0x01010000);	bm[j]=1;				break;
				case 13: setx32(0x01010001);	bm[j]=1;				break;
				case 14: setx32(0x01010100);	bm[j]=1;				break;
				case 15: setx32(0x01010101);	bm[j]=1;				break;
				}
#undef setx32
#define setx32(y) *(int*)(bm + i + 4) |= (y)

#if USE_SSE41
				mmmask = _mm_movemask_pd( _mm_castsi128_pd( _mm_cmpeq_epi64(mm_j,HH2L) ) );
				mmmask = (_mm_movemask_pd( _mm_castsi128_pd( _mm_cmpeq_epi64(mm_j,HH2H) ) ) << 2) | mmmask;
#else
				xmtmp = _mm_cmpeq_epi32(mm_j,HH2L);
				mmmask = _mm_movemask_pd( _mm_castsi128_pd( _mm_and_si128( xmtmp, _mm_slli_si128(xmtmp, 4) ) ) );
				xmtmp = _mm_cmpeq_epi32(mm_j,HH2H);
				mmmask = (_mm_movemask_pd( _mm_castsi128_pd( _mm_and_si128( xmtmp, _mm_slli_si128(xmtmp, 4)) ) ) << 2) | mmmask;
#endif
				switch( mmmask )
				{
				case 0: break;
				case 1: setx32(0x00000001);		bm[j]=1; 				break;
				case 2: setx32(0x00000100);		bm[j]=1;				break;
				case 3: setx32(0x00000101);		bm[j]=1;				break;
				case 4: setx32(0x00010000);		bm[j]=1;				break;
				case 5:	setx32(0x00010001);		bm[j]=1;				break;
				case 6: setx32(0x00010100);		bm[j]=1;				break;
				case 7: setx32(0x00010101);		bm[j]=1;				break;
				case 8: setx32(0x01000000);		bm[j]=1;				break;
				case 9: setx32(0x01000001);		bm[j]=1;				break;
				case 10: setx32(0x01000100);	bm[j]=1;				break;
				case 11: setx32(0x01000101);	bm[j]=1;				break;
				case 12: setx32(0x01010000);	bm[j]=1;				break;
				case 13: setx32(0x01010001);	bm[j]=1;				break;
				case 14: setx32(0x01010100);	bm[j]=1;				break;
				case 15: setx32(0x01010101);	bm[j]=1;				break;
				}
#undef setx32
			}//3515
			//2374 2482
	}	
postval;
}

//----------------------------------------------------
static void get_validate_map128( __m128i* rHash, size_t startpos, size_t maxcount, char* bm, int mthreads, int mprocs, int crank )
{
prefval;
#pragma omp parallel for schedule(dynamic,16)
	//for(int i=(int)startpos; i<(int)(maxcount-3); i+=4)
	for(int i=crank*4; i<(int)(maxcount); i += mprocs*4)
	{
		register __m128i hash1 = rHash[i];
		register __m128i hash2 = rHash[i+1];
		register __m128i hash3 = rHash[i+2];
		register __m128i hash4 = rHash[i+3];

#if USE_SSE41 //USE SSE41 =======================================================================================
		register __m128i mmone = _mm_cmpeq_epi64(mmone,mmone);
#define mmcmpeq128(a,b) _mm_testc_si128( _mm_cmpeq_epi64((a),(b)), mmone )
#else 		//USE SSE2 =======================================================================================
#define mmcmpeq128(a,b) (_mm_movemask_epi8( _mm_cmpeq_epi32((a),(b)) ) == 65535)
#endif

		if( mmcmpeq128(hash1,hash2) ) { bm[i]=bm[i+1]=1; }
		if( mmcmpeq128(hash1,hash3) ) { bm[i]=bm[i+2]=1; }
		if( mmcmpeq128(hash1,hash4) ) { bm[i]=bm[i+3]=1; }
		if( mmcmpeq128(hash2,hash3) ) { bm[i+1]=bm[i+2]=1; }
		if( mmcmpeq128(hash2,hash4) ) { bm[i+1]=bm[i+3]=1; }
		if( mmcmpeq128(hash3,hash4) ) { bm[i+2]=bm[i+3]=1; }

		register __m128i mm_j;
		for( int j=i+4; j<maxcount; ++j )
		{
			mm_j = rHash[j];
			if( mmcmpeq128(mm_j,hash1) ) { bm[j]=bm[i]=1; }
			if( mmcmpeq128(mm_j,hash2) ) { bm[j]=bm[i+1]=1; }
			if( mmcmpeq128(mm_j,hash3) ) { bm[j]=bm[i+2]=1; }
			if( mmcmpeq128(mm_j,hash4) ) { bm[j]=bm[i+3]=1; }
		}
	}
postval;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


int main( int argc, char** argv )
{
 setlocale( LC_ALL, "RUS" );
 srand( (unsigned)time(NULL) );
 
 if( argc <= 1 )
 {
	puts("Укажите файл для обработки");
	return 0;
 }
 
 //Init MPI -----------------------------------------
   int numprocs, rank, namelen;
   int maxthreads;
  char processor_name[256];

 MPI_Init(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Get_processor_name(processor_name, &namelen);
 
 maxthreads = omp_get_max_threads();
 //--------------------------------------------------
 
 printf("threads(%i)\tprocs(%i)\trank(%i)\tname=%s\n", maxthreads, numprocs, rank, processor_name);
 
 /*int i;
 #pragma omp parallel for private(i) //schedule(static,256)
 for(i=maxthreads*rank; i<20; i+=numprocs*maxthreads)
 {
  //printf("omp_get_num_threads = %i\nomp_get_thread_num = %i\n", omp_get_num_threads(), omp_get_thread_num());
  //printf("%i) i=%i\n", omp_get_thread_num(), i);
  printf("%i,%i) i=%i\n", omp_get_thread_num(),rank, i);
 }
 */
 
 FILE* fsource = NULL;
 FILE* ftemp=NULL;
 long long fsource_size=0;

 fsource = fopen64(argv[1], "r+b"); //Открываем файл для чтения и записи в бинарном режиме.
 if( fsource == NULL )
 {
  	puts("Ошибка открытия файла");
	usleep(1000000);
	return 0;
 }
 
 //ftemp = fopen("tmp0000001.tmp", "w+bD");
 if(rank == 0)
 {
  ftemp = tmpfile64();
  if( ftemp == NULL )
  {
	puts("Ошибка создания временного файла.\n");
	MPI_Abort( MPI_COMM_WORLD, MPI_ERR_OTHER);
	return 0;
  }
 }
 
 MPI_Barrier( MPI_COMM_WORLD );
 
 char* bmap = NULL;			//байт карта
 void* arrayV = NULL;		//массивы индексов
 long long scount;
 long long ssize;
 long long sfmt;

 bool bcont = true;

 fseeko64(fsource, 0, SEEK_END);
 fsource_size = ftello64(fsource);
 fseeko64(fsource, 0, SEEK_SET);
 
 do {
		bmap = NULL;
		arrayV = NULL;
		//bcont = FRes->Read( sfmt, &arrayV, ssize, scount );
		//if( !bcont ) { _putws( L"Операции завершенны!" ); break; }
		if( ftello64(fsource) >= fsource_size ) { bcont=false; break; } //{ puts("Операции завершенны!" ); bcont=false; break; }

		if( fread_unlocked(&sfmt, sizeof(sfmt), 1, fsource) != 1 ) { puts("Ошибка чтения индентификатора заголовка из файла."); break; };
		if( fread_unlocked(&ssize, sizeof(ssize), 1, fsource) != 1 ) { puts("Ошибка чтения размера эелемента данных из файла."); break; };
		if( fread_unlocked(&scount, sizeof(scount), 1, fsource) != 1 ) { puts("Ошибка чтения количества элементов данных из файла."); break; };

		if( ssize <= 0 ) { puts("Неправильный размер элемента данных. Возможно файл повреждён."); break; }
		if( scount <= 0 ) { puts("Неправильное количество элементов данных. Возможно файл повреждён."); break; }

		//allocation ------------------------------------------------------------------------------------------------------
		arrayV = _mm_malloc( ssize * scount, 16 );
		if( arrayV == NULL ) { printf("Ошибка выделения памяти. Требуется = (%i) МБайт\n", (ssize*scount) >> 20); break; }
		bmap = _mm_malloc( scount, 16 );
		if( bmap == NULL ) { _mm_free(arrayV); arrayV=NULL; printf("Ошибка выделения памяти байт-карты. Требуется = (%i) МБайт\n", scount >> 20); break; }

		//zero fill-
		__stosb( (unsigned char*)bmap, 0, scount );
		//------------------------------------------------------------------------------------------------------------------
		
		if( fread_unlocked(arrayV, ssize, scount, fsource) != scount ) { puts("Ошибка чтения массива данных из файла."); break; };
		
		//debug info -------------------------------------------------------------------------------------------------------
		if(rank == 0) printf("fmt(%llx) size(%lli) count(%lli) MBytes(%lli)\n", sfmt, ssize, scount, (ssize*scount) >> 20);
		//-------------------------------------------------------------------------------------------------------------------
		
		char* reducedbmap = NULL;
		
		if( rank == 0 ) reducedbmap = _mm_malloc( scount, 16 );
		
		//MPI_Barrier( MPI_COMM_WORLD ); //111111111111
		
		long long cmpsum=scount;
		bool btestex = true;
		switch(sfmt)
		{
		 case 0x10101010ffffffff:
		  MPI_Barrier( MPI_COMM_WORLD );
		  get_validate_map32( (int*)arrayV, 0, scount, bmap,   maxthreads, numprocs, rank );
		  MPI_Reduce( bmap, reducedbmap, scount, MPI_BYTE, MPI_BOR, 0, MPI_COMM_WORLD );
		  if( rank == 0 ) scount=A32deleteZero( arrayV, scount, reducedbmap, scount );
		 break;
		case 0x20202020ffffffff:
		case 0x30303030ffffffff:
		case 0x40404040ffffffff:
		case 0x70707070ffffffff:
		  MPI_Barrier( MPI_COMM_WORLD );
		  get_validate_map64( (long long*)arrayV, 0, scount, bmap,   maxthreads, numprocs, rank );
		  MPI_Reduce( bmap, reducedbmap, scount, MPI_BYTE, MPI_BOR, 0, MPI_COMM_WORLD );
		  if( rank == 0 ) scount=A64deleteZero( arrayV, scount, reducedbmap, scount );
		 break;
		case 0x50505050ffffffff:
		case 0x60606060ffffffff:
		case 0x80808080ffffffff:
		  MPI_Barrier( MPI_COMM_WORLD );
		  get_validate_map128( (__m128i*)arrayV, 0, scount, bmap,   maxthreads, numprocs, rank );
		  MPI_Reduce( bmap, reducedbmap, scount, MPI_BYTE, MPI_BOR, 0, MPI_COMM_WORLD );
		  if( rank == 0 ) scount=A128deleteZero( arrayV, scount, reducedbmap, scount );
		 break;
		}
		
		//debug
		//int findsum=0;
		//for(int i=0; i<scount; ++i) findsum += bmap[i];
		//printf("sfmt(%llx) rank = %i\t\t sum = %i\n", sfmt, rank, findsum);
		//debug end
		
		if( bmap != NULL ) _mm_free(bmap);
		if( rank == 0 ) _mm_free( reducedbmap );
		
		if((rank == 0) && btestex)
		{
			printf("Однородных элементов = %i\tНайденоТ(%u:%0.2u:%0.2u)\\УдалленоТ(%u:%0.2u:%0.2u)\n", cmpsum - scount, FValidElapsedhour, FValidElapsedmin, FValidElapsedsec, FDelElapsedhour, FDelElapsedmin, FDelElapsedsec );
			fwrite_unlocked( &sfmt, sizeof(sfmt), 1, ftemp );
			fwrite_unlocked( &ssize, sizeof(ssize), 1, ftemp );
			fwrite_unlocked( &scount, sizeof(scount), 1, ftemp );
			fwrite_unlocked( arrayV, ssize, scount, ftemp );
		}
		
		if( arrayV != NULL ) _mm_free(arrayV);
		
		MPI_Barrier( MPI_COMM_WORLD ); //222222222222
	} while( bcont );
	
	if(rank == 0)
	{
	 unsigned char tbuf[4096];
	 size_t nread=0;
	 fseeko64( fsource, 0, SEEK_SET );
	 fseeko64( ftemp, 0, SEEK_SET );
	 do 
	 {
	  nread = fread_unlocked( tbuf, 1, 4096, ftemp );
	  fwrite_unlocked( tbuf, 1, nread, fsource );
	 } while ( nread == 4096 );
	 ftruncate64(fileno(fsource), ftello64(fsource)); //unix
	 //closing ftemp
	 fclose(ftemp);
	}
   
 fclose(fsource);
 
 //MPI_Barrier( MPI_COMM_WORLD );
 
 MPI_Finalize();

 return 0;
}