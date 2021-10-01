//Last update time : 20.11.2011 23:00:25
#include "stdafx.h"
#include "CMemStream.hpp"

#define min(a,b) (((a) < (b)) ? (a) : (b))

// Construction/Destruction
//Класс для работы с потоком данных в памяти (запись чтение)
CMemStream::CMemStream()
{
    Position = 0;
    buffer.reserve(16);
}

CMemStream::~CMemStream() {}

DWORD  CMemStream::Write(const void *data, DWORD data_size)
{
   if ((DWORD)data > 65536)
   {
       DWORD NeedSize = Position + data_size + 1;
       if (NeedSize >= buffer.size())
            buffer.resize(NeedSize);

       memcpy(&(buffer[Position]), data, data_size);
       
       Position += data_size;
			 TotalWriteBytes += data_size;
       return data_size;
   }
   else 
       return 0;
}

DWORD CMemStream::Read(void *data, DWORD data_size)
{
   if ((DWORD)data > 65536)
   {
       DWORD len;
       DWORD NeedSize = Position + data_size + 1;

	   len = (NeedSize > buffer.size()) ? (buffer.size() - Position - 1) : data_size;

       memcpy(data, &(buffer[Position]), len);

       Position += len;
			 TotalReadBytes += len;
       return len;
   }
   else 
       return 0;
}

void* CMemStream::Pointer()
{
    return &buffer[0];
}

void  CMemStream::SetPosition(int pos, ePos from)
{
    if      (from == FromBegin)
    {
        if      (pos < 0)                   Position = 0;
        else if (pos >= (int)buffer.size()) Position = buffer.size() - 1;
        else                                Position = pos;
    }
    else if (from == FromEnd)
    {
        if      (pos < 0)                   Position = buffer.size() - 1;
        else if (pos >= (int)buffer.size()) Position = 0;
        else                                Position = buffer.size() - 1 - pos;    
    }
    else if (from == FromCurrent)
    {
        if      (pos + Position < 0)                   Position = 0;
        else if (pos + Position >= (int)buffer.size()) Position = buffer.size() - 1;
        else                                           Position = Position + pos;    
    }
    else Position = Position;
}

DWORD CMemStream::GetPosition()
{
    return Position;
}

//DWORD CMemStream::Size() { return buffer.size(); }

void  CMemStream::CopyFrom(CMemStream* mem, DWORD data_size)
{
    byte tbuf[32768];
    DWORD ToCopy = 0;

    if (data_size = 0)
    {
        ToCopy = mem->Size() - mem->GetPosition();
    }
    else 
        ToCopy = min(data_size, mem->Size() - mem->GetPosition());

    if (Position + ToCopy + 1 > mem->Size())
        SetSize(Position + ToCopy + 1);    


    DWORD Copied = 0;
    do 
    {
        Copied = mem->Read(tbuf, min(ToCopy, sizeof(tbuf)));
        Write(tbuf, Copied);
        ToCopy -= Copied;
    }while (ToCopy > 0);    
}

void  CMemStream::FreeSpace()
{
    buffer.clear();
    buffer.swap(std::vector<byte>(buffer));
    Position = 0;
}

void  CMemStream::SetSize(DWORD size)
{
    Reserve(size);
    buffer.resize(size);
}