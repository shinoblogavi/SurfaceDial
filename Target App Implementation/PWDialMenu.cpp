#include "PWApplicationLib/PWDialMenu.h"


namespace Planeswalker
{

PWDialMenuItem::PWDialMenuItem( const UInt32 menuID, const PWString& name, const PWString& iconFilePath )
: m_menuID( menuID ), m_name( name ), m_iconFilePath( iconFilePath ),
m_rotation( 0.0 ), m_feedback( TRUE )
{
}

UInt32 PWDialMenuItem::GetMenuID() const
{
	return m_menuID;
}

PWString PWDialMenuItem::GetName() const
{
	return m_name;
}

PWString PWDialMenuItem::GetIconFilePath() const
{
	return m_iconFilePath;
}

void PWDialMenuItem::SetRotation( const double degree )
{
	m_rotation = degree;
}

double PWDialMenuItem::GetRotation() const
{
	return m_rotation;
}

void PWDialMenuItem::SetFeedback( const Bool use )
{
	m_feedback = use;
}

Bool PWDialMenuItem::IsFeedback() const
{
	return m_feedback;
}


PWDialMenu PWDialMenu::DefaultMenu;

PWDialMenu::PWDialMenu()
{
}

PWDialMenu::PWDialMenu( std::vector<PWDialMenuItem> menuItems )
	: m_menuItems( menuItems )
{
}

std::vector<PWDialMenuItem> PWDialMenu::GetMenuItems() const
{
	return m_menuItems;
}

PWDialMenuItem PWDialMenu::FindMenuItem( const UInt32 menuID, Bool &found ) const
{
	class L {
		UInt32 m_menuID;
	public:
		L( const UInt32 menuID ) : m_menuID( menuID ) {};
		bool operator()( const PWDialMenuItem& other ) {
			return m_menuID == other.GetMenuID();
		};
	};
	std::vector<PWDialMenuItem>::const_iterator it = std::find_if( m_menuItems.begin(), m_menuItems.end(), L( menuID ) );
	if ( it == m_menuItems.end() ) {
		found = FALSE;
	}
	
	found = TRUE;
	return *it;
}

}
