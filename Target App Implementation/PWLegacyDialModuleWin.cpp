//!
//!	@brief	�T�[�t�F�X�_�C�A��DLL�̃��b�p�[
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
//!	@brief	�T�[�t�F�X�_�C�������W���[���̏�����
//!
Bool PWLegacyDialModule::Initialize()
{
	PWModuleRef module = PWLegacyWindowRuntimeModuleWin::GetModule();
	if ( ! module ) {
		FPW_ASSERT( 0 );	// ���PWLegacyWindowRuntimeModuleWin::Initialize���Ăяo���Ă�������.
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
//!	@brief	�T�[�t�F�X�_�C�������W���[���̏I��
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
//!	@breif	dial_menu_item_t����PWLegacyDialMenuItemWin���擾
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
//!	@breif	dial_controller_t����PWLegacyDialControllerWin���擾
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
//!	@brief	�N���b�N�R�[���o�b�N�֐�
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
//!	@brief	�R���g���[���l���R�[���o�b�N�֐�
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
//!	@brief	�R���g���[���r���R�[���o�b�N�֐�
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
//!	@brief	��]�R�[���o�b�N�֐�
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
//!	@brief	�ڐG�p���R�[���o�b�N�֐�
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
//!	@brief	�ڐG�I���R�[���o�b�N�֐�
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
//!	@brief	�ڐG�J�n�R�[���o�b�N�֐�
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
//!	@brief	�R���X�g���N�^
//!
PWLegacyDialControllerWin::PWLegacyDialControllerWin()
	: m_controller( NULL )
{
}

//!
//!	@brief	�f�X�g���N�^
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
//!	@brief	�R���g���[���[�̍쐬
//!
//!	@param	target	[in]�쐬�Ώ�
//!
//!	@return	�R���g���[���[, ���s�����NULL
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
//!	@brief	��]�̎擾
//!
//!	@return	��]��
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
//!	@brief	��]�̐ݒ�
//!
//!	@return	��]��
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
//!	@brief	�t�B�[�h�o�b�N�̎擾
//!
//!	@retval	TRUE	�L
//!	@retval	FALSE	��
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
//!	@brief	�t�B�[�h�o�b�N�̐ݒ�
//!
//!	@param	value	[in]�L��
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
//!	@brief	���j���[�A�C�e���̃��Z�b�g
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
//!	@brief	���j���[�A�C�e���̐ݒ�
//!
//!	@param	menuItems	[in]���j���[�A�C�e���̔z��
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
//!	@brief	���j���[�A�C�e���̎擾
//!
//!	@return	���j���[�A�C�e���̔z��
//!
std::vector<PWLegacyDialMenuItemWinRef> PWLegacyDialControllerWin::GetMenuItems() const
{
	return m_menuItems;
}

//!
//!	@brief	�I�𒆂̃��j���[�A�C�e���̎擾
//!
//!	@return	���j���[�A�C�e��. �V�X�e�����j���[���I������Ă���Ƃ���NULL��Ԃ��܂�.
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
//!	@brief	���j���[�A�C�e���̑I��
//!
//!	@param	item	[in]���j���[�A�C�e��
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
//!	@brief	�O��I�����Ă������j���[�A�C�e���̑I�������݂�
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
//!	@brief	���j���[�A�C�e�����L����
//!
//!	@retval	TRUE	�L��
//!	@retval	FALSE	����
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
//!	@brief	���j���[�A�C�e�����L�����ݒ�
//!
//!	@param	enabled	[in]�L������
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
//!	@brief	�N���b�N�R�[���o�b�N��ݒ�
//!
//!	@param	callback	[in]�R�[���o�b�N�֐�
//!
void PWLegacyDialControllerWin::SetClicked( const ClickedCallback& callback )
{
	m_clicked = callback;
}

//!
//!	@brief	�l���R�[���o�b�N��ݒ�
//!
//!	@param	callback	[in]�R�[���o�b�N�֐�
//!
void PWLegacyDialControllerWin::SetAcquired( const AcquiredCallback& callback )
{
	m_acquired = callback;
}

//!
//!	@brief	�r���R�[���o�b�N��ݒ�
//!
//!	@param	callback	[in]�R�[���o�b�N�֐�
//!
void PWLegacyDialControllerWin::SetLost( const LostCallback& callback )
{
	m_lost = callback;
}

//!
//!	@brief	��]�R�[���o�b�N��ݒ�
//!
//!	@param	callback	[in]�R�[���o�b�N�֐�
//!
void PWLegacyDialControllerWin::SetRotationChanged( const RotationChangedCallback& callback )
{
	m_rotationChanged = callback;
}

//!
//!	@brief	�ڐG�p���R�[���o�b�N��ݒ�
//!
//!	@param	callback	[in]�R�[���o�b�N�֐�
//!
void PWLegacyDialControllerWin::SetContactContinue( const ContactContinueCallback& callback )
{
	m_contactContinue = callback;
}

//!
//!	@brief	�ڐG�I���R�[���o�b�N��ݒ�
//!
//!	@param	callback	[in]�R�[���o�b�N�֐�
//!
void PWLegacyDialControllerWin::SetContactEnded( const ContactEndedCallback& callback )
{
	m_contactEnded = callback;
}

//!
//!	@brief	�ڐG�J�n�R�[���o�b�N��ݒ�
//!
//!	@param	callback	[in]�R�[���o�b�N�֐�
//!
void PWLegacyDialControllerWin::SetContactStart( const ContactStartedCallback& callback )
{
	m_contactStarted = callback;
}

//!
//!	@brief	�N���b�N�R�[���o�b�N���擾
//!
//!	@return	�R�[���o�b�N�֐�
//!
PWLegacyDialControllerWin::ClickedCallback PWLegacyDialControllerWin::GetClicked() const
{
	return m_clicked;
}

//!
//!	@brief	�l���R�[���o�b�N���擾
//!
//!	@return	�R�[���o�b�N�֐�
//!
PWLegacyDialControllerWin::AcquiredCallback PWLegacyDialControllerWin::GetAcquired() const
{
	return m_acquired;
}

//!
//!	@brief	�r���R�[���o�b�N���擾
//!
//!	@return	�R�[���o�b�N�֐�
//!
PWLegacyDialControllerWin::LostCallback PWLegacyDialControllerWin::GetLost() const
{
	return m_lost;
}

//!
//!	@brief	��]�R�[���o�b�N���擾
//!
//!	@return	�R�[���o�b�N�֐�
//!
PWLegacyDialControllerWin::RotationChangedCallback PWLegacyDialControllerWin::GetRotationChanged() const
{
	return m_rotationChanged;
}

//!
//!	@brief	�ڐG�p���R�[���o�b�N���擾
//!
//!	@return	�R�[���o�b�N�֐�
//!
PWLegacyDialControllerWin::ContactContinueCallback PWLegacyDialControllerWin::GetContactContinue() const
{
	return m_contactContinue;
}

//!
//!	@brief	�ڐG�I���R�[���o�b�N���擾
//!
//!	@return	�R�[���o�b�N�֐�
//!
PWLegacyDialControllerWin::ContactEndedCallback PWLegacyDialControllerWin::GetContactEnded() const
{
	return m_contactEnded;
}

//!
//!	@brief	�ڐG�J�n�R�[���o�b�N���擾
//!
//!	@return	�R�[���o�b�N�֐�
//!
PWLegacyDialControllerWin::ContactStartedCallback PWLegacyDialControllerWin::GetContactStarted() const
{
	return m_contactStarted;
}


//!
//!	@brief	�R���X�g���N�^
//!
PWLegacyDialFileWin::PWLegacyDialFileWin()
	: m_file( NULL )
{
}

//!
//!	@brief	�f�X�g���N�^
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
//!	@brief	PWLegacyDialFileWin�̍쐬
//!
//!	@param	filePath	[in]�t�@�C���p�X
//!	@param	callback	[in]�쐬�R�[���o�b�N�֐�
//!
//!	@retval	TRUE	����. callback���Ăяo����܂�
//!	@retval	FALSE	���s. callback�͌Ăяo����܂���
//!
//!	@note	�쐬�����������Ƃ��R�[���o�b�N���Ăяo����PWLegacyDialFileWin���󂯎��܂�.
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
//!	@brief	dial_file_t�̃R�[���o�b�N����
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
//!	@brief	�t�@�C���p�X���擾
//!
//!	@return	�t�@�C���p�X
//!
PWString PWLegacyDialFileWin::GetFilePath() const
{
	return m_path;
}

//!
//!	@brief	dial_file_t���擾
//!
//!	@return	dial_file_t
//!
dial_file_t * PWLegacyDialFileWin::GetFileHandle() const
{
	return m_file;
}

//!
//!	@brief	���j���[�A�C�e���I���R�[���o�b�N�̎���
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
//!	@brief	�R���X�g���N�^
//!
PWLegacyDialMenuItemWin::PWLegacyDialMenuItemWin()
	: m_item( NULL )
{
}

//!
//!	@brief	�f�X�g���N�^
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
//!	@brief	�_�C�������j���[�A�C�e���̍쐬
//!
//!	@param	name	[in]�\����
//!	@param	iconFile[in]�\���A�C�R���̃t�@�C��
//!
//!	@return	�_�C�������j���[�A�C�e��. ���s�����NULL.
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
//!	@brief	dial_menu_item_t�̎擾
//!
dial_menu_item_t * PWLegacyDialMenuItemWin::GetItemHandle() const
{
	return m_item;
}

//!
//!	@brief	���j���[���I�����ꂽ�R�[���o�b�N�֐��̐ݒ�
//!
//!	@param	callback	[in]�R�[���o�b�N�֐�
//!
void PWLegacyDialMenuItemWin::SetInvoked( const InvokedCallback& callback )
{
	m_invoked = callback;
}

//!
//!	@brief	���j���[���I�����ꂽ�R�[���o�b�N�֐��̎擾
//!
//!	@return	�R�[���o�b�N�֐�
//!
PWLegacyDialMenuItemWin::InvokedCallback PWLegacyDialMenuItemWin::GetInvoked() const
{
	return m_invoked;
}

//!
//!	@brief	�R���X�g���N�^
//!
//!	@param	contact	[in]�R���^�N�g
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
//!	@brief	Bounds��Posistion�͗L����
//!
//!	@retval	TRUE	�L��
//!	@retval	FALSE	����
//!
Bool PWLegacyDialContactWin::IsValid() const
{
	return m_valid;
}

//!
//!	@brief	�ڐG�̈���擾
//!
//!	@return	�̈�
//!
SRectD PWLegacyDialContactWin::GetBounds() const
{
	return m_bounds;
}

//!
//!	@brief	�ڐG�ʒu���擾
//!
//!	@return	�ʒu
//!
SPointD PWLegacyDialContactWin::GetPosition() const
{
	return m_position;
}


}

}
