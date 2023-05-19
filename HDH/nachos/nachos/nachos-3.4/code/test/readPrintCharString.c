#include "syscall.h"

int main()
{
	/*char result;
	PrintString("\n\nNhap 1 ky tu bat ki: ");
	result = ReadChar();
	PrintString("\nIn ky tu vua nhap: ");
	PrintChar(result);*/

	//char space = 'b';
	//PrintChar(space);
	/*char string[255];
	ReadString(string,255);
	PrintString("Hello world!");*/

	//PrintString(string);

	//PrintChar('\n');
	/*PrintString("Ascii table shown below:\n\nHere's ascii table\n\n");
	PrintString("testing");
	PrintString("NACHOS project");*/
	
	char str[255];
	PrintString("\n\nNhap mot chuoi ky tu: ");
	ReadString(str,255);
	PrintString("\nChuoi vua nhap: ");
	PrintString(str);
	


	
	
	Halt();
}
