//!
//!	@brief	Windows Runtime APIを呼び出すDLL
//!
#pragma once


#ifndef TRIGLAV_WINRT_API
#define TRIGLAV_WINRT_API __declspec(dllimport)
#endif
#define TRIGLAV_WINRT_CALL __cdecl

extern "C" {

//!
//!	@brief	ライブラリの初期化. ほかのDLL関数を呼び出す前に必ず呼び出してください.
//!
//!	@param	multithread	[in]Windows::Foundation::InitializeをMultithreadで初期化するかどうか.
//!
//!	@retval	true	成功
//!	@retval	false	失敗
//!
TRIGLAV_WINRT_API bool TRIGLAV_WINRT_CALL winrt_initialize( bool multithread );

//!
//!	@brief	ライブラリの終了処理. 呼び出し後、ほかのあらゆる関数を呼び出してはいけません.
//!
//!	@retval	true	成功
//!	@retval	false	失敗
//!
TRIGLAV_WINRT_API bool TRIGLAV_WINRT_CALL winrt_terminate();

}
