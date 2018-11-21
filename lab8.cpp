#include "stdafx.h"
#include "lab8.h"

#define MAX_LOADSTRING 100

#define CELL 40.0
#define HEIGHT 300.0
#define X(x) ((int) (x * CELL))
#define Y(y) ((int) (y * CELL))
#define BTN_DRAW_ID  1
#define BTN_CLEAR_ID 2

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

RECT drawRect = { 0, 0, 800, 600 };

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

VOID DrawButton(LPDRAWITEMSTRUCT lpInfo);
void CreateControls(HWND &hWnd);
void DrawPicture(HWND &hWnd);
void DrawButton(LPDRAWITEMSTRUCT lpInfo);
void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap);
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB8));
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB8));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB8);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; 

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 900, 750, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

VOID CreateControls(HWND &hWnd) {
	CreateWindow(L"button", L"Draw", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		10, 600, 64, 64, hWnd, (HMENU)BTN_DRAW_ID, hInst, NULL);
	CreateWindow(L"button", L"Clear", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		80, 600, 64, 64, hWnd, (HMENU)BTN_CLEAR_ID, hInst, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		CreateControls(hWnd);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case BTN_DRAW_ID:
			DrawPicture(hWnd);
			break;
		case BTN_CLEAR_ID:
			InvalidateRect(hWnd, &drawRect, true);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DRAWITEM:
		DrawButton((LPDRAWITEMSTRUCT)lParam);
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void DrawPicture(HWND &hWnd) {
	PAINTSTRUCT ps;
	HDC	hdc = BeginPaint(hWnd, &ps);

	//House
	Rectangle(hdc, X(5), Y(5), X(15), Y(13));
	//Roof
	Line(hdc, X(5), Y(5), X(10), Y(1.5));
	Line(hdc, X(15), Y(5), X(10), Y(1.5));
	//Roof Window
	Ellipse(hdc, X(9), Y(3), X(11), Y(4.5));
	Line(hdc, X(10), Y(3), X(10), Y(4.5));
	Line(hdc, X(9), Y(3.75), X(11), Y(3.75));

	//Door
	Rectangle(hdc, X(11), Y(8), X(14), Y(13));
	Rectangle(hdc, X(11.25), Y(8.25), X(13.75), Y(10.75));
	Ellipse(hdc, X(13.5), Y(10.9), X(13.75), Y(11.15));

	//Carpet
	POINT poly[5];
	poly[0].x = X(11);
	poly[0].y = Y(13.25);
	poly[1].x = X(10.5);
	poly[1].y = Y(14);
	poly[2].x = X(14.5);
	poly[2].y = Y(14);
	poly[3].x = X(14);
	poly[3].y = Y(13.25);
	poly[4].x = X(11);
	poly[4].y = Y(13.25);
	Polyline(hdc, poly, 5);

	//Window
	Rectangle(hdc, X(6), Y(8.25), X(9), Y(10.75));
	Line(hdc, X(6), Y(9.35), X(7.4), Y(9.35));
	Line(hdc, X(7.6), Y(9.35), X(9), Y(9.35));
	Line(hdc, X(6), Y(9.55), X(7.4), Y(9.55));
	Line(hdc, X(7.6), Y(9.55), X(9), Y(9.55));
	Line(hdc, X(7.4), Y(9.35), X(7.4), Y(8.25));
	Line(hdc, X(7.4), Y(9.55), X(7.4), Y(10.75));
	Line(hdc, X(7.6), Y(9.35), X(7.6), Y(8.25));
	Line(hdc, X(7.6), Y(9.55), X(7.6), Y(10.75));

	//Flag
	POINT poly2[6];
	poly2[0].x = X(10);
	poly2[0].y = Y(1.5);
	poly2[1].x = X(10);
	poly2[1].y = Y(0.5);
	poly2[2].x = X(12);
	poly2[2].y = Y(0.5);
	poly2[3].x = X(11.5);
	poly2[3].y = Y(0.75);
	poly2[4].x = X(12);
	poly2[4].y = Y(1);
	poly2[5].x = X(10);
	poly2[5].y = Y(1);
	Polyline(hdc, poly2, 6);

	EndPaint(hWnd, &ps);
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	return LineTo(hdc, x2, y2);
}

VOID DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap) {
	HBITMAP hOldBitmap;
	HDC hMemDC;
	BITMAP bm;
	POINT  ptSize, ptOrg;

	hMemDC = CreateCompatibleDC(hDC);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	if (hOldBitmap) {
		SetMapMode(hMemDC, GetMapMode(hDC));

		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);

		ptSize.x = bm.bmWidth;
		ptSize.y = bm.bmHeight;

		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;

		DPtoLP(hMemDC, &ptOrg, 1);

		BitBlt(hDC, x, y, ptSize.x, ptSize.y,
			hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);

		SelectObject(hMemDC, hOldBitmap);
	}

	DeleteDC(hMemDC);
}

VOID DrawButton(LPDRAWITEMSTRUCT lpInfo) {
	HBITMAP hBitmap;
	int ResourceID;

	if (lpInfo->CtlType != ODT_BUTTON)
		return;

	switch (lpInfo->CtlID) {
	case BTN_DRAW_ID:
		if (lpInfo->itemState & ODS_SELECTED) {
			ResourceID = IDB_DRAW;
		}
		else {
			ResourceID = IDB_DRAW;
		}
		break;
	case BTN_CLEAR_ID:
		if (lpInfo->itemState & ODS_SELECTED) {
			ResourceID = IDB_CLEAR;
		}
		else {
			ResourceID = IDB_CLEAR;
		}
		break;
	default:
		return;
	}

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(ResourceID));

	if (!hBitmap) {
		return;
	};

	if ((lpInfo->itemAction & ODA_DRAWENTIRE) ||
		(lpInfo->itemAction & ODA_SELECT)) {
		DrawBitmap(lpInfo->hDC,
			(lpInfo->rcItem).left,
			(lpInfo->rcItem).top, hBitmap);
	}

	DeleteObject(hBitmap);
}