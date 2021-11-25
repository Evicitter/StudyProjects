//Last update time : 03.10.2015 17:37:54
#include "Protocols.h"
#include <memory.h>
#include <stdio.h>
#include <string.h>

Protocol* GetInstanceProtocol(eProtocolType fromStaticType)
{
	switch(fromStaticType)
	{
		case eProtocolType::OMNI_DIGIT: 	return new pOmnicomm();
		case eProtocolType::OMNI_FREQ:		return new pOmnicomm();
		case eProtocolType::ARNAVI:			return new pArnavi();
		case eProtocolType::ITALON:			return new pItalon();
		case eProtocolType::TRANSOPTIMA:    return new pTransOptima();
		default: return NULL;
    }
}


void Protocol::wCopyClassName(wchar_t* dstStr) const
{
  wcscpy_s(dstStr,64,ClassName);
}
void Protocol::wCopyProtocolName(wchar_t* dstStr) const
{
  wcscpy_s(dstStr,64,ProtocolName);
}

void Protocol::push1B(unsigned char value)
{
	*(container + cursor) = value;
	cursor+=sizeof(value);
	if (cursor >= sizeof(container)) cursor = 0;
}
void Protocol::push2B(unsigned short value)
{
	*(unsigned short*)(container + cursor) = value;
	cursor += sizeof(value);
	if (cursor >= sizeof(container)) cursor = 0;
}
void Protocol::push4B(unsigned int value)
{
	*(unsigned int*)(container + cursor) = value;
	cursor += sizeof(value);
	if (cursor >= sizeof(container)) cursor = 0;
}
unsigned char  Protocol::pop1B()
{
	register unsigned char t = *(unsigned char*)(container + cursor);
	cursor += sizeof(t);
	if (cursor >= sizeof(container)) cursor = 0;
	return t;
}
unsigned short Protocol::pop2B()
{
	register unsigned short t = *(unsigned short*)(container + cursor);
	cursor += sizeof(t);
	if (cursor >= sizeof(container)) cursor = 0;
	return t;
}
unsigned int   Protocol::pop4B()
{
	register unsigned int t = *(unsigned int*)(container + cursor);
	cursor += sizeof(t);
	if (cursor >= sizeof(container)) cursor = 0;
	return t;
}

Protocol::Protocol()
{
	cursor = 0;
	wcscpy_s(ProtocolName,64,L"NULL");
    wcscpy_s(ClassName,64,L"Protocol");
}

Protocol::~Protocol()
{

}

int Protocol::Pack(int channel, int address, int command)
{
	return 0;
}

int Protocol::UnPack(int channel, int address, int command)
{
	return 0;
}

void Protocol::PackSend()
{

}

void Protocol::UnPackRecv()
{

}

void Protocol::print()
{
	for (int i = 0; i < sizeof(container); ++i)
		wprintf_s(L"%0.2i ", container[i]);
	_putws(L"\n");
}

// для расчета контрольной суммы CRC
// принимает указатель на область памяти с данными 
// и количество принятых байт
unsigned short Protocol::ss_crc16(char *adr_buffer, unsigned short byte_cnt) const
{
	/////////////////////////////////////////////////
	static unsigned char auchCRCHi[] =
	{
		0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8,
		0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8,
		0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8,
		0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8,
		0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8,
		0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8,
		0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8,
		0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8,
		0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8,
		0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8,
		0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8,
		0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8,
		0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8,
		0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8,
		0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8,
		0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x01ui8, 0xC0ui8, 0x80ui8, 0x41ui8, 0x00ui8, 0xC1ui8, 0x81ui8, 0x40ui8,
	};
	/*Table of CRC values for low–order byte*/
	static unsigned char auchCRCLo[] =
	{
		0x00ui8, 0xC0ui8, 0xC1ui8, 0x01ui8, 0xC3ui8, 0x03ui8, 0x02ui8, 0xC2ui8, 0xC6ui8, 0x06ui8, 0x07ui8, 0xC7ui8, 0x05ui8, 0xC5ui8, 0xC4ui8, 0x04ui8,
		0xCCui8, 0x0Cui8, 0x0Dui8, 0xCDui8, 0x0Fui8, 0xCFui8, 0xCEui8, 0x0Eui8, 0x0Aui8, 0xCAui8, 0xCBui8, 0x0Bui8, 0xC9ui8, 0x09ui8, 0x08ui8, 0xC8ui8,
		0xD8ui8, 0x18ui8, 0x19ui8, 0xD9ui8, 0x1Bui8, 0xDBui8, 0xDAui8, 0x1Aui8, 0x1Eui8, 0xDEui8, 0xDFui8, 0x1Fui8, 0xDDui8, 0x1Dui8, 0x1Cui8, 0xDCui8,
		0x14ui8, 0xD4ui8, 0xD5ui8, 0x15ui8, 0xD7ui8, 0x17ui8, 0x16ui8, 0xD6ui8, 0xD2ui8, 0x12ui8, 0x13ui8, 0xD3ui8, 0x11ui8, 0xD1ui8, 0xD0ui8, 0x10ui8,
		0xF0ui8, 0x30ui8, 0x31ui8, 0xF1ui8, 0x33ui8, 0xF3ui8, 0xF2ui8, 0x32ui8, 0x36ui8, 0xF6ui8, 0xF7ui8, 0x37ui8, 0xF5ui8, 0x35ui8, 0x34ui8, 0xF4ui8,
		0x3Cui8, 0xFCui8, 0xFDui8, 0x3Dui8, 0xFFui8, 0x3Fui8, 0x3Eui8, 0xFEui8, 0xFAui8, 0x3Aui8, 0x3Bui8, 0xFBui8, 0x39ui8, 0xF9ui8, 0xF8ui8, 0x38ui8,
		0x28ui8, 0xE8ui8, 0xE9ui8, 0x29ui8, 0xEBui8, 0x2Bui8, 0x2Aui8, 0xEAui8, 0xEEui8, 0x2Eui8, 0x2Fui8, 0xEFui8, 0x2Dui8, 0xEDui8, 0xECui8, 0x2Cui8,
		0xE4ui8, 0x24ui8, 0x25ui8, 0xE5ui8, 0x27ui8, 0xE7ui8, 0xE6ui8, 0x26ui8, 0x22ui8, 0xE2ui8, 0xE3ui8, 0x23ui8, 0xE1ui8, 0x21ui8, 0x20ui8, 0xE0ui8,
		0xA0ui8, 0x60ui8, 0x61ui8, 0xA1ui8, 0x63ui8, 0xA3ui8, 0xA2ui8, 0x62ui8, 0x66ui8, 0xA6ui8, 0xA7ui8, 0x67ui8, 0xA5ui8, 0x65ui8, 0x64ui8, 0xA4ui8,
		0x6Cui8, 0xACui8, 0xADui8, 0x6Dui8, 0xAFui8, 0x6Fui8, 0x6Eui8, 0xAEui8, 0xAAui8, 0x6Aui8, 0x6Bui8, 0xABui8, 0x69ui8, 0xA9ui8, 0xA8ui8, 0x68ui8,
		0x78ui8, 0xB8ui8, 0xB9ui8, 0x79ui8, 0xBBui8, 0x7Bui8, 0x7Aui8, 0xBAui8, 0xBEui8, 0x7Eui8, 0x7Fui8, 0xBFui8, 0x7Dui8, 0xBDui8, 0xBCui8, 0x7Cui8,
		0xB4ui8, 0x74ui8, 0x75ui8, 0xB5ui8, 0x77ui8, 0xB7ui8, 0xB6ui8, 0x76ui8, 0x72ui8, 0xB2ui8, 0xB3ui8, 0x73ui8, 0xB1ui8, 0x71ui8, 0x70ui8, 0xB0ui8,
		0x50ui8, 0x90ui8, 0x91ui8, 0x51ui8, 0x93ui8, 0x53ui8, 0x52ui8, 0x92ui8, 0x96ui8, 0x56ui8, 0x57ui8, 0x97ui8, 0x55ui8, 0x95ui8, 0x94ui8, 0x54ui8,
		0x9Cui8, 0x5Cui8, 0x5Dui8, 0x9Dui8, 0x5Fui8, 0x9Fui8, 0x9Eui8, 0x5Eui8, 0x5Aui8, 0x9Aui8, 0x9Bui8, 0x5Bui8, 0x99ui8, 0x59ui8, 0x58ui8, 0x98ui8,
		0x88ui8, 0x48ui8, 0x49ui8, 0x89ui8, 0x4Bui8, 0x8Bui8, 0x8Aui8, 0x4Aui8, 0x4Eui8, 0x8Eui8, 0x8Fui8, 0x4Fui8, 0x8Dui8, 0x4Dui8, 0x4Cui8, 0x8Cui8,
		0x44ui8, 0x84ui8, 0x85ui8, 0x45ui8, 0x87ui8, 0x47ui8, 0x46ui8, 0x86ui8, 0x82ui8, 0x42ui8, 0x43ui8, 0x83ui8, 0x41ui8, 0x81ui8, 0x80ui8, 0x40ui8,
	};
	///////////////////////////////////////////////////////////// 

	// переменные для расчета CRC16 -?*:?????
	unsigned char uchCRCHi = 0xFF;
	unsigned char uchCRCLo = 0xFF;
	unsigned uIndex;

	/* CRC Generation Function */
	while (byte_cnt--) /* pass through message buffer */
	{
		uIndex = uchCRCHi ^ *adr_buffer++; /* calculate the CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}

unsigned char Protocol::ss_crc8(unsigned char *mas, unsigned short Len) const
{
	unsigned char i, dat, crc, fb, st_byt;
	st_byt = 0; crc = 0;
	do{
		dat = mas[st_byt];
		for (i = 0; i<8; i++) { 
			fb = crc ^ dat;
			fb &= 1;
			crc >>= 1;
			dat >>= 1;
			if (fb == 1) crc ^= 0x8c; 
		}
		st_byt++;
	} while (st_byt < Len);
	return crc;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

pOmnicomm::pOmnicomm()
{
	codop_from = address_from = 0ui8;
	temperature = 0i8;
	freq = difflevel = 0ui16;

	wcscpy_s(ProtocolName,64,L"OmnicommDigit");
    wcscpy_s(ClassName,64,L"pOmnicomm");
}

pOmnicomm::~pOmnicomm()
{
}

int pOmnicomm::Pack(int channel, int address, int command)
{
	char data5[] = { 0x31ui8, 0xFFui8, 0x22ui8, 0x02ui8, 0x00ui8, 0x07ui8, 0x01ui8, 0x00ui8, 0x00ui8, 0x19ui8, 0x00ui8, 0xFFui8, 0x0Fui8, 0x20ui8, 0xEEui8, 0x00
		, 0x16ui8, 0xCAui8, 0x05ui8, 0xF0ui8, 0x00ui8, 0xF0ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00
		, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x40ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00
		, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00
		, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0xE4ui8 };
	
	cursor = 0;
	
	push1B(0x31ui8); //request command
	push1B((unsigned char)address);

	//assembly struct
	switch (command)
	{
	case 0: push1B(0x02); break;
	case 1: push2B(0x0023); break; //get version
	case 2: push2B(0x0123); break;
	case 3: push2B(0x0323); break;
	case 4: memcpy_s(container + cursor, sizeof(container), data5, sizeof(data5)); cursor += sizeof(data5); break;
	case 5: push1B(0x06);  break;
	case 6: push2B(0x0223);  break;
	default: break;
	}
	push1B(ss_crc8(container, cursor));
	return cursor;
}

int pOmnicomm::UnPack(int channel, int address, int command)
{
	//from container
	cursor = 0;
	if (pop1B() != 0x3Eui8) return 0;

	address_from = pop1B();
	codop_from = pop1B();

	if (codop_from == 0x06ui8)
	{
		temperature = pop1B();
		difflevel	= pop2B();
		freq		= pop2B();
		register unsigned char c8 = ss_crc8(container, cursor);
		if (pop1B() != c8) return 10101;
		if (difflevel > 4095) return -1;
	}
	else if(codop_from == 0x23ui8)
	{
		if(pop1B() == 0x00ui8)
		{
			unsigned char cc;
			int ind=0;
			while((cc=pop1B()) != 0x0Aui8)
			{
			 version[ind] = (char)cc;
			 ++ind;
			 if(ind>=32) break;
            }
        }
    }
	return 0;
}

void pOmnicomm::GetProtocolVersion(wchar_t* dstStr)
{
}

void pOmnicomm::print()
{
	//wprintf_s(L"address=%i level=%i temp=%i\n", address_from, difflevel, temperature);
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

pItalon::pItalon()
{
	codop_from = address_from = 0ui8;
	temperature = 0i8;
	freq = difflevel = 0ui16;

	wcscpy_s(ProtocolName,64,L"Italon");
    wcscpy_s(ClassName,64,L"pItalon");
}

pItalon::~pItalon()
{
}

int pItalon::Pack(int channel, int address, int command)
{
	char data5[] = { 0x31ui8, 0xFFui8, 0x22ui8, 0x02ui8, 0x00ui8, 0x07ui8, 0x01ui8, 0x00ui8, 0x00ui8, 0x19ui8, 0x00ui8, 0xFFui8, 0x0Fui8, 0x20ui8, 0xEEui8, 0x00
		, 0x16ui8, 0xCAui8, 0x05ui8, 0xF0ui8, 0x00ui8, 0xF0ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00
		, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x40ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00
		, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0x00
		, 0x00ui8, 0x00ui8, 0x00ui8, 0x00ui8, 0xE4ui8 };

	cursor = 0;

	push1B(0x31ui8); //request command
	//push1B((unsigned char)address);

	//assembly struct
	switch (command)
	{
	 case 0: push1B(0x02); break;
	 case 1: push2B(0x0023); break;
	 case 2: push2B(0x0123); break;
	 case 3: push2B(0x0323); break;
	 case 4: memcpy_s(container + cursor, sizeof(container), data5, sizeof(data5)); cursor += sizeof(data5); break;
	 case 5: push4B(0x00000005); push2B(0x00); push1B(0x05); return cursor;
	 case 6: push2B(0x0223);  break;
	 default: break;
	}
	push1B(ss_crc8(container, cursor));
	return cursor;
}

int pItalon::UnPack(int channel, int address, int command)
{
	//from container
	cursor = 0;
	if (pop1B() != 0x3Eui8) return 0;

	//
	codop_from = pop1B();

	if (codop_from == 0x05ui8)
	{
		difflevel	= pop2B();
		address_from = pop1B();
		pop2B();
		//register unsigned char c8 = ss_crc8(container, cursor);
		//if (pop1B() != c8) return 10101;
		//if (difflevel > 4095) return -1;
	}
	return 0;
}

void pItalon::print()
{
	sprintf(StrBuffer, "%i,%i", codop_from, difflevel);
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

pTransOptima::pTransOptima()
{
	codop_from = address_from = 0ui8;
	temperature = 0i8;
	freq = difflevel = 0ui16;

	wcscpy_s(ProtocolName,64,L"TransOptima"); //Analog and Freq
	wcscpy_s(ClassName,64,L"pTransOptima");
}

pTransOptima::~pTransOptima()
{
}

int pTransOptima::Pack(int channel, int address, int command)
{
	cursor = 0;

	push1B(0x31ui8); //request command
	push1B(0x05ui8);

	push4B(0x00000000);
	push1B(0x00ui8);

	//crc8 - unknown
	push1B(0x05ui8);

	return cursor;
}

int pTransOptima::UnPack(int channel, int address, int command)
{
	//from container
	cursor = 0;
	if (pop1B() != 0x3Eui8) return 0;

	codop_from = pop1B();

	if(codop_from == 0x05ui8)
	{
		difflevel = pop2B();
		digit = difflevel;
		freq = difflevel;
		pop2B();
		pop1B();
	}
	//crc8 unknown
	pop1B();
	return 0;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
pArnavi::pArnavi()
{
	codop_from = address_from = 0ui8;
	temperature = 0i8;
	freq = difflevel = 0ui16;

	wcscpy_s(ProtocolName,64,L"Arnavi");
	wcscpy_s(ClassName,64,L"pArnavi");
}

pArnavi::~pArnavi()
{
}

int pArnavi::Pack(int channel, int address, int command)
{
	cursor = 0;

	//    DA 01 00 01 DC
	push1B(0xDAui8); //request command
	push4B(0xDC010001);

	//crc8 - unknown

	return cursor;
}

int pArnavi::UnPack(int channel, int address, int command)
{
	//from container
	cursor = 0;
	if (pop1B() != 0xDDui8) return 0;

	pop1B(); //01
	pop4B(); // 05 01 04 00
	pop4B(); // 00 00 00 05
	pop4B(); // 09 1E 00 00
	pop4B(); // 06 00 00 00
	pop2B(); // 00 14
	freq = pop2B(); //6C 07
	pop4B(); // 00 00 50 01
	pop1B(); // 11
	digit = pop2B(); //C8 0F
	pop1B(); // DA (crc8) unknown

	difflevel = digit;

	return 0;
}