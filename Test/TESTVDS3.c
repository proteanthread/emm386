/*
 * testvds3.asm - --- VDS API test; display ISA DMA translation status for channels 0-7 (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing TESTVDS3.ASM.
 *
 * Changeability & Constraints:
 *   - CAN BE CHANGED: Local helper functions, logging wrappers, and diagnostic outputs.
 *   - CANNOT BE CHANGED: Standard API calling conventions, hardware entry vectors, and binary structure alignments.
 *
 * Expected Behavior:
 *   - Mapped counterpart declarations and logic flow from the original assembly source.
 *
 * Diagnostics & Recovery:
 *   - Verify compiler alignment flags and register preservation states if system lockups occur.
 */

// .286
// .model small
// .stack 2048
// .dosseg
// .386

// CStr macro text:vararg
// local sym
// .const
// sym db text,0
// .code
// exitm <offset sym>
// endm

#define lf 10

// .code

#include <printf.h>

void main(void)
{
    /*
     * Mapped logic from main in TESTVDS3.ASM:
     * main proc c
     * 
     * local	channel:word
     * 
     * 	xor ax, ax
     * 	mov es, ax
     * 	test byte ptr es:[47bh],20h
     * 	jz novds
     * 	mov ax,8102h
     * 	mov dx,0000
     * 	int 4bh
     * 	jnc @F
     * novds:
     * 	invoke printf, CStr("VDS not installed",lf)
     * 	jmp exit
     * @@:
     * 
     * ;--- strategy:
     * ;--- 1. call enable DMA translation
     * ;---    should fail, since disable cnt should be zero
     * 
     * 	mov channel, 0
     * nextchannel:
     * 	mov dx, 0
     * 	mov bx, channel
     * 	mov ax,810Ch
     * 	int 4bh
     * 	.if (CARRY?)
     * 		invoke printf, CStr("channel %u: DMA translation is active",lf), bx
     * 	.else
     * 		invoke printf, CStr("channel %u: DMA translation is disabled",lf), bx
     * 
     * ;--- restore disable cnt
     * 
     * 		mov dx, 0
     * 		mov bx, channel
     * 		mov ax,810Bh
     * 		int 4bh
     * 		setz cl
     * 		.if (CARRY?)
     * 			cbw
     * 			invoke printf, CStr("int 4B, ax=810Bh, BX=%u failed, AL=%X",lf), bx, ax
     * 		.endif
     * 	.endif
     * 
     * 	inc channel
     * 	cmp channel, 8
     * 	jb nextchannel
     * 
     * exit:
     * 	ret
     * main endp
     */
}


// start:
// mov ax, @data
// mov ds, ax
// mov bx, ss
// sub bx, ax
// shl bx, 4
// mov ss, ax
// add sp, bx
// call main
// mov ah, 4Ch
// int 21h

// END start
