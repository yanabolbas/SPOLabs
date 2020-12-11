//Написать программу, запускающую два дочерних потока.Первый поток с
//помощью датчика случайных чисел заполняет элементы массива, состоящего
//из 10 чисел.После заполнения всех элементов, второй поток изменяет элементы массива: 
//если первый элемент больше последнего, то поменять их местами.
//Вывести на экран исходный и результирующий массивы.
//Задание выполнить для трех различных массивов.
//Потоки синхронизировать с помощью мьютекса.

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>

using namespace std;

const int n = 10;
int mas[n] = {};
HANDLE hTh01, hTh02;


int thread01()
{
	srand(time(NULL));
	HANDLE hMutex01 = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (hMutex01 == NULL)
		cout << "Open mutex01 failed" << GetLastError() << endl;
	Sleep(200);
	for (int i = 0; i < 3; i++)
	{
		WaitForSingleObject(hMutex01, INFINITE);
		cout << "Original array: ";
		for (int j = 0; j < n; j++)
		{
			mas[j] = rand() % 100;
			cout << mas[j] << " ";
		}
		
		cout << endl;
		ReleaseMutex(hMutex01);
	}		
	CloseHandle(hMutex01);
	
	return 0;
}

int thread02()
{
	HANDLE hMutex02 = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (hMutex02 == NULL)
		cout << "Open mutex02 failed" << GetLastError() << endl;
	Sleep(200);
	for (int i = 0; i < 3; i++)
	{
		WaitForSingleObject(hMutex02, INFINITE);
		cout << "Resulting array: ";
		for (int j = 0; j < n; j++)
		{
			int t = 0;
			if (mas[0] > mas[n-1])
			{
				t = mas[0];
				mas[0] = mas[n-1];
				mas[n-1] = t;
			}
			cout << mas[j] << " ";
		}
		cout << endl;
		ReleaseMutex(hMutex02);
	}
	CloseHandle(hMutex02);
	return 0;
}

int main()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, "Mutex");
	if (hMutex == NULL)
		cout << "Create mutex failed" << GetLastError() << endl;

	hTh01 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread01, NULL, 0, NULL);
	if (hTh01 == NULL)
		return GetLastError();

	hTh02 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread02, NULL, 0, NULL);
	if (hTh02 == NULL)
		return GetLastError();

	cin.get();

	CloseHandle(hMutex);
	CloseHandle(hTh01);
	CloseHandle(hTh02);
	return 0;
}



