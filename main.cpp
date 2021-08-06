/**
Reference https://zetcode.com/gui/winapi/gdi/
*/

#include <windows.h>


void DrawPixels(HWND hwnd);
void drawLine(HWND hwnd);
void drawRedLine(HWND hwnd);
void drawBitmap(HWND hwnd);

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
	    case WM_PAINT:
            //DrawPixels(hwnd);
            //drawLine(hwnd);
            //drawRedLine(hwnd);
            drawBitmap(hwnd);
			break;
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}


void DrawPixels(HWND hwnd) {

    PAINTSTRUCT ps;
    RECT r;

    GetClientRect(hwnd, &r);

    if (r.bottom == 0) {
    
        return;
    }

    HDC hdc = BeginPaint(hwnd, &ps);

    for (int i=0; i<1000; i++) {

        int x = rand() % r.right;
        int y = rand() % r.bottom;
        SetPixel(hdc, x, y, RGB(255, 0, 0));
    }

    EndPaint(hwnd, &ps);
    
}


void drawLine(HWND hwnd) {
  PAINTSTRUCT ps;
     
  HDC hdc = BeginPaint(hwnd, &ps);
  MoveToEx(hdc, 50, 50, NULL);
  LineTo(hdc, 250, 50);
  EndPaint(hwnd, &ps);          		
}

void drawRedLine(HWND hwnd) {
  HPEN hPenOld;
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);
  
  // Draw a red line
  HPEN hLinePen;
  COLORREF qLineColor;
  qLineColor = RGB(255, 0, 0); 
  hLinePen = CreatePen(PS_SOLID, 1, qLineColor); // 2 param is the thickness
  hPenOld = (HPEN)SelectObject(hdc, hLinePen); // to activate the new Pen

  MoveToEx(hdc, 100, 100, NULL); 
  LineTo(hdc, 500, 250);

  SelectObject(hdc, hPenOld); // old Pen reactivation
  DeleteObject(hLinePen);	// delete de Pen created before.
}

void drawBitmap(HWND hwnd) {
  HBITMAP hBitmap;
  HDC hdc;
  PAINTSTRUCT ps;
  BITMAP bitmap;
  HDC hdcMem;
  HGDIOBJ oldBitmap;
 
  // bitmap initialization better in case WM_CREATE:
  //hBitmap = (HBITMAP) LoadImageW(NULL, L"C:\\developer\\home\\project1\\slovak.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  hBitmap = (HBITMAP) LoadImageW(NULL, L"C:\\developer\\home\\project1\\pandy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

  if (hBitmap == NULL) {
    MessageBoxW(hwnd, L"Failed to load image", L"Error", MB_OK); 
    return;
  }
	
  hdc = BeginPaint(hwnd, &ps);

  hdcMem = CreateCompatibleDC(hdc);
  oldBitmap = SelectObject(hdcMem, hBitmap);

  GetObject(hBitmap, sizeof(bitmap), &bitmap);
  BitBlt(hdc, 5, 5, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

  SelectObject(hdcMem, oldBitmap);
  DeleteDC(hdcMem);

  EndPaint(hwnd, &ps);
	
}

