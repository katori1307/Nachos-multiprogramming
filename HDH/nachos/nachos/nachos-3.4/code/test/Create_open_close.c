#include "syscall.h"


int main()
{
	/*int test;
	int length;
	char filename[32];
	test = Open("stdin",2);
	if(test != -1)
	{
		length = Read(filename, 32, test);
		if(length<=1) PrintString("\n\nFile's name invalid");
		else
		{
			if(CreateFile(filename) == 0)
				PrintString("\n\nCreate file successfully");
		}
		Close(test);	
		
	}*/
	
	int test;
	int length;
	char buffer[32];
	test = Open("stdin", 2);
	if(test!=-1)
	{
		PrintString("\n\nOpen file successfully");
		PrintString("\n\nType something to write file: ");
		length = Read(buffer, 32, test);
	}
	else
		PrintString("\n\nFail to open file");
	Close(test);
	test = Open("new file", 0);
	if(test != -1)
	{
		PrintString("\n\nOpen file successfully");
		Write(buffer, length, test);
	}
	else
		PrintString("\n\nFail to open file");
	Close(test);

}
