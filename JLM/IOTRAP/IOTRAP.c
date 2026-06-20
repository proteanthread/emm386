/*
 * iotrap.asm - --- JLM sample IOTRAP --- use Makefile to create IOTRAP.DLL --- IOTRAP traps IO port 100h when the first client --- wants to register a callback. --- The client is notified whenever an IO access happens. (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing IOTRAP.ASM.
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
// .model flat, stdcall

#define PORT 100h // port to trap

#define CODE16_SEL 20h
#define DATA16_SEL 28h

// .nolist
#include <jlm.h>
// .list

#define DEVICE_ID 6661h

#define cr 13
#define lf 10

#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0

// .data

// --- the DDB must be make public. The linker will "export" this
// --- symbol. This is the simplest method to make JLoad know the
// --- device id.

// public ddb

// ddb VxD_Desc_Block <0,0,DEVICE_ID,1,0,0,"IOTRAP",0,0, v86_dispatch >

// szHello db "Hello from JLM IOTRAP",cr,lf,0

// cntCB dd 0
// callbacks label dword
// dd 8*2 dup (0) // allow 8 callbacks

// .code

// --- dispatcher for v86 services

void v86_dispatch(void)
{
    /*
     * Mapped logic from v86_dispatch in IOTRAP.ASM:
     * v86_dispatch proc
     * 
     * 	@VMMCall Simulate_Far_Ret	;emulate a RETF in v86
     * 
     * 	and [ebp].Client_Reg_Struc.Client_EFlags,not 1  ;clear Carry flag
     * 	movzx eax, word ptr [ebp].Client_Reg_Struc.Client_EAX
     * 	cmp eax,0
     * 	jz getversion
     * 	cmp eax,1
     * 	jz register_callback
     * 	or [ebp].Client_Reg_Struc.Client_EFlags,1  ;set Carry flag
     * 	ret
     * 	align 4
     * 
     * v86_dispatch endp
     */
}


void getversion(void)
{
    /*
     * Mapped logic from getversion in IOTRAP.ASM:
     * getversion proc
     * 
     * 	mov word ptr [ebp].Client_Reg_Struc.Client_EAX, 0100h
     * 	ret
     * 	align 4
     * 
     * getversion endp
     */
}


void register_callback(void)
{
    /*
     * Mapped logic from register_callback in IOTRAP.ASM:
     * register_callback proc
     * 	.if ( cntCB >= 8 )
     * 		or [ebp].Client_Reg_Struc.Client_EFlags,1  ;set Carry flag
     * 		mov word ptr [ebp].Client_Reg_Struc.Client_EAX, 8
     * 		ret
     * 	.endif
     * ;--- when the first client registers, install an io handler
     * 	.if ( cntCB == 0 )
     * 		mov esi, offset iocb
     * 		mov edx, PORT
     * 		@VMMCall Install_IO_Handler
     * 		.if ( CARRY? )
     * 			or [ebp].Client_Reg_Struc.Client_EFlags,1  ;set Carry flag
     * 			mov word ptr [ebp].Client_Reg_Struc.Client_EAX, 9
     * 			ret
     * 		.endif
     * 	.endif
     * 	mov cx, word ptr [ebp].Client_Reg_Struc.Client_ECX	;callback's CS
     * 	shl ecx,16
     * 	mov cx, word ptr [ebp].Client_Reg_Struc.Client_EDX	;callback's IP
     * 	mov eax, cntCB
     * 	mov [eax*8+callbacks+0], ecx
     * 	mov ecx, [ebp].Client_Reg_Struc.Client_EBX			;callback's param
     * 	mov [eax*8+callbacks+4], ecx
     * 	inc cntCB
     * 	ret
     * 	align 4
     * 
     * register_callback endp
     */
}


// --- io handler proc
// --- ecx=type of io
// --- edx=port
// --- ebp=client struct
// --- eax=io data

// PF32 typedef ptr far32

void iocb(void)
{
    /*
     * Mapped logic from iocb in IOTRAP.ASM:
     * iocb proc
     * if 0 ;if the client is to be called in v86-mode
     * 	push esi
     * 	push edi
     * 	sub esp, sizeof Client_Reg_Struc
     * 	mov edi, esp
     * 	push ecx
     * 	push edx
     * 	push eax
     * 	@VMMCall Save_Client_State
     * 	@VMMCall Begin_Nest_Exec 	;start nested execution
     * 	mov esi, offset callbacks
     * 	mov ecx, cntCB
     * 
     * 	.while ecx
     * 		push ecx
     * 		mov eax,[esp+4]
     * 		mov edx,[esp+8]
     * 		mov ecx,[esp+12]
     * 		mov [ebp].Client_Reg_Struc.Client_EDX, edx
     * 		mov [ebp].Client_Reg_Struc.Client_ECX, ecx
     * 		mov [ebp].Client_Reg_Struc.Client_EAX, eax
     * 		lodsd
     * 		movzx edx,ax
     * 		shr eax,16
     * 		mov ecx, eax
     * 		@VMMCall Simulate_Far_Call
     * 		lodsd
     * 		mov [ebp].Client_Reg_Struc.Client_EBX, eax
     * 		@VMMCall Resume_Exec		;run the VM
     * 		pop ecx
     * 		dec ecx
     * 	.endw
     * 
     * 	@VMMCall End_Nest_Exec		;end nested execution
     * 	lea esi, [esp+3*4]
     * 	@VMMCall Restore_Client_State
     * 	pop eax
     * 	pop edx
     * 	pop ecx
     * 	add esp, sizeof Client_Reg_Struc
     * 	pop edi
     * 	pop esi
     * 	ret
     * else ;if the client is to be called in protected-mode
     * 	pushad
     * 	push ecx
     * 	push edx
     * 	push eax
     * 	sub esp,8
     * 	sgdt [esp]
     * 	mov eax,[esp+2]
     * 	add esp,8
     * 	mov esi, offset callbacks
     * 	mov ecx, cntCB
     * 
     * 	.while ecx
     * 		push ecx
     * 		push eax
     * 		lodsd
     * 		push CODE16_SEL
     * 		pushw 0
     * 		push ax
     * 		shr eax,16
     * 		shl eax, 4
     * 		mov edx,[esp+2*4]
     * 		mov [edx+CODE16_SEL+2],ax
     * 		mov [edx+DATA16_SEL+2],ax
     * 		shr eax,16
     * 		mov [edx+CODE16_SEL+4],al
     * 		mov [edx+DATA16_SEL+4],al
     * 		lodsd
     * 		mov ebx, eax
     * 		mov eax,[esp+4*4+0]
     * 		mov edx,[esp+4*4+4]
     * 		mov ecx,[esp+4*4+8]
     * 		push ds
     * 		push DATA16_SEL
     * 		pop ds
     * 		call PF32 ptr [esp+4]
     * 		pop ds
     * 		add esp,2*4
     * 		pop eax
     * 		pop ecx
     * 		dec ecx
     * 	.endw
     * 
     * 	add esp,3*4
     * 	popad
     * 	ret
     * endif
     * 	align 4
     * iocb endp
     */
}


// --- install the JLM: just set eax=1
// --- this tells JLOAD that it's ok to add IOTRAP to the list of
// --- loaded modules.

void install(void)
{
    /*
     * Mapped logic from install in IOTRAP.ASM:
     * install proc uses esi pcomm:ptr JLCOMM
     * 
     * 	mov eax,1
     * 	ret
     * 	align 4
     * 
     * install endp
     */
}


// --- deinstall the JLM: just set eax=1.
// --- this tells JLOAD that it's ok to remove the module.

void deinstall(void)
{
    /*
     * Mapped logic from deinstall in IOTRAP.ASM:
     * deinstall proc pcomm:ptr JLCOMM
     * 
     * 	.if ( cntCB )
     * 		mov edx, PORT
     * 		@VMMCall Remove_IO_Handler
     * 	.endif
     * 	mov eax,1
     * 	ret
     * 	align 4
     * 
     * deinstall endp
     */
}


void DllMain(void)
{
    /*
     * Mapped logic from DllMain in IOTRAP.ASM:
     * DllMain proc stdcall public hModule:dword, dwReason:dword, dwRes:dword
     * 
     * 	mov eax,dwReason
     * 	cmp eax,DLL_PROCESS_ATTACH
     * 	jnz @F
     * 	invoke install, dwRes
     * 	jmp exit
     * @@:
     * 	cmp eax,DLL_PROCESS_DETACH
     * 	jnz @F
     * 	invoke deinstall, dwRes
     * @@:
     * exit:
     * 	ret
     * 	align 4
     * 
     * DllMain endp
     */
}


// end DllMain
