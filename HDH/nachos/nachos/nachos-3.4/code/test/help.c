#include "syscall.h"

int main()
{
	PrintString("\n\nThis program is to give a breif introduction about our group's member:\n");
	PrintString("Group member _  student ID:\n");
	PrintString("_ Vo Anh Quan _ 21127147\n");
	PrintString("_ Phan Phuoc Hai Nam _ 21127365\n");
	PrintString("_ Huynh Cao Minh _ 21127644\n");
	PrintString("\nBreif description about ascii.c:\n");
	PrintString("ascii.c is a program to print ascii table\nIt only prints the readable ascii (from 32 to 126)\n");
	PrintString("We combine many system call being built before: \n");
	PrintString(" - Use PrintChar(i); to print ascii element number i\n");
	PrintString(" - Use PrintInt(i); and PrintString(char buffer[]); to make a table form (little bit of design to make it looks more like an ascii table)\n");
	PrintString("\nBreif description about bubblesort.c:\n");
	PrintString("This is a program to sort an array of interger in increase order\n");
	PrintString(" - The array has the size of n in which n is a input get from user using ReadInt(); system call\n");
	PrintString(" - Use a loop to get array's elements from user using ReadInt(); system call\n");
	PrintString(" - The maximum size of the array is 100 elements\n");
	PrintString(" - Apply bubblesort algorithm in this program:\n");
	PrintString("   - Use 2 loops to compare one element (a[i]) to all of the remaining elements located after that element (a[i])\n");
	PrintString("   - If a[i] > a[j] then we swap those 2 numbers\n");
	PrintString(" - Use another loop and PrintInt system call to print the array after sorted\n");

	//CreateFile("ThisIsTestFile");
	
	
	Halt();
}
