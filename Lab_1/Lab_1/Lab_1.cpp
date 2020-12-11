//В программе предусмотреть сохранение вводимых данных в файл и 
//возможность чтения из ранее сохраненного файла. Результаты выводить на
//экран и в текстовой файл.
// Для получения места в общежитии формируется список студентов, 
//который включает ФИО студента, группу, средний балл, доход на члена 
//семьи.Вывести информацию о студентах, у которых доход на члена семьи 
//менее двух минимальных зарплат.

#include <iostream>
#include <Windows.h>

using namespace std;

int main()
{
	struct student {
		char lastName[50];
		int groupNumber;
		double averageScore;
		double income;
	} pupil;

	double minimumWage = 200;

	LPCTSTR pupils = "Pupils.txt";
	HANDLE myFile = CreateFile
	(
		pupils, //Имя файла
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (myFile == INVALID_HANDLE_VALUE)
	{
		cout << "Error by opening \"Pupils.txt\""; 
		return 0;
	}
	else
		cout << "File \"Pupils.txt\" is opened\n"; 

	LPCTSTR result = "Result.txt";
	HANDLE fileResult = CreateFile
	(
		result, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (fileResult == INVALID_HANDLE_VALUE)
	{
		cout << "Error by creating \"Result.txt\""; 
		return 0;
	}
	else
		cout << "File \"Result.txt\" is opened\n"; 

	SetFilePointer(myFile, 0, 0, FILE_END);

	DWORD n;
	string oneMore;
	cout << "Do you want to get a place in a hostel? (yes/no)\n";
	cin >> oneMore;
	if (oneMore == "yes")
	{
		cout << "Enter your full name, group number, GPA and family income:\n";
	}
	while (oneMore == "yes")
	{
		cin >> pupil.lastName;
		cin >> pupil.groupNumber;
		cin >> pupil.averageScore;
		cin >> pupil.income;
		WriteFile(myFile, &pupil, sizeof(pupil), &n, NULL);
		cout << "Do you want to get one more place? (yes/no)\n";
		cin >> oneMore;
	}

	cout << "\nInformation about students whose income per family member is less than two minimum wages:\n";

	FlushFileBuffers(myFile);
	SetFilePointer(myFile, 0, 0, FILE_BEGIN);
	while (ReadFile(myFile, &pupil, sizeof(student), &n, NULL) && n != 0)
	{
		if (pupil.income < minimumWage * 2)
		{
			WriteFile(fileResult, &pupil, sizeof(student), &n, NULL);
			cout << pupil.lastName << "\t" << pupil.groupNumber << "\t" << pupil.averageScore << "\t" << pupil.income << endl;
		}
	}

	cout << "\nInformation is in the file \"Result.txt\":\n";

	SetFilePointer(fileResult, 0, 0, FILE_BEGIN);
	while (ReadFile(fileResult, &pupil, sizeof(student), &n, NULL) && n != 0)
	{
		if (pupil.income < minimumWage * 2)
		{
			cout << pupil.lastName << "\t" << pupil.groupNumber << "\t" << pupil.averageScore << "\t" << pupil.income << endl;
		}
	}

	CloseHandle(myFile);
	CloseHandle(fileResult);
}