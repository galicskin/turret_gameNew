#include "CloseMenu.h"

using namespace std;

extern int Player_score;
extern TCHAR tmpname[128];


struct Data
{
    int num;
    int score;
    wstring name;

};

BOOL CALLBACK CloseMenu(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (iMsg)
    {
    case WM_INITDIALOG://WM_SETFONT : //WM_INITDIALOG:
    {
        Data current_player;
        current_player.score = Player_score;
        current_player.name = tmpname;
        current_player.num = 0;

        Data Rank_data;
        Data Rank_palyer[10];

        wifstream fin;

        fin.open("UserIdInfo.txt", ios_base::in | ios_base::binary);

        int i = 0;
        if (fin.is_open())  //데이터 받아옴
        {

            

            while(!fin.eof())
            {
                fin.read((wchar_t*)&Rank_data, sizeof Rank_data);
                Rank_palyer[i].num = Rank_data.num;
                Rank_palyer[i].score = Rank_data.score;
                Rank_palyer[i].name = Rank_data.name;
                i++;
                


            }

        }
        fin.close();
        //정렬

        if (i == 0)
        {
            current_player.num = 1;

            wofstream fout;

            fout.open("UserIdInfo.txt", ios_base::out | ios_base::binary);

            if (fout.is_open())  //데이터 정렬및 쓰기
            {

                std::wstring Sc = std::to_wstring(current_player.score);
                Sc = L"     " + current_player.name + L"   " + Sc;
                SetDlgItemText(hWnd, IDC_STATIC_RANK, Sc.c_str());

                fout.write((wchar_t*)&current_player, sizeof current_player);
                
            }

            fout.close();



        }
        else
        {

            bool rankchange = true;
            for (int j = i - 1; j >= 0; --j)
            {
                if (rankchange)
                {
                    if (j == 9)
                    {
                        current_player.score > Rank_palyer[j].score ? Rank_palyer[j] = current_player, rankchange = true : rankchange = false;
                    }
                    else
                    {
                        Data temp = Rank_palyer[j];
                        current_player.score > Rank_palyer[j].score ? Rank_palyer[j] = current_player, Rank_palyer[j + 1] = temp, rankchange = true : rankchange = false;
                    }
                }
                else
                {
                    break;
                }
            }

        //현재 아이디의 점수 출력

        //TCHAR output[258];

       
        //TextOut(hdc, 300, 300, Sc.c_str(), (int)rotation.size());
     
        std::wstring Sc = std::to_wstring(current_player.score);
        Sc = L"     " + current_player.name + L"   "+ Sc;
        SetDlgItemText(hWnd, IDC_STATIC_CURRENT, Sc.c_str());
        


        


        wofstream fout;

        fout.open("UserIdInfo.txt", ios_base::out | ios_base::binary);

        if (fout.is_open())  //데이터 정렬및 쓰기
        {
             for (int j = 0; j < 10; j++)
            {
                std::wstring Rc = std::to_wstring(Rank_palyer[j].score);
                Rc = L"     " + Rank_palyer[j].name + L"   " + Rc + L"\r\n";
                SetDlgItemText(hWnd, IDC_STATIC_RANK, Rc.c_str());

                fout.write((wchar_t*)&Rank_data, sizeof Rank_data);
            }
        }

        fout.close();
        }


        //EndDialog(hWnd, LOWORD(wParam));
        return TRUE;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
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

