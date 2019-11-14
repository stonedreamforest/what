#pragma once
#ifndef _DEBUGER_H_
#define _DEBUGER_H_
#include <windows.h>
#include <vector>
#include <utility>
class Debuger {
public:
	Debuger();
	~Debuger();

	void run();
	void stop();
	void set_bp(void* Address);
	bool unset_bp(void* Address);
public:
	HANDLE m_hProcess = nullptr;
private:
	unsigned char m_cli = 0xfa;
	std::vector<std::pair<void*,unsigned char>> m_vecBPList;
};












#endif // !_DEBUGER_H_
