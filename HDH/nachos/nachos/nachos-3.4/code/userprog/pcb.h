// PCB class: PROCESS CONTROL BLOCK.


#ifndef PCB_H
#define PCB_H

#include "copyright.h"
#include "thread.h"
#include "synch.h"

class PCB
{
private:
	Semaphore* joinsem;		// semaphore for joining.
	Semaphore* exitsem;		// semaphore for exiting.
	Semaphore* multex;		// semaphore for exclusive access retrieval process.
	
	int exitcode;			
	int numwait;			// number of joined process.

	Thread* thread;			// program's process.
	char FileName[32];		// process's name.
public: 
	int parentID; 			// ID for parent process.
	bool isBGprocess;	// checking if it is background process.
	
	PCB(); 
	PCB(int id); 			// constructor.
	~PCB();				// destructor.

	// load process with the name of "file name" and process's ID is "pid".
	int Exec(char* filename, int pid);	// create new thread with filename and process's ID.
	int GetID(); 				// return process ID of the process which is executed.
	int GetNumWait(); 			// return the total number of waiting process.
	
	void JoinWait(); 			// 1. parent process wait for child process to finish.			
	void ExitWait(); 			// 4. Child process finishing.
	void JoinRelease(); 			// 2. Signal parent process to continue executing.
	void ExitRelease(); 			// 3. Allow child process to finish.

	void IncNumWait(); 			// Increase the total of waiting process.
	void DecNumWait(); 			// Decrease the total of waiting process.

	void SetExitCode(int ec); 		// set up process's exitcode.
	int GetExitCode(); 			// return process's exitcode.

	void SetFileName(char* fn); 		// set file's name.
	char* GetFileName(); 			// return file's name.
};


#endif // PCB_H
