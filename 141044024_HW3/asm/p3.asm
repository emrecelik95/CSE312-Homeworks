        ; 8080 assembler code
        .binfile p3.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

    ; OS call list
PRINT_B     equ 1
PRINT_MEM   equ 2
READ_B      equ 3
READ_MEM    equ 4
PRINT_STR   equ 5
READ_STR    equ 6
GET_RND     equ 7
T_EXIT      equ 8
T_JOIN      equ 9
T_YIELD     equ 10
T_CREATE    equ 11

FILE_CREATE equ 12
FILE_CLOSE  equ 13
FILE_OPEN   equ 14
FILE_READ   equ 15
FILE_WRITE  equ 16
FILE_SEEK   equ 17
DIR_READ    equ 18  


    ; Position for stack pointer
stack   equ 0F000h

    org 000H
    jmp begin

    ; Start of our Operating System
GTU_OS: PUSH D
    push D
    push H
    push psw
    nop ; This is where we run our OS in C++, see the CPU8080::isSystemCall()
        ; function for the detail.
    pop psw
    pop h
    pop d
    pop D
    ret
    ; ---------------------------------------------------------------
    ; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        


dir_info: ds 2048 

begin:
    LXI SP,stack    ; always initialize the stack pointer
    
    LXI b, dir_info
    mvi a, DIR_READ
    call GTU_OS

    mvi a, PRINT_STR
    call GTU_OS


    hlt