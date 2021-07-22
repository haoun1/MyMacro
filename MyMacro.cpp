﻿// MyMacro.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MyMacro.h"
#include "resource.h"



using namespace std;
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
double mouse[20];                               // 클릭한 좌표를 저장해놓고 반복시작할때 참고하는 배열
int painton = 1;                                // 1이면 WM_PAINT의 반복호출을함 0이면 멈춤
int pcount = 0;                                 // mouse[20]에 값을 넣고 참조하는데 활용하는 변수
int write = 0;                                  // write = 1이면 처음 윈도우의 listbox에 mouse[20]에 저장 된 값들을 넣어줌
int bid;
POINT mmove;
POINT mclick;
BOOL check[10] = { FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE };


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//void AddMenus(HWND hWnd); //윈도우에 메뉴바를 추가하는 함수 입니다. WM_CREATE메시지를 받았을때 수행합니다.
wstring d2ws(double d); /*DOUBLE형식의 마우스 좌표를 받아서 wstring형식으로 바꿔서 반환합니다.
                        (paint작업이나 list, 버튼, 레이블 등의 입력이 LPCWSTR을 주로 받기 때문에 이용합니다*/
void MoveButton(HWND hWnd);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    // TODO: 여기에 코드를 입력합니다.
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYMACRO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYMACRO));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYMACRO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
      CW_USEDEFAULT, 0, 330, 270, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static HWND List;
    static HWND Button;
    if (pcount > 1 && write == 1)
        {
        write = 0;
        OutputDebugString(L"1");
        List = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL, 5, 0, 300, 200, hWnd, (HMENU)IDC_LISTBOX, hInst, 0);
            
            for (int i = 0; i < pcount; i++)
            {
                SendMessage(List, LB_DELETESTRING, 0, 0);
            }
            for (int i = 0; i < pcount; i += 2)
            {
                wchar_t buffer1[60], buffer2[20] = TEXT(".Xpos : "), buffer3[10], buffer4[20] = TEXT("  Ypos : "), buffer5[10];
                swprintf(buffer1, 4, TEXT("%d"), i / 2 + 1);
                swprintf(buffer3, 5, TEXT("%g"), mouse[i]);
                swprintf(buffer5, 5, TEXT("%g"), mouse[i + 1]);
                wcscat_s(buffer1, _countof(buffer1), buffer2);
                wcscat_s(buffer1, _countof(buffer1), buffer3);
                wcscat_s(buffer1, _countof(buffer1), buffer4);
                wcscat_s(buffer1, _countof(buffer1), buffer5);
                SendMessage(List, LB_ADDSTRING, 0, (LPARAM)buffer1);
            }
            InvalidateRect(hWnd, NULL, FALSE);
            UpdateWindow(hWnd);
        }
 
    switch (message)
    {
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
    {   
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDC_WRITE:
            fill_n(mouse, 50, NULL);
            fill_n(check, 10, FALSE);
            pcount = 0; 
            List = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL, 5, 0, 300, 200, hWnd, (HMENU)IDC_LISTBOX, hInst, 0);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, About);
            break;
        case IDC_START:
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            MoveButton(hWnd);
            break;
        case IDC_RESET:
            fill_n(mouse, 50, NULL);
            fill_n(check, 10, FALSE);
            pcount = 0;
            List = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL, 5, 0, 300, 200, hWnd, (HMENU)IDC_LISTBOX, hInst, 0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
    {
      //  AddMenus(hWnd);
        int x, y, width, height;
        RECT rtDesk, rtWindow;
        GetWindowRect(GetDesktopWindow(), &rtDesk);
        GetWindowRect(hWnd, &rtWindow);
        width = rtWindow.right - rtWindow.left;
        height = rtWindow.bottom - rtWindow.top;

        x = (rtDesk.right - width) / 2;
        y = (rtDesk.bottom - height) / 2;
        MoveWindow(hWnd, x, y, width, height, TRUE);
        CreateWindow(L"button", L"기록시작", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,0, 200, 100, 30, hWnd, (HMENU)IDC_WRITE, hInst, NULL);
        CreateWindow(L"button", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 200, 50, 30, hWnd, (HMENU)IDC_START, hInst, NULL);
        CreateWindow(L"button", L"Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 200, 50, 30, hWnd, (HMENU)IDC_RESET, hInst, NULL);
        List = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL , 5, 0, 300, 200, hWnd, (HMENU)IDC_LISTBOX, hInst, 0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

        
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        ShowWindow(hWnd, SW_MAXIMIZE);
        SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
        CreateWindow(TEXT("static"), TEXT("Cursor"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 0, 20, 100, 20, hWnd, (HMENU)IDC_STATIC, hInst, NULL);
        CreateWindow(TEXT("static"), TEXT("Xpos"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 105, 0, 100, 20, hWnd, (HMENU)IDC_STATIC, hInst, NULL);
        CreateWindow(TEXT("static"), TEXT("Ypos"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 205, 0, 100, 20, hWnd, (HMENU)IDC_STATIC, hInst, NULL);
        return (INT_PTR)TRUE;
    case WM_MOUSEMOVE:
    {
        mmove.x = GET_X_LPARAM(lParam);
        mmove.y = GET_Y_LPARAM(lParam);
        ClientToScreen(hWnd, &mmove);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        check[pcount/2] = TRUE;
        if (pcount <= 18)
        {
            mclick.x = GET_X_LPARAM(lParam);
            mclick.y = GET_Y_LPARAM(lParam);
            ClientToScreen(hWnd, &mclick);
            mouse[pcount] = mclick.x;
            mouse[pcount + 1] = mclick.y;
            pcount = pcount + 2;
            write = 1;
        }
        else
        {
            painton = 0;
            if (MessageBox(hWnd, TEXT("10번 이상은 불가능합니다."), TEXT("WARNING"), MB_OK))
                painton = 1;
         
        }
        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if(LOWORD(wParam) == MB_OK)
        break;
    case WM_PAINT:
    {   
        HDC MemDC;
        HBITMAP MyBitmap, OldBitmap;

        hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        TextOut(hdc, 105, 20, d2ws(mmove.x).c_str(), lstrlen(d2ws(mmove.x).c_str()));
        TextOut(hdc, 205, 20, d2ws(mmove.y).c_str(), lstrlen(d2ws(mmove.y).c_str()));

        MemDC = CreateCompatibleDC(hdc);
        
        for(int i=0;i<pcount;i+=2) //3번입력시 pcount = 6 이므로 i는 0,2,4가 들어감
        {
            bid = 144+(i/2);
            if (check[i/2] == TRUE)
            {
                MyBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(bid));
                OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
                BitBlt(hdc, int(mouse[i]), int(mouse[i+1]), 1000, 1000, MemDC, 0, 0, SRCCOPY);
            }
        }

        if (check[0] == TRUE)
        {
            SelectObject(MemDC, OldBitmap);
            DeleteObject(MyBitmap);
            DeleteDC(MemDC);
        }
        EndPaint(hWnd, &ps);
    }
    }
            
    return (INT_PTR)FALSE;
}

wstring d2ws(double d)
{

    string s = to_string(d);
    int len;

    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;

    return r;
}
void MoveButton(HWND hWnd)
{
    BOOL loop = 1;
    while (loop)
    {
        for (int i = 0; i < pcount; i += 2) {
            SetCursorPos(int(mouse[i]), int(mouse[i + 1])); // 정상
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(500);
            if (GetAsyncKeyState(VK_ESCAPE))
            {
                loop = 0;
                break;
            }
        }

    }
 }
