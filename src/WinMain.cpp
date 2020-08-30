#include <windows.h>
#include <Objbase.h> 

#include "ChromaSDK.h"
#include <exception>
#include <iostream>
#include <fstream>
#include <conio.h>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

UdpSocket *udp = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::wcout << (L"Starting") << std::endl;
	
	LPTSTR cmd  = GetCommandLine();

	 udp = new UdpSocket(L"192.168.0.199", 12000);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    IID iid;
	HRESULT hr = IIDFromString(L"{368EFB31-B894-41F3-8781-05F10F479519}", &iid);
	
 	if (hr != S_OK) {
		std::wcout << L"IIDFromString failed" << std::endl;
		exit(hr);
	}
	CChromaSDK chromaSdk;
	chromaSdk.Initialize(&iid);
     
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"RazerDevSim";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"RazerDevSim",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );


    if (hwnd == NULL)
    {
		std::wcout << L"Creating window failed" << std::endl;
        return -1;
    }

  //  ShowWindow(hwnd, nCmdShow);
    chromaSdk.RegisterEvendNotificationImpl();


    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    chromaSdk.UnRegisterEvendNotificationImpl();

    chromaSdk.UnInitialize();

    return 0;

}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {



    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }


    return 0;

    }


    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}