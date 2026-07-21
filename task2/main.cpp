#include<Windows.h>

constexpr int kButtonClickId = 1; 

namespace 
{
	void CenterButton(HWND hWnd)
	{
        HWND hButton = GetDlgItem(hWnd, kButtonClickId);
		if (!hButton)
			return;

		RECT rc = {};
		GetClientRect(hWnd, &rc);
		const int btnW = 100;
		const int btnH = 40;
		const int x = (rc.right - btnW) / 2;
		const int y = (rc.bottom - btnH) / 2;
		MoveWindow(hButton, x, y, btnW, btnH, true);
	}
} 

LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
        CreateWindowW(L"BUTTON", L"点击", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 40,
            hwnd, (HMENU)(INT_PTR)kButtonClickId, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CenterButton(hwnd);
		break;
	}
	case WM_SIZE:
		CenterButton(hwnd);
		break;
	case WM_COMMAND:
        if (LOWORD(wParam) == kButtonClickId)
            MessageBoxW(hwnd, L"Hello Wrold!", L"对话框", MB_OK | MB_ICONINFORMATION);
        break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS wndcls; 
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndcls.hInstance = hInstance;
	wndcls.lpfnWndProc = WinSunProc;
	wndcls.lpszClassName = L"task2";
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndcls);

	HWND hwnd = CreateWindow(L"task2", L"windows api 学习", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	RECT work = {};
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &work, 0);
    const int screenW = work.right - work.left;
    const int screenH = work.bottom - work.top;
    const int width = screenW / 2;
    const int height = screenH / 2;
    const int x = work.left + (screenW - width) / 2;
    const int y = work.top + (screenH - height) / 2;
    MoveWindow(hwnd, x, y, width, height, true);
    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
