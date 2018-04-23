//
// skulk@ioc.ninja
//
#include <stdio.h>
#include <stdint.h>
#include <Strsafe.h>
#include <Windows.h>

#include "xLibMaliciousApi.h"
#include "xOsApi.h"

#include "xUtility.h"



void __cdecl xPrint(const wchar_t* message, ...)
{
	if (message)
	{
		va_list args;

		va_start(args, message);

		size_t size = 256;

		wchar_t* temp = NULL;

		while (true)
		{
			if (!(temp = (wchar_t*) X_LIB_MALICIOUS_CALL(xMemoryAlloc)(size)))
			{
				return;
			}

			HRESULT hr = StringCbVPrintf(temp, size * sizeof(wchar_t), message, args);

			if (FAILED(hr))
			{
				X_LIB_MALICIOUS_CALL(xMemoryFree)(temp);

				if (hr != STRSAFE_E_INSUFFICIENT_BUFFER)
				{
					return;
				}

				size *= 2;
			}
			else
			{
				break;
			}
		}

		size_t length = wcslen(temp);

		size = (length + 2) * sizeof(wchar_t);

		wchar_t* buffer = (wchar_t*) X_LIB_MALICIOUS_CALL(xMemoryAlloc)(size);

		if (buffer)
		{
			memset(buffer, 0, size);

			wsprintf(buffer, L"%s\n", temp);

			wprintf(buffer);

			X_KERNEL32_CALL(OutputDebugString)(buffer);

			X_LIB_MALICIOUS_CALL(xMemoryFree)(buffer);
		}

		va_end(args);
	}
}
