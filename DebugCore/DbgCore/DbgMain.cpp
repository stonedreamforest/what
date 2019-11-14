#include <windows.h>
#include "stdio.h"

#pragma warning(disable:4477)
#pragma warning(disable:4313)


#pragma comment(lib,"ntdll.lib")
EXTERN_C NTSYSAPI LONG NTAPI NtSuspendProcess(HANDLE ProcessHandle);
EXTERN_C NTSYSAPI LONG NTAPI NtResumeProcess(HANDLE ProcessHandle);


void output_debug_info(PEXCEPTION_RECORD ExceptionRecord , PCONTEXT ContextRecord) {
	__try {
	#ifdef _WIN64
		printf_s(" Rax: 0x%p\n Rbx: 0x%p\n Rcx: 0x%p\n Rdx: 0x%p\n Rbp: 0x%p\n Rsp: 0x%p\n Rsi: 0x%p\n Rdi: 0x%p\n "
				 "R8: 0x%p\n R9: 0x%p\n R10: 0x%p\n R11: 0x%p\n R12: 0x%p\n R13: 0x%p\n R14: 0x%p\n R15: 0x%p\n "
				 ">>>Rip: 0x%p\n "
				 "SegGs: 0x%p\n SegFs: 0x%p\n SegEs: 0x%p\n SegDs: 0x%p\n SegCs: 0x%p\n SegSs: 0x%p\n" ,
				 ContextRecord->Rax ,
				 ContextRecord->Rbx ,
				 ContextRecord->Rcx ,
				 ContextRecord->Rdx ,
				 ContextRecord->Rbp ,
				 ContextRecord->Rsp ,
				 ContextRecord->Rsi ,
				 ContextRecord->Rdi ,
				 ContextRecord->R8 ,
				 ContextRecord->R9 ,
				 ContextRecord->R10 ,
				 ContextRecord->R11 ,
				 ContextRecord->R12 ,
				 ContextRecord->R13 ,
				 ContextRecord->R14 ,
				 ContextRecord->R15 ,
				 ContextRecord->Rip ,
				 ContextRecord->SegGs ,
				 ContextRecord->SegFs ,
				 ContextRecord->SegEs ,
				 ContextRecord->SegDs ,
				 ContextRecord->SegCs ,
				 ContextRecord->SegSs);
	#else
		printf_s(" Eax: 0x%p\n Ebx: 0x%p\n Ecx: 0x%p\n Edx: 0x%p\n Ebp: 0x%p\n Esp: 0x%p\n Esi: 0x%p\n Edi: 0x%p\n "
				 ">>>Eip: 0x%p\n "
				 "SegGs: 0x%p\n SegFs: 0x%p\n SegEs: 0x%p\n SegDs: 0x%p\n SegCs: 0x%p\n SegSs: 0x%p\n" ,
				 ContextRecord->Eax ,
				 ContextRecord->Ebx ,
				 ContextRecord->Ecx ,
				 ContextRecord->Edx ,
				 ContextRecord->Ebp ,
				 ContextRecord->Esp ,
				 ContextRecord->Esi ,
				 ContextRecord->Edi ,
				 ContextRecord->Eip ,
				 ContextRecord->SegGs ,
				 ContextRecord->SegFs ,
				 ContextRecord->SegEs ,
				 ContextRecord->SegDs ,
				 ContextRecord->SegCs ,
				 ContextRecord->SegSs);
	#endif // _WIN64
		puts("------------------------------------------------------------------");
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		printf_s("出现异常代码: 0x%p\n" , GetExceptionCode());
	}
}











LONG CALLBACK debug_event_vectored_handler(_In_ PEXCEPTION_POINTERS ExceptionInfo) {
	auto ExceptionRecord = ExceptionInfo->ExceptionRecord;
	auto ContextRecord = ExceptionInfo->ContextRecord;
	if (ExceptionRecord->ExceptionCode == EXCEPTION_PRIV_INSTRUCTION) {
	#pragma omp critical
		{
			output_debug_info(ExceptionRecord , ContextRecord);
			NtSuspendProcess((HANDLE)-1);
		}
	}
	return EXCEPTION_CONTINUE_EXECUTION;
}




BOOL WINAPI	DllMain(_In_ HINSTANCE hInstance , _In_ DWORD fdwReason , _In_ LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hInstance);
		AddVectoredExceptionHandler(1 , debug_event_vectored_handler);
		FILE *stream;
		AllocConsole();
		freopen_s(&stream , "con" , "w" , stdout);
		printf_s("成功启动\n");
	}
	return TRUE;
}