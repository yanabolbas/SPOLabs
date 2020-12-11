//Написать программу, запускающую два дочерних потока. Первый поток
//последовательно, с помощью датчика случайных чисел, изменяет элементы
//массива, числами из диапазона от - 150 до 150. На экран, с интервалом 1 секунда, 
//выводить текущее состояние элементов массива.Второй поток проверяет элементы массива 
//и элементы, имеющие нулевое значение, заменяет любым ненулевым элементом
//массива.

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>

using namespace std;

LONG access[15] = {};
LONG mas[15] = {1,0,2,3,0,4,5,6,0,7,8,0,9,10,0};

void thread01()
{
    srand(time(NULL));
    while (true)
    {
        Sleep(500);
        for (int i = 0; i < 15; i++)
        {
            if (InterlockedExchange(&access[i], 1) == 0) {
                mas[i] = rand() % 301 - 150;

                InterlockedExchange(&access[i], 0);
            }
        }
    }
}

void thread02()
{
    srand(time(NULL));
    while (true)
    {
        Sleep(500);
        for (int i = 0; i < 15; i++) 
        {
            if (InterlockedExchange(&access[i], 1) == 0) {
                if (mas[i] == 0)
                {
                    for (int j = 0; j < 15; j++)
                    {
                        if (j == i) {
                            continue;
                        }
                        if (InterlockedExchange(&access[j], 1) == 0) {
                            if (mas[j] != 0) {
                                mas[i] = mas[j];
                                InterlockedExchange(&access[j], 0);
                                break;
                            }
                            InterlockedExchange(&access[j], 0);
                        }
                    }
                }
                InterlockedExchange(&access[i], 0);
            }
        }
    }
}

void thread03()
{
    while (true) 
    {
        Sleep(1000);
        cout << "Massiv = ";
        for (int i = 0; i < 15; i++)
        {
            cout << mas[i] << " ";
        }
        cout << endl;
    }
}

int main()
{
    HANDLE hTh01;
    DWORD IDTh01;
    hTh01 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread01, NULL, 0, &IDTh01);
    if (hTh01 == NULL)
        return GetLastError();

    HANDLE hTh02;
    DWORD IDTh02;
    hTh02 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread02, NULL, 0, &IDTh02);
    if (hTh02 == NULL)
        return GetLastError();

    HANDLE hTh03;
    DWORD IDTh03;
    hTh03 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread03, NULL, 0, &IDTh03);
    if (hTh03 == NULL)
        return GetLastError();

    cin.get();

    TerminateThread(hTh01, 0);
    CloseHandle(hTh01);
    TerminateThread(hTh02, 0);
    CloseHandle(hTh02);
    TerminateThread(hTh03, 0);
    CloseHandle(hTh03);
}


