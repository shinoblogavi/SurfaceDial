//!
//! @file  URDialMenu.h
//! @brief SurfaceDialのメニューの設定
//! @date 2017/02/14
//! @author S.Yuasa
//!
#pragma once

#include "Urza/URDialMenuType.h"
#include "PWApplicationLib/PWDialMenuType.h"
#include <vector>

namespace Planeswalker
{

namespace Urza
{
//!
//! @brief SurfaceDialのメニューの設定
//!
//! @author S.Yuasa
//!
class URDialMenu
{
private:
	URDialMenu();		// コンストラクトしない
	PLANESWALKER_DISALLOW_COPY_AND_ASSIGN(URDialMenu);

public:
	static void		InitializeMenu		(std::vector<PWDialMenuItem>& menuItems);

};

}

}
