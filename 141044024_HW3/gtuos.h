#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"

#include <queue>
#include <ctime>
#include <vector>
#include <time.h>
#include <string.h>
#include <iostream>
#include <map>

using namespace std;

#define NUM_OF_THREADS 256
#define STACK_SPACE 255

// tanimlamalar

#define PRINT_B_CYCLE 10
#define PRINT_MEM_CYCLE 10
#define READ_B_CYCLE 10
#define READ_MEM_CYCLE 10
#define PRINT_STR_CYCLE 10
#define READ_STR_CYCLE 100
#define GET_RND_CYCLE 5

#define TCreate_CYCLE 80
#define TExit_CYCLE 50
#define TJoin_CYCLE 40
#define TYield_CYCLE 40



#define PRINT_B_CODE 1
#define PRINT_MEM_CODE 2
#define READ_B_CODE 3
#define READ_MEM_CODE 4
#define PRINT_STR_CODE 5
#define READ_STR_CODE 6
#define GET_RND_CODE 7


#define TCreate_CODE 11
#define TExit_CODE 8
#define TJoin_CODE 9
#define TYield_CODE 10


#define FileCreate_CODE 12
#define FileClose_CODE 13
#define FileOpen_CODE 14
#define FileRead_CODE 15
#define FileWrite_CODE 16
#define FileSeek_CODE 17
#define DirRead_CODE 18

#define QUANTUM_CYCLE 100

struct GtuFileAttr{
    char name[35];
    char creation_time[35];
    char last_modify_time[35];
    char last_access_time[35];
    int fileSize;
};

struct GtuFile{
    GtuFileAttr attr;
    char data[1024];
    int usedBlockCount;

};

struct OpenGtuFile{
    GtuFile* gf;
    int pos;
    bool isOpen;
};


enum T_State{
    READY, BLOCKED, RUNNING
};

struct Thread{
    State8080 regs;			    // The saved register set of 8080 chip
    uint8_t tid;				// The thread ID
    uint64_t start_cycle;		// Starting time of the thread (the cycle number of the CPU)
    uint64_t current_cycle;		// How many cycles the thread has used so far
    T_State state;				// The state of the thread (ready, blocked, running)
    uint32_t addr;				// The starting address of the memory location of the thread function
    uint8_t stack_space;		// Available empty stack space
    bool isDead;
    uint8_t joinTid;
};



class GTUOS{

	public:
        GTUOS(const CPU8080 & cpu, int & cycles, int debug);

		uint64_t handleCall(CPU8080 & cpu);
		string getMemory(const CPU8080 & cpu);
		uint16_t getPC(CPU8080 & cpu);

        bool readDatFile(const char* name);
        bool writeDatFile(const char* name);

        void printDir();
        void printTable();

		class Scheduler{
		public:
			friend class GTUOS;
			Scheduler();

			void T_Schedule(CPU8080 & cpu, uint16_t prePC, bool isSysCall);

		private:
			Thread* t_table;
			queue<Thread*> t_queue;

			int *cycles;
			int preCycles;
			uint8_t quantum;
            uint16_t prePC;
            bool switchFlag;

            int debug;

			void T_Context_Switch(CPU8080 & cpu);
			void T_Create(CPU8080 & cpu, Thread* th);
			void T_Join(uint8_t tid, CPU8080 & cpu);
			void T_Exit(CPU8080 & cpu);
			void T_Yield(CPU8080 & cpu);
		};
	private:

        Thread thread_table[NUM_OF_THREADS];
        int* cycles;

        vector<GtuFile> allFiles;
        vector<OpenGtuFile> openFiles;

        int blockSize;
        int blockCount;
        int usedBlockCount;
        int openCounter;


		uint64_t PRINT_B(const CPU8080 & cpu);
		uint64_t PRINT_MEM(const CPU8080 & cpu);
		uint64_t READ_B(const CPU8080 & cpu);
		uint64_t READ_MEM(const CPU8080 & cpu);
		uint64_t PRINT_STR(const CPU8080 & cpu);
		uint64_t READ_STR(const CPU8080 & cpu);
		uint64_t GET_RND(const CPU8080 & cpu);

		uint64_t TCreate(CPU8080 & cpu);
		uint64_t TExit(CPU8080 & cpu);
		uint64_t TJoin(CPU8080 & cpu);
		uint64_t TYield(CPU8080 & cpu);

		uint64_t FileCreate(const CPU8080 & cpu);
        uint64_t FileClose(const CPU8080 & cpu);
        uint64_t FileOpen(const CPU8080 & cpu);
        uint64_t FileRead(const CPU8080 & cpu);
        uint64_t FileWrite(const CPU8080 & cpu);
        uint64_t FileSeek(const CPU8080 & cpu);
        uint64_t DirRead(const CPU8080 & cpu);

	public:
		Scheduler scheduler;
};



#endif
