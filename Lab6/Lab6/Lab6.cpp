//Написать программу, запускающую два дочерних потока, работающих с
//разной скоростью.Первый поток с помощью датчика случайных чисел заполняет 
//элементы одномерного массива, состоящего из n элементов, числами из
//диапазона 0..500.Второй поток находит среднее арифметическое значение положительных 
//элементов массива.Потоки синхронизировать с помощью семафора.

#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <time.h>

using namespace std;

HANDLE hSemaphore;

int mas[21] = {};
int sum = 0;

int thread01()
{
    srand(time(NULL));
    for (int i = 0; i < 21; i++)
    {
        mas[i] = rand() % 300 - 151;
        Sleep(100);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        cout << "mas[" << i << "]= " << mas[i] << endl;
    }
    return 0;
}

int thread02()
{
    Sleep(150);
    int average, quan = 0;
    for (int i = 0; i < 21; i++)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        
        if (mas[i] > 0)
        {
            sum += mas[i];
            quan++;
            average = sum / quan;
            cout << "Average = " << average << endl;
        }
        else
            cout <<"Negative element is entered" << endl;
        average = 0;
    }
    return 0;
}

int main()
{
    hSemaphore = CreateSemaphore(NULL, 0, 21, "Semaphore");
    if (hSemaphore == NULL)
        cout << "Create semaphore failed" << GetLastError() << endl;

    HANDLE hTh01 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread01, NULL, 0, NULL);
    if (hTh01 == NULL)
        return GetLastError();

    HANDLE hTh02 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread02, NULL, 0, NULL);
    if (hTh02 == NULL)
        return GetLastError();

    cin.get();

    CloseHandle(hSemaphore);
    CloseHandle(hTh01);
    CloseHandle(hTh02);
    return 0;
}
