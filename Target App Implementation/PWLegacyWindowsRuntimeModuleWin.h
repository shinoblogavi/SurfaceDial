//!
//!	@brief	triglavwinrtモジュール
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
//!	@brief	triglavwinrtモジュール.
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
//!	@brief	triglavwinrtモジュールを使用するクラスのベース.
//!
//!	@note	このインスタンスが存在する限りPWLegacyWindowRuntimeModuleWin::Terminateは失敗する.
//!
class PWLegacyWindowRuntimeCallerWin : boost::noncopyable 
{
public:
	PWLegacyWindowRuntimeCallerWin();
	virtual ~PWLegacyWindowRuntimeCallerWin();
};

}

}
