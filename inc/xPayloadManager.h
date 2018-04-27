#pragma once



#include <stdint.h>



bool xPayloadManagerDrop(const wchar_t* fileName);

bool xPayloadManagerDropAndExecute(const wchar_t* fileName);

bool xPayloadManagerGetPayload(uint8_t** payload, uint32_t* payloadSize);
