
#define TRIGLAV_WINRT_API __declspec(dllexport)
#include "triglavwinrt.h"

#include "stdafx.h"

#include <roapi.h>

bool TRIGLAV_WINRT_CALL winrt_initialize( bool multithread )
{
	try {
		HRESULT hr = S_OK;

		if ( multithread ) {
			hr = Windows::Foundation::Initialize( RO_INIT_MULTITHREADED );
		} else {
			hr = Windows::Foundation::Initialize( RO_INIT_SINGLETHREADED );
		}
		if ( FAILED( hr ) ) {
			return false;
		}
	} catch (...) {
		return false;
	}
	return true;
}

bool TRIGLAV_WINRT_CALL winrt_terminate()
{
	try {
		Windows::Foundation::Uninitialize();
	} catch (...) {
		return false;
	}
	return true;
}
