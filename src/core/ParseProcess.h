#pragma once
#ifndef _PARSEPROCESS_H_
#define _PARSEPROCESS_H_
#include <vector>
#include <array>
#include <windows.h>
#include "Debuger.h"





class ParseProcess {
public:
	explicit ParseProcess(Debuger* debuger);
	~ParseProcess();



	typedef struct _MEMPAGEINFO {
		unsigned char* Buffer;
		MEMORY_BASIC_INFORMATION memBasicInfo;
	}MEMPAGEINFO , *PMEMPAGEINFO;


	typedef struct _DISINFO {
		char virtualAddress[0x20];
		char disByte[0x50];
		char disCode[0x100];
	}DISINFO , *PDISINFO;

	bool read_memory_by_pid(int Pid , std::vector<PMEMPAGEINFO>& vecMemPageInfo, size_t targetAddr);


	void zydis_data(size_t Address , const PMEMPAGEINFO vecMemPageInfo , std::vector<PDISINFO>& vecDisInfo);

	bool set_se_privilege();

private:
	HANDLE m_oldHProcess = nullptr;
	Debuger* m_Debuger;
public:
	int m_Pid = 0 ;
};












#endif // !_PARSEPROCESS_H_


