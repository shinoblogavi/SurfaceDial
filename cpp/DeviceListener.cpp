#include "DeviceListener.h"
#include <windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include <StrSafe.h>
#include <wrl\implements.h>
#include <wrl\module.h>
#include <wrl\event.h>
#include <roapi.h>
#include <wrl.h>
#include <map>
#include <vector>
#include <Pathcch.h>
#include <windows.foundation.h>
#include <windows.foundation.numerics.h>
#include <windows.foundation.collections.h>
#include <windows.ui.input.h>
#include <wrl\client.h>
#include <winstring.h>

#define RETURN_IF_FAILED(hr) { if(FAILED(hr)) return hr; }

using namespace ABI::Windows::UI::Input;
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

DeviceListener::DeviceListener(HWND hwnd)
{
	_hwnd = hwnd;
	Init();
}

// 
//  Window Initilize
//  + Register Surface Dial Event (call RegisterForEvents )
//  + Add Menu Item to Surface Dial Menu (Call CustmizeMenuItems )
//
HRESULT DeviceListener::Init()
{
    HRESULT hr = Windows::Foundation::Initialize(RO_INIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		hr = RegisterForEvents(_hwnd);
		if (SUCCEEDED(hr))
			hr = CustmizeMenuItems();
	}
    return hr;
}

//HRESULT DeviceListener::SetRotationResolution(double res)
//{
//    return _controller->put_RotationResolutionInDegrees(res);
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All Surface Dial event registration 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT DeviceListener::RegisterForEvents(HWND hwnd)
{

	//Get Radial Controller
    RETURN_IF_FAILED(Windows::Foundation::GetActivationFactory(
        HStringReference(RuntimeClass_Windows_UI_Input_RadialController).Get(),
        &_controllerInterop));

	//Create Window
    RETURN_IF_FAILED(_controllerInterop->CreateForWindow(hwnd, IID_PPV_ARGS(&_controller)));

    // Wire events
    RETURN_IF_FAILED(_controller->add_ScreenContactContinued(
        Callback<ITypedEventHandler<RadialController*, RadialControllerScreenContactContinuedEventArgs*>>(this, &DeviceListener::OnScreenContactContinued).Get(),
        &_screenContactContinuedToken));

	////////////////////////////////////// Messages for Surface Studio Onscreen ////////////////////////////////////////////////////

	// Onscreen Start
    RETURN_IF_FAILED(_controller->add_ScreenContactStarted(
        Callback<ITypedEventHandler<RadialController*, RadialControllerScreenContactStartedEventArgs*>>(this, &DeviceListener::OnScreenContactStarted).Get(),
        &_screenContactStartedToken));

	// OnScreen End
    RETURN_IF_FAILED(_controller->add_ScreenContactEnded(
        Callback<ITypedEventHandler<RadialController*, IInspectable*>>(this, &DeviceListener::OnScreenContactEnded).Get(),
        &_screenContactEndedToken));

	//Off Screen
    RETURN_IF_FAILED(_controller->add_ControlLost(
        Callback<ITypedEventHandler<RadialController*, IInspectable*>>(this, &DeviceListener::OnControlLost).Get(),
        &_controlLostToken));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Dial Control Acquired = Menu Selected 
    RETURN_IF_FAILED(_controller->add_ControlAcquired(
        Callback<ITypedEventHandler<RadialController*, RadialControllerControlAcquiredEventArgs*>>(this, &DeviceListener::OnControlAcquired).Get(),
        &_controlAcquiredToken));

	//Rotation 
    RETURN_IF_FAILED(_controller->add_RotationChanged(
        Callback<ITypedEventHandler<RadialController*, RadialControllerRotationChangedEventArgs*>>(this, &DeviceListener::OnRotationChanged).Get(),
        &_rotatedToken));

	//Button Click 
	RETURN_IF_FAILED(_controller->add_ButtonClicked(
		Callback<ITypedEventHandler<RadialController*, RadialControllerButtonClickedEventArgs*>>(this, &DeviceListener::OnButtonClicked).Get(),
		&_buttonClickedToken));

    return S_OK;
}

/////////////////////////////////////////// Surface Dial Menu Item Add and Selected ////////////////////////////////////////////

// Add 3 Menu Items to Surface Dial Menu 
HRESULT DeviceListener::CustmizeMenuItems()
{
    RETURN_IF_FAILED(_controller->get_Menu(&_menu));

    PrintMsg(L"Got Menu \n");

    RETURN_IF_FAILED(Windows::Foundation::GetActivationFactory(
        HStringReference(RuntimeClass_Windows_UI_Input_RadialControllerMenuItem).Get(), &_menuItemStatics));

    RETURN_IF_FAILED(AddMenuItem(HStringReference(L"My Ink").Get(),
        RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_InkColor, _knownIconItem1Token));

    RETURN_IF_FAILED(AddMenuItem(HStringReference(L"Ruler").Get(),
        RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_Ruler, _knownIconItem2Token));
	
	RETURN_IF_FAILED(AddMenuItem(HStringReference(L"PenType").Get(),
		RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_PenType, _knownIconItem3Token));

    return S_OK;
}

//Add Item to Dial Menu
HRESULT DeviceListener::AddMenuItem(_In_ HSTRING itemName, _In_ RadialControllerMenuKnownIcon icon, _In_ EventRegistrationToken registrationToken)
{
    // Get menu items
    ComPtr<Collections::IVector<RadialControllerMenuItem*>> menuItems;
    RETURN_IF_FAILED(_menu->get_Items(&menuItems));

    // Create item from known icon
    ComPtr<IRadialControllerMenuItem> menuItem;
    RETURN_IF_FAILED(_menuItemStatics->CreateFromKnownIcon(itemName, icon, &menuItem));

	// Set Callback
	RETURN_IF_FAILED(menuItem->add_Invoked(
		Callback<ITypedEventHandler<RadialControllerMenuItem*, IInspectable*>>(this, &DeviceListener::OnItemInvoked).Get(),
		&registrationToken));

	// Add item to menu
	RETURN_IF_FAILED(menuItems->Append(menuItem.Get()));

	// Log new item
	wchar_t message[2000];
	swprintf_s(message, 2000, L"Added %s to menu\n", WindowsGetStringRawBuffer(itemName, nullptr));
	PrintMsg(message);

	//return S_OK;

    return S_OK;
}

///////////////////////////////////////// for Surface Studio On Screen Event Handler /////////////////////////////////////////////

//On Screent 
HRESULT DeviceListener::OnScreenContactContinued(_In_ IRadialController* /*sender*/, _In_ IRadialControllerScreenContactContinuedEventArgs* args)
{
    RETURN_IF_FAILED(LogContact(args));
    return S_OK;
}

//On Screen Start
HRESULT DeviceListener::OnScreenContactStarted(_In_ IRadialController* /*sender*/, _In_ IRadialControllerScreenContactStartedEventArgs* args)
{
    RETURN_IF_FAILED(LogContact(args));
    return S_OK;
}

//Off Screen
HRESULT DeviceListener::OnScreenContactEnded(_In_ IRadialController* /*sender*/, _In_ IInspectable* args)
{
    PrintMsg(L"ScreenContactEnded\n");
    return S_OK;
}

//////////////////////////////////////////  On and Off Screent Surface Dial Events ///////////////////////////////////////////////////

//Selected Item of Surface Dial Menu
HRESULT DeviceListener::OnItemInvoked(_In_ IRadialControllerMenuItem* item, _In_ IInspectable* args)
{
	if (item != nullptr)
	{
		wchar_t message[2000];
		HSTRING str;

		item->get_DisplayText(&str);
		swprintf_s(message, 2000, L"ItemInvoked %s\n", WindowsGetStringRawBuffer(str, nullptr));
		PrintMsg(message);
	}

	return S_OK;
}

//Menu Open
HRESULT DeviceListener::OnControlLost(_In_ IRadialController* /*sender*/, _In_ IInspectable* args)
{
    PrintMsg(L"ControlLost\n");
	return S_OK;
}

//Menu Selected
HRESULT DeviceListener::OnControlAcquired(_In_ IRadialController* /*sender*/, _In_ IRadialControllerControlAcquiredEventArgs* args)
{
    PrintMsg(L"ControlAcquired\n");
    RETURN_IF_FAILED(LogContact(args));
    return S_OK;
}

//Rotation Event
HRESULT DeviceListener::OnRotationChanged(_In_ IRadialController* /*sender*/, _In_ IRadialControllerRotationChangedEventArgs* args)
{
    wchar_t message[2000];
    double delta = 0;

    args->get_RotationDeltaInDegrees(&delta);
    swprintf_s(message, 2000, L"RotationChanged delta=%lf\n", delta);
    PrintMsg(message);

    RETURN_IF_FAILED(LogContact(args));

    return S_OK;
}

//Click Event
HRESULT DeviceListener::OnButtonClicked(_In_ IRadialController* /*sender*/, _In_ IRadialControllerButtonClickedEventArgs* args)
{
	PrintMsg(L"Button Clicked");
	RETURN_IF_FAILED(LogContact(args));
	return S_OK;
}


////////////////////////////////////////////////////////////////////////////
// for this test Application Output
////////////////////////////////////////////////////////////////////////////

UINT c = 10;
void DeviceListener::PrintMsg(WCHAR *message)
{
	HDC hdc = GetDC(_hwnd);
	TextOut(hdc, 10, c, message, lstrlen(message));
	ReleaseDC(_hwnd, hdc);
	c += 26;
}

template<typename TContactArgs>
HRESULT DeviceListener::LogContact(_In_ TContactArgs* args)
{
    // Get contact
    ComPtr<IRadialControllerScreenContact> contact;
    RETURN_IF_FAILED(args->get_Contact(&contact));

    if (contact != nullptr)
    {
        // Get contact info
        Point position;
        Rect bounds;
        RETURN_IF_FAILED(contact->get_Position(&position));
        RETURN_IF_FAILED(contact->get_Bounds(&bounds));

        // Log contact info
        wchar_t message[2000];
        swprintf_s(message, 2000, L"\t Postion X=%lf, Y=%lf & \n \t Bounds Height=%lf, Width=%lf, X=%lf, Y=%lf\n",
            position.X, position.Y,
            bounds.Height, bounds.Width, bounds.X, bounds.Y);

        PrintMsg(message);
    }

    return S_OK;
}

