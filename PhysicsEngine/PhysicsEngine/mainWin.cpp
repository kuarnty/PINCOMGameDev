#include "mainWin.h"
#include <cstdlib>

LRESULT MainWindow::OnCreate()
{
	GetClientRect(hWnd, &clientSize);

	if(!InitD2D()) return -1;

	rcCenter = D2D1::Point2F((clientSize.right - clientSize.left) / 2, 40);
	rcSize = D2D1::SizeF(40, 40);

	ground[0].bottom = clientSize.bottom;
	ground[0].left = 0.f;
	ground[0].right = clientSize.right;
	ground[0].top = clientSize.bottom - 40.0f;

	ground[1].bottom = 300.f;
	ground[1].left = 300.f;
	ground[1].right = 450.f;
	ground[1].top = ground[1].bottom - 30.0f;

	ground[2].bottom = ground[0].top;
	ground[2].left = 300.f;
	ground[2].right =340.f;
	ground[2].top = ground[2].bottom - 30.0f;

	return 0;
}

LRESULT MainWindow::OnPaint()
{
	PAINTSTRUCT ps;
	pRT->BeginDraw();
	BeginPaint(hWnd, &ps);

	pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White),&pBrush);

	pRT->DrawRectangle(D2D1::RectF(rcCenter.x - rcSize.width / 2, rcCenter.y - rcSize.height / 2, rcCenter.x + rcSize.width / 2, rcCenter.y + rcSize.height / 2),pBrush,3);
	
	for (size_t i = 0; i < 3; i++)
	{
		pRT->FillRectangle(ground[i], pBrush);
	}

	EndPaint(hWnd, &ps);
	return pRT->EndDraw();
}

LRESULT MainWindow::OnSize()
{
	GetClientRect(hWnd, &clientSize);

	pRT->Resize(D2D1::SizeU(clientSize.right - clientSize.left, 
		clientSize.bottom - clientSize.top));

	return 0;
}

bool MainWindow::InitD2D()
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

	if (!SUCCEEDED(hr)) return false;
	hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(
		hWnd, D2D1::SizeU(
			clientSize.right - clientSize.left, clientSize.bottom - clientSize.top
		), D2D1_PRESENT_OPTIONS_IMMEDIATELY
	), &pRT);

	if (!SUCCEEDED(hr)) return false;

	return true;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE) PostQuitMessage(0);
	else if (uMsg == WM_DESTROY) DestroyWindow(hWnd);


	switch (uMsg) {
		case WM_CREATE:
			{
				if (OnCreate()) throw;
			}
		case WM_PAINT:
			{
				return OnPaint();
			}
		case WM_SIZE:
			{
				return OnSize();
			}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void MainWindow::Calculate()
{
	static float gravity = 2.f;
	static float dropSpeed = 0.f;
	dropSpeed += gravity;

	if (dropSpeed >= 0.f) {
		for (size_t i = 0; i < 3; ++i) {
			if (!(ground[i].left < rcCenter.x + rcSize.width / 2.f &&
				rcCenter.x - rcSize.width / 2.f < ground[i].right)) continue;

			if (ground[i].top < rcCenter.y + rcSize.height / 2) continue;

			if (abs(ground[i].top - rcCenter.y - rcSize.height / 2) < 0.001f) {
				dropSpeed = 0.f;
				break;
			}

			if (rcCenter.y + rcSize.height / 2 + dropSpeed > ground[i].top) {
				float gap = ground[i].top - rcCenter.y - rcSize.height / 2.f;
				dropSpeed = gap;

			}

		}
	}
	rcCenter.y += dropSpeed;
}