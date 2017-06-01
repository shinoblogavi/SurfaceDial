#include "PWApplicationLib/Legacy/PWLegacyWindowsRuntimeModuleWin.h"

#include "triglavwinrt/triglavwinrt.h"

#include "PWApplicationLib/PWSystemInfo.h"
#include "PWApplicationLib/PWModule.h"
#include "PWKernelLib/PWFilePath.h"
#include "PWKernelLib/PWAssert.h"

#include <boost/optional.hpp>

namespace Planeswalker
{

namespace Legacy
{

static const char * kModuleName = "triglavwinrt.dll";

static	PWModuleRef	s_module;			//!< ���W���[��.
static	SInt32		s_moduleCaller;		//!< ���W���[���Ăяo�����C���X�^���X��.

//!
//!	@brief	triglavwinrt.dll���g�p�\��
//!
//!	@retval	TRUE	�g�p�\
//!	@retval	FALSE	�g�p�s��
//!
Bool PWLegacyWindowRuntimeModuleWin::IsAvailable()
{
	static boost::optional<Bool> s_available;
	if ( ! s_available ) {
		s_available = FALSE;
		try {
			if ( PWSystemInfo::GetSystemKind() == kSystemKindWindows ) {
				SSystemVersion version = {};
				PWSystemInfo::GetSystemVersion( version );
				if ( version.majorVersion > 10 ) {
					s_available = TRUE;
				} else if ( version.majorVersion == 10 ) {
					if ( version.minorVersion > 0 ) {
						s_available = TRUE;
					} else if ( version.minorVersion == 0 ) {
						if ( version.bulidNumber >= 14393 ) {
							s_available = TRUE;
						}
					}
				}
			}

			if ( *s_available ) {
				PWString modulePath;
				PWFilePath::GetSpecialPath( modulePath, kFilePathSpecialPathModule );
				PWFilePath::AddLastPath( modulePath, kModuleName );
				s_available = PWFilePath::IsExist( modulePath );
			}
		} catch (...) {
			s_available = FALSE;
		}
	}

	return *s_available;
}

//!
//!	@biref	���W���[����������
//!
//!	@retval	TRUE	����
//!	@retval	TRUE	���s
//!
Bool PWLegacyWindowRuntimeModuleWin::Initalize()
{
	if ( ! PWLegacyWindowRuntimeModuleWin::IsAvailable() ) {
		return FALSE;
	}

	try {
		s_module = PWModuleRef( new PWModule( kModuleName ) );

		typedef bool ( TRIGLAV_WINRT_CALL *F )( bool );
		F f = (F)s_module->GetFunctionPointer( "winrt_initialize" );
		if ( ! f ) {
			s_module.reset();
			return FALSE;
		}

		if ( ! f( FALSE ) ) {
			s_module.reset();
			return FALSE;
		}
	} catch ( ... ) {
		return FALSE;
	}

	return TRUE;
}

//!
//!	@biref	���W���[�����I��
//!
//!	@note	PWLegacyWindowRuntimeCallerWin�����݂��Ă���Ƃ����s���܂�.
//!
//!	@retval	TRUE	����
//!	@retval	FALSE	���s
//!
Bool PWLegacyWindowRuntimeModuleWin::Terminate()
{
	if ( ! s_module ) {
		return FALSE;
	}

	if ( s_moduleCaller > 0 ) {
		FPW_ASSERT( 0 );	// ���W���[�����g�p���Ă���C���X�^���X���c���Ă��܂�.
		return FALSE;
	}

	try {
		typedef bool ( TRIGLAV_WINRT_CALL *F )( );
		F f = (F)s_module->GetFunctionPointer( "winrt_terminate" );
		if ( ! f ) {
			return FALSE;
		}

		if ( ! f() ) {
			return FALSE;
		}

		s_module.reset();
	} catch ( ... ) {
		return FALSE;
	}
	return TRUE;
}

//!
//!	@brief	triglavwinrt���W���[�����擾����.
//!
//!	@return	���W���[��.
//!
PWModuleRef PWLegacyWindowRuntimeModuleWin::GetModule()
{
	return s_module;
}

//!
//!	@brief	�R���X�g���N�^.
//!
PWLegacyWindowRuntimeCallerWin::PWLegacyWindowRuntimeCallerWin()
{
	s_moduleCaller++;
}

//!
//!	@brief	�f�X�g���N�^.
//!
PWLegacyWindowRuntimeCallerWin::~PWLegacyWindowRuntimeCallerWin()
{
	s_moduleCaller--;
}

}

}
