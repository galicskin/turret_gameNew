#pragma once
#include "framework.h"
#include "resource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
struct UserInfo
{
	WCHAR ID[128], PASSWORD[128];
	int unique_num;
};


bool CALLBACK LoginMenu(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);





