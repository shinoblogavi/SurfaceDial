//!
//! @file  PWDiaymenuEventParameter.h
//! @brief ダイヤルメニューイベントパラメータクラスヘッダー
//!
//! @author rinaba
//!
#pragma once

#include "PWApplicationLib/PWDialMenuEventParameterType.h"
#include "PWApplicationLib/PWEventType.h"
#include "PWBasicStructLib/PWPoint.h"
#include "PWBasicStructLib/PWRect.h"

namespace Planeswalker
{

//!
//! @brief	ダイヤルメニューイベントパラメータクラス
//!
//! @author rinaba
//!
class	PWDialMenuEventParameter
{
public:
							PWDialMenuEventParameter();
	explicit				PWDialMenuEventParameter( const EventKind idleEventKind );
	explicit				PWDialMenuEventParameter( const PWEvent& rEvent );
							PWDialMenuEventParameter( const PWDialMenuEventParameter& rIdleEventParameter );
	virtual					~PWDialMenuEventParameter();

	static	Bool			IsDialMenuEventKind( const EventKind eventKind );

			void			SetDialMenuEventKind( const EventKind eventKind );
			UInt32			GetDialMenuEventKind() const;

			void			SetMenuID( const UInt32 menuID );
			UInt32			GetMenuID() const;
			void			SetRotatin( const double degree );
			double			GetRoation() const;
			void			SetContact( const Bool contact );
			Bool			IsContact() const;
			void			SetBounds( const SRect& bounds );
			SRect			GetBounds() const;
			void			SetPosition( const SPoint& position );
			SPoint			GetPosition() const;

			void			SetEvent( const PWEvent& rEvent ); 
			void			GetEvent( PWEvent& rEvent );
private:
			void			InitializeMembers();

			struct	SDialMenuEventParameter
			{
				UInt32	menuID;
				double	rotation;
				Bool	contact;
				SRect	bounds;
				SPoint	position;
			};
			SDialMenuEventParameter	m_parameter;
			EventKind				m_dialMenuEventKind;
};

}
