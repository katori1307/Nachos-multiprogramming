#include "syscall.h"

int main()
{
	int i = 32;
	/*for(;i<=126;i++)
	{
		PrintInt(i);
		PrintString(":  ");
		PrintChar(i);
		PrintChar('\n');
	}*/
	for(;i<=126;i++)
	{
		//if(i>126) break;
		PrintInt(i);
		PrintString(": ");
		PrintChar(i);
		PrintString("		");
		i++;
		PrintInt(i);
		PrintString(": ");
		PrintChar(i);
		PrintString("		");
		i++;
		PrintInt(i);
		PrintString(": ");
		PrintChar(i);
		PrintString("		");
		i++;
		PrintInt(i);
		PrintString(": ");
		PrintChar(i);
		PrintChar('\n');
	}


	Halt();
}
