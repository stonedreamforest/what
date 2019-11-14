#include "Debuger.h"

#pragma comment(lib,"ntdll.lib")
EXTERN_C NTSYSAPI LONG NTAPI NtSuspendProcess(HANDLE ProcessHandle);
EXTERN_C NTSYSAPI LONG NTAPI NtResumeProcess(HANDLE ProcessHandle);


Debuger::Debuger() {
}


Debuger::~Debuger() {
}

void Debuger::run() {
	if (m_hProcess != nullptr) {
		NtResumeProcess(m_hProcess);
	}
}

void Debuger::stop() {
	if (m_hProcess != nullptr) {
		NtSuspendProcess(m_hProcess);
	}
}

void Debuger::set_bp(void* Address) {
	if (m_hProcess != nullptr && Address != nullptr) {
		DWORD Old = 0;
		unsigned char byteCode = 0;
		SIZE_T BytesRead = 0;
		SIZE_T BytesWritten = 0;
		NtSuspendProcess(m_hProcess);
		ReadProcessMemory(m_hProcess , Address , &byteCode , 1 , &BytesRead);
		VirtualProtectEx(m_hProcess , Address , 1 , PAGE_EXECUTE_READWRITE , &Old);
		WriteProcessMemory(m_hProcess , Address , &m_cli , 1 , &BytesWritten);
		VirtualProtectEx(m_hProcess , Address , 1 , Old , &Old);
		m_vecBPList.push_back(std::make_pair(Address , byteCode));
		NtResumeProcess(m_hProcess);

	}
}

bool Debuger::unset_bp(void* Address) {
	if (m_hProcess != nullptr && Address != nullptr) {
		for (size_t i = 0; i < m_vecBPList.size(); i++) {
			if (Address == m_vecBPList[i].first) {
				DWORD Old = 0;
				SIZE_T BytesWritten = 0;
				VirtualProtectEx(m_hProcess , Address , 1 , PAGE_EXECUTE_READWRITE , &Old);
				WriteProcessMemory(m_hProcess , Address , &m_vecBPList[i].second , 1 , &BytesWritten);
				VirtualProtectEx(m_hProcess , Address , 1 , Old , &Old);
				m_vecBPList.erase(m_vecBPList.begin() + i);
				return true;
			}
		}
	}
	return false;
}
