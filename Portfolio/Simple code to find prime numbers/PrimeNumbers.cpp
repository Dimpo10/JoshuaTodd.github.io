//prime number calculator
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <math.h>

using namespace std;

#define KEY_DOWN(vk_code) GetAsyncKeyState(vk_code)

void main(void)
{
	long i, j;
	int c = 0, flag;
	int z[10000];
	double starttime, endtime;
	double totaltime;

	starttime = GetTickCount();//get start time

	for (i = 3; i< 10000; i = i + 2) //numbers to check if prime, note only up to 10,000
	{
		flag = 0;

		for (j = 3; j< i; j += 2)
		{
			if (i%j == 0)
			{
				flag = 1; j = i;
			}
		}

		if (flag == 0)
		{
			z[c] = i; c++;
		}
		/*if (flag == 1) { cout << i << "is NOT prime!\n"; }*/
	}

	endtime = GetTickCount();//get finish time
							 //calc time
	totaltime = (endtime - starttime) / 10000.0;//calculate total time in secs
	cout << "Totaltime = " << totaltime << "sec\n";
	cout << "Primes found = " << c + 1 << endl;
	cout << "Press space to see all prime numbers found\n";

	do {

		if (KEY_DOWN(VK_SPACE))
		{
			for (i = 0; i < c; i++) cout << z[i] << " \n";
		}

	} while (KEY_DOWN(VK_ESCAPE) == false);

	_getch();
}