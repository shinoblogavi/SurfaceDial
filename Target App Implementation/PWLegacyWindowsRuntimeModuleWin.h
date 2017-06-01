//!
//!	@brief	triglavwinrt���W���[��
//!
//!	@author	rinaba	
//!
#pragma once

#include "PWApplicationLib/PWModuleType.h"

#include "PWKernelLib/PWType.h"

#include <boost/noncopyable.hpp>

namespace Planeswalker
{

namespace Legacy
{

//!
//!	@brief	triglavwinrt���W���[��.
//!
class PWLegacyWindowRuntimeModuleWin
{
public:
	static	Bool		IsAvailable		();
	static	Bool		Initalize		();
	static	Bool		Terminate		();
	static	PWModuleRef	GetModule		();
};

//!
//!	@brief	triglavwinrt���W���[�����g�p����N���X�̃x�[�X.
//!
//!	@note	���̃C���X�^���X�����݂������PWLegacyWindowRuntimeModuleWin::Terminate�͎��s����.
//!
class PWLegacyWindowRuntimeCallerWin : boost::noncopyable 
{
public:
	PWLegacyWindowRuntimeCallerWin();
	virtual ~PWLegacyWindowRuntimeCallerWin();
};

}

}
