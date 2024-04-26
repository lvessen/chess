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

	window->hwnd = CreateWindowExW(
		WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW,
		L"className",
		window_name,
		WS_BORDER | WS_VISIBLE | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		nullptr,
		0,
		hInstance,
		nullptr);

	ShowWindow(window->hwnd, SW_SHOW);

	return window;
};

Window::Window(int width, int height, const char * name) : 
	pImpl(createWindow(width, height, name)) {
}

void* Window::GetAnyProcAddress(const char *name) {
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

void Window::MakeCurrentContext() {
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

	pImpl->hdc = GetDC(pImpl->hwnd);

	std::cout << "hdc: " << pImpl->hdc << std::endl; 
	const int iPixelFormat = ChoosePixelFormat(pImpl->hdc, &pfd);
	SetPixelFormat(pImpl->hdc, iPixelFormat, &pfd);

	pImpl->hglrc = wglCreateContext(pImpl->hdc);
	std::cout << "hglrc: " << pImpl->hglrc << std::endl; 

	wglMakeCurrent(pImpl->hdc, pImpl->hglrc);
}

void Window::SwapBuffers() {
	::SwapBuffers(pImpl->hdc);
}

bool Window::ShouldClose() {

	BOOL result = GetMessage(&(pImpl->msg), pImpl->hwnd, NULL, NULL);

	if (result == -1) {
		return true;
	} else {
		TranslateMessage(&(pImpl->msg));
		DispatchMessage(&(pImpl->msg));
	}

	return result == 0;
}

Window::~Window() {
	if (pImpl != nullptr) {
		wglDeleteContext(pImpl->hglrc);
	}
}

LRESULT CALLBACK wndProc(HWND unnamedParam1, UINT unnamedParam2, WPARAM wParam, LPARAM lParam) { 
	return DefWindowProcW(unnamedParam1, unnamedParam2, wParam, lParam);
}
