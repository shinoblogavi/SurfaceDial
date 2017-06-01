# Surface Dial Implement for Cpp 

For supporting Surface Dial to Windows Desktop、we need create some process.

1. GetRadialController and RadianControllerConfiguration Instance  
2. Custom Radial Controller menu 
3. Implement Event Handler for Surface Dial Event  

# Sample file

This file construct with 3 files

1. Main Windows (RadialController.cpp)
2. Radial Controller Listener (DeviceListener.cpp)
3. Radial Controller Listener Header (DeviceListener.h)

Main code is implement in DeviceListener.cpp


# How to get Instance

For controling Surface Dial, we have to get 2 instances.

## Get Radial Controller Instance
we get Radial Conroller Instance from each Windows. So basically we define Radial Controller for each window.

```
Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialController> _controller;

RETURN_IF_FAILED(_controllerInterop->CreateForWindow(hwnd, IID_PPV_ARGS(&_controller)));
```

## Get Radial Controller Menu

We can Get Radial Controller menu instance from Radial Controller instance.

```
Microsoft::WRL::ComPtr<ABI::Windows::UI::Input::IRadialControllerMenu> _menu;

(_controller->get_Menu(&_menu));
```
# How to custom Surface Dial Menu

We can add and delete surface Dial Menu Items. 
Surface Dial Menu Items is constructed with Item name and Icon.
We make menu item and set callback for ItemSelected.
finally we add Menu Item to Surface Dial Menu.

```

    ComPtr<Collections::IVector<RadialControllerMenuItem*>> menuItems;
    _menu->get_Items(&menuItems);

    // Create Menu Item
    ComPtr<IRadialControllerMenuItem> menuItem;
    _menuItemStatics->CreateFromKnownIcon(
        L"Pen Icon",
        RadialControllerMenuKnownIcon::RadialControllerMenuKnownIcon_PenType,
        &_knownIconItemToken
        );


	// Set Callback
	menuItem->add_Invoked(
		Callback<ITypedEventHandler<RadialControllerMenuItem*, 
        IInspectable*>>(this, &DeviceListener::OnItemInvoked).Get(),
		&registrationToken
        );

	// Add item to menu
	menuItems->Append(menuItem.Get());

```


# How to set Event Handler

we define callback entry and set EventHandler from Surface Dial Events.

```
HRESULT OnRotationChanged(
    _In_ ABI::Windows::UI::Input::IRadialController* sender, 
    _In_ ABI::Windows::UI::Input::IRadialControllerRotationChangedEventArgs* args
    );

EventRegistrationToken _rotatedToken;

_controller->add_RotationChanged(
    Callback<ITypedEventHandler<RadialController*, 
    RadialControllerRotationChangedEventArgs*>>(this, &DeviceListener::OnRotationChanged).Get(),
    &_rotatedToken
    );

//Rotation Event
HRESULT DeviceListener::OnRotationChanged(_In_ IRadialController*, _In_ IRadialControllerRotationChangedEventArgs* args)
{
        :
    return S_OK;
}
```
# Surface Dial Events

We have get Surface Dial Events from devices. 

## Surface Dial Manu Events

When user launch surface Dial Menu with Long tapping Surface Dial Device, OnControllerLost event will fire.
And when user select a item in Surface Dial menu, OnControllerAcquired event will fire.

* OnControllLost 
* OnControllAcquired

## Device Controll Events
It is similer from Mouse Wheel Events. When User rorate Dial, OnRorationChanged event will fire. 
We can get delta angle of user changed from last Event. Basically this event fire each 10 degrees changed.

* OnRotationChanged
* OnButtonClicked

## for Surface Studio OnScreen Events

* OnScreenContractStarted
* OnScreenContractEnded
* OnScreenContractContinued

#reference

* UWPでのSurface Dial 実装方法 <https://blogs.msdn.microsoft.com/shintak/2017/05/29/surfacedialdevoverview/>
* Surface Dial 実装のための概要 <https://blogs.msdn.microsoft.com/shintak/2017/05/29/surfacedialdevuwp/>
* Surface Dial WPF Implement Memo <https://blogs.msdn.microsoft.com/shintak/2017/05/29/surfacedialdevuwp/>
