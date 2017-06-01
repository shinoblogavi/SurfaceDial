# About Surface Dial mounting method

this target applicaion is implemented in proprietary application framework Triglav. 
Therefore, incorporating Surface Dial corresponds to Surface Dial to Triglav framework, and it is implemented in application.


# Constitution

The incorporation of Surface Dial into this target application consists of the following four parts.

## Triglavwinrt.dll

It is a DLL that implements the WinRT API used in Surface Dial with the C interface. 
So we implemented a C interface DLL for calling Surface Dial's WinRT.

## Window (Triglav)

Set the Surface Dial menu for the window. This target applicaion has multiple windows because it is a classic Windows application. 
On the other hand, Surface Dial sets a menu for one window. In order to operate as one Surface Dial menu throughout the application, 
we need to deal with this structure well.Triglav limits the type of windows that display application-specific Surface Dial menus.

## Application (Triglav)

We added the function to set the Surface Dial menu in the application class and implement the function to receive events from the Surface Dial.

## Application (this target applicaion)

When launching the application, set the Surface Dial menu in the application class. Upon receiving an event from Surface Dial, it receives an event in the application class and performs actions on the canvas according to the parameter of that event type.


# Implementation

## Triglavwinrt.dll

Please look at the project in the attached triglavwinrt folder. A complete project is included.
Basically there are the WinRT API used in Surface Dial with the C interface. 

## Window (Triglav)

The extension to Triglav 's window consists of two parts.

### Calling DLL

These two classes are implementations for calling triglavwinrt.dll. Instead of a simple DLL wrapper, it has an interface close to the original Surface Dial WinRT.

* PWLegacyWindowRuntimeModuleWin
* PWLegacyDialModuleWin

### Window implementation

PWLegacyWindowWin is the most important implementation part. 
I will extract only the source code related to Surface Dial.


#### initialization is executed.
```
if ( PWLegacyWindowRuntimeModuleWin::IsAvailable() ) {
	PWLegacyWindowRuntimeModuleWin::Initalize();
	PWLegacyDialModule::Initialize();
}
```

#### Execute termination processing when the application terminates
```
// サーフェスダイヤルの終了
if ( PWLegacyWindowRuntimeModuleWin::IsAvailable() ) {
	PWLegacyWindowWin::SetDialMenu( PWDialMenu::DefaultMenu );
	PWLegacyDialModule::Terminate();
	PWLegacyWindowRuntimeModuleWin::Terminate();
}
```

#### creating a window, 
Call ApplyDialMenu on the window and set the Surface Dial menu set for the application to the window.
```
PWLegacyWindowWin::ApplyDialMenu( windowObject );
```

#### When the window receives WM_SETFOCUS
It calls ApplyDialMenuSelection for that window and sets the Surface Dial menu selected by the application in the focus window.

```
if( message == WM_SETFOCUS ) {
    PWLegacyWindowWin::ApplyDialMenuSelection( windowObject );
}
```

Check the PWLegacyWindowWin.cpp for the above functions and other implementations. There is an excerpt part of the code relating to Surface Dial.

## Application (Triglav)

The Triglav application class provides callbacks to receive functions and events that set the Surface Dial menu.

PWApplication.h.
```
//! @brief アプリケーションクラス
class PWApplication
{
public:
    //!	サーフェスダイヤルメニューの設定
    void	SetDialMenu( const PWDialMenu& dialMenu );

    //! サーフェスダイヤルメニューからのコールバック
    void	EventDialMenu( PWLegacyApplicationEventDialMenuParameterObject dialMenuParameterObject );
};
```

The implementation of PWApplication::SetDialMenu is simply a call to PWLegacyWindowWin::SetDialMenu. PWApplication::EventDialMenu is called by PWLegacyWindowWin which received the event of Surface Dial, it converts the PWLegacyApplicationEventDialMenuParameterObject created by PWLegacyWindowWin into PWDialMenuEventParameter and sends the event.

PWDialMenu represents the menu to set to Surface Dial.


## Application (this target applicaion)

this target applicaion When starting up, create PWDialMenu and call PWApplication::SetDialMenu to set the menu. In this target applicaion, the content of the menu is always decided, so setting up the menu is the only one here.

```
// サーフェスダイヤルメニューの設定.
std::vector<PWDialMenuItem> items;
URDialMenu::InitializeMenu(items);
SetDialMenu( PWDialMenu( items ) );
```

Also, at this time, make setting to connect the event of the application class, accept the event like OperationDialMenu and process it.

```
Bool OperationDialMenu( PWEvent& rEvent )
{
    Bool result = FALSE;
    EventKind eventKind = rEvent.GetEventKind();
    if (eventKind == kEventKindDialMenuClick ) {
        PWDialMenuEventParameter parameter(rEvent);
        UInt32 menuID = parameter.GetMenuID();
        // サーフェスダイヤルがクリックされた. 選択状態のメニューはmenuIDのものである.
    } else if (eventKind == kEventKindDialMenuRotate) {
        PWDialMenuEventParameter parameter(rEvent);
        UInt32 menuID = parameter.GetMenuID();
        double rotation = parameter.Rotation();
        // サーフェスダイヤルを回転した. 選択状態のメニューはmenuIDのものであり, 回転角度はrotation.
    } else {
        // ...
    }
    return TRUE;
}
```
In this way we will receive the Surface Dial event and its parameters in the application part and implement the code to operate on the canvas.