//Last update time : 03.10.2015 17:09:22
#pragma once

class Protocol
{
protected:
	//--------------------------------
	wchar_t  ProtocolName[64];
	wchar_t  ClassName[64];
	//--------------------------------
public:
	Protocol();
	virtual ~Protocol();

	unsigned char container[2048];
    unsigned char StrBuffer[2048];
	int cursor;

	void push1B(unsigned char value);
	void push2B(unsigned short value);
	void push4B(unsigned int value);
	unsigned char  pop1B();
	unsigned short pop2B();
	unsigned int   pop4B();

	virtual int Pack(int channel, int address, int command);
	virtual int UnPack(int channel, int address, int command);

	virtual void PackSend();
	virtual void UnPackRecv();
	virtual void print();

	unsigned short ss_crc16(char *adr_buffer, unsigned short byte_cnt) const;
	unsigned char  ss_crc8(unsigned char *mas, unsigned short Len) const;

	void wCopyClassName(wchar_t* dstStr) const;
	void wCopyProtocolName(wchar_t* dstStr) const;

	//specific
	virtual void GetProtocolVersion(wchar_t* dstStr) {};
};

class pOmnicomm : public Protocol
{
public:
	pOmnicomm();
	virtual ~pOmnicomm();

	unsigned char address_from;
	unsigned char codop_from;
	signed   char temperature; //[-128, 127] Градусы цельсия
	unsigned short difflevel; //[0000, ffff] Относительный уровень
	unsigned short freq;		//[0000, ffff] Частота
	unsigned short digit;

	char version[32];
	//char vendor

	virtual int Pack(int channel, int address, int command);
	virtual int UnPack(int channel, int address, int command);
	virtual void print();

    virtual void GetProtocolVersion(wchar_t* dstStr);
};

class pItalon : public pOmnicomm
{
public:
	pItalon();
	virtual ~pItalon();
   	virtual int Pack(int channel, int address, int command);
	virtual int UnPack(int channel, int address, int command);
	virtual void print();

};

class pTransOptima : public pOmnicomm
{
public:
	pTransOptima();
	virtual ~pTransOptima();
   	virtual int Pack(int channel, int address, int command);
	virtual int UnPack(int channel, int address, int command);
	//virtual void print();


};

class pArnavi : public pOmnicomm
{
public:
	pArnavi();
	virtual ~pArnavi();
	virtual int Pack(int channel, int address, int command);
	virtual int UnPack(int channel, int address, int command);
	//virtual void print();

};


enum eProtocolType
{
	OMNI_DIGIT,
	OMNI_FREQ,
	ARNAVI,
	ITALON,
	TRANSOPTIMA
};

extern Protocol* GetInstanceProtocol(eProtocolType fromStaticType);