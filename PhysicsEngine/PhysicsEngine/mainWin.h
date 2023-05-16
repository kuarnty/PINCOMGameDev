#pragma once
#include "baseWin.h"

#include <d2d1.h>
#pragma comment(lib, "d2d1")

class MainWindow : public BaseWindow<MainWindow> {
private:
	LRESULT OnCreate();
	LRESULT OnPaint();
	LRESULT OnSize();
private:
	bool InitD2D();

private:
	RECT clientSize;

	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRT;
	ID2D1SolidColorBrush* pBrush;

	D2D1_POINT_2F rcCenter;
	D2D1_SIZE_F rcSize;

	D2D_RECT_F ground[3];

public:
	virtual LPCWSTR ClassName() const { return L"Physics Engine"; }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Calculate();
};