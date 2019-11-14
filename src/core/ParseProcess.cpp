#include "ParseProcess.h"
#include <Zydis/Zydis.h>
#include <stdio.h>
#include <QMessageBox>


using namespace std;


typedef enum _MEMORY_INFORMATION_CLASS {
	MemoryBasicInformation , // MEMORY_BASIC_INFORMATION
	MemoryWorkingSetInformation , // MEMORY_WORKING_SET_INFORMATION
	MemoryMappedFilenameInformation , // UNICODE_STRING
	MemoryRegionInformation , // MEMORY_REGION_INFORMATION
	MemoryWorkingSetExInformation , // MEMORY_WORKING_SET_EX_INFORMATION
	MemorySharedCommitInformation , // MEMORY_SHARED_COMMIT_INFORMATION
	MemoryImageInformation , // MEMORY_IMAGE_INFORMATION
	MemoryRegionInformationEx ,
	MemoryPrivilegedBasicInformation ,
	MemoryEnclaveImageInformation , // MEMORY_ENCLAVE_IMAGE_INFORMATION // since REDSTONE3
	MemoryBasicInformationCapped
} MEMORY_INFORMATION_CLASS;

EXTERN_C NTSYSCALLAPI NTSTATUS WINAPI NtQueryVirtualMemory(HANDLE                   ProcessHandle ,
														   PVOID                    BaseAddress ,
														   MEMORY_INFORMATION_CLASS MemoryInformationClass ,
														   PVOID                    MemoryInformation ,
														   SIZE_T                   MemoryInformationLength ,
														   PSIZE_T                  ReturnLength);





ParseProcess::ParseProcess(Debuger* debuger) {
	set_se_privilege();
	m_Debuger = debuger;
}

ParseProcess::~ParseProcess() {
	CloseHandle(m_Debuger->m_hProcess);
}


bool ParseProcess::read_memory_by_pid(int Pid , std::vector<PMEMPAGEINFO>& vecMemPageInfo , size_t targetAddr) {
	PVOID baseAddress = 0;
	SIZE_T realSize = 0;
	MEMORY_BASIC_INFORMATION memBasicInfo = { 0 };
	auto isFound = false;

	m_Debuger->m_hProcess = OpenProcess(PROCESS_ALL_ACCESS , FALSE , Pid);
	if (m_oldHProcess != nullptr &&m_oldHProcess != m_Debuger->m_hProcess) {
		CloseHandle(m_oldHProcess);
	}
	m_oldHProcess = m_Debuger->m_hProcess;
	if (m_Debuger->m_hProcess == NULL) {
		return false;
	}
	while (NtQueryVirtualMemory(m_Debuger->m_hProcess ,
		   baseAddress ,
		   MemoryBasicInformation ,
		   &memBasicInfo ,
		   sizeof(MEMORY_BASIC_INFORMATION) ,
		   NULL
		   ) >= NULL) {
		baseAddress = (PVOID)((size_t)baseAddress + memBasicInfo.RegionSize);
		if (memBasicInfo.BaseAddress != NULL) {
			if (targetAddr >= reinterpret_cast<size_t>(memBasicInfo.BaseAddress)
				&& targetAddr <= (reinterpret_cast<size_t>(memBasicInfo.BaseAddress) + memBasicInfo.RegionSize)) {
				// 已找到目标地址
				vecMemPageInfo.push_back((PMEMPAGEINFO)malloc(sizeof(MEMPAGEINFO)));
				vecMemPageInfo[0]->Buffer = (unsigned char*)malloc(memBasicInfo.RegionSize);
				memcpy(&vecMemPageInfo[0]->memBasicInfo , &memBasicInfo , sizeof(MEMORY_BASIC_INFORMATION));
				ReadProcessMemory(m_Debuger->m_hProcess , memBasicInfo.BaseAddress , vecMemPageInfo[0]->Buffer , memBasicInfo.RegionSize , &realSize);
				isFound = true;
				break;
			}
		}
	}

	if (isFound == false) {
		QMessageBox::warning(NULL , QStringLiteral("警告") , QStringLiteral("找到目标地址") , QMessageBox::Yes , QMessageBox::Yes);
		return false;
	}

	return true;
}

void ParseProcess::zydis_data(size_t inputAddress , const PMEMPAGEINFO vecMemPageInfo , std::vector<PDISINFO>& vecDisInfo) {
		// Initialize decoder context.
	ZydisDecoder decoder;
#ifdef _WIN64
	ZydisDecoderInit(
		&decoder ,
		ZYDIS_MACHINE_MODE_LONG_64 ,
		ZYDIS_ADDRESS_WIDTH_64);
#else
	ZydisDecoderInit(
		&decoder ,
		ZYDIS_MACHINE_MODE_LONG_COMPAT_32 ,
		ZYDIS_ADDRESS_WIDTH_32);
#endif // _WIN64



	// Initialize formatter. Only required when you actually plan to
	// do instruction formatting ("disassembling"), like we do here.
	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter , ZYDIS_FORMATTER_STYLE_INTEL);

	// Loop over the instructions in our buffer.
	// The IP is chosen arbitrary here in order to better visualize
	// relative addressing.

	size_t virtualAddress = reinterpret_cast<size_t>(vecMemPageInfo->memBasicInfo.BaseAddress);
	size_t offset = 0;
	size_t length = vecMemPageInfo->memBasicInfo.RegionSize;

	ZydisDecodedInstruction instruction;
	int iCount = 0;
_retry:
	while (ZYDIS_SUCCESS(ZydisDecoderDecodeBuffer(
		&decoder , (uint8_t *)vecMemPageInfo->Buffer + offset , length - offset ,
		virtualAddress , &instruction))) {


		if (iCount == 0x1000) {
			return;
		}

		if (virtualAddress > (inputAddress - 0x200)) {
			vecDisInfo.push_back((PDISINFO)calloc(1 , sizeof(DISINFO)));
			sprintf_s(vecDisInfo[iCount]->virtualAddress , 0x20 , "0x%p" , reinterpret_cast<void*>(virtualAddress));

			char tempByte[0x50] = { 0 };
			for (auto i = 0; i < instruction.length; i++) {
				sprintf_s(tempByte , _countof(tempByte) , "%02X " , instruction.data[i]);
				strcat_s(vecDisInfo[iCount]->disByte , 0x50 , tempByte);
			}
			ZydisFormatterFormatInstruction(&formatter , &instruction , vecDisInfo[iCount]->disCode , 0x100);
			iCount++;
		}

		offset += instruction.length;
		virtualAddress += instruction.length;
	}
	if (offset < length) {
		offset++;
		virtualAddress++;
		goto _retry;
	}
}

bool ParseProcess::set_se_privilege() {
	TOKEN_PRIVILEGES tp = { 0 };
	HANDLE hToken = NULL;

	if (OpenProcessToken(GetCurrentProcess() , TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY , &hToken)) {
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL , SE_DEBUG_NAME , &tp.Privileges[0].Luid)) {
			if (AdjustTokenPrivileges(hToken , FALSE , &tp , 0 , NULL , NULL) == 0) {
				if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
					return FALSE;
				}
			}
		}
		CloseHandle(hToken);
	} else {
		return false;
	}

	return true;
}




