/**********************************************************/
//
// File: windows_dps.hpp (windows dependencies)
//
// Purpose: windows header includes
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#define NOMINMAX // Prevents min/max macros from being defined
#include <windows.h>
#include <gdiplus.h>
#include <Richedit.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")