//
// skulk@ioc.ninja
//
#include <stdio.h>
#include <stdint.h>
#include <Strsafe.h>
#include <time.h>
#include <Windows.h>

#include "xLibMaliciousApi.h"

#include "xEvade01.h"
#include "xEvade02.h"
#include "xUtility.h"

#include "xSandboxDetection.h"

int wmain(int argc, wchar_t* argv[])
{
#ifdef X_EVADE_01
	xPrint(L"Evade01 - \"Fight fire with fire\"");

	xEvade01(argc, argv);
#endif

#ifdef X_EVADE_02
	xPrint(L"Evade02 - \"I'm a survivor\"");
		
	xEvade02(argc, argv);
#endif

	return 0;
}
