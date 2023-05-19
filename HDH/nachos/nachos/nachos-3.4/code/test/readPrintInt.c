#include "syscall.h"

int main()
{
	int result;
	PrintString("Input an integer: ");
	result = ReadInt();
	PrintString("Print the integer: ");
	PrintInt(result);
	//PrintInt(48);
	//PrintInt(92);
	
	Halt();
}
