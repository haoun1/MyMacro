// MyMacro.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MyMacro.h"
#include "resource.h"
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3
#define MAX_LOADSTRING 100
#define IDC_SEND 4


using namespace std;
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
double mouse[20];
int pcount = 0;
int painton = 1;
int write = 1;

POINT mousep;
// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//void AddMenus(HWND hWnd); //윈도우에 메뉴바를 추가하는 함수 입니다. WM_CREATE메시지를 받았을때 수행합니다.
wstring d2ws(double d); /*DOUBLE형식의 마우스 좌표를 받아서 wstring형식으로 바꿔서 반환합니다.
                        (paint작업이나 list, 버튼, 레이블 등의 입력이 LPCWSTR을 주로 받기 때문에 이용합니다*/
void OnBnClickButton1(HWND hWnd);


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
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYMACRO);
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
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
    RECT r = { 105, 20, 305, 40 };
    if (painton == 1)
        InvalidateRect(hWnd, &r, FALSE);



    switch (message)
    {
    case WM_RBUTTONDOWN:
    {
    }

        break;
    case WM_LBUTTONDOWN:
    {
       
        if (pcount <= 18 && write == 1)
        {
            mousep.x = GET_X_LPARAM(lParam);
            mousep.y = GET_Y_LPARAM(lParam);
            ClientToScreen(hWnd, &mousep);
            mouse[pcount] = mousep.x;
            mouse[pcount + 1] = mousep.y;
            wchar_t buffer1[60], buffer2[20] = TEXT(".Xpos : "), buffer3[10], buffer4[20] = TEXT("  Ypos : "), buffer5[10];
            swprintf(buffer1, 4, TEXT("%d"), pcount / 2 + 1);
            swprintf(buffer3, 5, TEXT("%g"), mouse[pcount]);
            swprintf(buffer5, 5, TEXT("%g"), mouse[pcount + 1]);
            wcscat_s(buffer1, _countof(buffer1), buffer2);
            wcscat_s(buffer1, _countof(buffer1), buffer3);
            wcscat_s(buffer1, _countof(buffer1), buffer4);
            wcscat_s(buffer1, _countof(buffer1), buffer5);
            SendMessage(List, LB_ADDSTRING, 0, (LPARAM)buffer1);
            pcount = pcount + 2;
        }
        else
        {
            painton = 0;
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, About);
        }

        UpdateWindow(hWnd);
       
    }
        break;
    case WM_LBUTTONUP:
        break;
    case WM_MOUSEMOVE:
    {
        mousep.x = GET_X_LPARAM(lParam);
        mousep.y = GET_Y_LPARAM(lParam);
        ClientToScreen(hWnd, &mousep);
        break;
    }
    case WM_COMMAND:
    {   
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDC_START:
            OnBnClickButton1(hWnd);
            write = 1;
            break;
        case IDC_STOP:
            break;
        case IDC_RESET:
            fill_n(mouse, 50, NULL);
            pcount = 0;
            List = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL, 0, 40, 300, 200, hWnd, (HMENU)IDC_LISTBOX, hInst, 0);
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
        TextOut(hdc, 105, 20, d2ws(mousep.x).c_str(), lstrlen(d2ws(mouse[0]).c_str()));
        TextOut(hdc, 205, 20, d2ws(mousep.y).c_str(), lstrlen(d2ws(mouse[1]).c_str()));
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
    {
      //  AddMenus(hWnd);
        CreateWindow(L"button", L"기록시작", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,0, 300, 100, 30, hWnd, (HMENU)IDC_START, hInst, NULL);
        CreateWindow(L"button", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 300, 50, 30, hWnd, (HMENU)IDC_STOP, hInst, NULL);
        CreateWindow(L"button", L"Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 300, 50, 30, hWnd, (HMENU)IDC_RESET, hInst, NULL);
        CreateWindow(TEXT("static"), TEXT("Cursor"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 0, 20, 100, 20, hWnd, (HMENU)IDC_STATIC, hInst, NULL);
        CreateWindow(TEXT("static"), TEXT("Xpos"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 105, 0, 100, 20, hWnd, (HMENU)IDC_STATIC, hInst, NULL);
        CreateWindow(TEXT("static"), TEXT("Ypos"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 205, 0, 100, 20, hWnd, (HMENU)IDC_STATIC, hInst, NULL);
        List = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL , 0, 40, 300, 200, hWnd, (HMENU)IDC_LISTBOX, hInst, 0);
    }
    break;
    case WM_INITDIALOG:
        break;
    case WM_TIMER:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, LOWORD(wParam));
            painton = 1;
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
/*
void AddMenus(HWND hWnd) {
    
    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&menu");


    SetMenu(hWnd, hMenubar);
}*/

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
void OnBnClickButton1(HWND hWnd)
{
    write = 0;
    for (int i = 0; i < pcount; i += 2) {
        SetCursorPos(mouse[i], mouse[i+1]); // 정상
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        Sleep(1000);
    }
    write = 1;
 }
