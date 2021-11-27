#include<iostream>
#include<mpi.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

/*
 Компиляция: mpic++ -o MPI_all MPI_all.cpp
 Запуск:     mpiexec -n 4 ./MPI_all
             (4 - количество процессов, может быть другим)
 */

 
double f(double x)
{
    return sin(x)*x+cos(x*x)*x*x;
}

double findLocalMin(double begin, double end, double delta)
{
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
 
int main(int argc, char **argv)
{
    int myid, numprocs, namelen;
    double delta, localMin, globalMin;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    double startwtime = 0.0, endwtime;
 
    // Инициализация подсистемы MPI
    MPI_Init(&argc, &argv);
    // Получить размер коммуникатора MPI_COMM_WORLD (общее число процессов в рамках задачи)
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    // Получить номер текущего процесса в рамках коммуникатора MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
 
    cout << "Process " << myid+1 << " of " << numprocs << " is on " << processor_name << endl;

    // Если это главный процесс, принимаем значение шага от пользователя
    if(myid == 0)
    {
        // ожидаем остальные потоки, чтобы вывод был последовательный
        usleep(300);
        cout << "Enter delta (e.g. 0.0001): (0 - exit) --> ";
        cin >> delta;
        startwtime = MPI_Wtime();
    }
    
    // Рассылка количества значения шага (в том числе и себе)
    MPI_Bcast(&delta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (delta > 50 || delta <= 0)
    {
        // Освобождение подсистемы MPI (т.к. шаг задан неадекватно - выходим)
        MPI_Finalize();
        return 0;
    }

    double wideOfInterval = 100/numprocs, begin = 0, end;
    begin += myid * wideOfInterval;
    end = begin + wideOfInterval;
            
    localMin = findLocalMin(begin, end, delta);
 
    // Сброс результатов со всех процессов и нахождение минимального
    MPI_Reduce(&localMin, &globalMin, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
 
    // Если это главный процесс, вывод полученного результата
    if(myid==0)
    {
        cout << "MPI (MPI_Bcast + MPI_Reduce)" << endl;
        cout << "f = sin(x)*x+cos(x*x)*x*x" << endl;
        cout << "min(f) = " << globalMin << endl;
        endwtime = MPI_Wtime();
        cout << "Spent time = " << endwtime-startwtime << endl;
    }
 
    // Освобождение подсистемы MPI
    MPI_Finalize();
    return 0;
}

