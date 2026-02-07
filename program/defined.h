//! ===================================================================
//! @file defined.h
//! @details 定義.
//! 
//! @author yuzuki
//! ===================================================================
#pragma once
#include <windows.h>
#include <cstdio>
#include <crtdbg.h>

namespace kk
{
#define KK_PRINT(fmt, ...) { char buf[256]; sprintf_s(buf, fmt, __VA_ARGS__); OutputDebugStringA(buf); }

}