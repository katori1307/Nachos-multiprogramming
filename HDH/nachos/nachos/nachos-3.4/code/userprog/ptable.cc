#include "copyright.h"
#include "ptable.h"
#include "system.h"
#include "openfile.h"

// constructor.
PTable::PTable(int size)
{
	    if (size < 0) return;
	    bm = new BitMap(size);
	    psize = size;
	    bmsem = new Semaphore("bmsem", 1);
	    for (int i = 0; i < MAX_PROCESS; i++) pcb[i] = 0;
	    bm->Mark(0);
	    pcb[0] = new PCB(0);
	    pcb[0]->SetFileName("./test/scheduler");
	    pcb[0]->parentID = -1;
}

// destructor.
PTable::~PTable()
{
	    if (bm != 0) delete bm;
	   
	    if (bmsem != 0) delete bmsem;
}



int PTable::ExecUpdate(char *name)
{
	    bmsem->P();			// Avoid 2 processes being called at the same time.
	    int index = this->GetFreeSlot();

	
	    if (name == NULL					// if the program doesn't exist.
	     || index < 0					// if there is no free slot.
	     || strcmp(name, "./test/scheduler") == 0		// if the program is the main process.
	     || strcmp(name, currentThread->getName()) == 0) {	
		printf("\n\n Unable to execute.");
		bmsem->V();
		return -1;
	    }

	    pcb[index] = new PCB(index);
	    pcb[index]->SetFileName(name);
	    pcb[index]->parentID = currentThread->processID;

	    int pid = pcb[index]->Exec(name, index);
	    bmsem->V();
	    return pid;
}



int PTable::ExitUpdate(int ec)
{
	    int id = currentThread->processID;
	    if (id == 0) {		// if the process if the main one
		currentThread->FreeSpace();
		interrupt->Halt();
		return 0;
	    }

	    if (IsExist(id) == false) {
		printf("\n\n Invalid process id.");
		return -1;
	    }

	    pcb[id]->SetExitCode(ec);
	    pcb[pcb[id]->parentID]->DecNumWait();
	    pcb[id]->JoinRelease();	
	    pcb[id]->ExitWait();	
	    Remove(id);
	    return ec;
}


int PTable::JoinUpdate(int id)
{
	    if (id < 0			// if the process doesn't exist or if the caller is not
	     || currentThread->processID != pcb[id]->parentID) { // the parent of the process
		printf("\n\n Unable to join processes.");
		return -1;
	    }

	    pcb[pcb[id]->parentID]->IncNumWait();
	    pcb[id]->JoinWait();	// Wait for the process to finish executing
	    int ec = pcb[id]->GetExitCode();
	    pcb[id]->ExitRelease();	// Allow the process to exit
	    return ec;
}

// call Find function.
int PTable::GetFreeSlot()
{
    	return bm->Find();
}


bool PTable::IsExist(int pid)
{
    	return bm->Test(pid);
}


void PTable::Remove(int pid)
{
	    bm->Clear(pid);
	    if (pcb[pid] != 0) delete pcb[pid];
}


char* PTable::GetFileName(int id)
{
    	return pcb[id]->GetFileName();
}
