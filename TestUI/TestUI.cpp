// TestUI.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <Windows.h>

//// Global Objects
WNDCLASSEX HostWindowClass; /// Our Host Window Class Object
MSG loop_message; /// Loop Message for Host Window
HINSTANCE hInstance = GetModuleHandle(NULL); /// Application Image Base Address
HWND cpphwin_hwnd; /// Host Window Handle
HWND wpf_hwnd; /// WPF Wrapper Handle

typedef HWND(*CreateUserInterfaceFunc)(void);
CreateUserInterfaceFunc CreateUserInterface;
typedef void(*DisplayUserInterfaceFunc)(void);
DisplayUserInterfaceFunc DisplayUserInterface;
typedef void(*DestroyUserInterfaceFunc)(void);
DestroyUserInterfaceFunc DestroyUserInterface;

RECT hwin_rect;

//// Global Configs
const wchar_t cpphwinCN[] = L"CppMAppHostWinClass"; /// Host Window Class Name
bool isHWindowRunning = false; /// Host Window Running State

//// Host Window Callback, NOTE :Define This Before Your Entrypoint Function
LRESULT CALLBACK HostWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		isHWindowRunning = false;
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

//// Our Application Entry Point

int main()
{
	std::cout << "C++ Main App Started..." << std::endl;

	/// We Code Here ...
	/// Creating Icon Object From Resources, Don't forget to include resource.h!
	//HICON app_icon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON));

	/// Loading dotNet UI Library

	HMODULE dotNetGUILibrary = LoadLibrary(L"WpfUI.dll");
	CreateUserInterface = (CreateUserInterfaceFunc)GetProcAddress
	(dotNetGUILibrary, "CreateUserInterface");
	DisplayUserInterface = (DisplayUserInterfaceFunc)GetProcAddress
	(dotNetGUILibrary, "DisplayUserInterface");
	DestroyUserInterface = (DestroyUserInterfaceFunc)GetProcAddress
	(dotNetGUILibrary, "DestroyUserInterface");

	typedef void(__cdecl* AllocMsgFunc)();
	typedef void(__cdecl* FreeMsgFunc)();

	((AllocMsgFunc)GetProcAddress(dotNetGUILibrary, "AllocMsg"))();

	typedef TCHAR* (*GetMsg)();

	auto pGetMsg = (GetMsg)GetProcAddress(dotNetGUILibrary, "GetMsg");
	const TCHAR* Msg = pGetMsg();

	((FreeMsgFunc)GetProcAddress(dotNetGUILibrary, "FreeMsg"))();

	/// Defining Our Host Window Class
	HostWindowClass.cbSize = sizeof(WNDCLASSEX); HostWindowClass.lpfnWndProc = HostWindowProc;
	HostWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	HostWindowClass.cbClsExtra = 0; HostWindowClass.style = 0;
	HostWindowClass.cbWndExtra = 0;    HostWindowClass.hInstance = hInstance;
	//HostWindowClass.hIcon = app_icon; HostWindowClass.hIconSm = app_icon;
	HostWindowClass.lpszClassName = cpphwinCN; HostWindowClass.lpszMenuName = NULL;

	//// Register Window
	if (!RegisterClassEx(&HostWindowClass))
	{
		std::cout << "Error, Code :" << GetLastError() << std::endl;
		getchar(); return 0;
	}

	/// Creating Unmanaged Host Window
	cpphwin_hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		cpphwinCN,
		L"TestUI",
		WS_THICKFRAME | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 500,
		NULL, NULL, hInstance, NULL);

	/// Check if How Window is valid
	if (cpphwin_hwnd == NULL)
	{
		std::cout << "Error, Code :" << GetLastError() << std::endl;
		getchar(); return 0;
	}
	::SetWindowLong(cpphwin_hwnd, GWL_STYLE, GetWindowLong(cpphwin_hwnd, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);

	/// Centering Host Window
	RECT window_r; RECT desktop_r;
	GetWindowRect(cpphwin_hwnd, &window_r); GetWindowRect(GetDesktopWindow(), &desktop_r);
	int xPos = (desktop_r.right - (window_r.right - window_r.left)) / 2;
	int yPos = (desktop_r.bottom - (window_r.bottom - window_r.top)) / 2;

	/// Set Window Position
	::SetWindowPos(cpphwin_hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	/// Set Thread to STA
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	/// Creating .Net GUI
	wpf_hwnd = CreateUserInterface();

	/// Check if WPF Window is valid
	if (wpf_hwnd != nullptr) {
		/// Disable Host Window Updates & Draws
		SendMessage(cpphwin_hwnd, WM_SETREDRAW, FALSE, 0);

		/// Disable Host Window Double Buffering
		long dwExStyle = GetWindowLong(cpphwin_hwnd, GWL_EXSTYLE);
		dwExStyle &= ~WS_EX_COMPOSITED;
		SetWindowLong(cpphwin_hwnd, GWL_EXSTYLE, dwExStyle);

		/// Set WPF Window to a Child Control
		SetWindowLong(wpf_hwnd, GWL_STYLE, WS_CHILD);

		/// Get your host client area rect
		GetClientRect(cpphwin_hwnd, &hwin_rect);

		/// Set WPF Control Order , Size and Position
		MoveWindow(wpf_hwnd, 0, 0, hwin_rect.right - hwin_rect.left, hwin_rect.bottom - hwin_rect.top, TRUE);
		SetWindowPos(wpf_hwnd, HWND_TOP, 0, 0, hwin_rect.right - hwin_rect.left, hwin_rect.bottom - hwin_rect.top, SWP_NOMOVE);

		/// Set WPF as A Child to Host Window...
		SetParent(wpf_hwnd, cpphwin_hwnd);

		/// Skadoosh!
		ShowWindow(wpf_hwnd, SW_RESTORE);

		/// Display WPF Control by Reseting its Opacity
		DisplayUserInterface();
	}

	/// Display Window
	ShowWindow(cpphwin_hwnd, SW_SHOW);
	UpdateWindow(cpphwin_hwnd);
	BringWindowToTop(cpphwin_hwnd);
	isHWindowRunning = true;

	/// Adding Message Loop
	while (GetMessage(&loop_message, NULL, 0, 0) > 0 && isHWindowRunning)
	{
		TranslateMessage(&loop_message);
		DispatchMessage(&loop_message);
	}

	std::cout << "C++ Main App Finished." << std::endl;
	getchar();
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln