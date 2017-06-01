//!
//!	@brief	サーフェスダイアルDLLのラッパー
//!
//!	@author	rinaba	
//!
#pragma once

#include <boost/shared_ptr.hpp>

namespace Planeswalker
{

namespace Legacy
{

class PWLegacyDialControllerWin;
typedef boost::shared_ptr<PWLegacyDialControllerWin> PWLegacyDialControllerWinRef;

class PWLegacyDialMenuItemWin;
typedef boost::shared_ptr<PWLegacyDialMenuItemWin>	PWLegacyDialMenuItemWinRef;

class PWLegacyDialFileWin;
typedef boost::shared_ptr<PWLegacyDialFileWin>	PWLegacyDialFileWinRef;

}

}
