#pragma once

//Last update time : 28.11.2011 13:03:35

struct s_scanusers_inout //(UNORDERED)
{
	wchar_t profile[32];
	wchar_t rcompName[32];
	unsigned char md5hash[16];
	int iclient;
	int iserver;
	int query;		//если не 0, то запрос, иначе ответ.
};

struct s_scanfiles_inout //(UNORDERED)
{
	wchar_t r_filename[32];
	wchar_t rcompName[32];
	wchar_t r_profile[32];
	wchar_t s_profile[32];
	unsigned char md5hash[16];
	int iclient;
	int iserver;
	int query;		//если не 0, то запрос, иначе ответ.
};

struct s_getfile_inout //(UNORDERED)
{
	wchar_t r_filename[32];
	wchar_t rcompName[32];
	wchar_t r_profile[32];
	wchar_t s_profile[32];
	unsigned char md5hash_chunk[16];
	unsigned char chunk[64]; //1024
	unsigned int	bufsize;
	long long position;
	int iclient;
	int iserver;
	int query;		//если не 0, то запрос, иначе ответ.
};

struct s_checkfile_inout //(UNORDERED)
{
	wchar_t r_filename[32];
	wchar_t rcompName[32];
	wchar_t r_profile[32];
	wchar_t s_profile[32];
	unsigned char md5hash_file[16];
	long long filelength;
	int iclient;
	int iserver;
	int query;		//если не 0, то запрос, иначе ответ.
};


class FileShareProtocol
{
public:
	typedef int (*cbgenHASHb)(unsigned char* buffer, unsigned int bufsize_inbyte, unsigned char** out);
	typedef int (*cbgenHASHf)(wchar_t* filename, unsigned char** out);

	wchar_t* w_profilename;

	int idCurrentClient;		//-- deprecated
	int idCurrentServer;		//-- deprecated
	int lerror;						//Ошибка последней функции
	cbgenHASHb HashB;				//Внешнаяя функция генерации хэша от буфера
	cbgenHASHf HashF;				//Внешнаяя функция генерации хэша от файла

	FileShareProtocol(void);
	~FileShareProtocol(void);

	//Установка индетификатора клиента и сервера, и имени профиля
	int setup(int ids, int idc, const wchar_t* profilename); //-- deprecated

	// Функция возвращает тип заголовка, иначе -1 если ошибка
	int getheader(unsigned char** buf, int buflen);
	// Генерирует заголовок по типу, и возвращает его размер
	int genheader(unsigned char** buf, int idtype);
	// Освобождает выделеный буфер
	int gq_FreeResource(unsigned char** fbuffer);
	
	//-----------------------------------------------------------------------------------
	// Сборка пакета запроса scanusers
	int gq_ScanUsers_sen(unsigned char** inbuf, int& len, s_scanusers_inout* in_param);
	// Сборка пакета ответа и его проверка на запрос
	int gq_ScanUsers_rec(unsigned char* inbuf, int len, s_scanusers_inout** out_param);

	//-----------------------------------------------------------------------------------
	// Сборка пакета запроса scanfiles
	int gq_ScanFiles_sen(unsigned char** inbuf, int& len, s_scanfiles_inout* in_param);
	// Сборка пакета ответа и его проверка на запрос
	int gq_ScanFiles_rec(unsigned char* inbuf, int len, s_scanfiles_inout** out_param);
	
	//-----------------------------------------------------------------------------------
	// Сборка пакета запроса getfile
	int gq_GetFile_sen(unsigned char** inbuf, int& len, s_getfile_inout* in_param);
	// Сборка пакета ответа и его проверка на запрос
	int gq_GetFile_rec(unsigned char* inbuf, int len, s_getfile_inout** out_param);

	//-----------------------------------------------------------------------------------
	// Сборка пакета запроса checkfile
	int gq_CheckFile_sen(unsigned char** inbuf, int& len, s_checkfile_inout* in_param);
	// Сборка пакета ответа и его проверка на запрос
	int gq_CheckFile_rec(unsigned char* inbuf, int len, s_checkfile_inout** out_param);
};

extern FileShareProtocol globalFSC;