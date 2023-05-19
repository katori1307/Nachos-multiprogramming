

#include "copyright.h"
#include "stable.h"

// constructor for sem class.
Sem::Sem(char *na, int i)
{
	    strcpy(this->name, na);
	    sem = new Semaphore(this->name, i);
}

// destructor for sem class.

Sem::~Sem()
{
    	if (sem) delete sem;
}

// same to down function.
void Sem::Wait()
{
    	sem->P();		
}

// same to up function.
void Sem::Signal()
{
	sem->V();		
}

// retunr semaphore's name.
char* Sem::GetName()
{
    	return this->name;
}


// constructor for STable class.
STable::STable()
{
	this->bm = new BitMap(MAX_SEMAPHORE);
	for (int i = 0; i < MAX_SEMAPHORE; i++) this->semTab[i] = NULL;
}


// destructor for STable class.
STable::~STable()
{
	    if (this->bm) {
		delete this->bm;
		this->bm = NULL;
	    }

	    for (int i = 0; i < MAX_SEMAPHORE; i++)
		if (this->semTab[i]) {
		    delete this->semTab[i];
		    this->semTab[i] = NULL;
		}
}

// create a semafore table.
int STable::Create(char *name, int init)
{
	    for (int i = 0; i < MAX_SEMAPHORE; i++)
		if (bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0)
		    return -1; //return -1 in failed case.

	    printf("\nSuccessfully created semaphore tab.");
	    int id = this->FindFreeSlot();
	    printf("%d", id);
	    if (id < 0) 
		return -1;
	    this->semTab[id] = new Sem(name, init);
	    return 0;
}


int STable::Wait(char *name)
{
	    for (int i = 0; i < MAX_SEMAPHORE; i++)
		if (bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0) {
		    semTab[i]->Wait(); // call wait function.
		    return 0;
		}
	    printf("\n\n Invalid semaphore.");
	    return -1;
}


int STable::Signal(char *name)
{
	    for (int i = 0; i < MAX_SEMAPHORE; i++)
		if (bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0) {
		    semTab[i]->Signal(); // call signal function.
		    return 0;
		}
	    printf("\n\n Invalid semaphore.");
	    return -1;
}


int STable::FindFreeSlot()
{
	return this->bm->Find();
}




