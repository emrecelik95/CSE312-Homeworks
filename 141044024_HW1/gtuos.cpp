#include <iostream>
#include <time.h>
#include <string.h>

#include "8080emuCPP.h"
#include "gtuos.h"


// tanimlamalar

#define PRINT_B_CYCLE 10
#define PRINT_MEM_CYCLE 10
#define READ_B_CYCLE 10
#define READ_MEM_CYCLE 10
#define PRINT_STR_CYCLE 10
#define READ_STR_CYCLE 100
#define GET_RND_CYCLE 5 


#define PRINT_B_CODE 1
#define PRINT_MEM_CODE 2
#define READ_B_CODE 3
#define READ_MEM_CODE 4
#define PRINT_STR_CODE 5
#define READ_STR_CODE 6
#define GET_RND_CODE 7


uint64_t GTUOS::handleCall(const CPU8080 & cpu){

	// PRINT_B
	if(cpu.state->a == PRINT_B_CODE)
		return PRINT_B(cpu);

	// PRINT_MEM
	else if(cpu.state->a == PRINT_MEM_CODE)
		return PRINT_MEM(cpu);
	
	// READ_B
	else if(cpu.state->a == READ_B_CODE)
		return READ_B(cpu);

	// READ_MEM
	else if(cpu.state->a == READ_MEM_CODE)
		return READ_MEM(cpu);

	// PRINT_STR
	else if(cpu.state->a == PRINT_STR_CODE)
		return PRINT_STR(cpu);

	// READ_STR
	else if(cpu.state->a == READ_STR_CODE)
		return READ_STR(cpu);

	// GET_RND
	else if(cpu.state->a == GET_RND_CODE)
		return GET_RND(cpu);


	return 0;
}


// exe.mem ixin olusturulan string tipinde memory contenti
string GTUOS::getMemory(const CPU8080 & cpu)
{
	int memSize = 65536;
	string out = "";
	for(int i = 0 ; i < memSize; i += 16)
	{
		char st[50];
		sprintf(st, "0x%04x", i);
		out += st;
		out += "\t\t";
		for(int j = i; j < i + 16; ++j){
			sprintf(st, "0x%02x", cpu.memory->at(j));
			out += st; 
			out += "\t";
		}
		out += "\n";
	}	

	return out;
}

// register b'nin printi
uint64_t GTUOS::PRINT_B(const CPU8080 & cpu){

	uint16_t reg = cpu.state->b;

	cout << reg << endl;

	return PRINT_B_CYCLE;

}

// adresi BC olan memory contentinin print edilmesi
uint64_t GTUOS::PRINT_MEM(const CPU8080 & cpu){

	uint32_t addr = (cpu.state->b << 8) | cpu.state->c;
	uint32_t memVal = cpu.memory->at(addr);

	cout << memVal << endl;

	return PRINT_MEM_CYCLE;
}

// alınan inputun B register'ina koyulmasi
uint64_t GTUOS::READ_B(const CPU8080 & cpu){

	int input;
	cin >> input;

	cin.ignore();

	cpu.state->b = (uint8_t)input;

	return READ_B_CODE;
}

// alınan inputun, adresi BC olan memory contentine koyulmasi
uint64_t GTUOS::READ_MEM(const CPU8080 & cpu)
{
	int input;
	cin >> input;

	cin.ignore();

	uint32_t addr = (cpu.state->b << 8) | cpu.state->c;
	uint8_t* memPtr = &(cpu.memory->at(addr));

	*memPtr = (uint8_t)input;

	return READ_MEM_CYCLE;
}

// adresi BC olan null terminated stringin print edilmesi
uint64_t GTUOS::PRINT_STR(const CPU8080 & cpu){

	uint32_t addr = (cpu.state->b << 8) | cpu.state->c;
	
	while(cpu.memory->at(addr) != '\0')
	{
		cout << cpu.memory->at(addr);
		++addr;
	}
	cout << endl;
	return PRINT_STR_CYCLE;
	 
}
// adresi bc olan memory contentine input yazılması
uint64_t GTUOS::READ_STR(const CPU8080 & cpu){

	uint32_t addr = (cpu.state->b << 8) | cpu.state->c;

	uint8_t ch = ' ';
	
	string line;
	getline(cin, line);
	
	uint8_t i = 0;

	while(ch != '\0'){
		ch = line[i++];
		cpu.memory->at(addr) = ch;
		++addr;
	}
	cpu.memory->at(addr) = '\0';

	return READ_STR_CYCLE;

}
// nanosaniyeler kullanilarak olusturulan randomun B register'ina koyulması
uint64_t GTUOS::GET_RND(const CPU8080 & cpu){
	
	struct timespec t;
	clock_gettime(CLOCK_REALTIME,&t);

	srand(t.tv_nsec);

	cpu.state->b = rand()%256;

	return GET_RND_CYCLE;
}
