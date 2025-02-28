#include <windows.h>
#include <memory>
#include "window.h"
#include <iostream>
#include <wingdi.h>
#include <winuser.h>

struct WindowImpl
{
	HDC hdc = nullptr;
	HGLRC hglrc = nullptr;
	HWND hwnd = nullptr;
	MSG msg;
};

LRESULT CALLBACK wndProc(HWND unnamedParam1, UINT unnamedParam2, WPARAM wParam, LPARAM lParam);

std::unique_ptr<WindowImpl> createWindow(int width, int height, const char * name)
{
	wchar_t window_name[50];

	size_t ind = 0;
	while(name[ind] != '\0' && ind < 49) {
		window_name[ind] = name[ind];
		ind++;
	}
	window_name[ind] = '\0';


	std::unique_ptr<WindowImpl> window = std::make_unique<WindowImpl>();
	HINSTANCE hInstance = GetModuleHandle(NULL);

	std::cout << hInstance << std::endl;

	WNDCLASSEXW wndClass { 
		sizeof(WNDCLASSEX),
		CS_OWNDC,
		&wndProc,
		0,
		0,
		hInstance,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		L"className"
	};

	RegisterClassExW(&wndClass);

	RECT rect {0, 0, width, height};
	AdjustWindowRect(&rect, WS_BORDER | WS_VISIBLE |  WS_SYSMENU | WS_OVERLAPPEDWINDOW, false);

	window->hwnd = CreateWindowExW(
		WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW,
		L"className",
		window_name,
		WS_BORDER | WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr,
		0,
		hInstance,
		nullptr);

	ShowWindow(window->hwnd, SW_SHOW);
	UpdateWindow(window->hwnd);

	return window;
};

Window::Window(int width, int height, const char * name) : 
	pImpl(createWindow(width, height, name)), width(width), height(height) {
	SetWindowLongPtrW(pImpl->hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

void* GetAnyProcAddress(const char *name) {
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) )
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}

void MakeCurrentContext(Window& window) {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	window.pImpl->hdc = GetDC(window.pImpl->hwnd);

	std::cout << "hdc: " << window.pImpl->hdc << std::endl; 
	const int iPixelFormat = ChoosePixelFormat(window.pImpl->hdc, &pfd);
	SetPixelFormat(window.pImpl->hdc, iPixelFormat, &pfd);

	window.pImpl->hglrc = wglCreateContext(window.pImpl->hdc);
	std::cout << "hglrc: " << window.pImpl->hglrc << std::endl; 

	wglMakeCurrent(window.pImpl->hdc, window.pImpl->hglrc);
}

void SwapBuffers(Window& window) {
	::SwapBuffers(window.pImpl->hdc);
}

bool ShouldClose(Window& window) {

	BOOL result = false;

	while (PeekMessage(&(window.pImpl->msg), window.pImpl->hwnd, NULL, NULL, PM_NOREMOVE) == TRUE) {
		if ((result = GetMessage(&(window.pImpl->msg), NULL, 0, 0)) != 0) {
			TranslateMessage(&(window.pImpl->msg));
			DispatchMessage(&(window.pImpl->msg));
		} else {
			return true;
		}
	}

	return result == -1;
}

Window::~Window() {
	if (pImpl != nullptr) {
		wglDeleteContext(pImpl->hglrc);
	}

	if (pImpl->hwnd != NULL) {
		DestroyWindow(pImpl->hwnd);
	}
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { 

	Window * window = reinterpret_cast<Window*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (window) {
		if (msg == WM_SIZE) {
			window->width = LOWORD(lParam); 
			window->height = HIWORD(lParam); 
		}
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}
