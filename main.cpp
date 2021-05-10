#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
using std::vector;
using std::string;

/* This is where all the input to the window goes to */
#define ID_TEXTBOX_1 1
#define ID_TEXTBOX_2 2
#define ID_BUTTON_1 3
#define ID_BUTTON_2 4
#define ID_BUTTON_3 5

static HWND hwndTextBox_1;
static HWND hwndTextBox_2;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_PAINT:
  			{
    			PAINTSTRUCT ps;
    			HDC hdc = BeginPaint( hwnd, &ps );
    			TextOut( hdc, 10 /* X */, 10 /* Y */, "Enter text to be encoded", 24 /* Number of chars */);
    			TextOut( hdc, 10 /* X */, 110 /* Y */, "Enter text to be decoded", 24 /* Number of chars */);
    			EndPaint( hwnd, &ps );
  			break;
  			}
		
		case WM_CREATE: {
			
			hwndTextBox_1 = CreateWindow(TEXT("EDIT"), TEXT(""),
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
				20,30,200, 20,
				hwnd, (HMENU) ID_TEXTBOX_1, NULL, NULL
			);
			
			CreateWindow(TEXT("BUTTON"), TEXT("encode"),
				WS_CHILD | WS_VISIBLE,
				250,30,80, 20,
				hwnd, (HMENU) ID_BUTTON_1, NULL, NULL
			);
			
			hwndTextBox_2 = CreateWindow(TEXT("EDIT"), TEXT(""),
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
				20,130,200, 20,
				hwnd, (HMENU) ID_TEXTBOX_2, NULL, NULL
			);
				

			CreateWindow(TEXT("BUTTON"), TEXT("decode"),
				WS_CHILD | WS_VISIBLE,
				250,130,80, 20,
				hwnd, (HMENU) ID_BUTTON_2, NULL, NULL
			);
			
			CreateWindow(TEXT("BUTTON"), TEXT("clear"),
				WS_CHILD | WS_VISIBLE,
				250,80,80, 20,
				hwnd, (HMENU) ID_BUTTON_3, NULL, NULL
			);
			
			
			/*
			HMENU hMenuBar = CreateMenu();
			HMENU hFile = CreateMenu();
			HMENU hOptions = CreateMenu();
			HMENU hEdit = CreateMenu();
			
			AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, "File");
			
			AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hEdit, "Edit");
			AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hOptions, "Options");

			AppendMenu(hFile, MF_STRING, NULL, "Exit");
			AppendMenu(hOptions, MF_STRING, NULL, "Option 1");
			AppendMenu(hOptions, MF_STRING, NULL, "Option 2");
			AppendMenu(hOptions, MF_STRING, NULL, "Option 3");
			AppendMenu(hOptions, MF_STRING, NULL, "Option 4");
			AppendMenu(hOptions, MF_STRING, NULL, "Option 5");
			AppendMenu(hOptions, MF_STRING, NULL, "Option 6");
			AppendMenu(hEdit, MF_STRING, NULL, "Delete");
			AppendMenu(hEdit, MF_STRING, NULL, "Create");
			
			SetMenu(hwnd, hMenuBar);
			*/
			break;
		}
		case WM_COMMAND: {
			
			if (LOWORD(wParam) == ID_BUTTON_1){
				int length = GetWindowTextLength(hwndTextBox_1) + 1;
				char title[length];
				
				GetWindowText(hwndTextBox_1, title, length);
				
				string inp = title;
				int len = inp.length();
				char result[len]; 
				char out[len];
				int val;
				char character;
				for (int i = 0; i < sizeof(out); i++) {
					out[i] = inp[i];
					val = (int)out[i] + ((i+(i%5))+1);
					character = (char)val;
					result[i] = character;
				}
				
				SetWindowText(hwndTextBox_1, result);
				
			} else if (LOWORD(wParam) == ID_BUTTON_2){
				
				int length = GetWindowTextLength(hwndTextBox_2) + 1;
				char title[length];
				
				GetWindowText(hwndTextBox_2, title, length);
				
				string inp = title;
				int len = inp.length();
				char result[len]; 
				char out[len];
				int val;
				char character;
				for (int i = 0; i < sizeof(out); i++) {
					out[i] = inp[i];
					val = (int)out[i] - ((i+(i%5))+1);
					character = (char)val;
					result[i] = character;
				}
				
				SetWindowText(hwndTextBox_2, result);
				
			} else if (LOWORD(wParam) == ID_BUTTON_3) {
				SetWindowText(hwndTextBox_2, "");
				SetWindowText(hwndTextBox_1, "");
			}else {
				break;
			}
			
			break;
		}	
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

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Aarav's Amazing App",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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
