// turret_game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "turret_game.h"
#include <iostream>
#include "StartMenu.h"
#include <vector>
#include "InGameWindow.h"
#include <ctime>
#include"CloseMenu.h"
#define MAX_LOADSTRING 100

using namespace Gdiplus;
// Gdi
ULONG_PTR g_GdiToken;
void Gdi_Init();
void Gdi_End();
//더블버퍼
HBITMAP hBackImage;
BITMAP bitBack;


HBITMAP hDoubleBufferImage;
void doublebuffer(HWND hWnd, HDC hdc);
void settingWall(int interval, int num, std::vector<Game_Object_Manager*>& CannonBall_Wall);
void settingarrow(std::vector<Enemy_Missile*>& arrow);
void settingCannonball(std::vector<Game_Object_Manager*>& T, cannon& turret, double R, RECT Clientrc);


void UpdateFrame(HWND hWnd);
void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK CreateArrow(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK collide(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);





// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


std::vector<Game_Object_Manager*> CannonBall_Wall;

std::vector<Enemy_Missile*> arrow;

cannon turret({ 0,-1 });


void Update();
int Run_Frame_Max = 0;
int Run_Frame_Min = 0;
int curFrame = Run_Frame_Min;
clock_t start, end;

int rot = 0;
RECT Clientrc;

int Player_score;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_TURRETGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TURRETGAME));

    MSG msg;

    // 기본 메시지 루프입니다:
    /*
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    */

    Gdi_Init();
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg); //메세지 번역(어떤 메세지인지
                DispatchMessage(&msg);
            }
        }
        //else
        {
            Update();
        }
    }

    Gdi_End();
    return (int)msg.wParam;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TURRETGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TURRETGAME);
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
   // static bool login = false;
    GetClientRect(hWnd, &Clientrc);
    
    
    switch (message)
    {
    case WM_CREATE:
    {
        
        
            DialogBox(hInst, MAKEINTRESOURCE(IDD_LOGIN_BOX), hWnd, (DLGPROC)LoginMenu);  
           Image temp((WCHAR*)L"images/rectangle.png");       

           settingWall(1,14, CannonBall_Wall);//간격크기,갯수
           Player_score=0;
            
           start = clock();

        SetTimer(hWnd, 1, 1, (TIMERPROC)TimerProc);
        SetTimer(hWnd, 2, 2000, (TIMERPROC)CreateArrow);
        SetTimer(hWnd, 3, 1, (TIMERPROC)collide);

    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
                KillTimer(hWnd, 3);
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_CLOSEMENU), hWnd, (DLGPROC)CloseMenu);
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

          

            doublebuffer(hWnd, hdc);

              
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...


            for (auto iter = arrow.begin(); iter != arrow.end(); ++iter)
            {
                if ((*iter)->getDestroy())
                {
                    delete (*iter);
                    (*iter) = nullptr;
                    arrow.erase(iter);
                    break;
                }
            }

            for (auto iter = CannonBall_Wall.begin(); iter != CannonBall_Wall.end(); ++iter)
            {
                if ((*iter)->getDestroy())
                {
                    delete (*iter);
                    (*iter) = nullptr;
                    CannonBall_Wall.erase(iter);
                    break;
                }
            }


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
       
        PostQuitMessage(0);
        KillTimer(hWnd,1);
        KillTimer(hWnd, 2);
        KillTimer(hWnd, 3);

        for (auto iter = arrow.begin(); iter != arrow.end(); ++iter)
        {
                delete (*iter);
                break;
        }

        for (auto iter = CannonBall_Wall.begin(); iter != CannonBall_Wall.end(); ++iter)
        {
            if ((*iter)->getDestroy())
            {
                delete (*iter);            
                break;
            }
        }



        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
//업데이트마다 입력받기
void Update()
{
    DWORD newTime = GetTickCount64();
    static DWORD oldTime = newTime;
    
    if (newTime - oldTime < 100)
    {
        return;
    }

    oldTime = newTime;

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        if(rot < 80 )
        rot += 10;
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        if(rot>-80)
        rot -= 10;
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {

    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {

    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        end= clock();
        if (end - start > 1000)
        {
        settingCannonball(CannonBall_Wall, turret,20,Clientrc);
        start = clock();
        }
    }

}


void Gdi_Init()
{
    GdiplusStartupInput gpsi;
    GdiplusStartup(&g_GdiToken, &gpsi, NULL);
}

void Gdi_Draw(HDC hdc)
{

}

void Gdi_End()
{
    GdiplusShutdown(g_GdiToken);
}
//더블버퍼
void doublebuffer(HWND hWnd, HDC hdc)
{
   //배경 
    hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/background11.bmp"),
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    GetObject(hBackImage, sizeof(BITMAP), &bitBack);

    HDC hMemDC;  //화면과 같은 형태의 메모리
    HBITMAP hOldBitmap; //이미지 라고 생각하면 편함
   
    HDC hMemDC2;
    HBITMAP hOldBitmap2;




    hMemDC = CreateCompatibleDC(hdc); //hdc메모리 생성(기존)
    if (hDoubleBufferImage == NULL)  //hdc 로 부터 비트맵 이미지 제작(도화지)
    {
        hDoubleBufferImage = CreateCompatibleBitmap(hdc, Clientrc.right, Clientrc.bottom);
    }
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);   // DC라는 화면(메모리)에 도화지를 넣음


    //for background

    hMemDC2 = CreateCompatibleDC(hMemDC);  //기존 DC에서 DC2(hdc메모리) 생성 ==(도화지 생성)
    hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage);  //위에서 DC2로 부터 비트맵2 생성 ==((수지)그림을 DC2에 그려줌, 그걸 올드비트맵2라고 저장)
    int bx = bitBack.bmWidth;
    int by = bitBack.bmHeight;

    StretchBlt(hMemDC, 0, 0, Clientrc.right, Clientrc.bottom, hMemDC2, 0, 0, bx, by, SRCCOPY);  //DC의 화면에 DC2가 있는 화면을 그려줌==(현재 DC2에 있는 화면을  기존 DC에 옮겨줌)
    
    SelectObject(hMemDC2, hOldBitmap);   //DC2 지움
    DeleteDC(hMemDC2);

    Image wall_img((WCHAR*)L"images/rectangle.png");
    Image ball_img((WCHAR*)L"images/검은원.png");

    for (auto iter = CannonBall_Wall.begin(); iter != CannonBall_Wall.end(); ++iter)
    {
        if ((*iter)->getWhat() == shape::Wall)
        {
            (*iter)->Draw(hMemDC, wall_img);
        }
        else if ((*iter)->getWhat() == shape::CannonBall)
        {
            (*iter)->Draw(hMemDC, ball_img);
        }
    }

    Image arrow_img((WCHAR*)L"images/적 미사일.png");
    for (auto iter = arrow.begin(); iter != arrow.end(); ++iter)
    {
        (*iter)->Draw(hMemDC, arrow_img);
    }

    Image cannon_img((WCHAR*)L"images/cannon.png");
    turret.Draw(hMemDC, cannon_img, Clientrc,rot);


    std::wstring Score = std::to_wstring(Player_score);
    Graphics graphics(hMemDC);
    // >> : txt
    SolidBrush brush(Color(255, 255, 0, 0)); //(Color(alpha, R, G, B))
    FontFamily fontFamily(L"Times New Roman");
    Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF pointF(10.0f, 20.0f);
    graphics.DrawString(Score.c_str(), -1, &font, pointF, &brush);
    // << :
    

    BitBlt(hdc, 0, 0, Clientrc.right, Clientrc.bottom,
        hMemDC, 0, 0, SRCCOPY);  //현재 그려진 DC를 모니터(hdc)에 그려줌  -> 그리는 단계가 사라지고 바로 보여줌..



    std::wstring rotation = std::to_wstring(rot);
    TextOut(hdc, 300, 300, rotation.c_str(), (int)rotation.size() );
   


    SelectObject(hMemDC, hOldBitmap);  //DC해제
    DeleteDC(hMemDC);



   // BitBlt(hdc, 0, 0, Clientrc.right, Clientrc.bottom,
       // hMemDC, 0, 0, SRCCOPY);

}
//벽 만들기
void settingWall( int interval, int num, std::vector<Game_Object_Manager*>& CannonBall_Wall )
{
   
    for (int i = 0; i < num; ++i)
    {
        Game_Object_Manager* wall = nullptr;
        wall = new LifeBlock;
        wall->setPos(100 + (80+interval) * i, 500);

        CannonBall_Wall.push_back(wall);
    }

    
}
//적 미사일 만들기
void settingarrow(std::vector<Enemy_Missile*>& arrow)
{
    
    srand(time(NULL));

    Enemy_Missile* arrowR = nullptr;
    arrowR = new Enemy_Missile;

    int cases = rand() % 30;

    
    arrowR->setPos(120 + cases*40, 100);


    arrowR->setVelocity(0, 15);

    arrow.push_back(arrowR);
}
//아군총알 만들기
void settingCannonball(std::vector<Game_Object_Manager*>& T,cannon &turret,double R, RECT Clientrc)
{

    Vector CL((Clientrc.right - Clientrc.left)/2, Clientrc.bottom - 85);
    Game_Object_Manager* ball = nullptr;
    ball = new Friend_Missile(CL);
    


    turret.Shot((*ball),rot, R);
    
    T.push_back(ball);
}


//프레임
void UpdateFrame(HWND hWnd)
{
    curFrame++;
    if (curFrame > Run_Frame_Max)
    {
        curFrame = Run_Frame_Min;
    }
}

//프레임 업데이트, 각 물체들의 움직임
void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    UpdateFrame(hWnd);


    for (auto iter = arrow.begin(); iter != arrow.end(); ++iter)
    {

        (*iter)->down();
    }

    for (auto iter = CannonBall_Wall.begin(); iter != CannonBall_Wall.end(); ++iter)
    {

        if ((*iter)->getWhat() == CannonBall)
        {
            (*iter)->MOVE();
        }
    }




    InvalidateRect(hWnd, NULL, FALSE);
}
//일정 시간마다 적 미사일 생성
void CALLBACK CreateArrow(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{

    settingarrow(arrow);


    InvalidateRect(hWnd, NULL, FALSE);
}
//매 시간마다 충돌체크
void CALLBACK collide(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{

    for (auto iter1 = arrow.begin(); iter1 != arrow.end(); ++iter1)
    {

        if ((*iter1)->inWindow(Clientrc))
        {
            for (auto iter2 = CannonBall_Wall.begin(); iter2 != CannonBall_Wall.end(); ++iter2)
            {
                if ((*iter2)->inWindow(Clientrc))
                {

                    if ((*iter1)->is_collide(*(*iter2)))
                    {
                        if ((*iter2)->getWhat() == Wall)
                        {
                            (*iter2)->Lifedown();
                            (*iter1)->setDestroy(true);
                            break;
                        }
                        else if ((*iter2)->getWhat() == CannonBall)
                        {
                            (*iter1)->setDestroy(true);
                            (*iter2)->setDestroy(true);
                            
                            Player_score+=10;
                            break;
                        }

                    }
                }
                else
                {
                    (*iter2)->setDestroy(true);


                }

            }
        }
        else
        {
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            KillTimer(hWnd, 3);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_CLOSEMENU), hWnd, (DLGPROC)CloseMenu);
            (*iter1)->setDestroy(true);

        }
    }




    InvalidateRect(hWnd, NULL, FALSE);
}