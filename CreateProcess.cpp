#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <limits.h>

#include "atlstr.h"

using namespace std;

void main(int argc, char* argv[])
{
    const int N = 4;
    STARTUPINFO si[N];
    PROCESS_INFORMATION pi[N];

    for (int i = 0; i < N; i++)
    {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));
    }

    double begin = 0;
    double end;
    double end_g = 100;

    for (int i = 0; i < N; i++)
    {
        end = begin + end_g / N;

        WCHAR cmd_process[300]; 
        int b = int(begin);
        int e = int(end); // потому что wsprintf не принимает double...
        wsprintf(cmd_process, TEXT("C:\\Users\\g2011\\Documents\\app1\\Debug\\new_process_CreateProcess.exe %d %d %d"), i+1, b, e);

        if (!CreateProcess(NULL, cmd_process, NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i]))
        {
            printf("CreateProcess failed (%d).\n", GetLastError());
            return;
        }

        begin += end_g / N;
    }

    for (int i = 0; i < N; i++)
    {
        // Wait until child process exits.
        WaitForSingleObject(pi[i].hProcess, INFINITE);
        // Close process and thread handles. 
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }


    std::string line;
    double min = DBL_MAX;
    for (int i = 0; i < N; i++)
    {
        char firstPart[50] = "C:\\Users\\g2011\\Documents\\";
        char lastPart[10] = ".txt";
        char fileName[90];
        sprintf_s(fileName, "%s%d%s", firstPart, i+1, lastPart);
        double cur_min;
        std::ifstream in(fileName); // окрываем файл для чтения
        if (in.is_open())
        {
            std::getline(in, line);
            //std::cout << line << std::endl;
            cur_min = atof(line.c_str());
            if (min > cur_min)
                min = cur_min;
        }
        in.close();     // закрываем файл
    }

    cout << "CreateProcess + files" << endl;
    cout << "f = sin(x)*x+cos(x*x)*x*x" << endl;
    cout << "min(f) = " << min << endl;
}
