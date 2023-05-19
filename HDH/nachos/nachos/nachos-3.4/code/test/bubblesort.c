#include "syscall.h"

int main()
{
	int i = 0, j = 0;
	int n;
	int arr[100];
	PrintString("\n\nSort the interger array with size of n using bubble sort:\n");
	PrintString("Enter n (the size of the array): ");
	//PrintString("\n\nNhap kich thuoc cua mang (n): ");
	n = ReadInt();
	for(;i<n;i++)
	{
		PrintString("a[");
		PrintInt(i);
		PrintString("] = ");
		arr[i] = ReadInt();
	}
	for(i = 0;i<n-1;i++)
		for(j = i+1;j<n;j++)
		{
			if(arr[i]>arr[j])
			{
				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	PrintString("Array after sorted: ");
	for(i = 0;i<n;i++)
	{
		PrintInt(arr[i]);
		PrintString(", ");
	}


	Halt();
}
