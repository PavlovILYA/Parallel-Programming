# Parallel Programming
```
MEPhI Parallel programming course
```
#### The goal is to apply as many parallel methods and utilities as you can to do simple task


# Progress

0.   **Simple one-thread function minimum and maximum calculating** *(main.cpp)*

1.   **CreateThread:** 
      - CreateThread + global array *(CreateThread+GlobalArray.cpp)*
      - CreateThread + CriticalSection *(CreateThread+CriticalSection.cpp)*
      - CreateThread + global array + CreateEvent *(CreateThread+GlobalAarray+CreateEvent.cpp)*

2.   **pthread_create** *(pthread_create+GlobalArray.cpp)*

3.   **thread/mutex** *(thread.py)*

4.   **CreateProcess + files** *(CreateProcess.cpp, new_process_CreateProcess.cpp)*
```
- сначала нужно скомпилировать файл дочернего потока new_process_CreateProcess.cpp
- также в обоих файлах нужно захардкодить свои пути (к текстовым файлам (1.txt, 2.txt, 3.txt, 4.txt) и исполняемому файлу дочернего процесса)
- текстовые файлы создаются сами, но нужно указать одинаковый путь к ним в обоих .cpp файлах)
```

5.   **fork + execl + files** *(fork.cpp, new_process_fork.cpp)*
```
- сначала нужно скомпилировать файл дочернего потока new_process_fork.cpp
- также в обоих файлах нужно захардкодить свои пути (к текстовым файлам (1.txt, 2.txt, 3.txt, 4.txt) и исполняемому файлу дочернего процесса)
- текстовые файлы создаются сами, но нужно указать одинаковый путь к ним в обоих .cpp файлах
```

6.   **Message Passing Interface:**
      - MPI_Send + MPI_Recv *(MPI_2.cpp)*
      - MPI_Bcast + MPI_Reduce *(MPI_all.cpp)*

7.   **Open Multi-Processing** *(OMP.cpp)*
