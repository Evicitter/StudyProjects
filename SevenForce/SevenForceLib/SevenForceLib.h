//Last update time : 24.10.2010 03:06:40
struct i128 
{
	unsigned char x[16];
};

extern i128 __fastcall GetHashKBD(i128 curHash, int p1, int p2); //KeyBoard 1
extern i128 __fastcall GetHashMSE(i128 curHash, int p1, int p2); //Mouse 2
extern i128 __fastcall GetHashRAM(i128 curHash, int p1, int p2); //Memory - 3
extern i128 __fastcall GetHashCPU(i128 curHash, int p1, int p2); //CPU - 4
extern i128 __fastcall GetHashGPU(i128 curHash, int p1, int p2); //Video - 5
extern i128 __fastcall GetHashHDD(i128 curHash, int p1, int p2); //HDD - 6
extern i128 __fastcall GetHashNET(i128 curHash, int p1, int p2); //NetWork - 7
extern i128 __fastcall GetHashMON(i128 curHash, int p1, int p2); //Monitor - 8
extern i128 __fastcall GetHashRSM(i128 curHash, int p1, int p2); //RSMB - 8

bool __fastcall GetVerOS_XP32( __int32 fparam, __int32 sparam );