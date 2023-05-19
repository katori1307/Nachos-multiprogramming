#include "pcb.h"
#include "synch.h"
#include "copyright.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

extern void StartProcessID(int pid);


// constructor with process's ID.
PCB::PCB(int id)
{
	if(id) this->parentID = id;
	else this->parentID = -1;
	this->numwait = 0;
	this->exitcode = 0;
	this->isBGprocess = 0;
	this->thread = NULL;
	this->joinsem = new Semaphore("joinsem", 0);
	this->exitsem = new Semaphore("exitsem", 0);
	this->multex = new Semaphore("multex", 1);
}

// Destructor.
PCB::~PCB()
{
	if(joinsem != NULL) delete this->joinsem;
	if(exitsem != NULL) delete this->exitsem;
	if(multex != NULL) delete this->multex;
	if(thread != NULL)
	{
		thread->FreeSpace();
		thread->Finish();
	}	

}


int PCB::Exec(char* filename, int pid)
{
	multex->P(); // prevent the chance that 2 processes are called at the same time.

	this->thread = new Thread(filename);
	// check whether or not the thread has been successfully created.
	if(this->thread == NULL)
	{
		printf("\n\nProcess cannot be executed (ERROR: NOT ENOUGH MEMORY)");
		multex->V();
		return -1; // return -1 in this case.
	}

	this->thread->processID = pid; // set this process's to pid.
	this->parentID = currentThread->processID; // set parent ID to process ID (of the process which is executed.)
	this->thread->Fork(StartProcessID, pid);
	multex->V();
	return pid; // return the id.

}

// get methods for ID, numwait, exitcode.

int PCB::GetID()
{
	return this->thread->processID;
}

int PCB::GetNumWait()
{
	return this->numwait;
}

int PCB::GetExitCode()
{
	return this->exitcode;
}

//set method for exitcode.
void PCB::SetExitCode(int ec)
{
	this->exitcode = ec;
}

// turn process into blocked status, wait for release command (JoinRelease signal).
void PCB::JoinWait()
{
	joinsem->P();
}

// free the process which called JoinWait.
void PCB::JoinRelease()
{
	joinsem->V();
}

// turn process into blocked status, wait for release command (ExitRelease signal).
void PCB::ExitWait()
{
	exitsem->P();
}


// free the process which called ExitWait.
void PCB::ExitRelease()
{
	exitsem->V();
}


void PCB::IncNumWait()
{
	multex->P();
	numwait++;
	multex->V();
}


void PCB::DecNumWait()
{
	multex->P();
	if(numwait > 0) numwait--;
	multex->V();
}

void PCB::SetFileName(char *fn)
{
    strcpy(FileName, fn);
}


char* PCB::GetFileName()
{
    return this->FileName;
}











