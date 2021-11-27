//Date: 28/07/2015 05:29:14

#pragma once

class Protocol
{
public:
	Protocol();
	virtual ~Protocol();
	unsigned char container[2048];
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

	virtual int Pack(int channel, int address, int command);
	virtual int UnPack(int channel, int address, int command);
	virtual void print();
};

class pMercury : public Protocol
{
public:
	pMercury();
	virtual ~pMercury();

	unsigned char serialnumber[4];
	char dateD;
	char dateM;
	char dateY;
	unsigned char VerPO[3];
	unsigned short netaddr;
	//Коэфф. трансформации Кн, Кт
	unsigned short Kn;
	unsigned short Kt;

	virtual int Pack(int channel, int address, int command);
	virtual int UnPack(int channel, int address, int command);
	virtual void print();
};