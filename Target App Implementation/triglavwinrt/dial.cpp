#define TRIGLAV_WINRT_API __declspec(dllexport)

#include "dial.h"
#include "stdafx.h"

#include <Pathcch.h>
#include <roapi.h>
#include <wrl.h>
#include <windows.ui.input.h>
#include <RadialControllerInterop.h>

#include <map>

struct _dial_file {
	HSTRING path;
	void *userdata;
	Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFile>	storageFile;
};

struct _dial_menu_item {
	void *userdata;
	Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerMenuItem> radial_item;
	std::map<dial_menu_item_invoked_f, EventRegistrationToken> token_invoked;
};

struct _dial_rect {
	ABI::Windows::Foundation::Rect	rect;
};

struct _dial_point {
	ABI::Windows::Foundation::Point	point;
};

struct _dial_contact {
	dial_rect_t		bounds;
	dial_point_t	position;
};

struct _dial_controller {
	void * userdata;
	Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialController>				radial_controller;
	Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerMenu>			radial_menu;
	Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerConfiguration> radial_configuration;
	std::map<dial_controller_clicked_f, EventRegistrationToken>						token_clicked;
	std::map<dial_controller_acquired_f, EventRegistrationToken>					token_acquired;
	std::map<dial_controller_control_lost_f, EventRegistrationToken>				token_control_lost;
	std::map<dial_controller_rotation_changed_f, EventRegistrationToken>			token_rotation_changed;
	std::map<dial_controller_contact_continue_f, EventRegistrationToken>			token_contact_continue;
	std::map<dial_controller_contact_ended_f, EventRegistrationToken>				token_contact_ended;
	std::map<dial_controller_contact_started_f, EventRegistrationToken>				token_contact_started;
	std::map<ABI::Windows::UI::Input::IRadialControllerMenuItem *, dial_menu_item_t *>	item_map;
};

template <class T>
class EmptyIterator : public Microsoft::WRL::RuntimeClass<ABI::Windows::Foundation::Collections::IIterator<T> > {
	virtual /* propget */ HRESULT STDMETHODCALLTYPE get_Current( _Out_ T *current )
	{
		current = nullptr;
		return S_OK;
	}

	virtual /* propget */ HRESULT STDMETHODCALLTYPE get_HasCurrent( _Out_ boolean *hasCurrent )
	{
		*hasCurrent = false;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE MoveNext( _Out_ boolean *hasCurrent )
	{
		hasCurrent = false;
		return S_OK;
	}
};

template <class T>
class EmptyIterable : public Microsoft::WRL::RuntimeClass<ABI::Windows::Foundation::Collections::IIterable<T> > {
	virtual HRESULT STDMETHODCALLTYPE First( _Outptr_result_maybenull_ ABI::Windows::Foundation::Collections::IIterator<T> **first )
	{
		Microsoft::WRL::ComPtr<EmptyIterator<T> > it = Microsoft::WRL::Make<EmptyIterator<T> >();
		*first = it.Detach();
		return S_OK;
	};
};

Microsoft::WRL::ComPtr<IRadialControllerInterop>											s_radialControllerInterop;
Microsoft::WRL::ComPtr<IRadialControllerConfigurationInterop>								s_radialControllerConfigurationInterop;
Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerStatics>					s_radialControllerStatics;
Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerMenuItemStatics>			s_radialControllerMenuItemStatics;
Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFileStatics>							s_storageFileStatics;
Microsoft::WRL::ComPtr<ABI::Windows::Storage::Streams::IRandomAccessStreamReferenceStatics>	s_randomAccessStreamReferenceStatics;

dial_result_t TRIGLAV_WINRT_CALL dial_initialize()
{
	try {
		HRESULT hr = S_OK;

		hr = Windows::Foundation::GetActivationFactory(
			Microsoft::WRL::Wrappers::HStringReference( RuntimeClass_Windows_UI_Input_RadialController ).Get(),
			&s_radialControllerInterop
		);
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		hr = Windows::Foundation::GetActivationFactory(
			Microsoft::WRL::Wrappers::HStringReference( RuntimeClass_Windows_UI_Input_RadialController ).Get(),
			&s_radialControllerStatics
		);
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		hr = Windows::Foundation::GetActivationFactory(
			Microsoft::WRL::Wrappers::HStringReference( RuntimeClass_Windows_UI_Input_RadialControllerMenuItem ).Get(),
			&s_radialControllerMenuItemStatics
		);
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		hr = Windows::Foundation::GetActivationFactory(
			Microsoft::WRL::Wrappers::HStringReference( RuntimeClass_Windows_UI_Input_RadialControllerConfiguration ).Get(),
			&s_radialControllerConfigurationInterop
		);
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		hr = Windows::Foundation::GetActivationFactory(
			Microsoft::WRL::Wrappers::HStringReference( RuntimeClass_Windows_Storage_StorageFile ).Get(),
			&s_storageFileStatics
		);
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		hr = Windows::Foundation::GetActivationFactory(
			Microsoft::WRL::Wrappers::HStringReference( RuntimeClass_Windows_Storage_Streams_RandomAccessStreamReference ).Get(),
			&s_randomAccessStreamReferenceStatics
		);
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_uninitialize()
{
	try {
		s_radialControllerInterop.Reset();
		s_radialControllerConfigurationInterop.Reset();
		s_radialControllerStatics.Reset();
		s_radialControllerMenuItemStatics.Reset();
		s_storageFileStatics.Reset();
		s_randomAccessStreamReferenceStatics.Reset();
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_create( dial_handle_t hwnd, void *userdata, dial_controller_t **controller )
{
	dial_controller_t *c = nullptr;
	try {
		c = new dial_controller_t;
		c->userdata = userdata;
	} catch ( ... ) {
		return dial_failed;
	}

	try {
		HRESULT hr = S_OK;
		hr = s_radialControllerInterop->CreateForWindow( (HWND)hwnd, IID_PPV_ARGS( &c->radial_controller ) );
		if ( FAILED( hr ) ) {
			dial_controller_release( c );
			return dial_failed;
		}

		hr = s_radialControllerConfigurationInterop->GetForWindow( (HWND)hwnd, IID_PPV_ARGS( &c->radial_configuration ) );
		if ( FAILED( hr ) ) {
			dial_controller_release( c );
			return dial_failed;
		}

		hr = c->radial_controller->get_Menu( &c->radial_menu );
		if ( FAILED( hr ) ) {
			dial_controller_release( c );
			return dial_failed;
		}

		*controller = c;
	} catch (...) {
		dial_controller_release( c );
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_release( dial_controller_t *controller )
{
	try {
		delete controller;
	} catch ( ... ) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_get_userdata( dial_controller_t *controller, void **userdata )
{
	*userdata = controller->userdata;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_is_supported( dial_bool_t *supported )
{
	try {
		HRESULT hr = S_OK;

		boolean result = FALSE;
		hr = s_radialControllerStatics->IsSupported( &result );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		if ( result ) {
			*supported = dial_true;
		} else {
			*supported = dial_false;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_get_rotation( dial_controller_t *controller, dial_double_t *degree )
{
	try {
		HRESULT hr = controller->radial_controller->get_RotationResolutionInDegrees( degree );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_set_rotation	( dial_controller_t *controller, dial_double_t degree )
{
	try {
		HRESULT hr = controller->radial_controller->put_RotationResolutionInDegrees( degree );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_is_feedback( dial_controller_t *controller, dial_bool_t *enabled )
{
	try {
		boolean value = false;
		HRESULT hr = controller->radial_controller->get_UseAutomaticHapticFeedback( &value );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
		if ( value ) {
			*enabled = dial_true;
		} else {
			*enabled = dial_false;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_set_feedback( dial_controller_t *controller, dial_bool_t enabled )
{
	try {
		HRESULT hr = S_OK;
		if ( enabled ) {
			hr = controller->radial_controller->put_UseAutomaticHapticFeedback( true );
		} else {
			hr = controller->radial_controller->put_UseAutomaticHapticFeedback( false );
		}
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

static HRESULT dial_contact_make( ABI::Windows::UI::Input::IRadialControllerScreenContact *contact, dial_contact_t *result )
{
	if ( !contact ) {
		return E_FAIL;
	}

	dial_contact_t r = {};
	HRESULT hr = S_OK;
	hr = contact->get_Bounds( &r.bounds.rect );
	if ( FAILED( hr ) ) {
		return hr;
	}

	hr = contact->get_Position( &r.position.point );
	if ( FAILED( hr ) ) {
		return hr;
	}

	*result = r;
	return S_OK;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_clicked( dial_controller_t *controller, dial_controller_clicked_f f )
{
	if ( !f ) {
		return dial_failed;
	}

	try {
		std::map<dial_controller_clicked_f, EventRegistrationToken>::iterator it = controller->token_clicked.find( f );
		if ( it != controller->token_clicked.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		EventRegistrationToken token;
		hr = controller->radial_controller->add_ButtonClicked( Microsoft::WRL::Callback<ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Input::RadialController *, ABI::Windows::UI::Input::RadialControllerButtonClickedEventArgs*> > (
			[controller, f]( ABI::Windows::UI::Input::IRadialController*, ABI::Windows::UI::Input::IRadialControllerButtonClickedEventArgs* args ) {
			Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerScreenContact> c;
			HRESULT hr = args->get_Contact( &c );
			if ( FAILED( hr ) ) {
				return hr;
			}

			if ( c ) {
				dial_contact_t contact = {};
				hr = dial_contact_make( c.Get(), &contact );
				if ( FAILED( hr ) ) {
					return hr;
				}
				f( controller, &contact );
			} else {
				f( controller, nullptr );
			}
			return S_OK;
		} ).Get(), &token );

		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_clicked[f] = token;
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_acquired( dial_controller_t *controller, dial_controller_acquired_f f )
{
	if ( !f ) {
		return dial_failed;
	}

	try {
		std::map<dial_controller_acquired_f, EventRegistrationToken>::iterator it = controller->token_acquired.find( f );
		if ( it != controller->token_acquired.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		EventRegistrationToken token;
		hr = controller->radial_controller->add_ControlAcquired( Microsoft::WRL::Callback<ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Input::RadialController *, ABI::Windows::UI::Input::RadialControllerControlAcquiredEventArgs*> > (
			[controller, f]( ABI::Windows::UI::Input::IRadialController*, ABI::Windows::UI::Input::IRadialControllerControlAcquiredEventArgs* args ) {
			Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerScreenContact> c;
			HRESULT hr = args->get_Contact( &c );
			if ( FAILED( hr ) ) {
				return hr;
			}

			if ( c ) {
				dial_contact_t contact = {};
				hr = dial_contact_make( c.Get(), &contact );
				if ( FAILED( hr ) ) {
					return hr;
				}
				f( controller, &contact );
			} else {
				f( controller, nullptr );
			}
			return S_OK;
		} ).Get(), &token );

		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_acquired[f] = token;
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_control_lost( dial_controller_t *controller, dial_controller_control_lost_f f )
{
	if ( !f ) {
		return dial_failed;
	}

	try {
		std::map<dial_controller_control_lost_f, EventRegistrationToken>::iterator it = controller->token_control_lost.find( f );
		if ( it != controller->token_control_lost.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		EventRegistrationToken token;
		hr = controller->radial_controller->add_ControlLost( Microsoft::WRL::Callback<ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Input::RadialController *, IInspectable*> > (
			[controller, f]( ABI::Windows::UI::Input::IRadialController*, IInspectable* ) {
			f( controller );
			return S_OK;
		} ).Get(), &token );

		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_control_lost[f] = token;
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_rotation_changed( dial_controller_t *controller, dial_controller_rotation_changed_f f )
{
	if ( !f ) {
		return dial_failed;
	}

	try {
		std::map<dial_controller_rotation_changed_f, EventRegistrationToken>::iterator it = controller->token_rotation_changed.find( f );
		if ( it != controller->token_rotation_changed.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		EventRegistrationToken token;
		hr = controller->radial_controller->add_RotationChanged( Microsoft::WRL::Callback<ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Input::RadialController *, ABI::Windows::UI::Input::RadialControllerRotationChangedEventArgs*> > (
			[controller, f]( ABI::Windows::UI::Input::IRadialController*, ABI::Windows::UI::Input::IRadialControllerRotationChangedEventArgs* args ) {
			Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerScreenContact> c;
			HRESULT hr = args->get_Contact( &c );
			if ( FAILED( hr ) ) {
				return hr;
			}

			double degree = 0.0;
			hr = args->get_RotationDeltaInDegrees( &degree );
			if ( FAILED( hr ) ) {
				return hr;
			}

			if ( c ) {
				dial_contact_t contact = {};
				hr = dial_contact_make( c.Get(), &contact );
				if ( FAILED( hr ) ) {
					return hr;
				}
				f( controller, &contact, degree );
			} else {
				f( controller, nullptr, degree );
			}
			return S_OK;
		} ).Get(), &token );

		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_rotation_changed[f] = token;
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_continue( dial_controller_t *controller, dial_controller_contact_continue_f f )
{
	if ( !f ) {
		return dial_failed;
	}

	try {
		std::map<dial_controller_contact_continue_f, EventRegistrationToken>::iterator it = controller->token_contact_continue.find( f );
		if ( it != controller->token_contact_continue.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		EventRegistrationToken token;
		hr = controller->radial_controller->add_ScreenContactContinued( Microsoft::WRL::Callback<ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Input::RadialController *, ABI::Windows::UI::Input::RadialControllerScreenContactContinuedEventArgs*> > (
			[controller, f]( ABI::Windows::UI::Input::IRadialController*, ABI::Windows::UI::Input::IRadialControllerScreenContactContinuedEventArgs* args ) {
			Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerScreenContact> c;
			HRESULT hr = args->get_Contact( &c );
			if ( FAILED( hr ) ) {
				return hr;
			}

			if ( c ) {
				dial_contact_t contact = {};
				hr = dial_contact_make( c.Get(), &contact );
				if ( FAILED( hr ) ) {
					return hr;
				}
				f( controller, &contact );
			} else {
				f( controller, nullptr );
			}
			return S_OK;
		} ).Get(), &token );

		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_contact_continue[f] = token;
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_ended( dial_controller_t *controller, dial_controller_contact_ended_f f )
{
	if ( !f ) {
		return dial_failed;
	}

	try {
		std::map<dial_controller_contact_ended_f, EventRegistrationToken>::iterator it = controller->token_contact_ended.find( f );
		if ( it != controller->token_contact_ended.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		EventRegistrationToken token;
		hr = controller->radial_controller->add_ScreenContactEnded( Microsoft::WRL::Callback<ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Input::RadialController *, IInspectable*> > (
			[controller, f]( ABI::Windows::UI::Input::IRadialController*, IInspectable* ) {
			f( controller );
			return S_OK;
		} ).Get(), &token );

		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_contact_ended[f] = token;
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_started( dial_controller_t *controller, dial_controller_contact_started_f f )
{
	if ( !f ) {
		return dial_failed;
	}

	try {
		std::map<dial_controller_contact_started_f, EventRegistrationToken>::iterator it = controller->token_contact_started.find( f );
		if ( it != controller->token_contact_started.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		EventRegistrationToken token;
		hr = controller->radial_controller->add_ScreenContactStarted( Microsoft::WRL::Callback<ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Input::RadialController *, ABI::Windows::UI::Input::RadialControllerScreenContactStartedEventArgs*> > (
			[controller, f]( ABI::Windows::UI::Input::IRadialController*, ABI::Windows::UI::Input::IRadialControllerScreenContactStartedEventArgs* args ) {
			Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerScreenContact> c;
			HRESULT hr = args->get_Contact( &c );
			if ( FAILED( hr ) ) {
				return hr;
			}

			if ( c ) {
				dial_contact_t contact = {};
				hr = dial_contact_make( c.Get(), &contact );
				if ( FAILED( hr ) ) {
					return hr;
				}
				f( controller, &contact );
			} else {
				f( controller, nullptr );
			}
			return S_OK;
		} ).Get(), &token );

		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_contact_started[f] = token;
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_clicked( dial_controller_t *controller, dial_controller_clicked_f f )
{
	try {
		std::map<dial_controller_clicked_f, EventRegistrationToken>::iterator it = controller->token_clicked.find( f );
		if ( it == controller->token_clicked.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		hr = controller->radial_controller->remove_ButtonClicked( (*it).second );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_clicked.erase( it );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_acquired( dial_controller_t *controller, dial_controller_acquired_f f )
{
	try {
		std::map<dial_controller_acquired_f, EventRegistrationToken>::iterator it = controller->token_acquired.find( f );
		if ( it == controller->token_acquired.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		hr = controller->radial_controller->remove_ControlAcquired( (*it).second );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_acquired.erase( it );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_control_lost( dial_controller_t *controller, dial_controller_control_lost_f f )
{
	try {
		std::map<dial_controller_control_lost_f, EventRegistrationToken>::iterator it = controller->token_control_lost.find( f );
		if ( it == controller->token_control_lost.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		hr = controller->radial_controller->remove_ControlLost( (*it).second );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_control_lost.erase( it );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_rotation_changed( dial_controller_t *controller, dial_controller_rotation_changed_f f )
{
	try {
		std::map<dial_controller_rotation_changed_f, EventRegistrationToken>::iterator it = controller->token_rotation_changed.find( f );
		if ( it == controller->token_rotation_changed.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		hr = controller->radial_controller->remove_RotationChanged( (*it).second );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_rotation_changed.erase( it );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_continue( dial_controller_t *controller, dial_controller_contact_continue_f f )
{
	try {
		std::map<dial_controller_contact_continue_f, EventRegistrationToken>::iterator it = controller->token_contact_continue.find( f );
		if ( it == controller->token_contact_continue.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		hr = controller->radial_controller->remove_ScreenContactContinued( (*it).second );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_contact_continue.erase( it );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_ended( dial_controller_t *controller, dial_controller_contact_ended_f f )
{
	try {
		std::map<dial_controller_contact_ended_f, EventRegistrationToken>::iterator it = controller->token_contact_ended.find( f );
		if ( it == controller->token_contact_ended.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		hr = controller->radial_controller->remove_ScreenContactEnded( (*it).second );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_contact_ended.erase( it );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_started( dial_controller_t *controller, dial_controller_contact_started_f f )
{
	try {
		std::map<dial_controller_contact_started_f, EventRegistrationToken>::iterator it = controller->token_contact_started.find( f );
		if ( it == controller->token_contact_started.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		hr = controller->radial_controller->remove_ScreenContactStarted( (*it).second );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->token_contact_started.erase( it );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_set_default_menu_items( dial_controller_t *controller, dial_default_menu_item_t *items, dial_int_t item_count )
{
	try {
		HRESULT hr = S_OK;

		Microsoft::WRL::ComPtr<EmptyIterable<ABI::Windows::UI::Input::RadialControllerSystemMenuItemKind> > empty = Microsoft::WRL::Make<EmptyIterable<ABI::Windows::UI::Input::RadialControllerSystemMenuItemKind> >();
		hr = controller->radial_configuration->SetDefaultMenuItems( empty.Get() );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_try_select_default_menu_item( dial_controller_t *controller, dial_default_menu_item_t item )
{
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_reset_to_default_menu_items( dial_controller_t *controller )
{
	try {
		HRESULT hr = S_OK;
		hr = controller->radial_configuration->ResetToDefaultMenuItems();
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_items( dial_controller_t *controller, dial_menu_item_t **items, dial_int_t item_count )
{
	try {
		HRESULT hr = S_OK;

		Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IVector<ABI::Windows::UI::Input::RadialControllerMenuItem*> > menuItems;
		hr = controller->radial_menu->get_Items( &menuItems );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		hr = menuItems->Clear();
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		controller->item_map.clear();

		for ( dial_menu_item_t **it = items; it != items + item_count; ++it ) {
			ABI::Windows::UI::Input::IRadialControllerMenuItem *item = (*it)->radial_item.Get();
			hr = menuItems->Append( item );
			if ( FAILED( hr ) ) {
				return dial_failed;
			}
			controller->item_map[item] = *it;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_get_selected_menu_item( dial_controller_t *controller, dial_menu_item_t **item )
{
	try {
		HRESULT hr = S_OK;
		ABI::Windows::UI::Input::IRadialControllerMenuItem *selected = nullptr;
		hr = controller->radial_menu->GetSelectedMenuItem( &selected );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		std::map<ABI::Windows::UI::Input::IRadialControllerMenuItem *, dial_menu_item_t *>::iterator it = controller->item_map.find( selected );
		if ( it == controller->item_map.end() ) {
			return dial_failed;
		}

		*item = (*it).second;
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_selected_menu_item( dial_controller_t *controller, dial_menu_item_t *items )
{
	try {
		HRESULT hr = S_OK;
		hr = controller->radial_menu->SelectMenuItem( items->radial_item.Get() );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_try_select_previous( dial_controller_t *controller )
{
	try {
		HRESULT hr = S_OK;
		boolean result = false;
		hr = controller->radial_menu->TrySelectPreviouslySelectedMenuItem( &result );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
		if ( !result ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;

}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_is_enabled( dial_controller_t *controller, dial_bool_t *enabled )
{
	try {
		HRESULT hr = S_OK;
		boolean result = false;
		hr = controller->radial_menu->get_IsEnabled( &result );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
		if ( result ) {
			*enabled = dial_true;
		} else {
			*enabled = dial_false;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_enabled( dial_controller_t *controller, dial_bool_t enabled )
{
	try {
		HRESULT hr = S_OK;
		if ( enabled ) {
			hr = controller->radial_menu->put_IsEnabled( true );
		} else {
			hr = controller->radial_menu->put_IsEnabled( false );
		}
		if ( FAILED( hr ) ) {
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_file_create( const wchar_t *path, void *userdata, dial_file_created_f f )
{
	dial_file_t *file = nullptr;
	try {
		file = new dial_file_t;
		file->path = nullptr;
		file->userdata = userdata;
	} catch (...) {
		return dial_failed;
	}

	try {
		HRESULT hr = S_OK;

		// ファイルパスをcanonicalizeしておかないとGetFileFromPathAsyncが失敗する.
		WCHAR canonicalPath[MAX_PATH];		
		hr = ::PathCchCanonicalize( canonicalPath, MAX_PATH, path );
		if ( FAILED( hr ) ) {
			dial_file_release( file );
			return dial_failed;
		}

		hr = ::WindowsCreateString( canonicalPath, static_cast<::UINT32>(::wcslen( canonicalPath )), &file->path );
		if ( FAILED( hr ) ) {
			dial_file_release( file );
			return dial_failed;
		}
	
		Microsoft::WRL::ComPtr<__FIAsyncOperation_1_Windows__CStorage__CStorageFile> operation;
		hr = s_storageFileStatics->GetFileFromPathAsync( file->path, &operation );
		if ( FAILED( hr ) ) {
			dial_file_release( file );
			return dial_failed;
		}

		hr = operation->put_Completed( Microsoft::WRL::Callback<ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::Windows::Storage::StorageFile*> >(
			[file, f]( ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Storage::StorageFile*>* async, AsyncStatus status ) {
			if ( status == AsyncStatus::Completed ) {
				if ( SUCCEEDED( async->GetResults( &file->storageFile ) ) ) {
					f( file, dial_ok );
				} else {
					f( file, dial_failed );
				}
			} else if ( status == AsyncStatus::Started ) {
			} else {
				f( file, dial_failed );
			}
			return S_OK;
		} ).Get() );

		if ( FAILED( hr ) ) {
			dial_file_release( file );
			return dial_failed;
		}
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_file_release( dial_file_t *file )
{
	try {
		if ( file->path ) {
			::WindowsDeleteString( file->path );
		}
		delete file;
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_file_get_path( dial_file_t *file, const wchar_t **path, dial_int_t *len )
{
	try {
		::UINT32 length = 0;
		*path = ::WindowsGetStringRawBuffer( file->path, &length );
		*len = static_cast<dial_int_t>( length );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_file_get_userdata( dial_file_t *file, void **userdata )
{
	*userdata = file->userdata;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_create( const wchar_t *name, const dial_icon_t icon, void *userdata, dial_menu_item_t **item )
{
	dial_menu_item_t *i = nullptr;
	try {
		i = new dial_menu_item_t;
		i->userdata = userdata;
	} catch ( ... ) {
		return dial_failed;
	}

	try {
		Microsoft::WRL::Wrappers::HStringReference nameRef( name );

		HRESULT hr = S_OK;
		if ( icon == dial_icon_scroll ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_Scroll, &i->radial_item );
		} else if ( icon == dial_icon_zoom ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_Zoom, &i->radial_item );
		} else if ( icon == dial_icon_undo_redo ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_UndoRedo, &i->radial_item );
		} else if ( icon == dial_icon_volume ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_Volume, &i->radial_item );
		} else if ( icon == dial_icon_next_previous_track ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_NextPreviousTrack, &i->radial_item );
		} else if ( icon == dial_icon_ruler ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_Ruler, &i->radial_item );
		} else if ( icon == dial_icon_ink_color ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_InkColor, &i->radial_item );
		} else if ( icon == dial_icon_ink_thickness ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_InkThickness, &i->radial_item );
		} else if ( icon == dial_icon_pen_type ) {
			hr = s_radialControllerMenuItemStatics->CreateFromKnownIcon( nameRef.Get(), ABI::Windows::UI::Input::RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_PenType, &i->radial_item );
		} else {
			dial_menu_item_release( i );
			return dial_failed;
		}
		if ( FAILED( hr ) ) {
			dial_menu_item_release( i );
			return dial_failed;
		}

		*item = i;
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_create_with_icon( const wchar_t *name, dial_file_t *file, void *userdata, dial_menu_item_t **item )
{
	dial_menu_item_t *i = nullptr;
	try {
		i = new dial_menu_item_t;
		i->userdata = userdata;
	} catch ( ... ) {
		return dial_failed;
	}

	try {
		Microsoft::WRL::Wrappers::HStringReference nameRef( name );
		Microsoft::WRL::ComPtr<ABI::Windows::Storage::Streams::IRandomAccessStreamReference> iconStream;

		HRESULT hr = S_OK;
		hr = s_randomAccessStreamReferenceStatics->CreateFromFile( file->storageFile.Get(), &iconStream );
		if ( FAILED( hr ) ) {
			dial_menu_item_release( i );
			return dial_failed;
		}

		hr = s_radialControllerMenuItemStatics->CreateFromIcon( nameRef.Get(), iconStream.Get(), &i->radial_item);
		if ( FAILED( hr ) ) {
			dial_menu_item_release( i );
			return dial_failed;
		}

		*item = i;
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_release( dial_menu_item_t *item )
{
	try {
		delete item;
	} catch ( ... ) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_get_userdata( dial_menu_item_t *item, void **userdata )
{
	*userdata = item->userdata;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_add_invoked( dial_menu_item_t *item, dial_menu_item_invoked_f f )
{
	if ( !f ) {
		return dial_failed;
	}

	try {
		std::map<dial_menu_item_invoked_f, EventRegistrationToken>::iterator it = item->token_invoked.find( f );
		if ( it != item->token_invoked.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		EventRegistrationToken token;
		hr = item->radial_item->add_Invoked( Microsoft::WRL::Callback<ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Input::RadialControllerMenuItem*, IInspectable*> > (
			[item, f]( ABI::Windows::UI::Input::IRadialControllerMenuItem*, IInspectable* ) {
			f( item );
			return S_OK;
		} ).Get(), &token );

		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		item->token_invoked[f] = token;
	} catch (...) {
		return dial_failed;
	}

	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_remove_invoked( dial_menu_item_t *item, dial_menu_item_invoked_f f )
{
	try {
		std::map<dial_menu_item_invoked_f, EventRegistrationToken>::iterator it = item->token_invoked.find( f );
		if ( it == item->token_invoked.end() ) {
			return dial_failed;
		}

		HRESULT hr = S_OK;
		hr = item->radial_item->remove_Invoked( (*it).second );
		if ( FAILED( hr ) ) {
			return dial_failed;
		}

		item->token_invoked.erase( it );
	} catch (...) {
		return dial_failed;
	}
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_contact_get_bounds( const dial_contact_t *contact, const dial_rect_t **bounds )
{
	*bounds = &contact->bounds;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_contact_get_position( const dial_contact_t *contact, const dial_point_t **point )
{
	*point = &contact->position;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_x( const dial_rect_t *r, dial_double_t *x )
{
	*x = r->rect.X;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_y( const dial_rect_t *r, dial_double_t *y )
{
	*y = r->rect.Y;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_width( const dial_rect_t *r, dial_double_t *w )
{
	*w = r->rect.Width;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_height( const dial_rect_t *r, dial_double_t *h )
{
	*h = r->rect.Height;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_point_get_x( const dial_point_t *p, dial_double_t *x )
{
	*x = p->point.X;
	return dial_ok;
}

dial_result_t TRIGLAV_WINRT_CALL dial_point_get_y( const dial_point_t *p, dial_double_t *y )
{
	*y = p->point.Y;
	return dial_ok;
}
