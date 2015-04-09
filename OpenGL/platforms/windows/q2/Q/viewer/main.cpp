#include <stdio.h>
#include <Winsock2.h>
#include <windows.h>
#include <Commctrl.h>
#include <commdlg.h>
#include "resource.h"
#include <assert.h>

#include "render.h"


#include "MTracerFile.h"

#define BOOL_MENU_ADJUSTMENT TRUE

#define LOG_MSG
#define HOLOCAM_WINDOW_TITLE_DEFAULT "Viewer"

static const TCHAR class_name[] = "Viewer";

typedef struct {
    int     id;
    int     enabled;
    int     checked;
} MENUITEMS;


HDC         hDC=NULL;       // Private GDI Device Context
HGLRC       hRC=NULL;       // Permanent Rendering Context
HWND        hWnd=NULL;      // Holds Our Window Handle
HWND        hStatus = NULL; // Hold Our Status Bar
HINSTANCE   hInstance;      // Holds The Instance Of The Application

int         WindowNotMinimized = 1;

RENDER_STATE    state;

//////////////////////////////////////////////////////////////////

void UpdateViewAngle(WPARAM param) {
    state.rotate_x = state.rotate_y = state.rotate_z = 0;

    switch (LOWORD(param)) {
        case ID_VIEW_SIDE:
            state.rotate_y = -90;
            break;
        case ID_VIEW_BOTTOM:
            state.rotate_z = 180;
            break;
        case ID_VIEW_FRONT:
            state.rotate_y = -90;
            state.rotate_x = 90;
            break;
    }
}

void ReSizeScene(int width, int height)                 // Handle resize envent
{
    if (height==0) height=1;                            // Prevent A Divide By Zero By
    ReSizeSceneGL(width, height);
}

void WindowCleanup()
{
    if (hRC) {
        if (!wglMakeCurrent(NULL,NULL)) {
            assert("Release Of DC And RC Failed."==0);
        }
        if (!wglDeleteContext(hRC)) {
            assert("Release Rendering Context Failed."==0);
        }
        hRC = NULL;
    }

    if (hDC && !ReleaseDC(hWnd,hDC)) {
        assert("Release Device Context Failed."==0);
        hDC = NULL;
    }

    if (hWnd && !DestroyWindow(hWnd)) {
        assert("Could Not Release hWnd."==0);
        hWnd = NULL;
    }

    if (!UnregisterClass(class_name, hInstance)) {
        assert("Could Not Unregister Class."==0);
        hInstance = NULL;
    }
}


int GetKeyData(HKEY hRootKey, TCHAR *subKey, TCHAR *value, LPBYTE data, DWORD cbData)
{
    HKEY hKey;
    if(RegOpenKeyEx(hRootKey, subKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
        return 0;
    if(RegQueryValueEx(hKey, value, NULL, NULL, data, &cbData) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return 0;
    }
    RegCloseKey(hKey);
    return 1;
}

int SetKeyData(HKEY hRootKey, TCHAR *subKey, DWORD dwType, TCHAR *value, LPBYTE data, DWORD cbData)
{
    HKEY hKey;
    if(RegCreateKey(hRootKey, subKey, &hKey) != ERROR_SUCCESS)
        return 0;
    if(RegSetValueEx(hKey, value, 0, dwType, data, cbData) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return 0;
    }
    RegCloseKey(hKey);
    return 1;
}

LRESULT CALLBACK HandleMouse(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int leftdragging     = 0;
    static int rightdragging    = 0;

    static int leftpoint_x_end  = 0;
    static int leftpoint_y_end  = 0;
    static int leftlast_x       = 0;
    static int leftlast_y       = 0;

    static int rightpoint_x_end = 0;
    static int rightpoint_y_end = 0;
    static int rightlast_x      = 0;
    static int rightlast_y      = 0;

    switch (uMsg)
    {
        case WM_MOUSEWHEEL:
            {
                int mouse_scroll = (short)HIWORD(wParam);
                if (rightdragging) {
                    state.translate_z += 0.05f*(mouse_scroll/WHEEL_DELTA);
                }
                if (leftdragging) {
                    state.rotate_z += (mouse_scroll/WHEEL_DELTA);
                }
                else
                {
                    state.scale *= 1.0f + 0.05f*(mouse_scroll/WHEEL_DELTA);
                }
            }
            return 0;
        case WM_LBUTTONDOWN:
            leftlast_x = leftpoint_x_end = LOWORD(lParam);
            leftlast_y = leftpoint_y_end = HIWORD(lParam);
            leftdragging = 1;
            return 0;
        case WM_RBUTTONDOWN:
            rightlast_x = rightpoint_x_end = LOWORD(lParam);
            rightlast_y = rightpoint_y_end = HIWORD(lParam);
            rightdragging = 1;

            return 0;
        case WM_MOUSEMOVE:
            if (leftdragging) {
                leftpoint_x_end = LOWORD(lParam);
                leftpoint_y_end = HIWORD(lParam);
                if(!HIWORD(GetKeyState(VK_SHIFT))){
                    state.rotate_x += LOWORD(lParam) - leftlast_x;
                    state.rotate_y += HIWORD(lParam) - leftlast_y;
                }
                leftlast_x = leftpoint_x_end;
                leftlast_y = leftpoint_y_end;
            }
            if (rightdragging) {
                rightpoint_x_end = LOWORD(lParam);
                rightpoint_y_end = HIWORD(lParam);
                state.translate_x += (LOWORD(lParam) - rightlast_x) / 25.0f;
                state.translate_y -= (HIWORD(lParam) - rightlast_y) / 25.0f;
                rightlast_x = rightpoint_x_end;
                rightlast_y = rightpoint_y_end;
            }
            return 0;
        case WM_LBUTTONUP:
            if (leftdragging) {
                leftpoint_x_end = LOWORD(lParam);
                leftpoint_y_end = HIWORD(lParam);
                if(HIWORD(GetKeyState(VK_SHIFT))){
                }else{
                    state.rotate_x += LOWORD(lParam) - leftlast_x;
                    state.rotate_y += HIWORD(lParam) - leftlast_y;
                }
                leftdragging = 0;
            }
            return 0;
        case WM_RBUTTONUP:
            if (rightdragging) {
                rightpoint_x_end = LOWORD(lParam);
                rightpoint_y_end = HIWORD(lParam);
                state.translate_x += (LOWORD(lParam) - rightlast_x) / 25.0f;
                state.translate_y -= (HIWORD(lParam) - rightlast_y) / 25.0f;
                rightdragging = 0;
            }
            return 0;
    }

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd,uMsg,wParam,lParam);

}

LRESULT CALLBACK HandleKeyPresses(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int pause = 0;
    static int drawmodel = 1;
    static int backface_culling = 1;
    static int clipping = 0;

    LOG_MSG("Pressing button <%u>\n", wParam);

    switch (wParam) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case 'J':
            state.draw_mtracer_data = 1; 
            break;
    }

    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void HandleCommandEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    assert(uMsg == WM_COMMAND);

    switch (LOWORD(wParam)) {
        case ID_FILE_EXIT:
            PostQuitMessage(0);
            break;
        case ID_FILE_OPENMTRACER:
        {
            TCHAR filename[MAX_PATH] = TEXT("");
            int    clickedOK;
            TCHAR OpenTitle[] = TEXT("Open M-Tracer Swing File");
            TCHAR szFilter[] = TEXT("M-Tracer Swing Files (*.txt)\0*.txt\0") \
                TEXT("All Files (*.*)\0*.*\0\0");
            OPENFILENAME data = {
                sizeof(OPENFILENAME),       // DWORD         lStructSize;
                hWnd,                       // HWND          hwndOwner;
                hInstance,                  // INSTANCE      hInstance;
                szFilter,                   // LPCTSTR       lpstrFilter;
                NULL,                       // LPTSTR        lpstrCustomFilter;
                0,                          // DWORD         nMaxCustFilter;
                0,                          // DWORD         nFilterIndex;
                filename,                   // LPTSTR        lpstrFile;
                MAX_PATH,                   // DWORD         nMaxFile;
                NULL,                       // LPTSTR        lpstrFileTitle;
                0,                          // DWORD         nMaxFileTitle;
                NULL,                       // LPCTSTR       lpstrInitialDir;
                OpenTitle,                  // LPCTSTR       lpstrTitle;
                OFN_FILEMUSTEXIST,          // DWORD         Flags;
                0,                          // WORD          nFileOffset;
                0,                          // WORD          nFileExtension;
                TEXT("txt"),                // LPCTSTR       lpstrDefExt;
                0L,                         // LPARAM        lCustData;
                NULL,                       // LPOFNHOOKPROC lpfnHook;
                NULL,                       // LPCTSTR       lpTemplateName;
                NULL,                       // void          *pvReserved;
                NULL,                       // DWORD         dwReserved;
                0,                          // DWORD         FlagsEx;
            };
            clickedOK = GetOpenFileName(&data);
            if (!clickedOK) break;
            LOG_MSG("Opening file <%s>\n", data.lpstrFile);
            char *filenameA;

            filenameA = filename;
            if (LoadMTracerFile(filenameA) != 0)
            {
                LOG_MSG("Fail to open M-Tracer swing file");
            }
        }
            break;
        case ID_HELP_CONTROLS:
            MessageBox(hWnd, TEXT(""), TEXT("Controls"), MB_OK);
            break;
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            return 0;
        case WM_SYSCOMMAND:							// Intercept System Commands
        {
            switch (wParam)							// Check System Calls
            {
                case SC_SCREENSAVE:					// Screensaver Trying To Start?
                case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
                LOG_MSG("Screensaver/Powersave attempted");
                return 0;							// Prevent From Happening
            }
            break;
        }

        case WM_COMMAND:
            HandleCommandEvents(hWnd, uMsg, wParam, lParam);
            break;

        case WM_MOUSEWHEEL:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_MOUSEMOVE:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
            return HandleMouse(hWnd, uMsg, wParam, lParam);     // Handle all mouse-related messages

        case WM_CLOSE:								// Did We Receive A Close Message?
        {
            PostQuitMessage(0);						// Send A Quit Message
            return 0;								// Jump Back
        }

        case WM_KEYDOWN:
            return HandleKeyPresses(hWnd, uMsg, wParam, lParam);

        case WM_SIZE:
        {
            ReSizeScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
            SendMessage(hStatus, WM_SIZE, 0, 0); // Resize the Statusbar
            LOG_MSG("resizing: w(%d), h(%d)\n", LOWORD(lParam), HIWORD(lParam));
            return 0;
        }
    }

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

BOOL CreateGLWindow(TCHAR* title, int width, int height, int bits)
{
    int         PixelFormat;            // Holds The Results After Searching For A Match
    WNDCLASS    wc;                     // Windows Class Structure
    DWORD       dwExStyle;              // Window Extended Style
    DWORD       dwStyle;                // Window Style
    RECT        WindowRect;             // Grabs Rectangle Upper Left / Lower Right Values

    WindowRect.left     =(long)0;       // Set Left Value To 0
    WindowRect.right    =(long)width;   // Set Right Value To Requested Width
    WindowRect.top      =(long)0;       // Set Top Value To 0
    WindowRect.bottom   =(long)height;  // Set Bottom Value To Requested Height

    hInstance			= GetModuleHandle(NULL);                // Grab An Instance For Our Window
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   // Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc		= (WNDPROC) WndProc;                    // WndProc Handles Messages
    wc.cbClsExtra		= 0;                                    // No Extra Window Data
    wc.cbWndExtra		= 0;                                    // No Extra Window Data
    wc.hInstance		= hInstance;                            // Set The Instance
    wc.hIcon            = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);          // Load The Arrow Pointer
    wc.hbrBackground	= NULL;                                 // No Background Required For GL
#ifdef HOLOCAM_USE_MENU_BAR
    wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MENU1);           // We Want A Menu
#else  // HOLOCAM_USE_MENU_BAR
    wc.lpszMenuName		= NULL;                                 // We Don't Want A Menu
#endif // HOLOCAM_USE_MENU_BAR
    wc.lpszClassName	= class_name;                           // Set The Class Name

    if (!RegisterClass(&wc)) {
        return FALSE;
    }

    dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;               // Window Extended Style
    dwStyle=WS_OVERLAPPEDWINDOW;                                // Windows Style
    AdjustWindowRectEx(&WindowRect, dwStyle, BOOL_MENU_ADJUSTMENT, dwExStyle); // Adjust Window To True Requested Size

    // Create The Window
    if (!(hWnd=CreateWindowEx(  dwExStyle,							// Extended Style For The Window
                                class_name,							// Class Name
                                title,								// Window Title
                                dwStyle |							// Defined Window Style
                                WS_CLIPSIBLINGS |					// Required Window Style
                                WS_CLIPCHILDREN,					// Required Window Style
                                0, 0,								// Window Position
                                WindowRect.right-WindowRect.left,	// Calculate Window Width
                                WindowRect.bottom-WindowRect.top,	// Calculate Window Height
                                NULL,								// No Parent Window
                                NULL,								// No Menu
                                hInstance,							// Instance
                                NULL)))								// Dont Pass Anything To WM_CREATE
    {
        WindowCleanup();
        assert("Window Creation Error."==0);
        return FALSE;
    }

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
        1,											// Version Number
        PFD_DRAW_TO_WINDOW |						// Format Must Support Window
        PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
        PFD_DOUBLEBUFFER,							// Must Support Double Buffering
        PFD_TYPE_RGBA,								// Request An RGBA Format
        bits,										// Select Our Color Depth
        0, 0, 0, 0, 0, 0,							// Color Bits Ignored
        0,											// No Alpha Buffer
        0,											// Shift Bit Ignored
        0,											// No Accumulation Buffer
        0, 0, 0, 0,									// Accumulation Bits Ignored
        16,											// 16Bit Z-Buffer (Depth Buffer)  
        8,											// Stencil for mtracer rotation testing
        0,											// No Auxiliary Buffer
        PFD_MAIN_PLANE,								// Main Drawing Layer
        0,											// Reserved
        0, 0, 0										// Layer Masks Ignored
    };
    
    if (!(hDC=GetDC(hWnd))) {
        WindowCleanup();
        assert("Can't Create A GL Device Context."==0);
        return FALSE;
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd))) {
        WindowCleanup();
        assert("Can't Find A Suitable PixelFormat."==0);
        return FALSE;
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd)) {
        WindowCleanup();
        assert("Can't Set The PixelFormat."==0);
        return FALSE;
    }

    if (!(hRC=wglCreateContext(hDC))) {
        WindowCleanup();
        assert("Can't Create A GL Rendering Context."==0);
        return FALSE;
    }

    if(!wglMakeCurrent(hDC,hRC)) {
        WindowCleanup();
        assert("Can't Activate The GL Rendering Context."==0);
        return FALSE;
    }

    ShowWindow(hWnd,SW_SHOW);                       // Show The Window
    SetForegroundWindow(hWnd);                      // Slightly Higher Priority
    SetFocus(hWnd);                                 // Sets Keyboard Focus To The Window
    ReSizeScene(width, height);                     // Set Up Our Perspective GL Screen

    if (!InitGL(&state))                                  // Initialize Our Newly Created GL Window
    {
        WindowCleanup();
        assert("Initialization Failed."==0);
        return FALSE;
    }

    return TRUE;
}

int __stdcall WinMain (__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
    MSG     msg;                                    // Windows Message Structure
    BOOL    done=FALSE;                             // Bool Variable To Exit Loop

    // Create Our OpenGL Window
    if (!CreateGLWindow(TEXT(HOLOCAM_WINDOW_TITLE_DEFAULT),640,480,24)) {
        return 0;
    }

    LoadMTracerFile("temp");

    while(!done) {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
            if (msg.message==WM_QUIT) {
                done=TRUE;
            } else {
                // Default Message handling
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            if (WindowNotMinimized) {
                RenderGL(&state);
                SwapBuffers(hDC);
            }
        }
    }

    // Shutdown
    WindowCleanup();
    return ((int)msg.wParam);
}
