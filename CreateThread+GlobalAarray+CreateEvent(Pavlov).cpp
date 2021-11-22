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
double results[N];
HANDLE events[N];


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

    results[prms->id] = cur_min;
    bool b = SetEvent(events[prms->id]);
    return 0;
}

int main()
{
    double begin = 0;
    double end = 100;

    for  (int i = 0; i < N; i++)
    {
        events[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

        params[i].begin = begin;
        params[i].end = begin + end/N;
        params[i].id = i;

        handles[i] = CreateThread(NULL, 0, thread_f, &params[i], 0, &th_id[i]);


        begin += end/N;
    }

    WaitForMultipleObjects(N, events, true, INFINITY);

    double cur_min = results[0];
    for  (int i = 1; i < N; i++)
        if (results[i] < cur_min)
            cur_min = results[i];

    cout << "CreateThread + global array + CreateEvent" << endl;
    cout << "f = sin(x)*x+cos(x*x)*x*x" << endl;
    cout << "min(f) = " << cur_min << endl;
    return 0;
}
