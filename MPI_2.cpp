#include<iostream>
#include<mpi.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

/*
 Компиляция: mpic++ -o MPI_2 MPI_2.cpp
 Запуск:     mpiexec -n 4 ./MPI_2
             (4 - количество процессов, может быть другим)
 */

 
double f(double x)
{
    return sin(x)*x+cos(x*x)*x*x;
}

double findLocalMin(int id, int num, double delta)
{
    double wideOfInterval = 100/num, begin = 0, end;
    begin += id * wideOfInterval;
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
    if (delta > 50 || delta <= 0)
        return false;
    else
        return true;
}
 
int main(int argc, char **argv)
{
    int myid, numprocs, namelen;
    double delta, localMin, globalMin;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    double startwtime = 0.0, endwtime;
    MPI_Status stat;
 
    // Инициализация подсистемы MPI
    MPI_Init(&argc, &argv);
    // Получить размер коммуникатора MPI_COMM_WORLD (общее число процессов в рамках задачи)
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    // Получить номер текущего процесса в рамках коммуникатора MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name,&namelen);
 
    cout << "Process " << myid+1 << " of " << numprocs << " is on " << processor_name << endl;

    // Главный процесс
    if (myid == 0)
    {
        // Ожидаем остальные потоки, чтобы вывод был последовательный
        usleep(300);
        cout << "Enter delta (e.g. 0.0001): (0 - exit) --> ";
        cin >> delta;
        startwtime = MPI_Wtime();
        
        // Рассылка значения шага (но не себе)
        for (int i = 1; i < numprocs; i++)
            MPI_Send(&delta, 1, MPI_DOUBLE, i, i+1000, MPI_COMM_WORLD);
        
        if(!isItGoodDelta(delta))
        {
            // Освобождение подсистемы MPI (т.к. шаг задан неадекватно - выходим)
            MPI_Finalize();
            return 0;
        }
        
        localMin = findLocalMin(myid, numprocs, delta);
        globalMin = localMin;
        
        // Получение локальных минимумов других процессов и поиск глобального среди них
        for (int i = 1; i < numprocs; i++)
        {
            MPI_Recv(&localMin, 1, MPI_DOUBLE, i, i+2000, MPI_COMM_WORLD, &stat);
            if (globalMin > localMin)
                globalMin = localMin;
        }
        
        cout << "MPI (MPI_Send + MPI_Recv)" << endl;
        cout << "f = sin(x)*x+cos(x*x)*x*x" << endl;
        cout << "min(f) = " << globalMin << endl;
        endwtime = MPI_Wtime();
        cout << "Spent time = " << endwtime-startwtime << endl;
    }
    else
    {
        // Получение значения шага
        MPI_Recv(&delta, 1, MPI_DOUBLE, 0, myid+1000, MPI_COMM_WORLD, &stat);
        
        if(!isItGoodDelta(delta))
        {
            // Освобождение подсистемы MPI (т.к. шаг задан неадекватно - выходим)
            MPI_Finalize();
            return 0;
        }
        
        localMin = findLocalMin(myid, numprocs, delta);
        
        // Отправка локального минимума
        MPI_Send(&localMin, 1, MPI_DOUBLE, 0, myid+2000, MPI_COMM_WORLD);
    }
 
    // Освобождение подсистемы MPI
    MPI_Finalize();
    return 0;
}

