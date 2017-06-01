//!
//!	@brief	Windows Runtime API���Ăяo��DLL
//!
#pragma once


#ifndef TRIGLAV_WINRT_API
#define TRIGLAV_WINRT_API __declspec(dllimport)
#endif
#define TRIGLAV_WINRT_CALL __cdecl

extern "C" {

//!
//!	@brief	���C�u�����̏�����. �ق���DLL�֐����Ăяo���O�ɕK���Ăяo���Ă�������.
//!
//!	@param	multithread	[in]Windows::Foundation::Initialize��Multithread�ŏ��������邩�ǂ���.
//!
//!	@retval	true	����
//!	@retval	false	���s
//!
TRIGLAV_WINRT_API bool TRIGLAV_WINRT_CALL winrt_initialize( bool multithread );

//!
//!	@brief	���C�u�����̏I������. �Ăяo����A�ق��̂�����֐����Ăяo���Ă͂����܂���.
//!
//!	@retval	true	����
//!	@retval	false	���s
//!
TRIGLAV_WINRT_API bool TRIGLAV_WINRT_CALL winrt_terminate();

}
