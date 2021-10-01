//Last update time : 14.11.2011 02:40:24
#pragma once

#include <vector>
#include <Windows.h>

enum ePos
{
  FromBegin, FromEnd, FromCurrent
};

struct wpairarr
{
	int size;
	void* data;
	wpairarr(void* d, int s) : data(d), size(s) {}
};

struct rpairarr
{
	int size;
	void* data;
	rpairarr() : data(NULL), size(0) {}
	~rpairarr() { if(data) { free(data); data=NULL; }  }
};

class CMemStream  
{
public:
    DWORD Write(const void *data, DWORD data_size);
    DWORD Read(void *data, DWORD data_size);
    void* Pointer();
    void  SetPosition(int pos, ePos from = FromBegin);
    DWORD GetPosition();
    inline DWORD Size(){return buffer.size();};
    void  CopyFrom(CMemStream* mem, DWORD data_size = 0);
    void  FreeSpace();
    void  SetSize(DWORD size);

	//special functions
	//FIX for DRY
	#define WRITE_ROW const int sz = sizeof(b); Write( &sz, sizeof(sz) ); Write( &b, sizeof(b) ); return *this;
	CMemStream& operator <<( const bool& b )				{ WRITE_ROW }
	CMemStream& operator <<( const unsigned char& b )		{ WRITE_ROW }
	CMemStream& operator <<( const unsigned short& b )		{ WRITE_ROW }
	CMemStream& operator <<( const unsigned int& b )		{ WRITE_ROW }
	CMemStream& operator <<( const unsigned long long& b )	{ WRITE_ROW }
	CMemStream& operator <<( const char& b )				{ WRITE_ROW }
	CMemStream& operator <<( const short& b )				{ WRITE_ROW }
	CMemStream& operator <<( const int& b )					{ WRITE_ROW }
	CMemStream& operator <<( const long long& b )			{ WRITE_ROW }
	#undef WRITE_ROW

	//FIX for DRY
	#define READ_ROW int sz=0; Read( &sz, sizeof(sz) ); if(sz==sizeof(b)) Read( &b, sz ); return *this;
	CMemStream& operator >>( bool& b )						{ READ_ROW }
	CMemStream& operator >>( unsigned char& b )				{ READ_ROW }
	CMemStream& operator >>( unsigned short& b )			{ READ_ROW }
	CMemStream& operator >>( unsigned int& b )				{ READ_ROW }
	CMemStream& operator >>( unsigned long long& b )		{ READ_ROW }
	CMemStream& operator >>( char& b )						{ READ_ROW }
	CMemStream& operator >>( short& b )						{ READ_ROW }
	CMemStream& operator >>( int& b )						{ READ_ROW }
	CMemStream& operator >>( long long& b )					{ READ_ROW }
	#undef READ_ROW
		
		
		CMemStream& operator <<( const wpairarr& b )
		{ 
			Write( &b.size, sizeof(b.size) );
			Write( b.data, b.size );
			return *this;
		}
		CMemStream& operator >>( rpairarr& b )
		{
			Read( &b.size, sizeof(b.size) );
			if(b.data != NULL) free(b.data);
			b.data = malloc( b.size );
			Read( b.data, b.size ); 
			return *this;
		}
		//-----------------

		inline void ResetWriteB() { TotalWriteBytes=0; }
		inline void ResetReadB() { TotalReadBytes=0; }

    CMemStream();
    virtual ~CMemStream();

protected:
    std::vector<byte> buffer;
    DWORD Position;
	DWORD TotalWriteBytes;
	DWORD TotalReadBytes;

    inline void Reserve(int NeedSize)
    {
        if (NeedSize < 128)
            buffer.reserve(NeedSize);
        else
            buffer.reserve(NeedSize + (NeedSize >> 1));
    };
};