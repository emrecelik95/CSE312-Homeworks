        ; 8080 assembler code
        .binfile f4.com
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

arr: ds 50    ; array


; loop indisleri
ind  : db 0
i1	 : db 0
i2   : db 0

begin:
	LXI SP,stack 	; always initialize the stack pointer
	mvi h, 50	; dongu degiskeni

	LXI d, arr	; d = (arr) baslangic adresi

init_loop:

	MVI A, GET_RND	; store the OS call code to A
	call GTU_OS		; call the OS				

	;MVI A, PRINT_B
	;call GTU_OS

	mov a, b 	; a = b

	STAX d		; (arr) = a
 
	INR d		; (arr++)

	DCR h		; h--
	mov c, h	; c = h
	JNZ init_loop	

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

end_init_loop:
	LXI d, arr


sort_loop_1:
	LDA i1
	INR A
	STA i1
	SUI 50
	JZ end_sort_loop_1 ; dis dongu bitme kosulu
	
	LDA i1	

	LXI d, arr	   ; d = (arr) bastaki elemana don

sort_loop_2:
	STA i2
	SUI 50
	JZ sort_loop_1    ; ic dongu bitme kosulu

if:
	LDAX d
	mov b, a

	INR d
	LDAX d	
	CMP b	; if(arr[i + 1] < a[i])
	JC swap	; swap();
	jmp end_sort_loop_2 ; continue

swap:
	LDAX d	  ; a = arr[i + 1]
	mov c, a  ; c = a
	mov a, b  ; a = b, arr[i]
	STAX d	  ; arr[i+1] = arr[i]
	DCR d	  ; arr[i]
	mov a, c  ; a = c
	STAX d    ; arr[i] = a (temp)
	INR d	  ; arr[i+1]
	
end_sort_loop_2:
	LDA i2    ; i2++
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
	mov a, h
	SUI 50
	JNZ print_loop


end:
	hlt		; end program
