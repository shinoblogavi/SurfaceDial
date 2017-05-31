
#include <StrSafe.h>
#include <wrl\implements.h>
#include <wrl\module.h>
#include <wrl\event.h>
#include <roapi.h>
#include <wrl.h>
#include "DeviceListener.h"
#include <windows.h>

#define CLASSNAME L"Radial Device Controller"
#define BUFFER_SIZE 2000


//
// Dial Device Listener instance Pointer
//
DeviceListener *listener = nullptr;
LRESULT CALLBACK WindowProc(__in HWND hWindow, __in UINT uMsg, __in WPARAM wParam, __in LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Register Window Class
    WNDCLASS wndClass = {
        CS_DBLCLKS, 
		(WNDPROC)WindowProc,
        0,0,0,0,0,0,0, 
		CLASSNAME
    };
    RegisterClass(&wndClass);

	//Create Window
	HWND hwnd = CreateWindow(
		CLASSNAME,
		L"Dial Message Listener Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, NULL, NULL);

	//
	//Dial Listener Instance Create
	//
	listener = new DeviceListener(hwnd);

	//Mesage 
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  // Dispatch message to WindowProc
    }
}


LRESULT CALLBACK WindowProc( __in HWND hWindow,  __in UINT uMsg,  __in WPARAM wParam,  __in LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWindow);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWindow, uMsg, wParam, lParam);
    }

    return 0;
}
