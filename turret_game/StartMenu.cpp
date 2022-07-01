#include "StartMenu.h"

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


            wifstream finID,finPS;
            finID.open("UserIdInfo.txt", ios_base::in | ios_base::binary);
            finPS.open("UserPsInfo.txt", ios_base::in | ios_base::binary);


            wstring saved_ID,saved_PS;//���� �ϳ� �߹ް� �ű⼭ �ֹ����Ÿ���
            
                if (finID.is_open()&& finPS.is_open())
                {
                    UserInfo Info;
                    getline(finID, saved_ID);
                    bool char_match = true;


                    for (int i = 0; i < saved_ID.size(); ++i)
                    {
                        if (isspace(saved_ID[i] == 0))
                        {
                            if (ID[i] == saved_ID[i])
                            {
                                Info.ID[i] = saved_ID[i];
                                char_match = true;
                            }
                            else
                            {
                                char_match = false;
                                break;
                            }
                        }

                        if (char_match)
                        {
                            int n=0;
                            for (int j = saved_ID.size()-1; j > i; --j)
                            {
                                Info.unique_num += (saved_ID[j]-48) * pow(10,n++);
                            }
                            break;
                        }



                    }
                    
                    if (char_match)
                    {
                        getline(finPS, saved_PS);

                    }











             wstring saved_PASSWORD;
             getline(fin, saved_PASSWORD);
                     while (saved_PASSWORD != PASSWORD)
                     {
                        
                         if (saved_PASSWORD == PASSWORD)
                         {
                             //PASSWORD �ν�
                             break;
                         }
                         //���� �ڷ� ������ (ã��)
                         getline(fin, saved_PASSWORD);
                         if (fin.peek() == EOF)
                         {
                             SetDlgItemText(hWnd, IDC_LOGIN_ERROR, _T("��й�ȣ�� ��ġ���� �ʽ��ϴ�! �ٽ� �Է����ּ���!"));
                             fin.seekg(0, ios::beg);
                         }
                        

                     }

                }
                else
                {
                    //������ ����������-> exit
                }

        }
        break;
        case IDC_CREATE_ACCOUNT:
        {
            SetDlgItemText(hWnd, IDC_LOGIN_INFO, _T(""));
            SetDlgItemText(hWnd, IDC_PASSWORD_INFO, _T(""));

            SetDlgItemText(hWnd, IDC_LOGIN_ERROR_TEXT, _T("����� ���̵� �Է����ּ���"));
            SetDlgItemText(hWnd, IDC_PASSWORD_ERROR_TEXT, _T("����� ��й�ȣ�� �Է����ּ���"));

            TCHAR ID[128];
            TCHAR PASSWORD[128];
            GetDlgItemText(hWnd, IDC_LOGIN_INFO, ID, 128);
            GetDlgItemText(hWnd, IDC_PASSWORD_INFO, PASSWORD, 128);

            UserInfo Info;
            Info.unique_num = 1;
            for (int i = 0; i < sizeof(ID) / sizeof(ID[0]); ++i)
            {
                Info.ID[i] = ID[i];
            }
           
            for (int i = 0; i < sizeof(PASSWORD) / sizeof(PASSWORD[0]); ++i)
            {
                Info.PASSWORD[i] = PASSWORD[i];
            }

            wifstream finch;
            finch.open("UserPSInfo.txt", ios_base::in | ios_base::app | ios_base::binary);

            if (finch.is_open())
            {
                wstring check;
                getline(finch, check);
                Info.unique_num++;
                if (finch.peek() == EOF)
                {
                    break;
                }
            }

            finch.close();



            wofstream foutID,foutPS;
            foutID.open("UserIdInfo.txt", ios_base::out |ios_base::app| ios_base::binary);
            foutPS.open("UserPsInfo.txt", ios_base::out | ios_base::app | ios_base::binary);

            if (foutID.is_open() && foutPS.is_open())
            {
                wstring table;

                foutID.write(Info.ID, sizeof(Info.ID));
                foutID.write(_T(" "), sizeof(_T(" ")));
                foutID.write((WCHAR*)Info.unique_num, sizeof(Info.unique_num));

                foutPS.write((WCHAR*)Info.unique_num, sizeof(Info.unique_num));
                foutPS.write(_T(" "), sizeof(_T(" ")));
                foutPS.write(Info.PASSWORD, sizeof(Info.PASSWORD));

            }
            foutID.close();
            foutPS.close();

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