//!
//! @file  PWLegacyWindowWin.cpp
//! @brief 共通フレームワークウインドウ　Windowsソース
//!
//! @author dkobayashi
//!
#include "PWApplicationLib/Legacy/PWLegacyWindow.h"
#include "PWApplicationLib/Legacy/PWLegacyWindowWin.h"

namespace Planeswalker
{

//! @brief	ウインドウオブジェクトのWinでの内容
struct	SLegacyWindow
{
	Legacy::SLegacyWindowEventCallback	eventCallback;
	UInt32								windowLayer;
	UInt32								windowStyle;
	UInt32								windowBehavior;
	UInt32								windowOption;

	//　一部省略...

	Legacy::PWLegacyDialControllerWinRef	dialController;
};
typedef	SLegacyWindow* PWLegacyWindowObject;


static	PWDialMenu											s_dialMenu;				//!< サーフェスダイヤルに設定するメニュー
static	std::vector<PWLegacyDialFileWinRef>					s_dialFiles;			//!< メニューアイテムが使用するファイル
static	std::vector<PWLegacyDialMenuItemWinRef>				s_dialMenuItems;		//!< メニューアイテム配列
static	std::map<PWLegacyDialMenuItemWin *, UInt32>			s_dialMenuIDMap;		//!< メニューアイテム, menuIDマップ
static	std::map<PWLegacyDialMenuItemWin *, UInt32>::iterator	s_dialMenuSelectedItem;	//!< 選択状態のメニューアイテム


//!
//!	@brief	 サーフェスダイヤルメニューが使用可能になった
//!
static void PWLegacyWindowDialMenuAcquireCallback( PWLegacyDialControllerWin *controller, const PWLegacyDialContactWin& contact )
{
	try {
		if ( s_dialMenuSelectedItem != s_dialMenuIDMap.end() ) {
			// 選択状態のメニューに合わせてコントローラを設定.
			const UInt32 menuID = (*s_dialMenuSelectedItem).second;
			Bool found = FALSE;
			PWDialMenuItem item = s_dialMenu.FindMenuItem( menuID, found );
			if ( found ) {
				const double rotation = item.GetRotation();
				const Bool feedback = item.IsFeedback();
				controller->SetRotation( rotation );
				controller->SetFeedback( feedback );
			}
		}	
	} catch (...) {
	}
}

//!
//!	@brief	サーフェスダイヤルメニューが選択された
//!
static void PWLegacyWindowDialMenuInvokedCallback( PWLegacyDialMenuItemWin *legacyItem )
{
	try {
		std::map<PWLegacyDialMenuItemWin *, UInt32>::iterator it = s_dialMenuIDMap.find( legacyItem );
		if ( it != s_dialMenuIDMap.end() && it != s_dialMenuSelectedItem ) {
			// メニューの選択状態を保持.
			s_dialMenuSelectedItem = it;

			// イベント送信
			const UInt32 menuID = (*s_dialMenuSelectedItem).second;
			const SRect bounds = FPWMakeEmptyRect();
			const SPoint position = FPWMakeZeroPoint();
			PWLegacyApplicationObject applicaionObject = PWLegacyApplicationWin::GetApplication();
			PWLegacyApplicationWin::SendEventDialMenu( applicaionObject, kLegacyApplicationEventDialMenuSelected, menuID, 0.0, FALSE, bounds, position );
		}	
	} catch (...) {
	}
}

//!
//!	@brief	サーフェスダイヤルメニュー汎用コールバック実装
//!
static void PWLegacyWindowDialMenuCallback( PWLegacyDialControllerWin *controller, const UInt32 legacyEventKind )
{
	try {
		if ( s_dialMenuSelectedItem != s_dialMenuIDMap.end() ) {
			// イベント送信.
			const UInt32 menuID = (*s_dialMenuSelectedItem).second;
			const Bool isContact = FALSE;
			const SRect bounds = FPWMakeEmptyRect();
			const SPoint position = FPWMakeZeroPoint();
			PWLegacyApplicationObject applicaionObject = PWLegacyApplicationWin::GetApplication();
			PWLegacyApplicationWin::SendEventDialMenu( applicaionObject, legacyEventKind, menuID, 0.0, isContact, bounds, position );
		}
	} catch (...) {
	}
}

//!
//!	@brief	サーフェスダイヤル接触コールバック	
//!
static void PWLegacyWindowDialMenuWithContactCallback( PWLegacyDialControllerWin *controller, const PWLegacyDialContactWin& contact, const UInt32 legacyEventKind )
{
	try {
		if ( s_dialMenuSelectedItem != s_dialMenuIDMap.end() ) {
			// 接触イベント送信
			const UInt32 menuID = (*s_dialMenuSelectedItem).second;
			const Bool isContact = contact.IsValid();
			const SRectD boundsD = contact.GetBounds();
			const SRect bounds = FPWCastToIntRectExpand( boundsD );
			const SPointD positionD = contact.GetPosition();
			const SPoint position = FPWCastToIntPointFloor( positionD );
			PWLegacyApplicationObject applicaionObject = PWLegacyApplicationWin::GetApplication();
			PWLegacyApplicationWin::SendEventDialMenu( applicaionObject, legacyEventKind, menuID, 0.0, isContact, bounds, position );
		}
	} catch (...) {
	}
}

//!
//!	@brief	サーフェスダイヤル回転コールバック実装
//!
static void PWLegacyWindowDialMenuRotationCallback( PWLegacyDialControllerWin *controller, const PWLegacyDialContactWin& contact, const double rotation )
{
	try {
		if ( s_dialMenuSelectedItem != s_dialMenuIDMap.end() ) {
			// 回転イベント送信.
			const UInt32 menuID = (*s_dialMenuSelectedItem).second;
			const Bool isContact = contact.IsValid();
			const SRectD boundsD = contact.GetBounds();
			const SRect bounds = FPWCastToIntRectExpand( boundsD );
			const SPointD positionD = contact.GetPosition();
			const SPoint position = FPWCastToIntPointFloor( positionD );
			PWLegacyApplicationObject applicaionObject = PWLegacyApplicationWin::GetApplication();
			PWLegacyApplicationWin::SendEventDialMenu( applicaionObject, kLegacyApplicationEventDialMenuRotate, menuID, rotation, isContact, bounds, position );
		}
	} catch (...) {
	}
}

//!
//!	@brief	hParentから子ウインドウを再帰的に辿りLegacyWindowに対して関数fを適用します.
//!
static void PWLegacyWindowApplyFunction( const ::HWND hParent, const boost::function<void (PWLegacyWindowObject)>& f )
{
	if ( hParent && PWLegacyWindowIsLegacyWindow( hParent ) ) {
		PWLegacyWindowObject windowObject = PWLegacyWindowGetLegacyWindowObject( hParent );
		f( windowObject );
	}

	::HWND hChild = NULL;
	while ( (hChild = ::FindWindowExW( hParent, hChild, NULL, NULL )) != NULL ) {
		PWLegacyWindowApplyFunction( hChild, f );
	}
}

//!
//!	@brief	ファイル作成コールバック
//!
static void PWLegacyDialFileWinMakedCallback( PWLegacyDialFileWinRef file )
{
	if ( ! file ) {
		return;
	}

	// LegacyDialFileとファイルパスの＝
	class EqualPredicate {
		PWString m_filePath;
	public:
		EqualPredicate( const PWString& filePath ) : m_filePath( filePath ) {};
		bool operator()( PWLegacyDialFileWinRef file ) const {
			if ( file ) {
				const PWString path = file->GetFilePath();
				if ( PWFilePath::IsEqualFilePath( m_filePath, path ) ) {
					return true;
				}
			}
			return false;
		};
	};

	// 既に同じファイルがあれば以前のファイルを消す.
	{
		const PWString filePath = file->GetFilePath();
		std::vector<PWLegacyDialFileWinRef>::iterator found = std::find_if( s_dialFiles.begin(), s_dialFiles.end(), EqualPredicate( filePath ) );
		if ( found != s_dialFiles.end() ) {
			s_dialFiles.erase( found );
		}
	}

	s_dialFiles.push_back( file );

	// すべてのメニューアイテムに対するファイルが揃っているか.
	const std::vector<PWDialMenuItem> items = s_dialMenu.GetMenuItems();
	for ( std::vector<PWDialMenuItem>::const_iterator it = items.begin(); it != items.end(); ++it ) {
		const PWString filePath = (*it).GetIconFilePath();
		std::vector<PWLegacyDialFileWinRef>::iterator found = std::find_if( s_dialFiles.begin(), s_dialFiles.end(), EqualPredicate( filePath ) );
		if ( found == s_dialFiles.end() ) {
			return;
		}
	}

	s_dialMenuSelectedItem = s_dialMenuIDMap.end();

	// メニューアイテムを作成.
	for ( std::vector<PWDialMenuItem>::const_iterator it = items.begin(); it != items.end(); ++it ) {
		const PWString filePath = (*it).GetIconFilePath();
		std::vector<PWLegacyDialFileWinRef>::iterator found = std::find_if( s_dialFiles.begin(), s_dialFiles.end(), EqualPredicate( filePath ) );
		if ( found != s_dialFiles.end() ) {
			PWLegacyDialMenuItemWinRef item = PWLegacyDialMenuItemWin::Make( (*it).GetName(), *found );
			if ( item ) {
				item->SetInvoked( &PWLegacyWindowDialMenuInvokedCallback );
				s_dialMenuItems.push_back( item );

				const std::pair<PWLegacyDialMenuItemWin *, UInt32> e( item.get(), (*it).GetMenuID() );
				const std::map<PWLegacyDialMenuItemWin *, UInt32>::iterator pos = s_dialMenuIDMap.insert( e ).first;
				if ( s_dialMenuSelectedItem == s_dialMenuIDMap.end() ) {
					// 先頭のアイテムを選択状態にする.
					s_dialMenuSelectedItem = pos;
				}
			}
		}
	}

	// サーフェスダイヤルメニューを設定.
	PWLegacyWindowApplyFunction( NULL, &PWLegacyWindowWin::ApplyDialMenu );
	PWLegacyWindowApplyFunction( NULL, &PWLegacyWindowWin::ApplyDialMenuSelection );
}

//!
//!	@brief	サーフェスダイヤルメニューを設定します.
//!
//!	@param	dialMenu	[in]ダイヤルメニュー
//!
//!	@note	実際の反映まで時間がかかることがあります.
//!
void PWLegacyWindowWin::SetDialMenu( const PWDialMenu& dialMenu )
{
	s_dialMenu = dialMenu;
	s_dialMenuItems.clear();
	s_dialMenuIDMap.clear();
	s_dialMenuSelectedItem = s_dialMenuIDMap.end();
	s_dialFiles.clear();

	if ( PWLegacyWindowRuntimeModuleWin::IsAvailable() ) {
		const std::vector<PWDialMenuItem> menuItems = s_dialMenu.GetMenuItems();
		if ( menuItems.empty() ) {
			// リセット.
			PWLegacyWindowApplyFunction( NULL, &PWLegacyWindowWin::ApplyDialMenu );
		} else {
			// まずファイルを生成する, コールバックでメニューは設定.
			for ( std::vector<PWDialMenuItem>::const_iterator it = menuItems.begin(); it != menuItems.end(); ++it ) {
				const PWString filePath = (*it).GetIconFilePath();
				PWLegacyDialFileWin::Make( filePath, PWLegacyDialFileWinMakedCallback );
			}
		}
	}
}

//!
//!	@brief	アプリケーションに設定されているサーフェスダイヤルメニューアイテムを対象のウインドウに適用します.
//!
//!	@param	windowObject	[in]対象のウインドウ.
//!
//!	@note	適用されるウインドウはkWindowBehaviorMain,kWindowBehaviorPalette,kWindowBehaviorDocumentのいずれか.
//!	@note	ほかのウインドウは無視されます.
//!
void PWLegacyWindowWin::ApplyDialMenu( PWLegacyWindowObject windowObject )
{
	if ( ! windowObject ) {
		return;
	}

	// 適用するウインドウの種類を限定する.
	const UInt32 behavior = PWLegacyWindow::GetWindowBehavior( windowObject );
	if ( ! ( behavior == kWindowBehaviorMain || behavior == kWindowBehaviorPalette || behavior == kWindowBehaviorDocument ) ) {
		return;
	}

	// コントローラの作成
	PWLegacyDialControllerWinRef dialController = windowObject->dialController;
	if ( ! dialController ) {
		if ( s_dialMenuItems.empty() ) {
			return;
		}

		const HWND hwnd = PWLegacyWindowWin::GetWindowHandle( windowObject );
		dialController = PWLegacyDialControllerWin::Make( hwnd );
		windowObject->dialController = dialController;

		dialController->SetAcquired( &PWLegacyWindowDialMenuAcquireCallback );
		dialController->SetClicked( boost::bind( &PWLegacyWindowDialMenuWithContactCallback, _1, _2, kLegacyApplicationEventDialMenuClick ) );
		dialController->SetContactStart( boost::bind( &PWLegacyWindowDialMenuWithContactCallback, _1, _2, kLegacyApplicationEventDialMenuContactBegin ) );
		dialController->SetContactContinue( boost::bind( &PWLegacyWindowDialMenuWithContactCallback, _1, _2, kLegacyApplicationEventDialMenuContactContinue ) );
		dialController->SetContactEnded( boost::bind( &PWLegacyWindowDialMenuCallback, _1, kLegacyApplicationEventDialMenuContactEnd ) );
		dialController->SetRotationChanged( &PWLegacyWindowDialMenuRotationCallback );
	}

	if ( ! dialController ) {
		return;
	}

	// メニューを設定.
	if ( s_dialMenuItems.empty() ) {
		dialController->ResetMenuItems();
	} else {
		dialController->SetMenuItems( s_dialMenuItems );
	}
}

//!
//!	@brief	アプリケーションが選択しているサーフェスダイヤルメニューアイテムを対象のウインドウに適用します.
//!
//!	@param	windowObject	[in]対象のウインドウ.
//!
void PWLegacyWindowWin::ApplyDialMenuSelection( PWLegacyWindowObject windowObject )
{
	if ( ! windowObject ) {
		return;
	}

	PWLegacyDialControllerWinRef dialController = windowObject->dialController;
	if ( ! dialController ) {
		return;
	}

	if ( s_dialMenuSelectedItem == s_dialMenuIDMap.end() ) {
		return;
	}

	// 選択状態を設定.
	if ( (*s_dialMenuSelectedItem).first != dialController->GetSelectedMenuItem() ) {
		dialController->SetSelectedMenuItem( (*s_dialMenuSelectedItem).first );
	}
}

}

}
