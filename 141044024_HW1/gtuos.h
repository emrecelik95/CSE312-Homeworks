#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"

using namespace std;

class GTUOS{
	public:
		uint64_t handleCall(const CPU8080 & cpu);

		string getMemory(const CPU8080 & cpu);
	private:
		uint64_t PRINT_B(const CPU8080 & cpu);
		uint64_t PRINT_MEM(const CPU8080 & cpu);
		uint64_t READ_B(const CPU8080 & cpu);
		uint64_t READ_MEM(const CPU8080 & cpu);
		uint64_t PRINT_STR(const CPU8080 & cpu);
		uint64_t READ_STR(const CPU8080 & cpu);
		uint64_t GET_RND(const CPU8080 & cpu);
};

#endif
