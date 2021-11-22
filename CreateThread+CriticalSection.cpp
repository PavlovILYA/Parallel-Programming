#include <iostream>
#include <math.h>
#include <windows.h>

using namespace std;

double f(double x)
{
    return sin(x)*x+cos(x*x)*x*x;
}

struct th_params
{
    int id;
    double begin;
    double end;
};

const int N = 4;
HANDLE handles[N];
DWORD th_id[N];
th_params params[N];
CRITICAL_SECTION cs;
double global_min = 10000000;

DWORD __stdcall thread_f(void *args)
{
    th_params *prms = (th_params*)args;
    double x = prms->begin;
    double cur_min = f(x);
    x += 0.0001;

    while(x < prms->end)
    {
        double cur_f = f(x);

        if (cur_f < cur_min)
            cur_min = cur_f;

        x += 0.0001;
    }

    EnterCriticalSection(&cs);
    if (cur_min < global_min)
        global_min = cur_min;
    LeaveCriticalSection(&cs);

    return 0;
}

int main()
{
    InitializeCriticalSection(&cs);

    double begin = 0;
    double end = 100;

    for  (int i = 0; i < N; i++)
    {
        params[i].begin = begin;
        params[i].end = begin + end/N;

        handles[i] = CreateThread(NULL, 0, thread_f, &params[i], 0, &th_id[i]);

        begin += end/N;
    }

    WaitForMultipleObjects(N, handles, true, INFINITY);

    cout << "CreateThread + CriticalSection" << endl;
    cout << "f = sin(x)*x+cos(x*x)*x*x" << endl;
    cout << "min(f) = " << global_min << endl;
    return 0;
}
