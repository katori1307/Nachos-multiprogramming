/* syscalls.h 
 * 	Nachos system call interface.  These are Nachos kernel operations
 * 	that can be invoked from user programs, by trapping to the kernel
 *	via the "syscall" instruction.
 *
 *	This file is included by user programs and by the Nachos kernel. 
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation 
 * of liability and disclaimer of warranty provisions.
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "copyright.h"

/* system call codes -- used by the stubs to tell the kernel which system call
 * is being asked for
 */
#define SC_Halt		0
#define SC_Exit		1
#define SC_Exec		2
#define SC_Join		3
#define SC_Create	4
#define SC_Open		5
#define SC_Read		6
#define SC_Write	7
#define SC_Close	8
#define SC_Fork		9
#define SC_Yield	10

#define SC_ReadInt	11
#define SC_PrintInt	12
#define SC_ReadChar	13
#define SC_PrintChar	14
#define SC_ReadString	15
#define SC_PrintString	16
#define SC_CreateFile	17
#define SC_CreateSemaphore	18
#define SC_Wait		19
#define SC_Signal	20
#define SC_Seek		21


#define SC_Sub		43

#ifndef IN_ASM

/* The system call interface.  These are the operations the Nachos
 * kernel needs to support, to be able to run user programs.
 *
 * Each of these is invoked by a user program by simply calling the 
 * procedure; an assembly language stub stuffs the system call code
 * into a register, and traps to the kernel.  The kernel procedures
 * are then invoked in the Nachos kernel, after appropriate error checking, 
 * from the system call entry point in exception.cc.
 */

/* Stop Nachos, and print out performance stats */
void Halt();		
 

/* Address space control operations: Exit, Exec, and Join */

/* This user program is done (status = 0 means exited normally). */
void Exit(int status);	

/* A unique identifier for an executing user program (address space) */
typedef int SpaceId;	
 
/* Run the executable, stored in the Nachos file "name", and return the 
 * address space identifier
 */
SpaceId Exec(char *name);
 
/* Only return once the the user program "id" has finished.  
 * Return the exit status.
 */
int Join(SpaceId id); 	
 

/* File system operations: Create, Open, Read, Write, Close
 * These functions are patterned after UNIX -- files represent
 * both files *and* hardware I/O devices.
 *
 * If this assignment is done before doing the file system assignment,
 * note that the Nachos file system has a stub implementation, which
 * will work for the purposes of testing out these routines.
 */
 
/* A unique identifier for an open Nachos file. */
typedef int OpenFileId;	

/* when an address space starts up, it has two open files, representing 
 * keyboard input and display output (in UNIX terms, stdin and stdout).
 * Read and Write can be used directly on these, without first opening
 * the console device.
 */

#define ConsoleInput	0  
#define ConsoleOutput	1  
 
/* Create a Nachos file, with "name" */
void Create(char *name);

/* Open the Nachos file "name", and return an "OpenFileId" that can 
 * be used to read and write to the file.
 */

//OpenFileId Open(char *name);

OpenFileId Open(char* name, int type); // rewrite the prototpye (add more arguments).

/* Write "size" bytes from "buffer" to the open file. */
void Write(char *buffer, int size, OpenFileId id);

/* Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
int Read(char *buffer, int size, OpenFileId id);

/* Close the file, we're done reading and writing to it. */
void Close(OpenFileId id);



/* User-level thread operations: Fork and Yield.  To allow multiple
 * threads to run within a user program. 
 */

/* Fork a thread to run a procedure ("func") in the *same* address space 
 * as the current thread.
 */
void Fork(void (*func)());

/* Yield the CPU to another runnable thread, whether in this address space 
 * or not. 
 */
void Yield();

/*Calculate a - b and return the result with a, b are 2 intergers*/
int Sub(int a, int b);		

/* Read an interger from user, return the result if successfull, return 0 if the input is not an interger (a real number or character, ...)*/
int ReadInt();

/* Print an interger */
void PrintInt(int num);

/* Read an character from user, return the result if the character has the length of 1, if not (length != 1) then return 0 */
char ReadChar();

/* Print a character */
void PrintChar(char c);

/* Read a string from user function, apply User2System and System2User to handle data from kernel and user */
void ReadString(char buffer[], int length);

/* Print a string, apply User2System and System2User to handle data from kernel (user space) and user */
void PrintString(char buffer[]);

int CreateFile(char* name);

//OpenFileID Open(char* name, int type);

/* Create a semaphore for multi-process coding */
int CreateSemaphore(char* name, int semval);


/* Wait system call to put one process in blocked status and wait for others process to finish themselves. */
int Wait(char* name);

/* Signal system call to call one process back in the main process to continue executing. */
int Signal(char* name);


/* System call to move pointer to "position" in the file which has "id", 
	if position = -1, it should move the pointer to the end of file
	return position of the pointer if succeed, return -1 in failed case. (implemented 24/04/2023)*/
int Seek(int position, OpenFileId id);




#endif /* IN_ASM */

#endif /* SYSCALL_H */
