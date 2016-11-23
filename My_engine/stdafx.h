#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers.
#endif

#include <DirectXMath.h>
using namespace DirectX;

#include <stdlib.h>
#include <iostream>
#include <tchar.h>
#include <assert.h>
#include "Windows.h"

using namespace std;

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw;
	}
}

