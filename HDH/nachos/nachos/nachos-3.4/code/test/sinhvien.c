#include "syscall.h"


int main()
{
	SpaceId si_sinhvien, si_voinuoc, si_output;
	    int getWater = 0;
	    int fileLen = 0, filePointer = 0;
	    char charRead = '0';

	    while (1) {
		// at different time point, the number of students and their required water should be different, so we set fileLen = 0 for every new loop.
		fileLen = 0;
		Wait("sinhvien");				// Wait for signal from main.

		si_sinhvien = Open("sinhvien.txt", 1);	// Open at read-only mode.
		if (si_sinhvien == -1) {
		    Signal("main");
		    return 1;
		}

		fileLen = Seek(-1, si_sinhvien);	// get sinhvien.txt's length using seek at -1.
							
		Seek(0, si_sinhvien);			// move the pointer back to the start point.
		filePointer = 0;			// reset the file pointer.

		if (CreateFile("voinuoc.txt") == -1) {
		    Close(si_sinhvien);
		    Signal("main");
		    return 1;
		}

		si_voinuoc = Open("voinuoc.txt", 0);		// Open at read-write mode.
		if (si_voinuoc == -1) {
		    Close(si_sinhvien);
		    Signal("main");
		    return 1;
		}

		while (filePointer < fileLen)
		{
		    getWater = 0;
		    Read(&charRead, 1, si_sinhvien);
			// encounter space character means we have read all the required water at current timepoint, so we should begin to get water.
		    if (charRead != ' ')
		    {
		        Write(&charRead, 1, si_voinuoc);

		        si_output = Open("output.txt", 0);	// Open as read and write.
		        if (si_output == -1) 
			{
		            Close(si_sinhvien);
		            Signal("main");
		            return 1;
		        }

		        Seek(-1, si_output);		// Starts writing at the end of .txt file.
		        Write(&charRead, 1, si_output);	
		        Close(si_output);		// Close output.txt
		    } else getWater = 1;

		    if (filePointer == fileLen - 1) 
		    {	
		        Write("*", 1, si_voinuoc);	// use '*' to mark at the end of timepoint which is being handled.	
		        getWater = 1;			
		    }

		    if (getWater == 1) 
		    {
		        Close(si_voinuoc);			// Close voinuoc.c
		        Signal("voinuoc");
		        Wait("sinhvien");			// Wait for signal from voinuoc.c.

		        if (CreateFile("voinuoc.txt") == -1) 
			{	
		            Close(si_sinhvien);			// Close sinhvien.c
		            Signal("main");			
		            return 1;				
		        }

		        si_voinuoc = Open("voinuoc.txt", 0);	// Open at read-write mode.
		        if (si_voinuoc == -1) {
		            Close(si_sinhvien);
		            Signal("main");
		            return 1;
		        }
		    }

		    filePointer++;			// Move the pointer to the next character.
		}

		Signal("main");				// Signal the main process.
	    }

	    return 0;
}

