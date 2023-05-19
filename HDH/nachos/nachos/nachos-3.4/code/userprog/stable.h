
#ifndef STABLE_H
#define STABLE_H

#include "copyright.h"
#include "synch.h"
#include "bitmap.h"

#define MAX_SEMAPHORE 10

class Sem 
{
	  public:
	    Sem(char* na, int i);		// Constructor
	    ~Sem();				// Destructor

	    void Wait();			// Set a process into wait-mode (blocked-mode).
	    void Signal();			// Send signal to free semaphore
	    char* GetName();			// Return semaphore's name.

	  private:
	    char name[50];
	    Semaphore *sem;			// create semaphore.
};

class STable 
{
	  public:
	    STable();				// Constructor
	    ~STable();				// Destructor

	    int Create(char *name, int init);	// Create a new semaphore with name if hasn't existed
	    int Wait(char *name);		// Set a process into wait-mode.
	    int Signal(char *name);		// Free the semaphore.
	    int FindFreeSlot();			// Return a free slot

	  private:
	    BitMap *bm;				
	    Sem *semTab[MAX_SEMAPHORE];		// up to 10 semaphores.
};

#endif // STABLE_H


