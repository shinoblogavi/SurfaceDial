//!
//!	@brief	サーフェスダイアルDLLのラッパー
//!
//!	@author	rinaba	
//!
#include "PWApplicationLib/Legacy/PWLegacyDialModuleWin.h"
#include "PWApplicationLib/PWModule.h"
#include "PWApplicationLib/PWSystemInfo.h"
#include "PWKernelLib/PWFilePath.h"

#include "PWKernelLib/PWAssert.h"

#include <boost/optional.hpp>

namespace Planeswalker
{

namespace Legacy
{

//!
//!	@brief	サーフェスダイヤルモジュールの初期化
//!
Bool PWLegacyDialModule::Initialize()
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
	if ( ! module ) {
		FPW_ASSERT( 0 );	// 先にPWLegacyWindowRuntimeModuleWin::Initializeを呼び出してください.
		return FALSE;
	}

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )();
	F f = (F)module->GetFunctionPointer( "dial_initialize" );
	if ( ! f ) {
		return FALSE;
	}

	if ( f() != dial_ok ) {
		return FALSE;
	}

	return TRUE;
}

//!
//!	@brief	サーフェスダイヤルモジュールの終了
//!
Bool PWLegacyDialModule::Terminate()
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
	if ( ! module ) {
		return FALSE;
	}

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( );
	F f = (F)module->GetFunctionPointer( "dial_uninitialize" );
	if ( ! f ) {
		return FALSE;
	}

	if ( f() != dial_ok ) {
		return FALSE;
	}

	return TRUE;
}

//!
//!	@breif	dial_menu_item_tからPWLegacyDialMenuItemWinを取得
//!
static PWLegacyDialMenuItemWin * GetDialMenuItemFromHandle( dial_menu_item_t *handle )
{
	if ( ! handle ) {
		return NULL;
	}

	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	PWLegacyDialMenuItemWin *item = NULL;
	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_menu_item_t *, void ** );
	F f = (F)module->GetFunctionPointer( "dial_menu_item_get_userdata" );
	if ( ! f ) {
		return NULL;
	}

	if ( f( handle, (void **)&item ) != dial_ok ) {
		return NULL;
	}
	return item;
}

//!
//!	@breif	dial_controller_tからPWLegacyDialControllerWinを取得
//!
static PWLegacyDialControllerWin * GetDialControllerFromHandle( dial_controller_t *controller )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, void ** );
	F f = (F)module->GetFunctionPointer( "dial_controller_get_userdata" );
	if ( ! f ) {
		return NULL;
	}

	PWLegacyDialControllerWin *p = NULL;
	if ( f( controller, (void **)&p ) != dial_ok ) {
		return NULL;
	}

	return p;
}

//!
//!	@brief	クリックコールバック関数
//!
static void TRIGLAV_WINRT_CALL PWLegacyDialControllerWinClicked( dial_controller_t *controller, const dial_contact_t *contact )
{
	PWLegacyDialControllerWin *c = GetDialControllerFromHandle( controller );
	PWLegacyDialControllerWin::ClickedCallback callback = c->GetClicked();
	if ( ! callback.empty() ) {
		callback( c, PWLegacyDialContactWin( contact ) );
	}
}

//!
//!	@brief	コントローラ獲得コールバック関数
//!
static void TRIGLAV_WINRT_CALL PWLegacyDialControllerWinAcquired( dial_controller_t *controller, const dial_contact_t *contact )
{
	PWLegacyDialControllerWin *c = GetDialControllerFromHandle( controller );
	PWLegacyDialControllerWin::AcquiredCallback callback = c->GetAcquired();
	if ( ! callback.empty() ) {
		callback( c, PWLegacyDialContactWin( contact ) );
	}
}

//!
//!	@brief	コントローラ喪失コールバック関数
//!
static void TRIGLAV_WINRT_CALL PWLegacyDialControllerWinControlLost( dial_controller_t *controller )
{
	PWLegacyDialControllerWin *c = GetDialControllerFromHandle( controller );
	PWLegacyDialControllerWin::LostCallback callback = c->GetLost();
	if ( ! callback.empty() ) {
		callback( c );
	}
}

//!
//!	@brief	回転コールバック関数
//!
static void TRIGLAV_WINRT_CALL PWLegacyDialControllerWinRotationChanged( dial_controller_t *controller, const dial_contact_t *contact, const dial_double_t degree )
{
	PWLegacyDialControllerWin *c = GetDialControllerFromHandle( controller );
	PWLegacyDialControllerWin::RotationChangedCallback callback = c->GetRotationChanged();
	if ( ! callback.empty() ) {
		callback( c, PWLegacyDialContactWin( contact ), degree );
	}
}

//!
//!	@brief	接触継続コールバック関数
//!
static void TRIGLAV_WINRT_CALL PWLegacyDialControllerWinContactContinue( dial_controller_t *controller, const dial_contact_t *contact )
{
	PWLegacyDialControllerWin *c = GetDialControllerFromHandle( controller );
	PWLegacyDialControllerWin::ContactContinueCallback callback = c->GetContactContinue();
	if ( ! callback.empty() ) {
		callback( c, PWLegacyDialContactWin( contact ) );
	}
}

//!
//!	@brief	接触終了コールバック関数
//!
static void TRIGLAV_WINRT_CALL PWLegacyDialControllerWinContactEnded( dial_controller_t *controller )
{
	PWLegacyDialControllerWin *c = GetDialControllerFromHandle( controller );
	PWLegacyDialControllerWin::ContactEndedCallback callback = c->GetContactEnded();
	if ( ! callback.empty() ) {
		callback( c );
	}
}

//!
//!	@brief	接触開始コールバック関数
//!
static void TRIGLAV_WINRT_CALL PWLegacyDialControllerWinContactStarted( dial_controller_t *controller, const dial_contact_t *contact )
{
	PWLegacyDialControllerWin *c = GetDialControllerFromHandle( controller );
	PWLegacyDialControllerWin::ContactStartedCallback callback = c->GetContactStarted();
	if ( ! callback.empty() ) {
		callback( c, PWLegacyDialContactWin( contact ) );
	}
}

//!
//!	@brief	コンストラクタ
//!
PWLegacyDialControllerWin::PWLegacyDialControllerWin()
	: m_controller( NULL )
{
}

//!
//!	@brief	デストラクタ
//!
PWLegacyDialControllerWin::~PWLegacyDialControllerWin()
{
	if ( m_controller ) {
		PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_clicked_f );
			F f = (F)module->GetFunctionPointer( "dial_controller_remove_clicked" );
			if ( f ) {
				f( m_controller, &PWLegacyDialControllerWinClicked );
			}
		}
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_acquired_f );
			F f = (F)module->GetFunctionPointer( "dial_controller_remove_acquired" );
			if ( f ) {
				f( m_controller, &PWLegacyDialControllerWinAcquired );
			}
		}
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_control_lost_f );
			F f = (F)module->GetFunctionPointer( "dial_controller_remove_control_lost" );
			if ( f ) {
				f( m_controller, &PWLegacyDialControllerWinControlLost );
			}
		}
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_rotation_changed_f );
			F f = (F)module->GetFunctionPointer( "dial_controller_remove_rotation_changed" );
			if ( f ) {
				f( m_controller, &PWLegacyDialControllerWinRotationChanged );
			}
		}
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_contact_continue_f );
			F f = (F)module->GetFunctionPointer( "dial_controller_remove_contact_continue" );
			if ( f ) {
				f( m_controller, &PWLegacyDialControllerWinContactContinue );
			}
		}
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_contact_ended_f );
			F f = (F)module->GetFunctionPointer( "dial_controller_remove_contact_ended" );
			if ( f ) {
				f( m_controller, &PWLegacyDialControllerWinContactEnded );
			}
		}
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_contact_started_f );
			F f = (F)module->GetFunctionPointer( "dial_controller_remove_contact_started" );
			if ( f ) {
				f( m_controller, &PWLegacyDialControllerWinContactStarted );
			}
		}
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t * );
			F f = (F)module->GetFunctionPointer( "dial_controller_release" );
			if ( ! f ) {
				return;
			}
			f( m_controller );
		}
	}
}

//!
//!	@brief	コントローラーの作成
//!
//!	@param	target	[in]作成対象
//!
//!	@return	コントローラー, 失敗するとNULL
//!
PWLegacyDialControllerWinRef PWLegacyDialControllerWin::Make( void * target )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
	if ( ! module ) {
		return PWLegacyDialControllerWinRef();
	}

	if ( ! target ) {
		return PWLegacyDialControllerWinRef();
	}

	PWLegacyDialControllerWinRef p( new PWLegacyDialControllerWin );
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_handle_t, void *, dial_controller_t ** );
		F f = (F)module->GetFunctionPointer( "dial_controller_create" );
		if ( ! f ) {
			return PWLegacyDialControllerWinRef();
		}

		if ( f( target, p.get(), (dial_controller_t **)&p->m_controller ) != dial_ok ) {
			return PWLegacyDialControllerWinRef();
		}
	}

	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_clicked_f );
		F f = (F)module->GetFunctionPointer( "dial_controller_add_clicked" );
		if ( f ) {
			f( p->m_controller, &PWLegacyDialControllerWinClicked );
		}
	}
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_acquired_f );
		F f = (F)module->GetFunctionPointer( "dial_controller_add_acquired" );
		if ( f ) {
			f( p->m_controller, &PWLegacyDialControllerWinAcquired );
		}
	}
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_control_lost_f );
		F f = (F)module->GetFunctionPointer( "dial_controller_add_control_lost" );
		if ( f ) {
			f( p->m_controller, &PWLegacyDialControllerWinControlLost );
		}
	}
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_rotation_changed_f );
		F f = (F)module->GetFunctionPointer( "dial_controller_add_rotation_changed" );
		if ( f ) {
			f( p->m_controller, &PWLegacyDialControllerWinRotationChanged );
		}
	}
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_contact_continue_f );
		F f = (F)module->GetFunctionPointer( "dial_controller_add_contact_continue" );
		if ( f ) {
			f( p->m_controller, &PWLegacyDialControllerWinContactContinue );
		}
	}
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_contact_ended_f );
		F f = (F)module->GetFunctionPointer( "dial_controller_add_contact_ended" );
		if ( f ) {
			f( p->m_controller, &PWLegacyDialControllerWinContactEnded );
		}
	}
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_controller_contact_started_f );
		F f = (F)module->GetFunctionPointer( "dial_controller_add_contact_started" );
		if ( f ) {
			f( p->m_controller, &PWLegacyDialControllerWinContactStarted );
		}
	}

	return p;
}

//!
//!	@brief	回転の取得
//!
//!	@return	回転°
//!
double PWLegacyDialControllerWin::GetRotation() const
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_double_t * );
	F f = (F)module->GetFunctionPointer( "dial_controller_get_rotation" );
	if ( ! f ) {
		return 0.0;
	}

	dial_double_t value = 0.0;
	if ( f( m_controller, &value ) != dial_ok ) {
		return 0.0;
	}

	return value;
}

//!
//!	@brief	回転の設定
//!
//!	@return	回転°
//!
void PWLegacyDialControllerWin::SetRotation( const double value )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_double_t );
	F f = (F)module->GetFunctionPointer( "dial_controller_set_rotation" );
	if ( ! f ) {
		return;
	}

	f( m_controller, value );
}

//!
//!	@brief	フィードバックの取得
//!
//!	@retval	TRUE	有
//!	@retval	FALSE	無
//!
Bool PWLegacyDialControllerWin::IsFeedback() const
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_bool_t * );
	F f = (F)module->GetFunctionPointer( "dial_controller_is_feedback" );
	if ( ! f ) {
		return FALSE;
	}

	dial_bool_t value = dial_false;
	if ( f( m_controller, &value ) != dial_ok ) {
		return FALSE;
	}

	return value;
}

//!
//!	@brief	フィードバックの設定
//!
//!	@param	value	[in]有無
//!
void PWLegacyDialControllerWin::SetFeedback( const Bool value )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_bool_t );
	F f = (F)module->GetFunctionPointer( "dial_controller_set_feedback" );
	if ( ! f ) {
		return;
	}

	if ( value ) {
		f( m_controller, dial_true );
	} else {
		f( m_controller, dial_false );
	}
}

//!
//!	@brief	メニューアイテムのリセット
//!
void PWLegacyDialControllerWin::ResetMenuItems()
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t * );
	F f = (F)module->GetFunctionPointer( "dial_controller_configuration_reset_to_default_menu_items" );
	if ( ! f ) {
		return;
	}
	f( m_controller );

	m_menuItems.clear();
}

//!
//!	@brief	メニューアイテムの設定
//!
//!	@param	menuItems	[in]メニューアイテムの配列
//!
void PWLegacyDialControllerWin::SetMenuItems( std::vector<PWLegacyDialMenuItemWinRef> menuItems )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_menu_item_t **, dial_int_t );
		F f = (F)module->GetFunctionPointer( "dial_controller_menu_set_items" );
		if ( ! f ) {
			return;
		}

		std::vector<dial_menu_item_t *> items;
		for ( std::vector<PWLegacyDialMenuItemWinRef>::iterator it = menuItems.begin(); it != menuItems.end(); ++it ) {
			dial_menu_item_t *p = ( *it )->GetItemHandle();
			items.push_back( p );
		}

		const dial_int_t count = static_cast<SInt32>( items.size() );
		if ( count > 0 ) {
			f( m_controller, &items[0], count );
		} else {
			f( m_controller, NULL, 0 );
		}

		m_menuItems = menuItems;
	}
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_default_menu_item_t *, dial_int_t );
		F f = (F)module->GetFunctionPointer( "dial_controller_configuration_set_default_menu_items" );
		if ( ! f ) {
			return;
		}

		f( m_controller, NULL, 0 );
	}
}

//!
//!	@brief	メニューアイテムの取得
//!
//!	@return	メニューアイテムの配列
//!
std::vector<PWLegacyDialMenuItemWinRef> PWLegacyDialControllerWin::GetMenuItems() const
{
	return m_menuItems;
}

//!
//!	@brief	選択中のメニューアイテムの取得
//!
//!	@return	メニューアイテム. システムメニューが選択されているときはNULLを返します.
//!
PWLegacyDialMenuItemWin * PWLegacyDialControllerWin::GetSelectedMenuItem() const
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_menu_item_t ** );
	F f = (F)module->GetFunctionPointer( "dial_controller_menu_get_selected_menu_item" );
	if ( ! f ) {
		return NULL;
	}

	dial_menu_item_t *item = NULL;
	if ( f( m_controller, &item ) == dial_ok && item ) {
		return GetDialMenuItemFromHandle( item );
	}

	return NULL;
}

//!
//!	@brief	メニューアイテムの選択
//!
//!	@param	item	[in]メニューアイテム
//!
void PWLegacyDialControllerWin::SetSelectedMenuItem( PWLegacyDialMenuItemWin *item )
{
	if ( ! item ) {
		return;
	}

	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_menu_item_t * );
	F f = (F)module->GetFunctionPointer( "dial_controller_menu_set_selected_menu_item" );
	if ( ! f ) {
		return;
	}

	dial_menu_item_t *handle = item->GetItemHandle();
	f( m_controller, handle );
}

//!
//!	@brief	前回選択していたメニューアイテムの選択を試みる
//!
void PWLegacyDialControllerWin::TrySelectPreviousMenuItem()
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t * );
	F f = (F)module->GetFunctionPointer( "dial_controller_menu_try_select_previous" );
	if ( ! f ) {
		return;
	}
	f( m_controller );
}

//!
//!	@brief	メニューアイテムが有効か
//!
//!	@retval	TRUE	有効
//!	@retval	FALSE	無効
//!
Bool PWLegacyDialControllerWin::IsEnabledMenu() const
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_bool_t * );
	F f = (F)module->GetFunctionPointer( "dial_controller_menu_is_enabled" );
	if ( ! f ) {
		return FALSE;
	}

	dial_bool_t enabled = dial_false;
	if ( f( m_controller, &enabled ) != dial_ok ) {
		return FALSE;
	}

	return enabled;
}

//!
//!	@brief	メニューアイテムが有効か設定
//!
//!	@param	enabled	[in]有効無効
//!
void PWLegacyDialControllerWin::SetEnabledMenu( Bool enabled )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_controller_t *, dial_bool_t );
	F f = (F)module->GetFunctionPointer( "dial_controller_menu_set_enabled" );
	if ( ! f ) {
		return;
	}
	if ( enabled ) {
		f( m_controller, dial_true );
	} else {
		f( m_controller, dial_true );
	}
}

//!
//!	@brief	クリックコールバックを設定
//!
//!	@param	callback	[in]コールバック関数
//!
void PWLegacyDialControllerWin::SetClicked( const ClickedCallback& callback )
{
	m_clicked = callback;
}

//!
//!	@brief	獲得コールバックを設定
//!
//!	@param	callback	[in]コールバック関数
//!
void PWLegacyDialControllerWin::SetAcquired( const AcquiredCallback& callback )
{
	m_acquired = callback;
}

//!
//!	@brief	喪失コールバックを設定
//!
//!	@param	callback	[in]コールバック関数
//!
void PWLegacyDialControllerWin::SetLost( const LostCallback& callback )
{
	m_lost = callback;
}

//!
//!	@brief	回転コールバックを設定
//!
//!	@param	callback	[in]コールバック関数
//!
void PWLegacyDialControllerWin::SetRotationChanged( const RotationChangedCallback& callback )
{
	m_rotationChanged = callback;
}

//!
//!	@brief	接触継続コールバックを設定
//!
//!	@param	callback	[in]コールバック関数
//!
void PWLegacyDialControllerWin::SetContactContinue( const ContactContinueCallback& callback )
{
	m_contactContinue = callback;
}

//!
//!	@brief	接触終了コールバックを設定
//!
//!	@param	callback	[in]コールバック関数
//!
void PWLegacyDialControllerWin::SetContactEnded( const ContactEndedCallback& callback )
{
	m_contactEnded = callback;
}

//!
//!	@brief	接触開始コールバックを設定
//!
//!	@param	callback	[in]コールバック関数
//!
void PWLegacyDialControllerWin::SetContactStart( const ContactStartedCallback& callback )
{
	m_contactStarted = callback;
}

//!
//!	@brief	クリックコールバックを取得
//!
//!	@return	コールバック関数
//!
PWLegacyDialControllerWin::ClickedCallback PWLegacyDialControllerWin::GetClicked() const
{
	return m_clicked;
}

//!
//!	@brief	獲得コールバックを取得
//!
//!	@return	コールバック関数
//!
PWLegacyDialControllerWin::AcquiredCallback PWLegacyDialControllerWin::GetAcquired() const
{
	return m_acquired;
}

//!
//!	@brief	喪失コールバックを取得
//!
//!	@return	コールバック関数
//!
PWLegacyDialControllerWin::LostCallback PWLegacyDialControllerWin::GetLost() const
{
	return m_lost;
}

//!
//!	@brief	回転コールバックを取得
//!
//!	@return	コールバック関数
//!
PWLegacyDialControllerWin::RotationChangedCallback PWLegacyDialControllerWin::GetRotationChanged() const
{
	return m_rotationChanged;
}

//!
//!	@brief	接触継続コールバックを取得
//!
//!	@return	コールバック関数
//!
PWLegacyDialControllerWin::ContactContinueCallback PWLegacyDialControllerWin::GetContactContinue() const
{
	return m_contactContinue;
}

//!
//!	@brief	接触終了コールバックを取得
//!
//!	@return	コールバック関数
//!
PWLegacyDialControllerWin::ContactEndedCallback PWLegacyDialControllerWin::GetContactEnded() const
{
	return m_contactEnded;
}

//!
//!	@brief	接触開始コールバックを取得
//!
//!	@return	コールバック関数
//!
PWLegacyDialControllerWin::ContactStartedCallback PWLegacyDialControllerWin::GetContactStarted() const
{
	return m_contactStarted;
}


//!
//!	@brief	コンストラクタ
//!
PWLegacyDialFileWin::PWLegacyDialFileWin()
	: m_file( NULL )
{
}

//!
//!	@brief	デストラクタ
//!
PWLegacyDialFileWin::~PWLegacyDialFileWin()
{
	if ( m_file ) {
		PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_file_t * );
		F f = (F)module->GetFunctionPointer( "dial_file_release" );
		if ( f ) {
			f( m_file );
		}
	}
}

//!
//!	@brief	PWLegacyDialFileWinの作成
//!
//!	@param	filePath	[in]ファイルパス
//!	@param	callback	[in]作成コールバック関数
//!
//!	@retval	TRUE	成功. callbackが呼び出されます
//!	@retval	FALSE	失敗. callbackは呼び出されません
//!
//!	@note	作成が完了したときコールバックが呼び出されPWLegacyDialFileWinを受け取ります.
//!
Bool PWLegacyDialFileWin::Make( const PWString& filePath, const MakedCallback& callback )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
	if ( ! module ) {
		return FALSE;
	}

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( const wchar_t *, void *, dial_file_created_f );
	F f = (F)module->GetFunctionPointer( "dial_file_create" );
	if ( f ) {
		PWLegacyDialFileWin *file = new PWLegacyDialFileWin;
		file->m_path = filePath;
		file->m_callback = callback;
		const wchar_t *fileCharacter = reinterpret_cast<const wchar_t *>(filePath.GetCString());
		if ( f( fileCharacter, file, PWLegacyDialFileWin::Callback ) != dial_ok ) {
			delete file;
		}
	}

	return TRUE;
}

//!
//!	@brief	dial_file_tのコールバック実装
//!
void TRIGLAV_WINRT_CALL PWLegacyDialFileWin::Callback( dial_file_t *file, dial_result_t result )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();

	PWLegacyDialFileWin *dialFile = NULL;
	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_file_t *, void ** );
		F f = (F)module->GetFunctionPointer( "dial_file_get_userdata" );
		if ( f ) {
			f( file, reinterpret_cast<void **>(&dialFile) );
		}
	}

	if ( dialFile ) {
		PWLegacyDialFileWinRef dialFileRef( dialFile );
		if ( result == dial_ok ) {
			dialFile->m_file = file;
			dialFile->m_callback( dialFileRef );
		}
	}
}

//!
//!	@brief	ファイルパスを取得
//!
//!	@return	ファイルパス
//!
PWString PWLegacyDialFileWin::GetFilePath() const
{
	return m_path;
}

//!
//!	@brief	dial_file_tを取得
//!
//!	@return	dial_file_t
//!
dial_file_t * PWLegacyDialFileWin::GetFileHandle() const
{
	return m_file;
}

//!
//!	@brief	メニューアイテム選択コールバックの実装
//!
static void TRIGLAV_WINRT_CALL PWLegacyDialMenuItemWinInvoked( dial_menu_item_t *item )
{
	PWLegacyDialMenuItemWin *p = GetDialMenuItemFromHandle( item );
	PWLegacyDialMenuItemWin::InvokedCallback callback = p->GetInvoked();
	if ( ! callback.empty() ) {
		callback( p );
	}
}

//!
//!	@brief	コンストラクタ
//!
PWLegacyDialMenuItemWin::PWLegacyDialMenuItemWin()
	: m_item( NULL )
{
}

//!
//!	@brief	デストラクタ
//!
PWLegacyDialMenuItemWin::~PWLegacyDialMenuItemWin()
{
	if ( m_item ) {
		PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_menu_item_t *, dial_menu_item_invoked_f );
			F f = (F)module->GetFunctionPointer( "dial_menu_item_remove_invoked" );
			if ( f ) {
				f( m_item, &PWLegacyDialMenuItemWinInvoked );
			}
		}
		{
			typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_menu_item_t * );
			F f = (F)module->GetFunctionPointer( "dial_menu_item_release" );
			if ( f ) {
				f( m_item );
			}
		}
	}
}

//!
//!	@brief	ダイヤルメニューアイテムの作成
//!
//!	@param	name	[in]表示名
//!	@param	iconFile[in]表示アイコンのファイル
//!
//!	@return	ダイヤルメニューアイテム. 失敗するとNULL.
//!
PWLegacyDialMenuItemWinRef PWLegacyDialMenuItemWin::Make( const PWString& name, PWLegacyDialFileWinRef iconFile )
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
	if ( ! module ) {
		return PWLegacyDialMenuItemWinRef();
	}

	PWLegacyDialMenuItemWinRef p( new PWLegacyDialMenuItemWin );

	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( const wchar_t *, dial_file_t *, void *, dial_menu_item_t ** );
		F f = (F)module->GetFunctionPointer( "dial_menu_item_create_with_icon" );
		if ( ! f ) {
			return NULL;
		}

		const wchar_t *nameCharacter = (const wchar_t *)name.GetCString();
		if ( f( nameCharacter, iconFile->GetFileHandle(), (void *)p.get(), &p->m_item ) != dial_ok ) {
			return NULL;
		}
	}

	{
		typedef dial_result_t ( TRIGLAV_WINRT_CALL *F )( dial_menu_item_t *, dial_menu_item_invoked_f );
		F f = (F)module->GetFunctionPointer( "dial_menu_item_add_invoked" );
		if ( f ) {
			f( p->m_item, &PWLegacyDialMenuItemWinInvoked );
		}
	}

	return p;
}

//!
//!	@brief	dial_menu_item_tの取得
//!
dial_menu_item_t * PWLegacyDialMenuItemWin::GetItemHandle() const
{
	return m_item;
}

//!
//!	@brief	メニューが選択されたコールバック関数の設定
//!
//!	@param	callback	[in]コールバック関数
//!
void PWLegacyDialMenuItemWin::SetInvoked( const InvokedCallback& callback )
{
	m_invoked = callback;
}

//!
//!	@brief	メニューが選択されたコールバック関数の取得
//!
//!	@return	コールバック関数
//!
PWLegacyDialMenuItemWin::InvokedCallback PWLegacyDialMenuItemWin::GetInvoked() const
{
	return m_invoked;
}

//!
//!	@brief	コンストラクタ
//!
//!	@param	contact	[in]コンタクト
//!
PWLegacyDialContactWin::PWLegacyDialContactWin( const dial_contact_t *contact )
	: m_valid( FALSE ), m_bounds( FPWMakeEmptyRectD() ), m_position( FPWMakeZeroPointD() )
{
	if ( ! contact ) {
		return;
	}
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
	if ( ! module ) {
		return;
	}

	m_valid = TRUE;

	typedef dial_result_t ( TRIGLAV_WINRT_CALL *dial_contact_get_bounds_f )(const dial_contact_t *, const dial_rect_t **);
	typedef dial_result_t ( TRIGLAV_WINRT_CALL *dial_contact_get_position )(const dial_contact_t *, const dial_point_t **);
	typedef dial_result_t ( TRIGLAV_WINRT_CALL *dial_rect_get_x )(const dial_rect_t *, const dial_double_t *);
	typedef dial_result_t ( TRIGLAV_WINRT_CALL *dial_rect_get_y )(const dial_rect_t *, const dial_double_t *);
	typedef dial_result_t ( TRIGLAV_WINRT_CALL *dial_rect_get_width )(const dial_rect_t *, const dial_double_t *);
	typedef dial_result_t ( TRIGLAV_WINRT_CALL *dial_rect_get_height )(const dial_rect_t *, const dial_double_t *);
	typedef dial_result_t ( TRIGLAV_WINRT_CALL *dial_point_get_x )(const dial_point_t *, const dial_double_t *);
	typedef dial_result_t ( TRIGLAV_WINRT_CALL *dial_point_get_y )(const dial_point_t *, const dial_double_t *);
	dial_contact_get_bounds_f get_bounds = (dial_contact_get_bounds_f)module->GetFunctionPointer( "dial_contact_get_bounds" );
	dial_contact_get_position get_position = (dial_contact_get_position)module->GetFunctionPointer( "dial_contact_get_position" );
	dial_rect_get_x get_rect_x = (dial_rect_get_x)module->GetFunctionPointer( "dial_rect_get_x" );
	dial_rect_get_y get_rect_y = (dial_rect_get_y)module->GetFunctionPointer( "dial_rect_get_y" );
	dial_rect_get_width get_rect_width = (dial_rect_get_width)module->GetFunctionPointer( "dial_rect_get_width" );
	dial_rect_get_height get_rect_height = (dial_rect_get_height)module->GetFunctionPointer( "dial_rect_get_height" );
	dial_point_get_x get_point_x = (dial_point_get_x)module->GetFunctionPointer( "dial_point_get_x" );
	dial_point_get_y get_point_y = (dial_point_get_y)module->GetFunctionPointer( "dial_point_get_y" );
	if ( get_bounds ) {
		const dial_rect_t *bounds = NULL;
		if ( get_bounds( contact, &bounds ) == dial_ok && bounds ) {
			dial_double_t x = 0.0;
			dial_double_t y = 0.0;
			dial_double_t w = 0.0;
			dial_double_t h = 0.0;
			if ( get_rect_x ) {
				get_rect_x( bounds, &x );
			}
			if ( get_rect_y ) {
				get_rect_y( bounds, &y );
			}
			if ( get_rect_width ) {
				get_rect_width( bounds, &w );
			}
			if ( get_rect_height ) {
				get_rect_height( bounds, &h );
			}
			m_bounds = FPWMakeRectD( x, y, x + w, y + h );
		}
	}
	if ( get_position ) {
		const dial_point_t *position = NULL;
		if ( get_position( contact, &position ) == dial_ok && position ) {
			dial_double_t x = 0.0;
			dial_double_t y = 0.0;
			if ( get_point_x ) {
				get_point_x( position, &x );
			}
			if ( get_point_y ) {
				get_point_y( position, &y );
			}
			m_position = FPWMakePointD( x, y );
		}
	}
}

//!
//!	@brief	BoundsとPosistionは有効か
//!
//!	@retval	TRUE	有効
//!	@retval	FALSE	無効
//!
Bool PWLegacyDialContactWin::IsValid() const
{
	return m_valid;
}

//!
//!	@brief	接触領域を取得
//!
//!	@return	領域
//!
SRectD PWLegacyDialContactWin::GetBounds() const
{
	return m_bounds;
}

//!
//!	@brief	接触位置を取得
//!
//!	@return	位置
//!
SPointD PWLegacyDialContactWin::GetPosition() const
{
	return m_position;
}


}

}
