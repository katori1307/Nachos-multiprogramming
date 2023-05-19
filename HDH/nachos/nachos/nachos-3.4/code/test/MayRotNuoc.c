#include "syscall.h"


int main()
{
	    SpaceId si_input, si_sinhvien; // for open and close input.txt, sinhvien.txt
	    int timePoints = 0; // for every time point.
	    char charRead = '0'; // for reading character from .txt file.

	    if (CreateSemaphore("main", 0) == -1) return 1; // Semaphore for main process.
	    if (CreateSemaphore("sinhvien", 0) == -1) return 1; // Semaphore for sinhvien.c process.
	    if (CreateSemaphore("voinuoc", 0) == -1) return 1; // Semaphore for voinuoc.c process.

	    if (CreateFile("output.txt") == -1) return 1; // To write the output.

	    si_input = Open("input.txt", 1);		// Open at read-only mode.
	    if (si_input == -1) return 1;

	    while (1)
	    {					
		Read(&charRead, 1, si_input); // Read the first timepoint. Store it in timePoints
		if (charRead == '\n') break;
		if (charRead >= '0' && charRead <= '9')
		    timePoints = timePoints * 10 + (charRead - 48); // Convert from char to int so that we can store it in timePoints
	    }

	    if (Exec("./test/sinhvien") == -1) // Execute sinhvien.c
	    {		
		Close(si_input);
		return 1;
	    }

	    if (Exec("./test/voinuoc") == -1)  // Execute voinuoc.c
	    {		
		Close(si_input);
		return 1;
	    }

	    while (timePoints--)
	    {
		if (CreateFile("sinhvien.txt") == -1)
		{	
		    Close(si_input);			
		    return 1;				
		}

		si_sinhvien = Open("sinhvien.txt", 0);	// Open at read-write mode.
		if (si_sinhvien == -1)
		{
		    Close(si_input);
		    return 1;
		}
			
		while (1) 
		{
		    if (Read(&charRead, 1, si_input) < 1 || charRead == '\n') // either cannot read anything from input.txt or encountering '\n' character means we get all the input data of current timePoint.
		        break;							
		    Write(&charRead, 1, si_sinhvien);
		}        

		Close(si_sinhvien); // Close sinhvien.txt
		Signal("sinhvien"); // Signal sinhvien.c
		Wait("main");	    // Tell main process to wait.
	    }

	    Close(si_input); // Close input.txt
	    PrintString("\nSuccessfully solved the problem.\nCheck output.txt at ./code to see the result.\n");
	    Halt();
	    return 0;
}

