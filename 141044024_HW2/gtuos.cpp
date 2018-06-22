#include <iostream>
#include <time.h>
#include <string.h>

#include "8080emuCPP.h"
#include "gtuos.h"



GTUOS::GTUOS(const CPU8080 & cpu, int & cycles, int debug) {

    scheduler.t_table = this->thread_table;
    this->cycles = &cycles;
    scheduler.cycles = this->cycles;
    scheduler.debug = debug;

    for (int i = 1; i < NUM_OF_THREADS; i++)
        thread_table[i].isDead = true;

    // this initilization is for main thread
    Thread *th = &thread_table[0];
    (*th).tid = 0;
    (*th).state = RUNNING;
    (*th).isDead = false;
    (*th).regs = *(cpu.state);
    (*th).addr = 0x0000;
    (*th).current_cycle = 0;
    (*th).stack_space = STACK_SPACE;
    (*th).start_cycle = 0;
    (*th).regs.sp = 0;

    scheduler.t_queue.push(th); 		///////////////////

    this->cycles = &cycles;
}


uint64_t GTUOS::handleCall(CPU8080 & cpu) {

    // PRINT_B
    if (cpu.state->a == PRINT_B_CODE)
        return PRINT_B(cpu);

        // PRINT_MEM
    else if (cpu.state->a == PRINT_MEM_CODE)
        return PRINT_MEM(cpu);

        // READ_B
    else if (cpu.state->a == READ_B_CODE)
        return READ_B(cpu);

        // READ_MEM
    else if (cpu.state->a == READ_MEM_CODE)
        return READ_MEM(cpu);

        // PRINT_STR
    else if (cpu.state->a == PRINT_STR_CODE)
        return PRINT_STR(cpu);

        // READ_STR
    else if (cpu.state->a == READ_STR_CODE)
        return READ_STR(cpu);

        // GET_RND
    else if (cpu.state->a == GET_RND_CODE)
        return GET_RND(cpu);

        // TCreate
    else if (cpu.state->a == TCreate_CODE)
        return TCreate(cpu);

        // TExit
    else if (cpu.state->a == TExit_CODE) {
        TExit(cpu);
        return TExit_CYCLE;
    }
        // TJoin
    else if(cpu.state->a == TJoin_CODE)
        return TJoin(cpu);

        // TYield
    else if(cpu.state->a == TYield_CODE)
        return TYield(cpu);

    return 0;
}

uint16_t GTUOS::getPC(CPU8080 &cpu) {
    return cpu.state->pc;
}

uint64_t GTUOS::TCreate(CPU8080 & cpu) {
    uint32_t addr = (cpu.state->b << 8) | cpu.state->c;

    for(int i = 0; i < NUM_OF_THREADS; i++) {
        Thread* th = &thread_table[i];
        if ((*th).isDead) {
            (*th).state = READY;
            (*th).isDead = false;
            (*th).tid = i;
            (*th).start_cycle = *cycles;
            (*th).stack_space = STACK_SPACE;
            (*th).current_cycle = *cycles;
            (*th).addr = addr;
            (*th).regs.sp = (uint16_t)(STACK_SPACE * i);

            bzero(&(*th).regs, sizeof(State8080));
            (*th).regs.pc = addr;

            scheduler.T_Create(cpu, th);

            return TCreate_CYCLE;
        }
    }
    return TCreate_CYCLE;
}


uint64_t  GTUOS::TExit(CPU8080 & cpu) {
    scheduler.T_Exit(cpu);
}


uint64_t GTUOS::TJoin(CPU8080 & cpu){
    scheduler.T_Join(cpu.state->b, cpu);
}


uint64_t GTUOS::TYield(CPU8080 &cpu) {
    scheduler.T_Yield(cpu);
}


//////////////////////////////////////////////////////////

GTUOS::Scheduler::Scheduler(){
    preCycles = 0;
    quantum = QUANTUM_CYCLE;
    this->switchFlag = false;
    this->prePC = 0;
}

void GTUOS::Scheduler::T_Schedule(CPU8080 & cpu, uint16_t prePC, bool isSysCall) {


    this->prePC = prePC;
    switchFlag = isSysCall;
    if(*cycles - preCycles >= quantum && t_queue.size() > 1){

        T_Context_Switch(cpu);

        preCycles = *cycles;
    }
/*
    if(isSysCall && !switchFlag)
        switchFlag = true;

    if(!switchFlag)
        ;
*/



}

void GTUOS::Scheduler::T_Context_Switch(CPU8080 &cpu) {
    Thread *th;
    th = t_queue.front();

    int tid1;

    if ((*th).isDead) {
        t_queue.pop();
        th = t_queue.front();
    } else {
        (*th).current_cycle = *cycles;
        (*th).regs = *(cpu.state);

        if (switchFlag) {
            //(*th).regs.pc = this->prePC + 3; // onceki pc(gtu call pc ' si yanlis!)
            switchFlag = false;
        }
    }

    tid1 = (*th).tid;

    if((*th).state == RUNNING) {
        (*th).state = READY;
    }
    if(t_queue.size() == 1)
        th = t_queue.front();
    else {
        t_queue.pop();
        t_queue.push(th);
        th = t_queue.front();
    }

    if(th->state == READY) {
        th->state = RUNNING;
    }
    else{
        while(th->state == BLOCKED) {
            if(t_table[th->joinTid].isDead == false) {
                th = t_queue.front();
                t_queue.pop();

                t_queue.push(th);
                th = t_queue.front();
            }
            else
                (*th).state = RUNNING;

        }
    }

    (*th).state = RUNNING;
    *(cpu.state) = th->regs;

    if(debug == 2){
        cout << "tid 1 :" << tid1 << " , " << "tid 2 : " << (int)(*th).tid << endl;
        cout << "the total cycles spent for the blocked thread : " << t_table[tid1].current_cycle << endl;
    }else if(debug == 3){
        cout << "Tid" << "\t";
        cout << "Starting_Time" << "\t\t";
        cout << "Total_Cycle" << "\t";
        cout << "State" << "\t\t";
        cout << "Starting_Adress" << "\t\t";
        cout << "Empty_Stack_Space" << "\t";
        cout << "PC" << "\t";
        cout << "SP" << "\t";
        cout << endl;
        queue<Thread*> temp(t_queue);
        while(!temp.empty()) {
            Thread* th = temp.front();
            temp.pop();
            cout << (int)(*th).tid << "\t\t";
            cout << (int)(*th).start_cycle << "\t\t";
            cout << (int)(*th).current_cycle << "\t\t";
            string st;
            if((*th).state == 0)
                st = "READY";
            else if((*th).state == 1)
                st = "BLOCKED";
            else
                st = "RUNNNING";

            cout << st << "\t\t";
            printf("%5d\t\t\t", (*th).addr);
            //cout << (int)(*th).addr << "\t\t";
            printf("%5d\t\t", (*th).stack_space - (*th).regs.sp);
            //cout << (int)(*th).stack_space - (*th).regs.sp << "\t\t";
            cout << (*th).regs.pc << "\t\t";
            cout << (*th).regs.sp << "\t\t";
            cout << endl;
        }
        /*
        for(int i = 0; i < NUM_OF_THREADS; ++i) {
            if(!t_table[i].isDead){
                cout << (int)(*th).tid << "\t\t";
                cout << (int)(*th).start_cycle << "\t\t";
                cout << (int)(*th).current_cycle << "\t\t";
                cout << (*th).state << "\t\t";
                cout << (int)(*th).addr << "\t\t";
                cout << (int)(*th).stack_space - (*th).regs.sp << "\t\t";
                cout << (int)(*th).regs.pc << "\t\t";
                cout << (int)(*th).regs.sp << "\t\t";
                cout << endl;
            }
        }
         */
        cout << endl;
    }


}

void GTUOS::Scheduler::T_Create(CPU8080 & cpu, Thread *th) {
    t_queue.push(th);
    cpu.state->b = (*th).tid;
    t_queue.front()->regs.b = (*th).tid;
}

void GTUOS::Scheduler::T_Exit(CPU8080 & cpu) {
    Thread* th = t_queue.front();
    (*th).isDead = true;

    switchFlag = true;
    //cout << "exiting..." << endl;
    T_Context_Switch(cpu);
    //cout << "exited..." << endl;
}

void GTUOS::Scheduler::T_Join(uint8_t tid, CPU8080 & cpu) {
    if(!t_table[tid].isDead) {
        //t_queue.front()->regs.pc = this->prePC;
        t_queue.front()->state = BLOCKED;
        t_queue.front()->joinTid = tid;

        switchFlag = true;
        T_Context_Switch(cpu);
    }
}

void GTUOS::Scheduler::T_Yield(CPU8080 & cpu) {
    switchFlag = true;
    T_Context_Switch(cpu);
}

//////////////////////////////////////////////////////////////////

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
