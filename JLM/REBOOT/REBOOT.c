/*
 * reboot.asm - --- sample how to handle DEVICE_REBOOT_NOTIFY control message --- requires Jemm v5.86+ (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing REBOOT.ASM.
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

// .386
// .MODEL FLAT, stdcall
// option casemap:none

#include <jlm.h>

#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0

#define DEVICE_ID 4435h

// ifdef @pe_file_flags // -pe option set?
// option dotname
// .drectve segment info
// db "-dll "
// db "-subsystem:native "
// db "-fixed:no "
// .drectve ends

// .hdr$2 segment flat
// db "PX"
// .hdr$2 ends
// endif

// .const

// public export ddb

// ddb VxD_Desc_Block <0,0,DEVICE_ID,1,0,0,"REBOOT",0, ctrlproc >

// string1 db 13,10,"rebooting - press a key...",0

// .CODE

void ctrlproc(void)
{
    /*
     * Mapped logic from ctrlproc in REBOOT.ASM:
     * ctrlproc proc
     * 	cmp eax, DEVICE_REBOOT_NOTIFY
     * 	jz reboot_notify
     * 	clc
     * 	ret
     * reboot_notify:
     * 	@VMMCall Begin_Nest_Exec	 ;start nested execution
     * 	mov eax,offset string1
     * 	call printstring
     * 	mov byte ptr [ebp].Client_Reg_Struc.Client_EAX+1, 0
     * 	mov eax,16h
     * 	@VMMCall Exec_Int
     * 	@VMMCall End_Nest_Exec		 ;end nested execution
     * 	clc
     * 	ret
     * ctrlproc endp
     */
}


void printstring(void)
{
    /*
     * Mapped logic from printstring in REBOOT.ASM:
     * printstring proc uses esi
     * 	mov esi, eax
     * 	cld
     * nextitem:
     * 	lodsb
     * 	and al,al
     * 	jz done
     * 	mov byte ptr [ebp].Client_Reg_Struc.Client_EAX, al
     * 	mov byte ptr [ebp].Client_Reg_Struc.Client_EAX+1, 0Eh
     * 	mov eax,10h
     * 	@VMMCall Exec_Int
     * 	jmp nextitem
     * done:
     * 	ret
     * printstring endp
     */
}


void DllMain(void)
{
    /*
     * Mapped logic from DllMain in REBOOT.ASM:
     * DllMain PROC stdcall hModule:dword, dwReason:dword, dwRes:dword
     * 
     * 	mov eax,dwReason
     * 	cmp eax, DLL_PROCESS_ATTACH
     * 	jnz @F
     * 	mov eax,1
     * 	jmp exit
     * @@:
     * 	cmp eax,DLL_PROCESS_DETACH
     * 	jnz @F
     * 	mov eax,1
     * @@:
     * exit:
     * 	ret
     * DllMain endp
     */
}


// END DllMain

