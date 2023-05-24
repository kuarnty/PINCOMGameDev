#pragma once

#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "dwrite")
#include <wincodec.h>
#pragma comment(lib, "WindowsCodecs")

#include "GameManager.h"
#include "Input.h"

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// 비트맵을 로드하는 함수 원형
HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);
HRESULT LoadBitmapFromRes(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);

class DemoApp
{
public:
	DemoApp();
	~DemoApp();
	HRESULT Initialize(HINSTANCE hInstance);

private:
	HRESULT CreateAppResource();
	HRESULT CreateDeviceResource();
	void DiscardDeviceResource();
	void OnPaint();
	void OnResize();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND hwnd;
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRenderTarget;

	// WIC 변수 선언
	IWICImagingFactory* pWICFactory;
};

DemoApp::DemoApp() :
	hwnd(NULL),
	pD2DFactory(NULL),
	pRenderTarget(NULL),
	pWICFactory(NULL)
{
}

DemoApp::~DemoApp()
{
	SAFE_RELEASE(pD2DFactory);
	SAFE_RELEASE(pRenderTarget);
	SAFE_RELEASE(pWICFactory);
}

HRESULT DemoApp::CreateAppResource()
{
	// D2D 팩토리를 생성함.
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr)) return hr;

	// WIC 팩토리를 생성함.
	// 주의: WIC 팩토리를 생성하는 CoCreateInstance 함수가 사용될 때에는 이전에 CoInitialize를 호출해주어야 함.
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
	if (FAILED(hr)) return hr;


	return hr;
}

HRESULT DemoApp::CreateDeviceResource()
{
	HRESULT hr = S_OK;

	if (pRenderTarget) return hr;

	RECT rc;
	GetClientRect(hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, size), &pRenderTarget);
	if (FAILED(hr)) return hr;

	return hr;
}

void DemoApp::DiscardDeviceResource()
{
	SAFE_RELEASE(pRenderTarget);
}

void DemoApp::OnPaint()
{
	HRESULT hr = CreateDeviceResource();
	if (FAILED(hr)) return;

	pRenderTarget->BeginDraw();

	hr = pRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		DiscardDeviceResource();
	}
}

void DemoApp::OnResize()
{
	if (!pRenderTarget) return;

	RECT rc;
	GetClientRect(hwnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	pRenderTarget->Resize(size);
}

LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		DemoApp* pDemoApp = (DemoApp*)pCreate->lpCreateParams;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pDemoApp);

		return 1;
	}

	DemoApp* pDemoApp = (DemoApp*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (!pDemoApp)
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	switch (message)
	{
	case WM_SIZE:
	{
		pDemoApp->OnResize();
		return 0;
	}

	case WM_DISPLAYCHANGE:
	{
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	}

	case WM_PAINT:
	{
		pDemoApp->OnPaint();
		ValidateRect(hwnd, NULL);
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = CreateAppResource();
	if (FAILED(hr)) return hr;

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DemoApp::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = hInstance;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = L"DemoApp";
	RegisterClassEx(&wcex);

	hwnd = CreateWindow(L"DemoApp", L"Direct2D Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, this);
	hr = hwnd ? S_OK : E_FAIL;
	if (FAILED(hr)) return hr;

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	return hr;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	Game::GameManager::init();
	Game::Input::init();

	// ?
	/*while (1)
	{
		msg = PeekMsg();
		GameManger.msgHandle(msg);
		GameManager.loop()
	}*/

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		DemoApp app;
		if (FAILED(app.Initialize(hInstance))) return 0;

		MSG msg = {};

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CoUninitialize();
	return 0;
}

//
// From here to the end of the file : LoadBitmapFromFile() and LoadBitmapFromRes().
// Copy the following code into your file.
// begin{
//

// Creates a Direct2D bitmap from the specified file name.
HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(uri, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(hr)) return hr;

	// Create the initial frame.
	hr = pDecoder->GetFrame(0, &pSource);
	if (FAILED(hr)) return hr;

	// Convert the image format to 32bppPBGRA (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
	hr = pIWICFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr)) return hr;

	// If a new width or height was specified, create an IWICBitmapScaler and use it to resize the image.
	if (destinationWidth != 0 || destinationHeight != 0)
	{
		UINT originalWidth, originalHeight;
		hr = pSource->GetSize(&originalWidth, &originalHeight);
		if (FAILED(hr)) return hr;

		if (destinationWidth == 0)
		{
			FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
			destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
		}
		else if (destinationHeight == 0)
		{
			FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
			destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
		}

		hr = pIWICFactory->CreateBitmapScaler(&pScaler);
		if (FAILED(hr)) return hr;

		hr = pScaler->Initialize(pSource, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
		if (FAILED(hr)) return hr;

		hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) return hr;
	}
	else // Don't scale the image.
	{
		hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) return hr;
	}

	// Create a Direct2D bitmap from the WIC bitmap.
	hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
	if (FAILED(hr)) return hr;

	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pScaler);

	return hr;
}

// Creates a Direct2D bitmap from a resource in the application resource file.
HRESULT LoadBitmapFromRes(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;

	// Locate the resource.
	imageResHandle = FindResourceW(NULL, resourceName, resourceType);
	HRESULT hr = imageResHandle ? S_OK : E_FAIL;
	if (FAILED(hr)) return hr;

	// Load the resource.
	imageResDataHandle = LoadResource(NULL, imageResHandle);
	hr = imageResDataHandle ? S_OK : E_FAIL;
	if (FAILED(hr)) return hr;

	// Lock it to get a system memory pointer.
	pImageFile = LockResource(imageResDataHandle);
	hr = pImageFile ? S_OK : E_FAIL;
	if (FAILED(hr)) return hr;

	// Calculate the size.
	imageFileSize = SizeofResource(NULL, imageResHandle);
	hr = imageFileSize ? S_OK : E_FAIL;
	if (FAILED(hr)) return hr;

	// Create a WIC stream to map onto the memory.
	hr = pIWICFactory->CreateStream(&pStream);
	if (FAILED(hr)) return hr;

	// Initialize the stream with the memory pointer and size.
	hr = pStream->InitializeFromMemory(reinterpret_cast<BYTE*>(pImageFile), imageFileSize);
	if (FAILED(hr)) return hr;

	// Create a decoder for the stream.
	hr = pIWICFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(hr)) return hr;

	// Create the initial frame.
	hr = pDecoder->GetFrame(0, &pSource);
	if (FAILED(hr)) return hr;

	// Convert the image format to 32bppPBGRA (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
	hr = pIWICFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr)) return hr;

	// If a new width or height was specified, create an IWICBitmapScaler and use it to resize the image.
	if (destinationWidth != 0 || destinationHeight != 0)
	{
		UINT originalWidth, originalHeight;
		hr = pSource->GetSize(&originalWidth, &originalHeight);
		if (FAILED(hr)) return hr;

		if (destinationWidth == 0)
		{
			FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
			destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
		}
		else if (destinationHeight == 0)
		{
			FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
			destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
		}

		hr = pIWICFactory->CreateBitmapScaler(&pScaler);
		if (FAILED(hr)) return hr;

		hr = pScaler->Initialize(pSource, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
		if (FAILED(hr)) return hr;

		hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) return hr;
	}
	else
	{
		// If destinationWidth == 0 and destinationHeight == 0 then, create bitmap with the original image size.
		hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) return hr;
	}

	//create a Direct2D bitmap from the WIC bitmap.
	hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
	if (FAILED(hr)) return hr;

	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pScaler);

	return hr;
}

//
// end}
//