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
TExit		equ 8
TJoin		equ 9
TYield		equ 10
TCreate		equ 11


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


begin:
	LXI SP,stack 	; always initialize the stack pointer
   
	LXI B, f1	; put the address of string in registers B and C
	MVI A, TCreate	; store the OS call code to A
	call GTU_OS	; call the OS

	MOV D, B

	LXI B, f3	; put the address of string in registers B and C
	MVI A, TCreate	; store the OS call code to A
	call GTU_OS	; call the OS

	MVI A, TJoin
	call GTU_OS

	MVI D, H

	MVI A, TJoin
	call GTU_OS

	hlt		; end program
	

f1:
    mvi c, 51		
	mvi a, 0		
loop1:
	MOV B, A	; B = A
	MVI A, PRINT_B	; store the OS call os code to A
	call GTU_OS	; call the OS

	MOV A, B 	; A = B
	INR A	
	DCR C		; --C

 	JNZ loop1	; goto loop if C!=0

 	MVI A, TExit
 	call GTU_OS



f3:
    mvi c, 51		
	mvi a, 50		
loop3:
	MOV B, A	; B = A
	MVI A, PRINT_B	; store the OS call os code to A
	call GTU_OS	; call the OS

	MOV A, B 	; A = B
	INR A	
	DCR C		; --C

 	JNZ loop3	; goto loop if C!=0

 	MVI A, TExit
 	call GTU_OS

