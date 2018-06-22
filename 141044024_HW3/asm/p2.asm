        ; 8080 assembler code
        .binfile p2.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 1
PRINT_MEM	equ 2
READ_B		equ 3
READ_MEM	equ 4
PRINT_STR	equ 5
READ_STR	equ 6
GET_RND		equ 7
T_EXIT		equ 8
T_JOIN 		equ 9
T_YIELD		equ 10
T_CREATE 	equ 11

FILE_CREATE	equ 12
FILE_CLOSE	equ 13
FILE_OPEN	equ 14
FILE_READ	equ 15
FILE_WRITE	equ 16
FILE_SEEK	equ 17
DIR_READ	equ 18  


	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        

filename: ds 100

num: dw '0','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28','29','30','31','32','33','34','35','36','37','38','39','40','41','42','43','44','45','46','47','48','49','50'

begin:
	LXI SP,stack 	; always initialize the stack pointer
    mvi h, 0		;

    LXI B, filename
    mvi A, READ_STR
    call GTU_OS

    LXI B,filename
    mvi A, FILE_OPEN
    call GTU_OS

    mov e, b

    LXI B, num
    MVI D, 51

    MVI A, FILE_READ
    call GTU_OS

    LXI B, num
    mvi A, PRINT_STR
    call GTU_OS

    MVI A, FILE_CLOSE	; store the OS call code to A
    call GTU_OS	; call the OS

    hlt