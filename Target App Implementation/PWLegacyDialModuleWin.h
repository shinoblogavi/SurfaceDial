//!
//!	@brief	サーフェスダイアルDLLのラッパー
//!
//!	@author	rinaba	
//!
#pragma once

#include "PWApplicationLib/Legacy/PWLegacyDialModuleTypeWin.h"
#include "PWApplicationLib/Legacy/PWLegacyWindowsRuntimeModuleWin.h"
#include "PWBasicStructLib/PWRect.h"
#include "PWBasicStructLib/PWPoint.h"
#include "PWKernelLib/PWString.h"

#include "triglavwinrt/dial.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <vector>

namespace Planeswalker
{

namespace Legacy
{

class PWLegacyDialContactWin;

//!
//!	@brief	ダイヤルモジュール
//!
class PWLegacyDialModule {
	PWLegacyDialModule();
public:
	static	Bool	Initialize	();
	static	Bool	Terminate	();
};

//!
//!	@brief	ダイヤルコントローラ
//!
class PWLegacyDialControllerWin : PWLegacyWindowRuntimeCallerWin {
	PWLegacyDialControllerWin	();
public:
	~PWLegacyDialControllerWin	();
	static PWLegacyDialControllerWinRef Make( void *target );

	double	GetRotation		() const;
	void	SetRotation		( const double value );
	Bool	IsFeedback		() const;
	void	SetFeedback		( const Bool value );

	void						ResetMenuItems				();
	void						SetMenuItems				( std::vector<PWLegacyDialMenuItemWinRef> menuItems );
	std::vector<PWLegacyDialMenuItemWinRef>	GetMenuItems		() const;
	PWLegacyDialMenuItemWin *				GetSelectedMenuItem	() const;
	void						SetSelectedMenuItem			( PWLegacyDialMenuItemWin *item );
	void						TrySelectPreviousMenuItem	();
	Bool						IsEnabledMenu				() const;
	void						SetEnabledMenu				( Bool enabled );

	typedef  boost::function<void ( PWLegacyDialControllerWin *, const PWLegacyDialContactWin& )>	ClickedCallback;
	typedef  boost::function<void ( PWLegacyDialControllerWin *, const PWLegacyDialContactWin& )>	AcquiredCallback;
	typedef  boost::function<void ( PWLegacyDialControllerWin * )>									LostCallback;
	typedef  boost::function<void ( PWLegacyDialControllerWin *, const PWLegacyDialContactWin&, const double )>	RotationChangedCallback;
	typedef  boost::function<void ( PWLegacyDialControllerWin *, const PWLegacyDialContactWin& )>	ContactContinueCallback;
	typedef  boost::function<void ( PWLegacyDialControllerWin * )>									ContactEndedCallback;
	typedef  boost::function<void ( PWLegacyDialControllerWin *, const PWLegacyDialContactWin& )>	ContactStartedCallback;

	void	SetClicked			( const ClickedCallback& callback );
	void	SetAcquired			( const AcquiredCallback& callback );
	void	SetLost				( const LostCallback& callback );
	void	SetRotationChanged	( const RotationChangedCallback& callback );
	void	SetContactContinue	( const ContactContinueCallback& callback );
	void	SetContactEnded		( const ContactEndedCallback& callback );
	void	SetContactStart		( const ContactStartedCallback& callback );

	ClickedCallback			GetClicked() const;
	AcquiredCallback		GetAcquired() const;
	LostCallback			GetLost() const;
	RotationChangedCallback	GetRotationChanged() const;
	ContactContinueCallback	GetContactContinue() const;
	ContactEndedCallback	GetContactEnded() const;
	ContactStartedCallback	GetContactStarted() const;

private:
	dial_controller_t	 *	m_controller;
	std::vector<PWLegacyDialMenuItemWinRef>	m_menuItems;

	ClickedCallback			m_clicked;
	AcquiredCallback		m_acquired;
	LostCallback			m_lost;
	RotationChangedCallback	m_rotationChanged;
	ContactContinueCallback	m_contactContinue;
	ContactEndedCallback	m_contactEnded;
	ContactStartedCallback	m_contactStarted;
};

//!
//!	@brief	ダイヤルファイル
//!
class PWLegacyDialFileWin : PWLegacyWindowRuntimeCallerWin {
	PWLegacyDialFileWin();
public:
	~PWLegacyDialFileWin();

	typedef boost::function<void (PWLegacyDialFileWinRef)>	MakedCallback;
	static	Bool	Make( const PWString& filePath, const MakedCallback& callback );

	PWString		GetFilePath	() const;
	dial_file_t *	GetFileHandle() const;
private:
	static void TRIGLAV_WINRT_CALL Callback( dial_file_t *file, dial_result_t result );

	MakedCallback	m_callback;
	dial_file_t *	m_file;
	PWString		m_path;
};

//!
//!	@brief	ダイヤルメニューアイテム
//!
class PWLegacyDialMenuItemWin : PWLegacyWindowRuntimeCallerWin {
	PWLegacyDialMenuItemWin	();
public:
	~PWLegacyDialMenuItemWin	();
	static PWLegacyDialMenuItemWinRef	Make( const PWString& name, PWLegacyDialFileWinRef iconFile );

	typedef boost::function<void ( PWLegacyDialMenuItemWin *item )>	InvokedCallback;
	void					SetInvoked( const InvokedCallback& callback );
	InvokedCallback			GetInvoked() const;

	dial_menu_item_t *		GetItemHandle	() const;
private:
	dial_menu_item_t *	m_item;
	InvokedCallback		m_invoked;
};

//!
//!	@brief	ダイヤルコンタクト
//!
class PWLegacyDialContactWin {
public:
	PWLegacyDialContactWin( const dial_contact_t *contact );
	Bool	IsValid		() const;
	SRectD	GetBounds	() const;
	SPointD	GetPosition	() const;
private:
	Bool	m_valid;
	SRectD	m_bounds;
	SPointD	m_position;
};


}

}
