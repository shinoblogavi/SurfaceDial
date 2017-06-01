//!
//! @file  PWDialMenuEventParameter.cpp
//! @brief ダイヤルメニューイベントパラメータクラスソース
//!
//! @author rinaba
//!
#include "PWApplicationLib/PWDialMenuEventParameter.h"
#include "PWApplicationLib/PWProcessUniqueKey.h"
#include "PWApplicationLib/PWEvent.h"

namespace Planeswalker
{
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuSelected);			//!< ダイヤルメニューが選択された
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuClick);				//!< ダイヤルをクリックした
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuRotate);				//!< ダイヤルを回転した
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuContactBegin);		//!< ダイヤルを画面に接触した
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuContactContinue);	//!< ダイヤルを画面上で動かした
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuContactEnd);			//!< ダイヤルを画面から離した

static	PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kDialMenuEventParameterKeyParameter);	//!< ダイヤルメニューパラメータ.

//-----------------------------
//!
//! @brief コンストラクタ.
//! 
//-----------------------------
PWDialMenuEventParameter::PWDialMenuEventParameter()
{
	InitializeMembers();
}

//-----------------------------
//!
//! @brief	引数付コンストラクタ(イベント種類).
//! @param	dialMenuEventKind	[in]イベント種類.
//! 
//-----------------------------
PWDialMenuEventParameter::PWDialMenuEventParameter( const EventKind dialMenuEventKind )
{
	InitializeMembers();
	SetDialMenuEventKind(dialMenuEventKind);
}

//-----------------------------
//!
//! @brief	引数付コンストラクタ(イベント).
//! @param	rEvent	[in]セットするイベント.
//! 
//-----------------------------
PWDialMenuEventParameter::PWDialMenuEventParameter( const PWEvent& rEvent )
{
	InitializeMembers();
	SetEvent(rEvent);
}

//-----------------------------
//!
//! @brief コピーコンストラクタ.
//! 
//-----------------------------
PWDialMenuEventParameter::PWDialMenuEventParameter( const PWDialMenuEventParameter& rDialMenuEventParameter )
{
	InitializeMembers();
	*this	= rDialMenuEventParameter;
}

//-----------------------------
//!
//! @brief ディストラクタ.
//! 
//-----------------------------
PWDialMenuEventParameter::~PWDialMenuEventParameter()
{
}

//-----------------------------
//!
//! @brief メンバー変数の初期化.
//! 
//-----------------------------
void PWDialMenuEventParameter::InitializeMembers()
{
	m_dialMenuEventKind = 0;

	SDialMenuEventParameter parameter = {};
	m_parameter = parameter;
}

//-----------------------------
//!
//! @brief	アイドルイベントか.
//! @param	eventKind	[in]イベント種類.
//! 
//-----------------------------
Bool PWDialMenuEventParameter::IsDialMenuEventKind( const EventKind eventKind )
{
	return
		eventKind == kEventKindDialMenuSelected ||
		eventKind == kEventKindDialMenuClick ||
		eventKind == kEventKindDialMenuRotate ||
		eventKind == kEventKindDialMenuContactBegin ||
		eventKind == kEventKindDialMenuContactContinue ||
		eventKind == kEventKindDialMenuContactEnd;
}

//-----------------------------
//!
//! @brief	ダイヤルメニューイベントの種類を設定.
//!	@param	dialMenuEventKind	[in]	ダイヤルメニューイベントの種類.
//! 
//-----------------------------
void PWDialMenuEventParameter::SetDialMenuEventKind( const EventKind dialMenuEventKind )
{
	if( IsDialMenuEventKind(dialMenuEventKind) )
	{
		m_dialMenuEventKind	= dialMenuEventKind;
	}
}

//-----------------------------
//!
//! @brief	ダイヤルメニューイベントの種類を取得.
//! @return	ダイヤルメニューイベントの種類.
//! 
//-----------------------------
UInt32 PWDialMenuEventParameter::GetDialMenuEventKind() const
{
	return m_dialMenuEventKind;
}

//!
//!	@brief	ダイヤルメニューIDの設定
//!
void PWDialMenuEventParameter::SetMenuID( const UInt32 menuID )
{
	m_parameter.menuID = menuID;
}

//!
//!	@brief	ダイヤルメニューIDの取得
//!
UInt32 PWDialMenuEventParameter::GetMenuID() const
{
	return m_parameter.menuID;
}

//!
//!	@brief	回転角度設定
//!
void PWDialMenuEventParameter::SetRotatin( const double degree )
{
	m_parameter.rotation = degree;
}

//!
//!	@brief	回転角度取得
//!
double PWDialMenuEventParameter::GetRoation() const
{
	return m_parameter.rotation;
}

//!
//!	@brief	画面に接触してるか設定
//!
void PWDialMenuEventParameter::SetContact( const Bool contact )
{
	m_parameter.contact = contact;
}

//!
//!	@brief	画面に接触してるか取得
//!
Bool PWDialMenuEventParameter::IsContact() const
{
	return m_parameter.contact;
}

//!
//!	@brief	画面に接触してる領域の設定
//!
void PWDialMenuEventParameter::SetBounds( const SRect& bounds )
{
	m_parameter.bounds = bounds;
}

//!
//!	@brief	画面に接触してる領域の取得
//!
SRect PWDialMenuEventParameter::GetBounds() const
{
	return m_parameter.bounds;
}

//!
//!	@brief	画面に接触してる位置の設定
//!
void PWDialMenuEventParameter::SetPosition( const SPoint& position )
{
	m_parameter.position = position;
}

//!
//!	@brief	画面に接触している位置を取得
//!
SPoint PWDialMenuEventParameter::GetPosition() const
{
	return m_parameter.position;
}

//-----------------------------
//!
//! @brief	イベントの設定.
//! @param	rEvent	[in]セットするイベント.
//! 
//-----------------------------
void PWDialMenuEventParameter::SetEvent( const PWEvent& rEvent )
{
	InitializeMembers();
	if( IsDialMenuEventKind(rEvent.GetEventKind()) )
	{
		m_dialMenuEventKind	= rEvent.GetEventKind();
		rEvent.GetValue(kDialMenuEventParameterKeyParameter,m_parameter);
	}
}

//-----------------------------
//!
//! @brief	イベントの取得.
//! @param	rEvent	[out]取得したイベント.
//! 
//-----------------------------
void PWDialMenuEventParameter::GetEvent( PWEvent& rEvent )
{
	rEvent.SetEventKind(m_dialMenuEventKind);
	rEvent.SetValue(kDialMenuEventParameterKeyParameter,m_parameter);
}

}
