//!
//!
//!
//!
#pragma once

#include "PWApplicationLib/PWDialMenuType.h"
#include "PWKernelLib/PWString.h"
#include <vector>

namespace Planeswalker
{

class PWDialMenuItem {
public:
	PWDialMenuItem	( const UInt32 menuID, const PWString& name, const PWString& iconFilePath );

	UInt32		GetMenuID		() const;
	PWString	GetName			() const;
	PWString	GetIconFilePath	() const;

	void		SetRotation	( const double degree );
	double		GetRotation	() const;
	void		SetFeedback	( const Bool use );
	Bool		IsFeedback	() const;
private:
	UInt32		m_menuID;
	PWString	m_name;
	PWString	m_iconFilePath;
	double		m_rotation;
	Bool		m_feedback;
};

class PWDialMenu {
public:
	PWDialMenu();
	PWDialMenu( std::vector<PWDialMenuItem> menuItems );
	std::vector<PWDialMenuItem>	GetMenuItems	() const;
	PWDialMenuItem FindMenuItem( const UInt32 menuID, Bool& found ) const;

	static	PWDialMenu	DefaultMenu;
private:
	std::vector<PWDialMenuItem> m_menuItems;
};

}
