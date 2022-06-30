#include "StartMenu.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool CALLBACK LoginMenu(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);
    switch (iMsg)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDC_LOGIN_BUTTON:
        {
            TCHAR ID[128];
            TCHAR PASSWORD[128];
            GetDlgItemText(hWnd, IDC_LOGIN_INFO, ID, 128);
            GetDlgItemText(hWnd, IDC_PASSWORD_INFO, PASSWORD, 128);

            wifstream fin;
            fin.open("UserInfo", ios_base::in | ios_base::binary);

            wstring saved_ID;

                if (fin.is_open())
                {

                    getline(fin, saved_ID);
                    while(saved_ID != ID);
                    {
                        getline(fin, saved_ID);
                        getline(fin, saved_ID);
                        if (fin.peek()==EOF)
                        {

                            break;
                        }
                    }
                    

                }

        }
        break;


        case IDOK:
        case IDCANCEL:
        {
            EndDialog(hWnd, LOWORD(wParam));
            return TRUE;
        }
        break;
        }


        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return FALSE;

}