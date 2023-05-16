#include "mainWin.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MainWindow win;

	if (!win.Create(L"Pyhsics Engines", WS_OVERLAPPEDWINDOW)) return -1;

	ShowWindow(win.Window(), nCmdShow);
	UpdateWindow(win.Window());

	MSG msg{};
	unsigned long long tick = GetTickCount64();
	unsigned long long nowTick = tick;
	while (1) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (msg.message == WM_QUIT) break;
		else {
			nowTick = GetTickCount64();
			if (nowTick - tick > 30) {
				win.Calculate();
				tick = nowTick;
				InvalidateRect(win.Window(), nullptr, false);
			}
		}
	}

	return msg.wParam;
}