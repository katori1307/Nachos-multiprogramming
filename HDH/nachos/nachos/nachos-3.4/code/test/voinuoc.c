#include "syscall.h"


int main()
{
	    SpaceId si_voinuoc, si_output; // for voinuoc.txt, output.txt
	    int tap1 = 0, tap2 = 0, tapTime = 0;
	    int problemSolved = 0; // to mark the time when every student got their water.
	    char charRead = '0';   // to read from .txt file

	    while (1) {
		while (1) {
		    Wait("voinuoc");			// Wait for signal from sinhvien.c
		    charRead = 0;
		    si_voinuoc = Open("voinuoc.txt", 1);	// Open as read-only.
		    if (si_voinuoc == -1) {
		        Signal("sinhvien");
		        return 1;
		    }

		    tapTime = 0;
		    problemSolved = 0;

		    while (1) {
		        if (Read(&charRead, 1, si_voinuoc) == -2) {	// Read to the end of file means return -2 (in system call Read).
		            Close(si_voinuoc);
		            break;
		        }
			
			// if we encounter '*' character, that means at the current timepoint, every students have had their water, so we set problemSolved = 1.
		        if (charRead == '*') 
			{ 
		            problemSolved = 1;		
		            Close(si_voinuoc);		
		            break;
		        }

		        tapTime = tapTime * 10 + (charRead - 48);
		    }

		    si_output = Open("output.txt", 0);	// Open at read-write mode.
		    if (si_output == -1) {
		        Close(si_voinuoc);
		        Signal("sinhvien");
		        return 1;
		    }

		    Seek(-1, si_output);		// Starts writing at the end of the file to avoid writing at the same position.

		    if (tapTime != 0) {
		        if (tap1 <= tap2) {
		            tap1 += tapTime;
		            Write(" 1", 2, si_output);	// Write tap 1 to output.txt.
		        } else {
		            tap2 += tapTime;
		            Write(" 2", 2, si_output);	// Write tap 2 to output.txt.
		        }
		    }

		    if (problemSolved == 1) {
		        tap1 = tap2 = 0;
		        Write("\r\n", 2, si_output);	// Write a newline to seperate every single time point.
							
		        Close(si_output);		
		        Signal("sinhvien");
		        break;
		    } else Write("   ", 3, si_output);	// write multiple space characters to seperate water from every student.

		    Close(si_output);			// Close output.txt
		    Signal("sinhvien");
		}
	    }

	    return 0;
}










