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
	GTUOS	theOS;

	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);	
 	string db2;
	do	
	{
		totalCycles += theCPU.Emulate8080p(DEBUG); // emulatorun hesapladigi cyclelar
		if(theCPU.isSystemCall())
			totalCycles += theOS.handleCall(theCPU); // benim hesapladigim cyclelar
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

