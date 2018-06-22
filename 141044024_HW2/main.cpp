#include <iostream>
#include <fstream>

#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"

	// This is just a sample main function, you should rewrite this file to handle problems 
	// with new multitasking and virtual memory additions.
int main (int argc, char**argv)
{
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1); 
	}
	int DEBUG = atoi(argv[2]);
	int totalCycles = 0;

	memory mem;
	CPU8080 theCPU(&mem);

	GTUOS	theOS(theCPU, totalCycles, DEBUG);

	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);	
 	string db2;
 	uint16_t prePC = 0;
	uint16_t pc = 0;
 	bool isSysCall;
 	int debug2 = DEBUG;
	if(DEBUG == 3)
		debug2 = 0;

 	do
	{
	    pc = theOS.getPC(theCPU);
		totalCycles += theCPU.Emulate8080p(debug2); // emulatorun hesapladigi cyclelar
        isSysCall = theCPU.isSystemCall();
        if(isSysCall)
			totalCycles += theOS.handleCall(theCPU); // benim hesapladigim cyclelar

		if(!isSysCall)
			prePC = pc;

		theOS.scheduler.T_Schedule(theCPU, prePC, isSysCall);
		if(DEBUG == 2)
		{
			getline(std::cin, db2); 
		}

	}	while (!theCPU.isHalted())
;
	

	ofstream f;
	f.open("exe.mem");

	f << theOS.getMemory(theCPU);

	f.close();

	
	cout << "Total cycles : " << totalCycles << endl;

	return 0;
}

