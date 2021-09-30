//Last update time : 26.10.2013 00:40:54
#pragma once

#include <mpi.h>
#include <intrin.h>
#include <math.h>

static int rank_core[16];
static int count_calls[16];
static int root_countcalls[16];
static char* procnames=NULL;

//stat
static double avg_ar=0.0;
static double avg_ge=1.0;
static double avg_hr=0.0;
static double avg_sq=0.0;
static double qlin=0.0; //среднее линейное отклонение
static double q=0.0; //средне квадратическое отклонение
static double qdisp=0.0; //дисперсия
static unsigned long long bitbin[8];
static unsigned long long bytebin[256];

//Support functions..............................................
void my_wait(MPI_Request* mpr)
{
	int flag;
	do
	{
		MPI_Test(mpr,&flag,MPI_STATUS_IGNORE);
		if(!flag) sleep(1);
	}while(!flag);
}

void my_wait_all(int mprcount, MPI_Request* mpr)
{
	int flag;
	do
	{
		MPI_Testall(mprcount,mpr,&flag,MPI_STATUSES_IGNORE);
		if(!flag) sleep(1);
	}while(!flag);
}

void my_wait_status(MPI_Request* mpr, MPI_Status* mst)
{
	int flag;
	do
	{
		MPI_Test(mpr,&flag,mst);
		if(!flag) sleep(1);
	}while(!flag);
}
//----------------------------------------------------------------

void send_recv_ServerConfiguration(int rank, int maxrank, int core, char* procname)
{
	int rrcore[16];
	memset(rank_core,0,sizeof(rank_core));
	memset(rrcore,0,sizeof(rrcore));
	memset(count_calls,0,sizeof(count_calls));
	memset(root_countcalls,0,sizeof(root_countcalls));
	rrcore[rank]=core;
	MPI_Allreduce(rrcore, rank_core, 16, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	procnames = (char*)_aligned_malloc(MPI_MAX_PROCESSOR_NAME * maxrank * sizeof(char), 8);
	
	memset(procnames, 0, MPI_MAX_PROCESSOR_NAME * maxrank * sizeof(char));

	MPI_Allgather(procname, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, procnames, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, MPI_COMM_WORLD);
}

void getstat(unsigned char* buf, __int64 count, short groups, short gstart)
{
	avg_ar=0.0;
	avg_ge=1.0;
	avg_hr=0.0;
	avg_sq=0.0;
	qlin=0.0; 	//среднее линейное отклонение
	q=0.0; 		//средне квадратическое отклонение
	qdisp=0.0;	//дисперсия
	memset(bitbin, 0, sizeof(bitbin));
	memset(bytebin, 0, sizeof(bytebin));

	double rcount = (double)(count*16);
	
	for(long long i=gstart*16; i<(count*groups*16); i+=groups*16)
	{
		for(int j=0; j<16; ++j)
		{
		  double dval = (double)buf[i+j] + 1.0;
		  avg_ar += dval;
		  avg_ge *= pow( dval, 1.0/rcount);
		  avg_hr += 1.0 / dval;
		  avg_sq += dval*dval;
		  ++bytebin[ (unsigned char)buf[i+j] ];
		  bitbin[0] += ( buf[i+j] & 0x01ui8 ) ? 1ui64 : 0ui64;
		  bitbin[1] += ( buf[i+j] & 0x02ui8 ) ? 1ui64 : 0ui64;
		  bitbin[2] += ( buf[i+j] & 0x04ui8 ) ? 1ui64 : 0ui64;
		  bitbin[3] += ( buf[i+j] & 0x08ui8 ) ? 1ui64 : 0ui64;
		  bitbin[4] += ( buf[i+j] & 0x10ui8 ) ? 1ui64 : 0ui64;
		  bitbin[5] += ( buf[i+j] & 0x20ui8 ) ? 1ui64 : 0ui64;
		  bitbin[6] += ( buf[i+j] & 0x40ui8 ) ? 1ui64 : 0ui64;
		  bitbin[7] += ( buf[i+j] & 0x80ui8 ) ? 1ui64 : 0ui64;
		}
	}

	avg_ar /= rcount;
	avg_hr = rcount / avg_hr;
	avg_sq = sqrt( avg_sq / rcount );

	for(long long i=gstart*16; i<(count*16); i+=groups*16)
	{
		for(int j=0; j<16; ++j)
		{
		  double sq = (double)buf[i+j] - avg_ar + 1.0;
		  q+=sq*sq;
		  qlin+=fabs(sq);
		}
	}
	qdisp = q / rcount;
	q = sqrt(q / rcount);
	qlin = qlin / rcount;

	//wwma(t) = sum(0,n-1){ w[t-i]*p[t-i] }
	//wwma(t) = sum(0,n-1){ w[t-i]*p[t-i] } / sum(0,n-1){ w[t-i] }
}