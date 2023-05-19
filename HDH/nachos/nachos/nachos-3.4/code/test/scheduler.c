#include "syscall.h"


int main()
{	
	int pingPID, pongPID;
	PrintString("\n\nTesting ping pong.");
	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong");
	Join(pingPID);
	Join(pongPID);


	Halt();
	
}
