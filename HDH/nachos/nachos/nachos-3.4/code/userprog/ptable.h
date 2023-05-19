
#ifndef PTABLE_H
#define PTABLE_H

#include "copyright.h"
#include "pcb.h"
#include "synch.h"
#include "bitmap.h"


#define MAX_PROCESS 10

class PTable
{

private: 
	BitMap* bm; 			// mark the position that has been used in PCB.
	PCB* pcb[MAX_PROCESS]; 		
	int psize; 
	Semaphore* bmsem; 		// prevent 2 processes is called at the same time.
public: 
	// initialize the size of PCB object to save size process. Initialize it to NULL.
	// remember to initialize bm and bmsem
	PTable(int size); 			//constructor.
	~PTable(); 				//destructor.
	int ExecUpdate(char* name); 		// for SC_Exit system call.
	int ExitUpdate(int ec); 		// for SC_Exit system call.
	int JoinUpdate(int id); 		// for SC_Join system call.
	int GetFreeSlot(); 			// find free slot for new process.
	bool IsExist(int pid); 			// check whether or not the process ID exists.
	void Remove(int pid); 			// remove process ID when the process finishes.

	char* GetFileName(int id);		// return process's name.

};

#endif // PTABLE_H

