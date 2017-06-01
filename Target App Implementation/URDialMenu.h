//!
//! @file  URDialMenu.h
//! @brief SurfaceDial�̃��j���[�̐ݒ�
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
//! @brief SurfaceDial�̃��j���[�̐ݒ�
//!
//! @author S.Yuasa
//!
class URDialMenu
{
private:
	URDialMenu();		// �R���X�g���N�g���Ȃ�
	PLANESWALKER_DISALLOW_COPY_AND_ASSIGN(URDialMenu);

public:
	static void		InitializeMenu		(std::vector<PWDialMenuItem>& menuItems);

};

}

}
