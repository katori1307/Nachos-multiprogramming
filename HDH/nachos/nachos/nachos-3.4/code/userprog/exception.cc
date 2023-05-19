// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MAX_LENGTH 32

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg,counter);
	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter + 4);
}

char* User2System(int virtAddr, int limit)
{
	int i, oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit+1]; //need for terminal string
	if(kernelBuf==NULL) {
		return kernelBuf;
	}
	memset(kernelBuf,0,limit+1); //Init memory space for kernelBuf
	for(i=0;i<limit;i++) {
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		if(oneChar==0) break;
	}
	return kernelBuf;
}

int System2User(int virtAddr, int len, char* buffer)
{
	if(len<0) return -1;
	if(!len) return len;
	int i = 0, oneChar = 0;
	do {
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i++;
	} while(i<len && oneChar !=0);
	return i;
}


void SubFunction()
{
	int op1, op2;
	op1 = machine->ReadRegister(4);
	op2 = machine->ReadRegister(5);
	int result;
	result = op1 - op2;
	machine->WriteRegister(2,result);
	printf("%d", result);
	//interrupt->Halt();
}

void ReadIntFunction()
{
	char* buffer;	// pointer to store the input, being converted into (int) form after.
	int MAX_BUFFER = 255; 
	buffer = new char[MAX_BUFFER + 1];
	int size = gSynchConsole->Read(buffer, MAX_BUFFER); // read interger into buffer with the size of 255
	int startPos, endPos;
	startPos = 0;
	endPos = 0;

	// if the input is a negative number, start reading the input from postition number 1, because buffer[0] is '-'
	if(buffer[0]=='-'){
		startPos = 1;
		endPos = 1;
	}
	int res = 0; // this is to store the result
	for(int i = startPos; i<size;i++)
	{
		if(buffer[i]<'0' || buffer[i] > '9')
		{
			DEBUG('a', "\n Invalid input\n");
			printf ("\n\n Invalid input");
			machine->WriteRegister(2,0);
			//IncreasePC();
			delete[] buffer;
			return;
		}
		else if (buffer[i]=='.') //handle if the input is x.000 for example: 17.0000
		{
			for(int j = i+1; j<size;j++)
				if(buffer[i]!='0') // if it's is a real number, not integer, then return 0
				{
					DEBUG('a', "\n Invalid input\n");
					printf ("\n\n Invalid input");
					machine->WriteRegister(2,0);
					//IncreasePC();
					delete[] buffer;
					return;
				}
			endPos = i-1;
			break;
		}
		endPos = i;
	}
	//convert the buffer to integer and store it in res
	for(int i = startPos;i<=endPos;i++)
		res = res*10 + (int)(buffer[i]-48);
	//if the input is negative then make the res negative
	if(buffer[0] == '-')
		res*=-1;
	machine->WriteRegister(2,res);
	//printf("\n\n Successful!");
	//IncreasePC();
	delete[] buffer;
	return;
}


void PrintIntFunction()
{
	char* buffer;
	int MAX_BUFFER = 255;
	buffer = new char[MAX_BUFFER+1];
	int num = machine->ReadRegister(4);
	if(!num) {
		gSynchConsole->Write("0",1);
		//IncreasePC();
		return;
	}
	int length = 0;
	int startPos = 0;
	int tempNum = num;
	int count = num;
	if(num<0) {
		num*=-1; // if the number is negative, convert it to positive
		startPos = 1; // begin write the number from the next position, because buffer[0] is '-'
	}
	while(count) {
		length++;
		count = count/10;	
	}
	//start write it into the buffer from the last position to the start position
	for(int i = startPos + length - 1; i>=startPos;i--) {
		int lastNum = tempNum%10;		
		buffer[i] = (char)(lastNum + 48);
		tempNum/=10;	
	}
	//case the number is negative
	if(num<0){
		buffer[0] = '-';
		buffer[length+1] = '\0';
		gSynchConsole->Write(buffer,length+1);
		delete[] buffer;
		IncreasePC();
		return;
	}
	buffer[length]='\0';
	gSynchConsole->Write(buffer,length);
	delete[] buffer;
	//IncreasePC();
	return;
}

void ReadCharFunction()
{
	char* buffer;
	int MAX_BUFFER = 255;
	buffer = new char[MAX_BUFFER+1];
	int length = gSynchConsole->Read(buffer,MAX_BUFFER);
	//Invalid case
	if(!length){
		DEBUG('a', "\n Invalid. The input character has the length of 0.");
		printf ("\n\n Invalid. The input character has the length of 0.");
		machine->WriteRegister(2,0); //if invalid then return 0;
	}
	if(length>1) {
		DEBUG('a', "\n Invalid. The input character has the length greater than 1.");
		printf ("\n\n Invalid. The input character has the length greater than 1.");
		machine->WriteRegister(2,0);
	}
	//Valid case
	if(length==1) {
		machine->WriteRegister(2,buffer[0]);
		//printf("\n\n Successfully read char!");
	}
	//IncreasePC();
	delete[] buffer;
	return;
}

void PrintCharFunction()
{
	char result;
	result = (char)machine->ReadRegister(4);
	gSynchConsole->Write(&result,1);
	//IncreasePC();
	return;
}



void ReadStringFunction()
{
	int VirtualAddr = machine->ReadRegister(4); // get the virual address from the register 4
	int length = machine->ReadRegister(5);	// get the string's length from the register 5
	char* buffer = User2System(VirtualAddr,length); // read data from virtual address with length (from userspace) and copy to the kernel
	gSynchConsole->Read(buffer,length);
	System2User(VirtualAddr,length,buffer); // Copy data from the kernel back to userspace
	//IncreasePC();
	delete[] buffer;
	//printf("\n\n Read string successfully!");
	return;
}	

void PrintStringFunction()
{
	int MAX_BUFFER = 255;
	int VirtualAddr = machine->ReadRegister(4); // get virtual address from register 4
	char* buffer = User2System(VirtualAddr,MAX_BUFFER); // get the string from user space to kernel with the memory of MAX_BUFFER
	int length = 0;
	while(buffer[length]!='\0') length++; // count the length of buffer
	gSynchConsole->Write(buffer,length+1); //print string using SynchConsole
	
	delete[] buffer;
	//IncreasePC();
	return;
}

void CreateFileFunction()
{
	int virtualAddr;
	char* name;
	
	virtualAddr = machine->ReadRegister(4); // read file's virtual address from register 4.
	name = User2System(virtualAddr, MAX_LENGTH + 1); //copy virtual Address from userspace to kernel.
	if(name == NULL) //fail to create file.
	{
		DEBUG('a',"\n\nFail to create file");
		printf("\n\nFail to create file");
		machine->WriteRegister(2,-1); // return -1 in failed case.
		delete[]name;
		return;
	}
	if(strlen(name)==0) // invalid file's name;
	{
		DEBUG('a',"\n\nInvalid file's name");
		printf("\n\nInvalid file's name");
		machine->WriteRegister(2,-1); // return -1 in failed case.
		delete[]name;
		return;
	}

	// use Create method in FileSystem to create file.
	if(fileSystem->Create(name,0)==0) // fail to create file
	{
		DEBUG('a',"\n\nFail to create file");
		printf("\n\nFail to create file");
		machine->WriteRegister(2,-1); // return -1 in failed case.
		delete[]name;
		return;
	}
	else
	{
		machine->WriteRegister(2,0); // return 0 in successful case.
		delete[]name;
		return;
	}
}


void OpenFunction()
{
	int type;
	int virtualAddr;
	char* name;
	int freeSlot;
	freeSlot = 0;
	//name = new char[MAX_LENGTH];
	virtualAddr = machine->ReadRegister(4); // get virtual address from register 4.
	name = User2System(virtualAddr, MAX_LENGTH); // copy virtual address from userspace to kernel.

	   //FindFreeSlot needs to be built in filesys.h and filesys.cc
	   //FindFreeSlot will return index i in openfile[i] if openfile[i] == NULL in range of [2, 10], if not -> return -1

	freeSlot = fileSystem->FindFreeSlot(); 

	type = machine->ReadRegister(5); // get type of file from register 5.
	if(freeSlot!=-1) 
	{
		if(type==0 || type == 1)
		{
			if((fileSystem->openfile[freeSlot] = fileSystem->Open(name,type))!=NULL)
				machine->WriteRegister(2,freeSlot); // return freeslot.
		}
		else if(type == 2) machine->WriteRegister(2,0); // return 0
		else if(type == 3) machine->WriteRegister(2,1);
		delete[]name; 
		return;
	}
	
	machine->WriteRegister(2,-1);
	delete[]name;
	return;
}


void CloseFunction()
{
	int fileID = machine->ReadRegister(4);
	if(fileID >= 0 && fileID <= 14)
	{
		if(fileSystem->openfile[fileID])
		{
			delete fileSystem->openfile[fileID];
			fileSystem->openfile[fileID] = NULL;
			machine->WriteRegister(2, 0);
			return;
		}
	}
	machine->WriteRegister(2,-1);
	return;
}


void ReadFunction()
{
	// load char* buffer with the size of "charcount" in to "id"
	int virtualAddr, charcount, id;
	char* buffer;
	int oldPos, newPos;
	virtualAddr = machine->ReadRegister(4); // read virual address from register 4.
	charcount = machine->ReadRegister(5); // get the number of bytes from register 5.
	id = machine->ReadRegister(6); // get file's id from register 6.
	if(id<0|| id>9) // out of the range of file's table in this project.
	{
		machine->WriteRegister(2,-1); // return -1 in this case.
		return;
	}
	if(fileSystem->openfile[id]==NULL) // file does not exist.
	{
		machine->WriteRegister(2,-1);
		return;
	}
	if(fileSystem->openfile[id]->type == 3) // 3 means file's type is console output (stdout file), cannot  read.
	{
		machine->WriteRegister(2,-1);
		return;
	}
	oldPos = fileSystem->openfile[id]->GetCurrentPos();
	buffer = User2System(virtualAddr, charcount); // copy buffer from userspace to kernel.
	
	if(fileSystem->openfile[id]->type == 2) // read file stdin
	{
		int size;
		size = gSynchConsole->Read(buffer,charcount);
		System2User(virtualAddr,size, buffer);
		machine->WriteRegister(2,size);
		delete[]buffer;
		return;
	}

	if((fileSystem->openfile[id]->Read(buffer,charcount)) > 0)
	{
		newPos = fileSystem->openfile[id]->GetCurrentPos(); // the actual bytes should be newPos - oldPos.
		System2User(virtualAddr, newPos - oldPos, buffer); // copy data from kernel back to userspace.
		machine->WriteRegister(2,newPos - oldPos);
	}
	else
		machine->WriteRegister(2,-2); // read to NULL character.

	delete[]buffer;
	return;
}





void WriteFunction()
{
	int virtualAddr, charcount, id;
	char* buffer;
	int oldPos, newPos;
	virtualAddr = machine->ReadRegister(4); // read virual address from register 4.
	charcount = machine->ReadRegister(5); // get the number of bytes from register 5.
	id = machine->ReadRegister(6); // get file's id from register 6.
	if(id<0|| id>9) // out of the range of file's table in this project.
	{
		machine->WriteRegister(2,-1); // return -1 in this case.
		return;
	}
	if(fileSystem->openfile[id]==NULL) // file does not exist.
	{
		machine->WriteRegister(2,-1);
		return;
	}
	if(fileSystem->openfile[id]->type == 1 || fileSystem->openfile[id]->type == 2)
	{
		printf("\n\nCannot write only read file (type == 1) and stdin file (type == 2)");
		machine->WriteRegister(2,-1); // return -1 in this case.
		return;
	}

	oldPos = fileSystem->openfile[id]->GetCurrentPos();
	buffer = User2System(virtualAddr, charcount); // copy data in virtual address from userspace to kernel.
	
	if(fileSystem->openfile[id]->type == 0) // in case file's type is read and write
	{
		if((fileSystem->openfile[id]->Write(buffer,charcount)) > 0)
		{
			newPos = fileSystem->openfile[id]->GetCurrentPos(); // the actual bytes should be newPos - oldPos.
			machine->WriteRegister(2,newPos - oldPos);
			delete[]buffer;
			return;
		}
	}
	if(fileSystem->openfile[id]->type == 3)
	{
		int i =0;
		while(buffer[i]!= 0 && buffer[i] != '\n')
		{
			gSynchConsole->Write(buffer + i, 1);
			i++;
		}
		buffer[i] = '\n';
		gSynchConsole->Write(buffer + i, 1); // add '\n' character.
		machine->WriteRegister(2 , i-1); // return the number of bytes written.
		return;
	}
	

}


void ExecFunction()
{
	int virtualAddr;
	char* progName;
	virtualAddr = machine->ReadRegister(4); // read the virtual address from register 4
	progName = User2System(virtualAddr, MAX_LENGTH + 1); // copy program's name from userspace to kernel with the size of MAX_LENGTH + 1

	OpenFile *file = fileSystem->Open(progName);	

	if(progName == NULL) // not enough memory
	{
		DEBUG('a',"\n\nNot enough memory");
		printf("\n\n Not enough memory");
		machine->WriteRegister(2,-1); // return -1 in this case
		return;
	}
	if(file == NULL) // fail to open file
	{
		DEBUG('a',"\n\n Cannot open file");
		printf("\n\n Cannot open file");
		machine->WriteRegister(2,-1); // return -1 in this case
		return;
	}
	
	// in successful case, return file's ID
	int ID = pTab->ExecUpdate(progName); // pTab needs to be implemented.
	machine->WriteRegister(2,ID);

	//printf("%d",ID); // just to test SC_Exec
	
	delete file;
	delete[]progName;
	return;
}


void JoinFunction()
{
	int id, exitcode;
	id = machine->ReadRegister(4); // Read process's ID from register 4.
	exitcode = pTab->JoinUpdate(id); // Join the process to process table.
	machine->WriteRegister(2, exitcode);
	return;
}

void ExitFunction()
{
	int status;
	status = machine->ReadRegister(4);
	if(status) return;
	int exitcode;
	exitcode = pTab->ExitUpdate(status);
	machine->WriteRegister(2, exitcode); // return exitcode in this case
	currentThread->FreeSpace();
	currentThread->Finish();
	return;
}

void CreateSemaphoreFunction()
{
	int virtualAddr, semval;
	virtualAddr = machine->ReadRegister(4); // Get virtual address from the register 4.
	semval = machine->ReadRegister(5);	// Get semaphore value from the register 5.
	
	char* name = User2System(virtualAddr, MAX_LENGTH + 1); // copy data in virtual address from user space to kernel.
	if(name == NULL)
	{
		DEBUG('a', "\n\n Not enough memory to create semaphore.");
		printf("\n\n Not enough memory to create semaphore.");
		machine->WriteRegister(2, -1); // return -1 in failed case.
		delete[]name;
		return;
	}
	//printf("\n\nsomething to check");
	int result = semTab->Create(name, semval);
	
	if(result == -1)
	{
		DEBUG('a', "\n\n Cannot initialize semaphore.");
		printf("\n\n Cannot initialize semaphore.");
		machine->WriteRegister(2, -1); // return -1 in failed case.
		delete[]name;
		return;
	}

	delete[]name;
	machine->WriteRegister(2, result);
	return;
}




void WaitFunction()
{
	int virtualAddr;
	virtualAddr = machine->ReadRegister(4); // Get virtual address from register 4.
	
	char* name = User2System(virtualAddr, MAX_LENGTH + 1); // copy data in virtual address from user space to kernel.
	if(name == NULL)
	{
		DEBUG('a', "\n\n Not enough memory.");
		printf("\n\n Not enough memory.");
		machine->WriteRegister(2, -1); // return -1 in failed case.
		delete[]name;
		return;
	}
	int result = semTab->Wait(name);
	
	if(result == -1)
	{
		DEBUG('a', "\n\n Semaphore does not exist.");
		printf("\n\n Semaphore does not exist.");
		machine->WriteRegister(2, -1); // return -1 in failed case.
		delete[]name;
		return;
	}

	delete[]name;
	machine->WriteRegister(2, result);
	return;
}


void SignalFunction()
{
	int virtualAddr;
	virtualAddr = machine->ReadRegister(4); // Get virtual address from register 4.
	
	char* name = User2System(virtualAddr, MAX_LENGTH + 1); // copy data in virtual address from user space to kernel.
	if(name == NULL)
	{
		DEBUG('a', "\n\n Not enough memory.");
		printf("\n\n Not enough memory.");
		machine->WriteRegister(2, -1); // return -1 in failed case.
		delete[]name;
		return;
	}
	int result = semTab->Signal(name);
	
	if(result == -1)
	{
		DEBUG('a', "\n\n Semaphore does not exist.");
		printf("\n\n Semaphore does not exist.");
		machine->WriteRegister(2, -1); // return -1 in failed case.
		delete[]name;
		return;
	}

	delete[]name;
	machine->WriteRegister(2, result);
	return;

}


void SeekFunction()
{
	int pos = machine->ReadRegister(4);
	int file_id = machine->ReadRegister(5);
	if(file_id < 0 || file_id > 9 || file_id == 0 || file_id == 1 || fileSystem->openfile[file_id] == NULL)
	{
		DEBUG('a', "\n\nCannot seek in this file.");
		printf("\n\nCannot seek in this file.");
		return;
	}
	
	if(pos == -1)
		pos = fileSystem->openfile[file_id]->Length();

	if(pos < 0 || pos > fileSystem->openfile[file_id]->Length())
	{
		DEBUG('a', "\n\nPosition input invalid.");
		printf("\n\nPosition input invalid.");
		return;
	}
	else
	{
		fileSystem->openfile[file_id]->Seek(pos);
		machine->WriteRegister(2, pos);
		return;
	}
}


void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which) {
	case NoException:
	return;
	
	// xu ly cac exception trong machine/machine.h, neu co loi in ra ten cua exception:
	case PageFaultException:
		DEBUG('a', "\n Error, PageFaultException.\n");
		printf("\n\n Error, PageFaultException.");
		interrupt->Halt();
		break;
	case ReadOnlyException:
		DEBUG('a', "\n Error, ReadOnlyException.\n");
		interrupt->Halt();
		break;
	case BusErrorException:
		DEBUG('a', "\n Error, BusErrorException.\n");
		interrupt->Halt();
		break;
	case AddressErrorException:
		DEBUG('a', "\n Error, AddressErrorException.\n");
		interrupt->Halt();
		break;
	case OverflowException:
		DEBUG('a', "\n Error, OverflowException.\n");
		interrupt->Halt();
		break;
	case IllegalInstrException:
		DEBUG('a', "\n Error, IllegalInstrException.\n");
		interrupt->Halt();
		break;
	case NumExceptionTypes:
		DEBUG('a', "\n Error, NumExceptionTypes.\n");
		interrupt->Halt();
		break;
		


	case SyscallException:
	switch (type){
		case SC_Halt:
			DEBUG('a', "\n Shutdown, initiated by user program.");
			printf ("\n\n Shutdown, initiated by user program.");
			interrupt->Halt();
			break;

		case SC_Sub:
			/*int op1, op2;
			op1 = machine->ReadRegister(4);
			op2 = machine->ReadRegister(5);
			int result;
			result = op1 - op2;
			machine->WriteRegister(2,result);
			printf("%d", result);
			interrupt->Halt();*/
			SubFunction();
			break;
		case SC_ReadInt:
			ReadIntFunction();
			//interrupt->Halt();
			break;
		case SC_PrintInt:
			PrintIntFunction();
			//interrupt->Halt();
			break;
		case SC_ReadChar:
			ReadCharFunction();
			//interrupt->Halt();
			break;
		case SC_PrintChar:
			PrintCharFunction();
			//interrupt->Halt();
			break;
		case SC_ReadString:
			ReadStringFunction();
			//interrupt->Halt();
			break;
		case SC_PrintString:
			PrintStringFunction();
			//interrupt->Halt();
			break;
		case SC_CreateFile:
			CreateFileFunction();
			break;
		case SC_Open:
			OpenFunction();
			break;
		case SC_Close:
			CloseFunction();
			break;
		case SC_Read:
			ReadFunction();
			break;
		case SC_Write:
			WriteFunction();
			break;
		case SC_Exec:
			ExecFunction();
			break;
		case SC_Join:
			JoinFunction();
			break;
		case SC_Exit:
			ExitFunction();
			break;
		case SC_CreateSemaphore:
			CreateSemaphoreFunction();
			break;
		case SC_Wait:
			WaitFunction();
			break;
		case SC_Signal:
			SignalFunction();
			break;
		case SC_Seek:
			SeekFunction();
			break;
		
	}
	default:
		/*printf("\n Unexpected user mode exception (%d %d)", which, type);
		interrupt->Halt();*/
	IncreasePC();
    }

}
