#include "stdafx.h"
#include "IntroGL.h"

#undef WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>	
#include <GL/glu.h>	

#include "GLApp.h"

#include <stdio.h>
#include <iostream>

using namespace Utils;
using namespace ord12929;

GLApp* app = 0;

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
    static PAINTSTRUCT ps;

    switch(uMsg) {
    case WM_PAINT:
	app->Render();
	BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
	return 0;

    case WM_SIZE:
	app->ReSize(LOWORD(lParam), HIWORD(lParam));
	PostMessage(hWnd, WM_PAINT, 0, 0);
	return 0;

    case WM_CHAR:
	switch (wParam) {
	case 27:			/* ESC key */
	    PostQuitMessage(0);
	    break;
	}
	return 0;

	case WM_KEYDOWN:
		app->KeyDown(wParam);
		break;

	case WM_KEYUP:
		app->KeyUp(wParam);
		break;

	case WM_MOUSEMOVE:
		app->MouseMove(LOWORD(lParam), HIWORD(lParam), wParam & MK_LBUTTON, wParam & MK_RBUTTON, wParam & MK_CONTROL, wParam & MK_SHIFT);
		break;

	case WM_LBUTTONDOWN:
		app->MouseDown(MOUSE_BUTTON_LEFT, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_LBUTTONUP:
		app->MouseUp(MOUSE_BUTTON_LEFT, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_RBUTTONDOWN:
		app->MouseDown(MOUSE_BUTTON_RIGHT, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_RBUTTONUP:
		app->MouseUp(MOUSE_BUTTON_RIGHT, LOWORD(lParam), HIWORD(lParam));
		break;

    case WM_CLOSE:
	PostQuitMessage(0);
	return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 

HWND CreateOpenGLWindow(int x, int y, int width, int height, BYTE type, DWORD flags) 
{
    int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance) {
	hInstance = GetModuleHandle(NULL);
	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = L"OpenGL";

	if (!RegisterClass(&wc)) {
	    MessageBox(NULL, L"RegisterClass() failed:  Cannot register window class.", L"Error", MB_OK);
	    return NULL;
	}
    }

    hWnd = CreateWindow(L"OpenGL", L"IntroGL - Tristan Lapalme", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	MessageBox(NULL, L"CreateWindow() failed:  Cannot create a window.", L"Error", MB_OK);
	return NULL;
    }

    hDC = GetDC(hWnd);

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = type;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
	MessageBox(NULL, L"ChoosePixelFormat() failed: Cannot find a suitable pixel format.", L"Error", MB_OK); 
	return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	MessageBox(NULL, L"SetPixelFormat() failed: Cannot set format specified.", L"Error", MB_OK);
	return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hWnd, hDC);

    return hWnd;
}    

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
    HDC hDC;				/* device context */
    HGLRC hRC;				/* opengl context */
    HWND  hWnd;				/* window */
    MSG   msg;				/* message */

    hWnd = CreateOpenGLWindow(0, 0, 800, 600, PFD_TYPE_RGBA, 0);
    if (hWnd == NULL)
	exit(1);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hWnd, nCmdShow);

	app = new GLApp(hWnd);
	app->Init();

	BOOL shouldContinue = TRUE;
	DWORD lastFrame = timeGetTime();

    while(shouldContinue) 
	{
		while(::PeekMessage(&msg, NULL, 0,0, PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				shouldContinue = FALSE;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD currentTime = timeGetTime();
		if(currentTime - lastFrame > 33)
		{
			app->Swap();
			app->Animate((int)(currentTime - lastFrame));
			app->Render();
			lastFrame = currentTime;
		}
		::Sleep(1);
    }

    delete app;
	
	wglMakeCurrent(NULL, NULL);
    ReleaseDC(hWnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);

    return msg.wParam;
}