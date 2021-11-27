#include <omp.h>
#include <iostream>
#include <math.h>

using namespace std;

/*
 Компиляция:  g++-11 -o OMP OMP.cpp -lstdc++ -fopenmp
 */

double f(double x)
{
    return sin(x)*x+cos(x*x)*x*x;
}

double findLocalMin(int i, int num, double delta)
{
    double wideOfInterval = 100/num, begin = 0, end;
    begin += i * wideOfInterval;
    end = begin + wideOfInterval;
    
    double x = begin;
    double cur_min = f(x);
    x += delta;
    
    while (x <= end)
    {
        double cur_f =  f(x);
        
        if (cur_f < cur_min)
            cur_min = cur_f;

        x += delta;
    }

    return cur_min;
}

bool isItGoodDelta(double delta)
{
    return (delta <= 50 && delta > 0);
}


int main(int argc, char **argv)
{
    // Запрет библиотеке OpenMP менять число потоков во время исполнения
    omp_set_dynamic(0);
    
    double delta;
    cout << "Enter delta (e.g. 0.0001): (0 - exit) --> ";
    cin >> delta;
    if (!isItGoodDelta(delta))
    {
        cout << "Wrong delta" << endl;
        return 0;
    }
    
    int N;
    cout << "Enter thread count (e.g. 4): (0 - exit) --> ";
    cin >> N;
    if (N <= 0)
    {
        cout << "Wrong thread count" << endl;
        return 0;
    }
    
    // Установка числа потоков
    omp_set_num_threads(N);
    double localMins[N];
    int i;
    
    // Каждая итерация цикла - отдельный поток
#pragma omp parallel for shared(localMins, N, delta) private(i)
    for (i = 0; i < N; i++)
        localMins[i] = findLocalMin(i, N, delta);
    
    // Однопоточное сравнение локальных минимумов
    double globalMin = localMins[0];
    for (i = 1; i < N; i++)
        if (globalMin > localMins[i])
            globalMin = localMins[i];
    
    cout << "OpenMP" << endl;
    cout << "f = sin(x)*x+cos(x*x)*x*x" << endl;
    cout << "min(f) = " << globalMin << endl;
    
    return 0;
}
