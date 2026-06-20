/*
 * xmstest.asm - --- XMSTEST: test xms memory moves --- Public Domain. --- to be assembled with JWasm or Masm v6. (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing XMSTEST.ASM.
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

// .model small
// DGROUP group _TEXT // use tiny model
// .386

// .dosseg
// .stack 2048

#define cr 13
#define lf 10

// --- XMS handle table

// XMSHT struct
// db ?
// bSize   db ?
// wHdls   dw ?
// dwArray dd ?
// XMSHT ends

// --- XMS handle

// XMSH struct
// bFlags db ? // flags, see below
// bLocks db ? // number of locks
// dwAddr dd ? // addr in KB
// dwSize dd ? // size in KB
// XMSH ends

#define XMSF_FREEB 1 // free block
#define XMSF_USEDB 2 // used block
#define XMSF_FREEH 4 // free handle

// --- define a string constant

// CStr macro string:vararg
// local xxx
// .const
// xxx db string
// db 0
// .code
// exitm <offset xxx>
// endm

// --- display word decimal

// @wordout_d macro number,format
// mov cl,format
// ifidni <number>,<ax>
// else
// mov ax,number
// endif
// call _wordout_d
// endm

// --- display dword decimal

// @dwordout_d macro number,format
// ifnb <number>
// mov ax,word ptr number+0
// mov dx,word ptr number+2
// endif
// mov cl, format
// call _dwordout_d
// endm

// .data

// xmsadr    dd 0 // XMS host call address
// dwTotal   dd 0 // total size EMBs
// freehdls  dw 0 // count free handles
// wVersion  dw 0 // XMS version
// bFlags    db 0 // flags

#define FL_NOSIZENULL 1
#define FL_NOUSEDEMBS 2
#define FL_NOFREEEMBS 4
#define FL_FREEHANDLES 8

// .code

// assume DS:DGROUP

#include <printf.h>

void protocol(void)
{
    /*
     * Mapped logic from protocol in XMSTEST.ASM:
     * protocol proc
     * 	push bx
     * 	invoke printf, CStr("%s"), dx
     * 	pop ax
     * 	cmp al,0
     * 	jz @F
     * 	mov ah,0
     * 	invoke printf, CStr(" failed, bl=%02X", 10), ax
     * 	ret
     * @@:
     * 	invoke printf, CStr(" ok",10)
     * 	ret
     * protocol endp
     */
}


// xms_move struct
// len           dd  ? // block length in bytes
// src_handle    dw  ? // source handle
// src_offset    dd  ? // offset into source
// dest_handle   dw  ? // destination handle
// dest_offset   dd  ? // offset into destination
// xms_move ends

// --- test xms move block function

void movetest(void)
{
    /*
     * Mapped logic from movetest in XMSTEST.ASM:
     * movetest proc
     * 
     * local handle:word
     * local emm:xms_move
     * 
     * 	mov ah,9
     * 	mov dx,64
     * 	mov bl,0
     * 	call [xmsadr]
     * 	cmp bl,0
     * 	jnz failed
     * 	mov handle, dx
     * 
     * ;--- test 1
     * 
     * 	mov dx,handle
     * 	mov emm.len, 10000
     * 	mov emm.src_handle, dx
     * 	mov emm.src_offset, 0
     * 	mov emm.dest_handle, dx
     * 	mov emm.dest_offset, 10000
     * 	lea si, emm
     * 	mov ah,0bh
     * 	mov bl,0
     * 	call [xmsadr]
     * 	mov dx, CStr("XMS block move, src < dest && src+len < dest")
     * 	call protocol
     * 
     * ;--- test 2
     * 
     * 	mov dx,handle
     * 	mov emm.len, 20000
     * 	mov emm.src_handle, dx
     * 	mov emm.src_offset, 0
     * 	mov emm.dest_handle, dx
     * 	mov emm.dest_offset, 10000
     * 	lea si, emm
     * 	mov ah,0bh
     * 	mov bl,0
     * 	call [xmsadr]
     * 	mov dx, CStr("XMS block move, src < dest && src+len > dest")
     * 	call protocol
     * 
     * ;--- test 3
     * 
     * 	mov dx,handle
     * 	mov emm.len, 10000
     * 	mov emm.src_handle, dx
     * 	mov emm.src_offset, 10000
     * 	mov emm.dest_handle, dx
     * 	mov emm.dest_offset, 0
     * 	lea si, emm
     * 	mov ah,0bh
     * 	mov bl,0
     * 	call [xmsadr]
     * 	mov dx, CStr("XMS block move, src > dest && src > dest+len")
     * 	call protocol
     * 
     * ;--- test 4
     * 
     * 	mov dx,handle
     * 	mov emm.len, 20000
     * 	mov emm.src_handle, dx
     * 	mov emm.src_offset, 10000
     * 	mov emm.dest_handle, dx
     * 	mov emm.dest_offset, 0
     * 	lea si, emm
     * 	mov ah,0bh
     * 	mov bl,0
     * 	call [xmsadr]
     * 	mov dx, CStr("XMS block move, src > dest && src < dest+len")
     * 	call protocol
     * 
     * 	mov dx,handle
     * 	mov ah,0ah
     * 	mov bl,0
     * 	call [xmsadr]
     * 	ret
     * failed:
     * 	invoke printf, CStr("XMS call AH=08 failed",lf)
     * 	ret
     * movetest endp
     */
}


// --- main

void main(void)
{
    /*
     * Mapped logic from main in XMSTEST.ASM:
     * main    proc c
     * 
     * 	mov ax,4300h
     * 	int 2fh
     * 	test al,80h 		 ;xms host found?
     * 	jnz main1
     * 	invoke printf, CStr("no XMS host found",lf)
     * 	jmp exit
     * main1:
     * 	mov ax,4310h		;get XMS call address
     * 	int 2fh
     * 	mov word ptr xmsadr+0,bx
     * 	mov word ptr xmsadr+2,es
     * 	invoke printf, CStr("XMS call address: %X:%X",10),
     * 		word ptr [xmsadr+2], word ptr [xmsadr+0]
     * 
     * 	call movetest
     * 
     * exit:
     * 	ret
     * main    endp
     */
}


// --- init

void start(void)
{
    /*
     * Mapped logic from start in XMSTEST.ASM:
     * start   proc
     * 
     * 	push cs
     * 	pop ds
     * 
     * 	mov cx,es
     * 	mov ax,ss
     * 	sub ax,cx
     * 	shl ax,4
     * 	add ax,sp
     * 	push cs
     * 	pop ss
     * 	mov sp,ax
     * 
     * 	pushf
     * 	pushf
     * 	pop ax
     * 	or	ah,70h			;a 80386 will have bit 15 cleared
     * 	push ax 			;if bits 12-14 are 0, it is a 80286
     * 	popf				;or a bad emulation
     * 	pushf
     * 	pop ax
     * 	popf
     * 	and ah,0f0h
     * 	js no386			;bit 15 set? then its a 8086/80186
     * 	jnz is386
     * no386:
     * 	invoke printf, CStr("a 80386 is needed",lf)
     * 	jmp done
     * is386:
     * 	call main
     * done:
     * 	mov ah,4Ch
     * 	int 21h
     * start   endp
     */
}


// END start
