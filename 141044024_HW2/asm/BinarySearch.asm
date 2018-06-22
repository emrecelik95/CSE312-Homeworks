        ; 8080 assembler code
        .hexfile BinarySearch.hex
        .binfile BinarySearch.com
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
GET_RND 	equ 7

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
; YOU SHOULD NOT CHANGE ANYT0HING ABOVE THIS LINE        

arr: ds 50

ind  : db 0

error_msg:	dw 'Not found!',00AH,00H ; 

begin:
	LXI SP,stack 	; always initialize the stack pointer
	mvi h, 50

	LXI d, arr

init_loop:

	MVI A, GET_RND	; store the OS call code to A
	call GTU_OS		; call the OS				

	;MVI A, PRINT_B
	;call GTU_OS

	mov a, b

	STAX d
	INR d

	DCR h
	mov c, h
	JNZ init_loop

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

end_init_loop:
	LXI d, arr


sort_loop_1:
	LDA i1
	INR A
	STA i1
	SUI 50
	JZ end_sort_loop_1
	
	LDA i1

	LXI d, arr

sort_loop_2:
	STA i2
	SUI 50
	JZ sort_loop_1

if:
	LDAX d
	mov b, a

	INR d
	LDAX d
	CMP b
	JC swap
	jmp end_sort_loop_2

swap:
	LDAX d
	mov c, a
	mov a, b
	STAX d
	DCR d
	mov a, c
	STAX d
	INR d
	
end_sort_loop_2:
	LDA i2
	INR A
	STA i2
	jmp sort_loop_2

end_sort_loop_1:

	LXI b, arr
	mvi h, 0


print_loop:

	MVI A, PRINT_MEM	; store the OS call code to A
	call GTU_OS	 		; call the OS

	INR b	
	inr h
	mvi a, 49
	cmp h
	JC read_input
	JMP print_loop


; aranacak sayiyi al
read_input:

	MVI A, READ_B
	call GTU_OS

LXI d, arr
mvi m, 24
mvi h, 0

prelo db 0
prehi db 50

LXI d, arr

; sol ve sag indisleri kuculterek ortada bulustur
go_mid:
	LDA prelo
	mov h, a
	LDA prehi
	mov l, a	
	
	INC h
	DCR l
	INR d	

	mov a, h	
	CMP l
	JC if
	jmp go_mid

if:
; son eleman kalmissa bitir
	mov a, h
	SUB l
	mvi c, 2
	CMP c
	JC not_found
; aranan elemana esitse bitir
	LDAX d
	SUB b	
	JZ found
; arananla kiyaslayip saga veya sola git
	cmp b
	JC go_right
	jmp go_left 	

go_left:
	LDA prelo
	mov l, a
	jmp go_mid

go_right:
	LDA prehi
	mov h, a
	jmp go_mid
	



found:
	LDAX d
	mov b, d
	
	MVI A, PRINT_B	
	call GTU_OS
	jmp end

not_found:
	LDA error_msg
	mov b, a
	
	MVI A, PRINT_B	
	call GTU_OS
	jmp end

end:
	hlt		; end program
