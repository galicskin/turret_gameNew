#include "StartMenu.h"

using namespace std;
TCHAR tmpname[128];

BOOL CALLBACK LoginMenu( HWND hWnd,  UINT iMsg,  WPARAM wParam,  LPARAM lParam)
{
    
    UNREFERENCED_PARAMETER(lParam);
    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
        HWND hide = GetDlgItem(hWnd, IDC_PASSWORD_INFO);
        ShowWindow(hide, SW_HIDE);
        hide= GetDlgItem(hWnd, IDC_PASSWORD_ERROR_TEXT);
        ShowWindow(hide, SW_HIDE);
        hide = GetDlgItem(hWnd, IDC_PASSWORD_TEXT);
        ShowWindow(hide, SW_HIDE);
        hide = GetDlgItem(hWnd, IDC_CREATE_ACCOUNT);
        ShowWindow(hide, SW_HIDE);
        hide = GetDlgItem(hWnd, IDOK);
        ShowWindow(hide, SW_HIDE);
        SetDlgItemText(hWnd, IDC_LOGIN_ERROR_TEXT, _T("기록을 저장할 아이디를 입력해주세요"));
        return TRUE;
        }
    
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_LOGIN_BUTTON:
        {
            
           
            GetDlgItemText(hWnd, IDC_LOGIN_INFO, tmpname, 128);
      

            EndDialog(hWnd, LOWORD(wParam));
            return TRUE;

        }
        break;
        case IDOK:
        case IDCANCEL:
        {
            PostQuitMessage(0);
            EndDialog(hWnd, LOWORD(wParam));
            return TRUE;
        }
        break;
        
        }
    }
    //return FALSE;

    }
    return FALSE;
}