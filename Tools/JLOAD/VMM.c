/*
 * vmm.asm - --- VMM functions --- best viewed with TAB size 4 (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing VMM.ASM.
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
// .model flat

// option casemap:none
void Int20_Handler(void)
{
    /*
     * Mapped logic from Int20_Handler in VMM.ASM:
     * Int20_Handler proc
     * 
     * 	test byte ptr [esp].IRETDV86._Efl+2,2	;called from v86-mode?
     * 	jnz isv86
     * 	pushad
     * 	mov esi,[esp+8*4].IRETDV86._Eip
     * 	mov eax,[esi]					;it is "INT 20h" followed by a dword
     * 	sub esi,2
     * 	mov [esp+8*4].IRETDV86._Eip, esi
     * 	movzx ecx, ax
     * 	shr eax,16
     * 	mov edx, offset ddb_list
     * @@nextdevice:
     * 	cmp ax, [edx].VxD_Desc_Block.DDB_Req_Device_Number	;is device known?
     * 	jz @@device_found
     * 	mov edx, [edx].VxD_Desc_Block.DDB_Next
     * 	and edx, edx
     * 	jnz @@nextdevice
     * @@error:
     * ;	popad
     * 	call Crash_Cur_VM	;AX:CX contain VM:Function to link to
     * @@device_found:
     * 	mov bh,ch
     * 	mov bl,0FFh
     * 	and bh,80h
     * 	and ch,7Fh
     * 	cmp ecx, [edx].VxD_Desc_Block.DDB_Service_Table_Size
     * 	jnc @@error
     * 	mov eax, [edx].VxD_Desc_Block.DDB_Service_Table_Ptr
     * 	shl ecx, 2
     * 	shr bh,3
     * 	add bh,15h
     * 	add eax, ecx
     * 	mov word ptr [esi],bx	;opcode for "call/jmp dword ptr []"
     * 	mov [esi+2],eax
     * 	popad
     * 	iretd
     * isv86:
     * 	jmp cs:[dfOldint20]
     * 	align 4
     * 
     * Int20_Handler endp
     */
}


// --- generic breakpoint
// --- has replaced Jemm's "REBOOT" breakpoint on init

void V86_Callback_Handler(void)
{
    /*
     * Mapped logic from V86_Callback_Handler in VMM.ASM:
     * V86_Callback_Handler  proc
     * 	mov eax,[ebp].Client_Reg_Struc.Client_CS
     * 	sub ax,word ptr [bprm+2]
     * 	jz isdefbp
     * 	inc ax
     * 	neg ax
     * 	movzx eax,ax
     * 	shl eax,3
     * 	mov edx, [eax+offset v86_bp].BPDYN.dwRefData
     * 	jmp [eax+offset v86_bp].BPDYN.dwCallback
     * isdefbp:
     * 	jmp [oldbp1]
     * 	align 4
     * V86_Callback_Handler endp
     */
}


void Get_VMM_Version(void)
{
    /*
     * Mapped logic from Get_VMM_Version in VMM.ASM:
     * Get_VMM_Version proc
     * 	mov eax, ?VERSIONLOW shl 16 + ?VERSIONHIGH
     * 	clc
     * 	ret
     * 	align 4
     * Get_VMM_Version endp
     */
}


void Get_Cur_VM_Handle(void)
{
    /*
     * Mapped logic from Get_Cur_VM_Handle in VMM.ASM:
     * Get_Cur_VM_Handle proc public
     * 	mov ebx, offset vmcb
     * 	clc
     * 	ret
     * 	align 4
     * Get_Cur_VM_Handle endp
     */
}


// --- deliberately cause a GPF. This will result in an
// --- invalid opcode exception reported to v86-mode

void Crash_Cur_VM(void)
{
    /*
     * Mapped logic from Crash_Cur_VM in VMM.ASM:
     * Crash_Cur_VM proc
     * 	push cs
     * 	pop ss
     * 	align 4
     * Crash_Cur_VM endp
     */
}


// --- get a v86 callback in EAX
// --- inp: ESI=callback, EDX=dwRefData
// --- C on error

void Allocate_V86_Call_Back(void)
{
    /*
     * Mapped logic from Allocate_V86_Call_Back in VMM.ASM:
     * Allocate_V86_Call_Back proc
     * 	push edi
     * 	mov edi, offset v86_bp
     * 	mov ecx, ?MAXBP
     * 	xor eax, eax
     * @@nextitem:
     * 	cmp eax, [edi].BPDYN.dwCallback
     * 	jz @@freecb
     * 	add edi, size BPDYN
     * 	loop @@nextitem
     * 	pop edi
     * 	stc
     * 	ret
     * @@freecb:
     * 	mov [edi].BPDYN.dwCallback, esi
     * 	mov [edi].BPDYN.dwRefData, edx
     * 	sub edi, offset v86_bp
     * 	shr edi, 3
     * 	inc edi
     * 	mov ax, word ptr [bprm+2]
     * 	sub ax, di
     * 	shl eax, 16
     * 	mov ax,di
     * 	shl ax,4
     * 	add ax, word ptr [bprm+0]
     * 	pop edi
     * 	ret
     * 	align 4
     * Allocate_V86_Call_Back endp
     */
}


// --- free a v86 callback in EAX

void Free_V86_Call_Back(void)
{
    /*
     * Mapped logic from Free_V86_Call_Back in VMM.ASM:
     * Free_V86_Call_Back proc
     * 	movzx edx, ax
     * 	shr eax, 16
     * 	inc eax
     * 	movzx ecx, word ptr [bprm+2]
     * 	sub ecx, eax
     * 	jc @@fail
     * 	cmp ecx, ?MAXBP
     * 	jae @@fail
     * 	push ecx
     * 	add ecx, eax
     * 	shl ecx, 4
     * 	dec eax
     * 	shl eax, 4
     * 	add eax, edx
     * 	movzx edx, word ptr [bprm+0]
     * 	add ecx, edx
     * 	cmp eax, ecx
     * 	pop ecx
     * 	jnz @@fail
     * 	mov [v86_bp+ecx*8].BPDYN.dwCallback,0
     * 	ret
     * @@fail:
     * 	stc
     * 	ret
     * 	align 4
     * Free_V86_Call_Back endp
     */
}


// --- eax = word/dword to push

void Simulate_Push(void)
{
    /*
     * Mapped logic from Simulate_Push in VMM.ASM:
     * Simulate_Push proc
     * 	MOVZX edx, word ptr [EBP].Client_Reg_Struc.Client_ESP
     * 	MOVZX ecx, word ptr [EBP].Client_Reg_Struc.Client_SS
     * 	sub dx,2
     * 	SHL ecx, 4
     * 	add ecx, edx
     * 	MOV word ptr [EBP].Client_Reg_Struc.Client_ESP,dx
     * 	mov [ecx],ax
     * 	ret
     * 	align 4
     * Simulate_Push endp
     */
}


// --- out: eax = word which has been poped

void Simulate_Pop(void)
{
    /*
     * Mapped logic from Simulate_Pop in VMM.ASM:
     * Simulate_Pop proc
     * 	MOVZX edx, word ptr [EBP].Client_Reg_Struc.Client_ESP
     * 	MOVZX ecx, word ptr [EBP].Client_Reg_Struc.Client_SS
     * 	SHL ecx, 4
     * 	add ecx, edx
     * 	movzx eax,word ptr [ecx]
     * 	add dx,2
     * 	MOV word ptr [EBP].Client_Reg_Struc.Client_ESP,dx
     * 	ret
     * 	align 4
     * Simulate_Pop endp
     */
}


// --- cx == segment
// --- edx == offset

void Simulate_Far_Jmp(void)
{
    /*
     * Mapped logic from Simulate_Far_Jmp in VMM.ASM:
     * Simulate_Far_Jmp proc
     * 	mov [EBP].Client_Reg_Struc.Client_EIP, edx
     * 	mov word ptr [EBP].Client_Reg_Struc.Client_CS,cx
     * 	ret
     * 	align 4
     * Simulate_Far_Jmp endp
     */
}


// --- eax == number of bytes to pop

void Simulate_Far_Ret_N(void)
{
    /*
     * Mapped logic from Simulate_Far_Ret_N in VMM.ASM:
     * Simulate_Far_Ret_N proc
     * 	push eax
     * 	call [Simulate_Far_Ret]
     * 	pop eax
     * 	add word ptr [EBP].Client_Reg_Struc.Client_ESP, ax
     * 	ret
     * 	align 4
     * Simulate_Far_Ret_N endp
     */
}


// --- cx=segment, edx=offset to call

void Build_Int_Stack_Frame(void)
{
    /*
     * Mapped logic from Build_Int_Stack_Frame in VMM.ASM:
     * Build_Int_Stack_Frame proc
     * 	push ecx
     * 	push edx
     * 	mov eax, [EBP].Client_Reg_Struc.Client_EFlags
     * 	call Simulate_Push
     * 	pop edx
     * 	pop ecx
     * 	call [Simulate_Far_Call]
     * 	and byte ptr [ebp].Client_Reg_Struc.Client_EFlags+1,not (1+2)
     * 	ret
     * 	align 4
     * Build_Int_Stack_Frame endp
     */
}


// --- in: EDI = save buffer
// ---     EBP = client_reg_struc
// --- no return value

void Save_Client_State(void)
{
    /*
     * Mapped logic from Save_Client_State in VMM.ASM:
     * Save_Client_State proc
     * 	push edi
     * 	push esi
     * 	mov esi, ebp
     * 	mov ecx, (size Client_Reg_Struc) / 4
     * 	rep movsd
     * 	pop esi
     * 	pop edi
     * 	ret
     * 	align 4
     * Save_Client_State endp
     */
}


// --- in: ESI = save buffer
// --- no return value

void Restore_Client_State(void)
{
    /*
     * Mapped logic from Restore_Client_State in VMM.ASM:
     * Restore_Client_State proc
     * 	push edi
     * 	push esi
     * 	mov edi, ebp
     * 	mov ecx, (size Client_Reg_Struc) / 4
     * 	rep movsd
     * 	pop esi
     * 	pop edi
     * 	ret
     * 	align 4
     * Restore_Client_State endp
     */
}


// --- Allocate_GDT_Selector( high32 descriptor, low32 descriptor, flags )
// --- returns selector in eax, GDT selector in loword edx, GDT size in hiword edx
// --- high32 desc: [esp+4][8]
// --- low32  desc: [esp+4][4]
// --- flags:       [esp+4][0]

void _Allocate_GDT_Selector(void)
{
    /*
     * Mapped logic from _Allocate_GDT_Selector in VMM.ASM:
     * _Allocate_GDT_Selector proc
     * 	mov ecx, 8
     * 	sub esp, ecx
     * 	sgdt [esp]
     * 	movzx edx, word ptr [esp]
     * 	mov eax, [esp+2]
     * 	inc edx
     * 	add esp, ecx
     * nextdesc:
     * 	cmp ecx, edx
     * 	jae notfound
     * 	cmp byte ptr [eax+ecx+5], 0
     * 	jz found
     * 	add ecx, 8
     * 	jmp nextdesc
     * found:
     * 	push edx
     * 	mov edx, [esp+8][1*4]
     * 	mov [eax+ecx+0], edx
     * 	mov edx, [esp+8][2*4]
     * 	mov [eax+ecx+4], edx
     * 	pop edx
     * 	mov eax, ecx
     * 	ret
     * notfound:
     * 	xor eax, eax
     * 	xor edx, edx
     * 	ret
     * _Allocate_GDT_Selector endp
     */
}


// --- Free_GDT_Selector( wSelector, flags )
// --- todo: limit checks

void _Free_GDT_Selector(void)
{
    /*
     * Mapped logic from _Free_GDT_Selector in VMM.ASM:
     * _Free_GDT_Selector proc
     * 	mov ecx, [esp+4]
     * 	sub esp, 8
     * 	sgdt [esp]
     * 	mov eax, [esp+2]
     * 	add esp, 8
     * 	mov byte ptr [eax+ecx+5],0
     * 	ret
     * _Free_GDT_Selector endp
     */
}


// --- Get_DDB
// --- find a device
// --- eax=device ID, edi=device name (or NULL)
// --- out: device DDB in ECX!
// --- modifies ECX only.

void Get_DDB(void)
{
    /*
     * Mapped logic from Get_DDB in VMM.ASM:
     * Get_DDB proc uses eax edx
     * 	call FindDevice
     * 	mov ecx, eax
     * 	jnc @F
     * 	xor ecx, ecx
     * @@:
     * 	ret
     * Get_DDB endp
     */
}


// -------------------------
// --- the IO-port trapping is implemented very simple
// --- there is an array of 65536 vectors in a 256 kB memory region
// --- for all possible IO ports. On init the memory is uncommitted,
// --- but if someone installs a port trap in a 400h set which has
// --- no other trap yet, a new page will be allocated.

// --- called with port in EDX, value in EAX, type in CX

void New_IO_Handler(void)
{
    /*
     * Mapped logic from New_IO_Handler in VMM.ASM:
     * New_IO_Handler proc
     * 	movzx edx,dx
     * 	mov esi, [IO_Handler_Region]
     * 	cmp dword ptr [esi+edx*4],0
     * 	jz @F
     * 	jmp dword ptr [esi+edx*4]
     * @@:
     * ;--- v5.84: jump to the old trap handler - it will handle
     * ;---        ISA DMA and A20
     * ;	VMMJmp Simulate_IO
     * 	jmp [oldiohandler]
     * 	align 4
     * 
     * New_IO_Handler endp
     */
}


// --- scan Jemm's table of IOTRAPENTRYs
// --- the table defines ranges, but there may be "holes" ( untrapped ports )
// --- so first check if the port is actually really trapped.

// --- v5.84: Jemm's table is no longer imported.
// ---        instead, if port isn't trapped, the old (=Jemm's) handler will be run.

// --- structure of IO_Trap_Table:
// ---   dd ?  ; the default trap handler
// ---   dd ?  ; number of IOTRAPENTRIES in the following table
// ---   IOTRAPENTRIES[]

void ImportJemmIOTraps(void)
{
    /*
     * Mapped logic from ImportJemmIOTraps in VMM.ASM:
     * ImportJemmIOTraps proc uses esi
     * 
     * 	@dprintf ?IODBG, <"ImportJemmIOTraps",10>
     * 	mov esi, [IO_Trap_Table]
     * if 1
     * 	mov eax, offset New_IO_Handler
     * 	xchg eax, [esi]
     * 	mov [oldiohandler], eax
     * 	ret
     * else
     * 	lodsd
     * 	mov [esi-4], offset New_IO_Handler
     * 	mov [oldiohandler],eax
     * 	lodsd
     * 	mov ecx, eax
     * 	jecxz @@done
     * @@nextrange:
     * 	lodsb			; get bStart field
     * 	movzx edx, al
     * 	lodsb			; get bEnd field
     * 	movzx eax, al
     * 	sub eax, edx
     * 	inc eax
     * 	push ecx
     * 	mov ecx, eax
     * 	lodsd			; get dwProc field
     * 	push esi
     * 	mov esi, eax
     * @@nextport:
     * 	push ecx
     * 	push edx
     * 
     * 	mov eax, [dwTSSbase]
     * 	movzx ecx, [eax].TSSSEG.tsOfs
     * 	add eax, ecx
     * 	bt [eax], edx				; port trapped?
     * 	jnc @F
     * 	call CommitTrapPage
     * 	jc @F
     * 	mov eax, [IO_Handler_Region]
     * 	mov [edx*4+eax], esi
     * @@:
     * 	pop edx
     * 	pop ecx
     * 	inc edx			; next port
     * 	loop @@nextport
     * 	pop esi
     * 	pop ecx
     * 	loop @@nextrange
     * @@done:
     * 	ret
     * endif
     * 	align 4
     * 
     * ImportJemmIOTraps endp
     */
}


// --- dx=port

void CommitTrapPage(void)
{
    /*
     * Mapped logic from CommitTrapPage in VMM.ASM:
     * CommitTrapPage proc
     * 	movzx edx, dx
     * 	mov eax, edx
     * 	shr eax, 10		;0-FFFF -> 0-3F
     * 	bt dword ptr [IO_Handler_Pages], eax
     * 	jc @@iscommitted
     * 	push edx
     * 	push PC_FIXED or PC_WRITEABLE
     * 	push 0
     * 	push PD_ZEROINIT
     * 	push 1
     * 	mov ecx, [IO_Handler_Region]
     * 	shr ecx, 12
     * 	add eax, ecx
     * 	push eax
     * 	call _PageCommit
     * 	add esp,5*4
     * 	pop edx
     * 	and eax, eax
     * 	jz @@error
     * 	mov eax, edx
     * 	shr eax, 10
     * 	bts dword ptr [IO_Handler_Pages], eax
     * @@iscommitted:
     * 	clc
     * 	ret
     * @@error:
     * 	stc
     * 	ret
     * CommitTrapPage endp
     */
}


// --- ESI=callback
// --- DX=port

void Install_IO_Handler(void)
{
    /*
     * Mapped logic from Install_IO_Handler in VMM.ASM:
     * Install_IO_Handler proc
     * 	cld
     * 	cmp [IO_Handler_Region],0
     * 	jnz @@isallocated
     * 	@dprintf ?IODBG, <"Install_IO_Handler: reserve 256 kB region",10>
     * 	push edx
     * 	push 0
     * 	push 64
     * 	push PR_SYSTEM
     * 	call _PageReserve	; reserve 64*4=256 kB [ 65536 ports, one dword per port ]
     * 	add esp,3*4
     * 	pop edx
     * 	cmp eax,-1
     * 	jz @@error
     * 	mov [IO_Handler_Region], eax
     * 	@dprintf ?IODBG, <"Install_IO_Handler: region=%X",10>, eax
     * 	push edx
     * 	call ImportJemmIOTraps
     * 	pop edx
     * @@isallocated:
     * 	@dprintf ?IODBG, <"Install_IO_Handler: port=%X",10>, dx
     * 	call CommitTrapPage
     * 	jc @@error
     * 	movzx eax, dx
     * 	shl eax, 2
     * 	add eax, [IO_Handler_Region]
     * 	cmp dword ptr [eax], 0		; port already trapped?
     * 	jnz @@error
     * 	mov [eax], esi
     * 	mov eax, [dwTSSbase]
     * 	movzx ecx, [eax].TSSSEG.tsOfs
     * 	add eax, ecx
     * 	bts [eax],edx				; mark port as "trapped" in IOPB
     * 	clc
     * 	ret
     * @@error:
     * 	@dprintf ?IODBG, <"Install_IO_Handler: trap port=%X failed",10>, dx
     * 	stc
     * 	ret
     * 	align 4
     * Install_IO_Handler endp
     */
}


// --- when untrapping a port, check if it's a port trapped by Jemm
// --- if yes, it must not be reset in the IOPB.
// --- in: edx=port
// --- out: C if port is trapped by Jemm
// --- register edx must be preserved

void DefaultTrappedPort(void)
{
    /*
     * Mapped logic from DefaultTrappedPort in VMM.ASM:
     * DefaultTrappedPort proc uses esi ebx
     * 	mov esi, [IO_Trap_Table]
     * 	add esi, 4
     * 	lodsd
     * 	mov ecx, eax
     * 	jecxz done
     * nextrange:
     * 	lodsb			; get bStart field
     * 	movzx ebx, al
     * 	lodsb			; get bEnd field
     * 	movzx eax, al
     * 	add esi, 4		; skip proc field
     * 	cmp edx, ebx
     * 	jb @F
     * 	cmp edx, eax
     * 	jbe found
     * @@:
     * 	loop nextrange
     * done:
     * 	clc
     * 	ret
     * found:
     * 	stc
     * 	ret
     * DefaultTrappedPort endp
     */
}


// --- untrap port, dx=port

void Remove_IO_Handler(void)
{
    /*
     * Mapped logic from Remove_IO_Handler in VMM.ASM:
     * Remove_IO_Handler proc
     * 	@dprintf ?IODBG, <"Remove_IO_Handler: untrap port=%X",10>, dx
     * 	xor ecx, ecx
     * 	cmp [IO_Handler_Region],ecx
     * 	jz @@error
     * 	movzx eax, dx
     * 	shr eax, 10		;0-FFFF -> 0-3F
     * 	bt dword ptr [IO_Handler_Pages], eax
     * 	jnc @@error
     * 	movzx eax, dx
     * 	shl eax, 2
     * 	add eax, [IO_Handler_Region]
     * 	cmp [eax],ecx
     * 	jz @@error
     * 	mov [eax], ecx
     * 	call DefaultTrappedPort
     * 	jc @F
     * 	mov eax, [dwTSSbase]
     * 	movzx ecx, [eax].TSSSEG.tsOfs
     * 	add eax, ecx
     * 	btc [eax],edx
     * @@:
     * 	clc
     * 	ret
     * @@error:
     * 	@dprintf ?IODBG, <"Remove_IO_Handler: untrap port=%X failed",10>, dx
     * 	stc
     * 	ret
     * 	align 4
     * Remove_IO_Handler endp
     */
}


// --- not implemented
// --- API?

void Install_Mult_IO_Handlers(void)
{
    /*
     * Mapped logic from Install_Mult_IO_Handlers in VMM.ASM:
     * Install_Mult_IO_Handlers proc
     * 	stc
     * 	ret
     * 	align 4
     * Install_Mult_IO_Handlers endp
     */
}


// --- not implemented
// --- API?

void Remove_Mult_IO_Handlers(void)
{
    /*
     * Mapped logic from Remove_Mult_IO_Handlers in VMM.ASM:
     * Remove_Mult_IO_Handlers proc
     * 	stc
     * 	ret
     * 	align 4
     * Remove_Mult_IO_Handlers endp
     */
}


// --- interrupt hooking

// --- get vector EAX in CX:EDX

void Get_V86_Int_Vector(void)
{
    /*
     * Mapped logic from Get_V86_Int_Vector in VMM.ASM:
     * Get_V86_Int_Vector proc
     * 	cmp eax,256
     * 	jae @@error
     * 	movzx edx,word ptr [eax*4+0]
     * 	mov cx,word ptr [eax*4+2]
     * 	clc
     * 	ret
     * @@error:
     * 	stc
     * 	ret
     * 	align 4
     * Get_V86_Int_Vector endp
     */
}


// --- set vector EAX in CX:EDX

void Set_V86_Int_Vector(void)
{
    /*
     * Mapped logic from Set_V86_Int_Vector in VMM.ASM:
     * Set_V86_Int_Vector proc
     * 	cmp eax,256
     * 	jae @@error
     * 	mov word ptr [eax*4+0],dx
     * 	mov word ptr [eax*4+2],cx
     * 	clc
     * 	ret
     * @@error:
     * 	stc
     * 	ret
     * 	align 4
     * Set_V86_Int_Vector endp
     */
}


// --- the Get_PM/Set_PM API is supposed to set the vectors of the current VM
// --- if it's in protected-mode. So not really needed by Jemm.

// --- in: eax=int#
// --- out: cx=segment
// ---     edx=offset

void Get_PM_Int_Vector(void)
{
    /*
     * Mapped logic from Get_PM_Int_Vector in VMM.ASM:
     * Get_PM_Int_Vector proc
     * 	stc
     * 	ret
     * 	align 4
     * Get_PM_Int_Vector endp
     */
}


// --- in: eax=int#
// ---      cx=segment
// ---     edx=offset

void Set_PM_Int_Vector(void)
{
    /*
     * Mapped logic from Set_PM_Int_Vector in VMM.ASM:
     * Set_PM_Int_Vector proc
     * 	stc
     * 	ret
     * 	align 4
     * Set_PM_Int_Vector endp
     */
}


// --- hook v86 interrupt chain
// --- eax = int#
// --- esi = hook proc

// --- v5.85:
// ---  IDT is no longer touched
// ---  instead the new ppV86Hooks variable is used to set V86Hooks
// ---  this makes the thunk code generation obsolete.
// --- sizeof HOOKSTR is now 4 (256*4=1024), the following 32 bytes are used to store
// --- the VME bitmap.


void Hook_V86_Int_Chain(void)
{
    /*
     * Mapped logic from Hook_V86_Int_Chain in VMM.ASM:
     * Hook_V86_Int_Chain proc
     * 	cmp eax,100h
     * 	jnc @@error
     * 
     * ;--- the "hook" page already allocated?
     * 
     * 	cmp [lpV86Hooks],0
     * 	jnz @@isallocated
     * 	push eax
     * 	push 0
     * 	push 1
     * 	push PR_SYSTEM
     * 	call _PageReserve
     * 	add esp,3*4
     * 	cmp eax, -1
     * 	jz @@error2
     * 	push eax
     * 	push PC_FIXED or PC_WRITEABLE
     * 	push 0
     * 	push PD_ZEROINIT
     * 	push 1
     * 	shr eax, 12
     * 	push eax
     * 	call _PageCommit
     * 	add esp,5*4
     * 	pop ecx
     * 	and eax, eax
     * 	jz @@error2
     * 	mov [lpV86Hooks], ecx
     * 
     * ;--- v5.85: set the pV86Hooks variable in Jemm32
     * 	mov eax, [ppV86Hooks]
     * 	mov [eax+0], ecx
     * 	mov [eax+4], offset CallV86Hooks
     * 
     * 	pop eax
     * @@isallocated:
     * 	mov edx,[lpV86Hooks]
     * 	cmp [edx+eax*4].HOOKV86.dwHook,0	; 4=sizeof HOOKV86 struct
     * 	jz @@newhook
     * 
     * ;--- chain with old hook proc
     * 
     * 	mov ecx,[edx+eax*4].HOOKV86.dwHook
     * 	mov [edx+eax*4].HOOKV86.dwHook, esi
     * 	mov eax, [esi-4]
     * 	mov [eax], ecx
     * 	jmp @@done
     * @@error2:
     * 	pop eax
     * @@error:
     * 	@dprintf ?V86HOOKDBG, <"Hook_V86_Int_chain: exit error",10>
     * 	stc
     * 	ret
     * @@newhook:
     * 	@dprintf ?V86HOOKDBG, <"Hook_V86_Int_chain: new hook, eax=%X",10>, eax
     * 	mov [edx+eax*4].HOOKV86.dwHook, esi
     * 
     * ;--- set the bit in TSS interrupt redirection bitmap for VME.
     * ;--- store the old state in [edx+1024] (behind the HOOKSTR items).
     * 
     * ;--- For hardware interrupts, this isn't needed, but shouldn't harm...
     * if 0
     * 	cmp al, 8
     * 	jb @F
     * 	cmp al, 10h
     * 	jb @@done
     * 	cmp al, 70h
     * 	jb @F
     * 	cmp al, 78h
     * 	jb @@done
     * @@:
     * endif
     * 
     * 	push ebx
     * 	push esi
     * if 0
     * 	mov ecx, [esi-4]
     * 	mov dword ptr [ecx], 0
     * endif
     * 
     * 	@dprintf ?V86HOOKDBG, <"Hook_V86_Int_chain: modifying Int bitmap, eax=%X",10>, eax
     * 	mov ebx, [dwTSSbase]
     * 	movzx ecx, [ebx].TSSSEG.tsOfs
     * 	add ebx, ecx
     * 	sub ebx, 32    ;=256/8
     * 	bts [ebx], eax
     * 	jc @F
     * 	btr [edx+1024], eax
     * 	jmp bitset
     * @@:
     * 	bts [edx+1024], eax
     * bitset:
     * 	pop esi
     * 	pop ebx
     * @@done:
     * 	@dprintf ?V86HOOKDBG, <"Hook_V86_Int_chain: exit ok",10>
     * 	clc
     * 	ret
     * 	align 4
     * Hook_V86_Int_Chain endp
     */
}


// --- eax = int#
// --- esi = hook proc

void Unhook_V86_Int_Chain(void)
{
    /*
     * Mapped logic from Unhook_V86_Int_Chain in VMM.ASM:
     * Unhook_V86_Int_Chain proc
     * 	@dprintf ?V86HOOKDBG, <"Unhook_V86_Int_Chain: int=%X",10>, eax
     * 	cmp eax,100h
     * 	jnc @@error
     * 	mov ecx,[lpV86Hooks]
     * 	jecxz @@error
     * 	mov edx,[ecx+eax*4].HOOKV86.dwHook
     * 	and edx,edx
     * 	jz @@error
     * 	xor ecx,ecx
     * @@nextitem:
     * 	cmp edx,esi
     * 	jz @@found
     * 	mov ecx,[edx-4]
     * 	mov edx,[ecx]
     * 	and edx,edx
     * 	jnz @@nextitem
     * @@error:
     * 	stc
     * 	ret
     * @@found:
     * 	mov edx,[edx-4]
     * 	mov edx,[edx]
     * 	and ecx,ecx
     * 	jnz @@notfirst
     * 	mov ecx,[lpV86Hooks]
     * 	and edx, edx
     * 	jz @@islast
     * 	lea ecx,[ecx+eax*4]
     * @@notfirst:
     * 	mov [ecx],edx
     * 	ret
     * @@islast:
     * 	mov [ecx+eax*4].HOOKV86.dwHook,edx
     * 	bt [ecx+1024], eax
     * 	jc @@nobitres
     * 	push ebx
     * 	mov ebx, [dwTSSbase]
     * 	movzx edx, [ebx].TSSSEG.tsOfs
     * 	add ebx, edx
     * 	sub ebx, 32
     * 	btr [ebx],eax
     * 	pop ebx
     * @@nobitres:
     * 	clc
     * 	ret
     * 	align 4
     * Unhook_V86_Int_Chain endp
     */
}


// --- call hooks for an int
// --- must have been ensured that there are hooks!
// --- a hook proc is called with:
// --- eax = int#
// --- ebp = client
// --- ecx = hook proc
// --- out: NC if interrupt has been serviced.

void CallV86Hooks(void)
{
    /*
     * Mapped logic from CallV86Hooks in VMM.ASM:
     * CallV86Hooks proc
     * @@nexthook:
     * 	push ecx
     * 	push eax
     * 	call ecx
     * 	pop eax
     * 	pop ecx
     * 	jnc @@done
     * 	mov edx,[ecx-4]
     * 	mov ecx,[edx]
     * 	and ecx, ecx
     * 	jnz @@nexthook
     * ifdef _DEBUG
     * 	cmp eax, 2Fh
     * 	jz @F
     * 	@dprintf ?V86HOOKDBG, <"CallV86Hooks: no hook proc handled int %X",10>, eax
     * @@:
     * endif
     * 	stc
     * @@done:
     * 	ret
     * 	align 4
     * CallV86Hooks endp
     */
}


// --- add a v86 fault hook
// --- eax=fault#
// --- esi=fault proc
// --- out: C if not installed
// ---  NC if installed, then ESI=previous handler (win31 compat)

void Hook_V86_Fault(void)
{
    /*
     * Mapped logic from Hook_V86_Fault in VMM.ASM:
     * Hook_V86_Fault proc
     * 	@dprintf ?V86HOOKDBG, <"Hook_V86_Fault: int=%X",10>, eax
     * 	cmp eax,32
     * 	cmc
     * 	jb exit
     * 	mov edx,[esi-4]
     * 	xchg esi,[eax*4+v86faulthooks]
     * 	mov [edx],esi
     * 	cmp esi,0
     * 	jnz exit
     * 	mov eax,[lpV86Faults]
     * 	mov [edx],eax
     * exit:
     * 	ret
     * Hook_V86_Fault endp
     */
}


// --- remove a v86 fault hook
// --- eax=fault#
// --- esi=fault proc
// --- out: C if not unhooked (invalid fault in eax, esi not a hook proc, hook proc not found,...)

void Unhook_V86_Fault(void)
{
    /*
     * Mapped logic from Unhook_V86_Fault in VMM.ASM:
     * Unhook_V86_Fault proc
     * 	@dprintf ?V86HOOKDBG, <"Unhook_V86_Fault: int=%X",10>, eax
     * 	cmp eax,32
     * 	cmc
     * 	jb exit
     * 	xor ecx, ecx
     * 	mov edx, [eax*4+v86faulthooks]
     * 	.while edx
     * 		.if edx == esi
     * 			mov edx,[esi-4]
     * 			mov edx,[edx]
     * 			.if !ecx
     * 				mov [eax*4+v86faulthooks], edx
     * 			.else
     * 				mov eax,[ecx-4]
     * 				mov [eax], edx
     * 			.endif
     * 			.break
     * 		.endif
     * 		mov ecx, edx
     * 		mov edx, [edx-4]
     * 		mov edx, [edx]
     * 	.endw
     * 	cmp edx,1
     * exit:
     * 	ret
     * Unhook_V86_Fault endp
     */
}


// --- call v86 fault hookers
// --- in: ebp=client_reg_struct
// ---    (ebx=vm handle in windows)
// ---     eax=fault#
// --- the fault handler may either
// --- + pass the fault to the previous handler (all regs preserved then) OR
// --- + do a RET (all GPRs excepts EBP/ESP may be modified)

void V86Faults(void)
{
    /*
     * Mapped logic from V86Faults in VMM.ASM:
     * V86Faults proc
     * 	cmp [eax*4+v86faulthooks],0
     * 	jz @F
     * 	jmp [eax*4+v86faulthooks]
     * @@:
     * 	jmp [lpV86Faults]
     * V86Faults endp
     */
}


// --- the "PM" functions are pretty useless, since Jemm has no support
// --- for protected-mode apps.

void Hook_PM_Fault(void)
{
    /*
     * Mapped logic from Hook_PM_Fault in VMM.ASM:
     * Hook_PM_Fault proc
     * Hook_PM_Fault endp
     */
}

void Unhook_PM_Fault(void)
{
    /*
     * Mapped logic from Unhook_PM_Fault in VMM.ASM:
     * Unhook_PM_Fault proc
     * Unhook_PM_Fault endp
     */
}


// stc
// ret

// align 4

// --- _PhysIntoV86(physpg, VM, VMLinPgNum, nPages, flags)
// --- map n physical pages into V86

#define physpg <esp+1*4>
#define VM <esp+2*4>
#define VMLinPgNum <esp+3*4>
#define nPages <esp+4*4>
#define flags <esp+5*4>

void _PhysIntoV86(void)
{
    /*
     * Mapped logic from _PhysIntoV86 in VMM.ASM:
     * _PhysIntoV86 proc
     * 	mov ecx, [nPages]
     * 	push esi
     * 	push edi
     * 	mov esi, 8[physpg]
     * 	mov edi, 8[VMLinPgNum]
     * 	mov eax, esi
     * 	mov edx, edi
     * 	cmp eax, 100h
     * 	jbe @@done
     * 	lea edi, [edi*4 + (?PAGEMAP shl 22)]
     * 	cld
     * 	shl esi,12
     * @@nextitem:
     * 	mov eax, esi
     * 	or eax, PTF_PRESENT or PTF_RW or PTF_USER
     * 	stosd
     * 	add esi,1000h
     * 	loop @@nextitem
     * 	mov eax,cr3
     * 	mov cr3,eax
     * @@done:
     * 	pop edi
     * 	pop esi
     * 	ret
     * 	align 4
     * _PhysIntoV86 endp
     */
}


// --- _LinMapIntoV86(LinPgNum, VM, VMLinPgNum, nPages, flags)
// --- map n linear pages into V86

#define LinPgNum <esp+1*4>
#define VM <esp+2*4>
#define VMLinPgNum <esp+3*4>
#define nPages <esp+4*4>
#define flags <esp+5*4>

void _LinMapIntoV86(void)
{
    /*
     * Mapped logic from _LinMapIntoV86 in VMM.ASM:
     * _LinMapIntoV86 proc
     * 	mov ecx, [nPages]
     * 	push esi
     * 	push edi
     * 	mov esi, 8[LinPgNum]
     * 	mov edi, 8[VMLinPgNum]
     * 	mov eax, esi
     * 	mov edx, edi
     * 	cmp eax, 100h
     * 	jbe @@done
     * 	lea esi, [esi*4 + (?PAGEMAP shl 22)]
     * 	lea edi, [edi*4 + (?PAGEMAP shl 22)]
     * 	cld
     * 	rep movsd
     * 	mov eax,cr3
     * 	mov cr3,eax
     * @@done:
     * 	pop edi
     * 	pop esi
     * 	ret
     * 	align 4
     * _LinMapIntoV86 endp
     */
}


// --- page memory

// --- bits in PTE:
// --- 200h = begin of a memory object
// --- 400h = PTE is a mapped phys page (don't free it!)
// --- 800h = not used

void GetFreePages(void)
{
    /*
     * Mapped logic from GetFreePages in VMM.ASM:
     * GetFreePages proc
     * 	push edi
     * 	push esi
     * 	push ecx
     * 	mov eax, [VCPI_Call_Table]
     * 	call dword ptr [eax+3*4]	;VCPI_GetFreePages
     * 	pop ecx
     * 	pop esi
     * 	pop edi
     * 	ret
     * 	align 4
     * GetFreePages endp
     */
}


void AllocPage(void)
{
    /*
     * Mapped logic from AllocPage in VMM.ASM:
     * AllocPage proc
     * 	push edi
     * 	push esi
     * 	push ecx
     * 	mov eax, [VCPI_Call_Table]
     * 	call dword ptr [eax+4*4]
     * 	pop ecx
     * 	pop esi
     * 	pop edi
     * 	ret
     * 	align 4
     * AllocPage endp
     */
}


void FreePage(void)
{
    /*
     * Mapped logic from FreePage in VMM.ASM:
     * FreePage proc
     * 	push edi
     * 	push esi
     * 	push ecx
     * 	mov eax, [VCPI_Call_Table]
     * 	call dword ptr [eax+5*4]
     * 	pop ecx
     * 	pop esi
     * 	pop edi
     * 	ret
     * 	align 4
     * FreePage endp
     */
}


// --- esi=V86TOPM struct
// --- ebp=client reg struc
// --- both structures have to be copied into first MB

void V86ToPM(void)
{
    /*
     * Mapped logic from V86ToPM in VMM.ASM:
     * V86ToPM proc public
     * 	mov edi, ?BASE+10h
     * 	mov ecx, 6	; size of VCPI_V86toPM struct
     * 	cld
     * 	rep movsd
     * 	mov esi, ebp
     * 	mov cl, sizeof Client_Reg_Struc shr 2
     * 	rep movsd
     * 	mov esi, ?BASE+10h
     * 	lea edi, [esi+6*4]
     * 	mov eax, [VCPI_Call_Table]
     * 	jmp dword ptr [eax+12*4]	; VCPI_V86ToPM
     * 	align 4
     * V86ToPM endp
     */
}


// --- to simplify memory management,
// --- a "mapping" page table is maintained which
// --- maps all page tables in linear address space
// --- at address ?PAGEMAP << 22 (currently 3FEh << 22 = FF800000h)

void InitMapPage(void)
{
    /*
     * Mapped logic from InitMapPage in VMM.ASM:
     * InitMapPage proc
     * 	pushad
     * 	call AllocPage
     * 	cmp ah,0
     * 	jnz @@error
     * 	or edx,3							;set P, R/W, S
     * 	mov ds:[?PAGEDIR+?PAGEMAP*4],edx	;set the PDE
     * 	mov ds:[?PAGEDIR+?PAGETABSYS+0FF8h],edx  ;map the table in scratch region
     * 
     * 	call AllocPage			;alloc a second page (scratch PTE)
     * 	cmp ah,0
     * 	jnz @@error
     * 	or edx,3
     * 	mov [ScratchPTE],edx
     * 
     * 	mov eax, cr3			;flush TLB
     * 	mov cr3, eax
     * 
     * 
     * 	mov eax, edx			;fill mapping table PDE with scratch PTE
     * 	mov edi, ?SYSBASE + 3FE000h
     * 	cld
     * 	mov ecx, 1000h/4
     * 	push edi
     * 	rep stosd
     * 	pop edi
     * 
     * 	mov eax,ds:[?PAGEDIR+0] 			;copy PDE page table 0
     * 	mov [edi+0],eax
     * 	mov eax,ds:[?PAGEDIR+(?SYSBASE shr 20)]		;copy PDE "jemm"
     * 	mov [edi+(?SYSBASE shr 20)],eax
     * 	mov eax,ds:[?PAGEDIR+(?SYSTEMADDR shr 20)]	;copy PDE "jload"
     * 	mov [edi+(?SYSTEMADDR shr 20)],eax
     * 	mov eax,ds:[?PAGEDIR+?PAGEMAP*4]	;copy PDE "pagemap"
     * 	mov [edi+?PAGEMAP*4],eax
     * 
     * ;--- now move scratch PTE to scratch region and clear it
     * 
     * 	mov dword ptr ds:[?PAGEDIR+?PAGETABSYS+0FF8h],edx
     * 
     * 	mov eax, cr3
     * 	mov cr3, eax
     * 
     * 	mov ecx, 1000h/4
     * 	xor eax, eax
     * 	rep stosd
     * 	mov dword ptr ds:[?PAGEDIR+?PAGETABSYS+0FF8h],0
     * 
     * 	popad
     * 	clc
     * 	ret
     * @@error:
     * 	popad
     * 	xor eax, eax
     * 	stc
     * 	ret
     * 	align 4
     * InitMapPage endp
     */
}


// --- _PageFree(hMem, flags)
// --- this function has no size parameter
// --- _page: linear address of block
// --- flags: 0 or PR_STATIC

#define _page <esp+1*4+4>
#define flags <esp+2*4+4>

void _PageFree(void)
{
    /*
     * Mapped logic from _PageFree in VMM.ASM:
     * _PageFree proc public
     * 
     * 	push esi
     * 	mov esi, [_page]
     * 	shr esi, 12
     * 	lea esi, [esi*4 + ?PAGEMAP shl 22]
     * 	lodsd
     * 	test ah,2	;begin of a block?
     * 	jz	@@error
     * @@nextitem:
     * 	test al,1	;committed page?
     * 	jz @F
     * 	test ah,4	;mapped page?
     * 	jnz @F
     * 	mov edx, eax
     * 	and dx,0F000h
     * 	call FreePage
     * @@:
     * 	mov dword ptr [esi-4],0
     * ;	cmp esi, (?PAGEMAP + 400000h) shl 22	;end of page map?
     * 	cmp esi, (?PAGEMAP shl 22 ) + 400000h	;end of page map?
     * 	jz @@done
     * 	lodsd
     * 	and eax, eax
     * 	jz @@done
     * 	test ah,2
     * 	jz @@nextitem
     * @@done:
     * 	mov eax,cr3
     * 	mov cr3,eax
     * 	push 1
     * 	pop eax
     * 	pop esi
     * 	ret
     * @@error:
     * 	pop esi
     * 	xor eax,eax
     * 	ret
     * 	align 4
     * _PageFree endp
     */
}


// --- _PageReserve(page, npages, flags)
// --- page = specific page or
// --- PR_PRIVATE (80000400h), PR_SHARED (80060000h), PR_SYSTEM (80080000h)
// --- flags: ???
// --- out: linear address in EAX or -1
// --- "private" starts at 400000h
// --- "shared" possibly at F8000000h downwards? (already used by 4MB page heap!)
// --- system at ?SYSTEMADDR (usually F8400000h)
// --- the first PTE will have the

#define _page <esp+1*4>
#define npages <esp+2*4>
#define flags <esp+3*4>

void _PageReserve(void)
{
    /*
     * Mapped logic from _PageReserve in VMM.ASM:
     * _PageReserve proc public
     * 	cmp dword ptr [npages],0
     * 	jz @@error
     * 	mov eax, ?SYSTEMADDR shr 10
     * 	mov ecx, 400000h/4 - (?SYSTEMADDR shr 12)
     * 	cmp dword ptr [_page],PR_SYSTEM
     * 	jz @F
     * 	cmp dword ptr [_page],PR_PRIVATE   ;currently only unspecified regions are supported
     * 	jnz @@error
     * 	mov eax, ?PRIVATEADDR shr 10
     * 	mov ecx, (?SYSTEMADDR - ?PRIVATEADDR) shr 12
     * @@:
     * 	mov edx, [npages]
     * 	dec edx
     * 	push edi
     * 	push ebx
     * 	lea edi, [eax + (?PAGEMAP shl 22)]
     * 	cld
     * 
     * ;--- the mapping region at FF800000-FFBFFFFF contains no
     * ;--- invalid addresses. A 00000000 entry means it is either an
     * ;--- unused PTE of an partially used PT, or it is a still unused PT.
     * ;--- so all what has to be done is scanning the region for a sequence
     * ;--- of 00000000 dwords.
     * 
     * @@continuescan:
     * 	xor eax, eax
     * 	repnz scasd
     * 	jnz @@error2
     * 	cmp ecx, edx	;enough potential entries left?
     * 	jc @@error2
     * 	lea ebx, [edi-4]
     * 	and edx, edx	;size just one page?
     * 	jz @@found
     * 	push ecx
     * 	mov ecx, edx
     * 	repz scasd
     * 	pop ecx
     * 	jz @@found
     * 	mov eax, edx
     * 	sub eax, ecx
     * 	sub ecx, eax
     * 	jmp @@continuescan
     * @@found:
     * 
     * ;--- now backup the PDEs and PTEs with true RAM
     * 
     * 	mov ecx, edx
     * 	inc ecx
     * 	mov edi, ebx
     * 	.while (ecx)
     * 		mov eax, edi
     * 		sub eax, ?PAGEMAP shl 22
     * 		shr eax, 10
     * 		cmp dword ptr ds:[?PAGEDIR + EAX],0	;does a PDE exist?
     * 		jnz @F
     * 		push eax
     * 		call AllocPage
     * 		cmp ah,0
     * 		pop eax
     * 		jnz @@error
     * 		or edx,7
     * 		mov ds:[?PAGEDIR + EAX],edx
     * 		and dl, not 4
     * 		mov ds:[(?PAGEMAP shl 22) + (?PAGEMAP shl 12) + EAX],edx
     * 		push edi
     * 		push ecx
     * 		mov edi, eax
     * 		shl edi, 10
     * 		add edi, ?PAGEMAP shl 22
     * 		mov eax, cr3
     * 		mov cr3, eax
     * 		xor eax, eax
     * 		mov ecx, 1000h/4
     * 		rep stosd
     * 		pop ecx
     * 		pop edi
     * @@:
     * 		mov eax, 6		;set USER + R/W + NP
     * 		cmp edi,ebx		;first PTE?
     * 		setz ah
     * 		shl ah,1		;set first "available" bit in PTE
     * 		stosd
     * 		dec ecx
     * 	.endw
     * 	mov eax, ebx
     * 	sub eax, ?PAGEMAP shl 22
     * 	shl eax, 10
     * 	pop ebx
     * 	pop edi
     * 	ret
     * @@error2:
     * 	pop ebx
     * 	pop edi
     * @@error:
     * 	or eax,-1
     * 	ret
     * 	align 4
     * _PageReserve endp
     */
}


// --- _PageCommit(page, npages, hpd, pagerdata, flags)
// --- returns eax != 0 if ok, eax == 0 on failure
// --- page: linear page number
// --- hpd: PD_FIXED | PD_FIXEDZERO
// --- pagerdata: must be ZERO
// --- flags: PC_FIXED, PC_LOCKED, PC_USER, PC_WRITEABLE
// --- if PC_FIXED or PC_LOCKED is set, none of the pages must be committed

#define _page <esp+1*4+8>
#define npages <esp+2*4+8>
#define hpd <esp+3*4+8>
#define pagerdata <esp+4*4+8>
#define flags <esp+5*4+8>

void _PageCommit(void)
{
    /*
     * Mapped logic from _PageCommit in VMM.ASM:
     * _PageCommit proc public
     * 	push esi
     * 	push ebx
     * 	mov esi, [_page]
     * 	shl esi, 2
     * 	add esi, ?PAGEMAP shl 22
     * 	mov ebx, [flags]
     * 	mov ecx, [npages]
     * 	xor edx, edx
     * 	cld
     * 	and ecx, ecx
     * 	jz @@error
     * @@nextitem:
     * 	lodsd
     * 	and al,al			;valid address space?
     * 	jz @@error
     * 	test al,P_PRES		;page committed?
     * 	setz al
     * 	test bl,PC_FIXED or PC_LOCKED
     * 	jz @F
     * 	cmp al,0			;no committed page allowed
     * 	jz @@error
     * @@:
     * 	movzx eax,al
     * 	add edx, eax
     * 	loop @@nextitem
     * 
     * 	push edx
     * 	call GetFreePages
     * 	cmp ah,0
     * 	pop eax
     * 	jnz @@error
     * 	cmp edx, eax
     * 	jc	@@error 				; not enough free pages
     * 
     * 	mov ecx, [npages]
     * 	mov esi, [_page]
     * 	shl esi, 2
     * 	add esi, ?PAGEMAP shl 22
     * 	mov ebx, [flags]
     * 	shr ebx, 16 				; move PC_WRITEABLE and PC_USER to BL
     * 	or bl,P_PRES				; also set the PRESENT bit
     * @@nextitem2:
     * 	lodsd
     * 	test al,P_PRES
     * 	jnz @F
     * 	call AllocPage
     * 	cmp ah,0
     * 	jnz @@error
     * 	mov eax,[esi-4]
     * 	mov al,0
     * 	or al, bl
     * 	or eax, edx
     * 	mov [esi-4], eax
     * 	test byte ptr [hpd],1	;zero init?
     * 	jz @F
     * 	push ecx
     * 	push edi
     * 	mov edi, esi
     * 	sub edi, 4 + (?PAGEMAP shl 22)
     * 	shl edi, 10
     * 	mov ecx,1000h/4
     * 	xor eax, eax
     * 	rep stosd
     * 	pop edi
     * 	pop ecx
     * @@:
     * 	loop @@nextitem2
     * 	pop ebx
     * 	pop esi
     * 	push 1
     * 	pop eax
     * 	ret
     * @@error:
     * 	pop ebx
     * 	pop esi
     * 	xor eax, eax
     * 	ret
     * 	align 4
     * _PageCommit endp
     */
}


// --- _PageDecommit(page, npages, flags)
// --- page: linear page number
// --- flags: must be zero
// --- returns: eax == 0 on failure

#define _page <esp+1*4>
#define npages <esp+2*4>
#define flags <esp+3*4>

void _PageDecommit(void)
{
    /*
     * Mapped logic from _PageDecommit in VMM.ASM:
     * _PageDecommit proc public
     * 
     * 	mov ecx, [npages]
     * 	jecxz @@error
     * 	push esi
     * 	mov esi, 4[_page]
     * 	shl esi, 2
     * 	add esi, ?PAGEMAP shl 22
     * @@nextitem:
     * 	lodsd
     * 	mov edx, eax
     * 	test al,P_PRES		;committed page?
     * 	jz @@notcommitted
     * 	test ah,4			;mapped page?
     * 	jnz @F
     * 	mov dl,0
     * 	and dh,0F0h
     * 	push eax
     * 	call FreePage
     * 	pop edx
     * @@:
     * 	and edx, 0200h		;preserve the "begin block" bit
     * 	or dl,P_WRITE or P_USER
     * 	mov [esi-4],edx
     * @@notcommitted:
     * 	loop @@nextitem
     * 	pop esi
     * 	mov ecx,cr3
     * 	mov cr3,ecx
     * 	ret
     * @@error:
     * 	xor eax, eax
     * 	ret
     * 	align 4
     * _PageDecommit endp
     */
}


// --- _PageCommitPhys(page, npages, physpg, flags)
// --- page: linear page number
// --- npages:# of pages (region must NOT be committed)
// --- physpg: physical page # to commit.
// --- flags: use PC_INCR to map a contiguous phys region;
// --- PC_USER and PC_WRITEABLE also relevant;
// --- v5.83: added PC_CACHEWT and PC_CACHEDIS;
// --- returns: eax == 0 on failure

#define _page <esp+1*4>
#define npages <esp+2*4>
#define physpg <esp+3*4>
#define flags <esp+4*4>

void _PageCommitPhys(void)
{
    /*
     * Mapped logic from _PageCommitPhys in VMM.ASM:
     * _PageCommitPhys proc
     * 
     * 	mov ecx, [npages]
     * 	jecxz @@error
     * 	push esi
     * 	mov esi, 4[_page]
     * 	shl esi, 2
     * 	add esi, ?PAGEMAP shl 22
     * 	mov edx, esi
     * 	cld
     * @@nextitem:
     * 	lodsd
     * 	cmp al,0			;valid PTE?
     * 	jz @@error2
     * 	test al,P_PRES		;all PTEs must be non-committed!
     * 	jnz @@error2
     * 	loop @@nextitem
     * 	push edi
     * 	push ebx
     * 	mov esi, edx
     * 	mov ecx, 12[npages]
     * 	mov ebx, 12[physpg]
     * 	mov edi, 12[flags]
     * 	xor edx, edx
     * 	test edi,PC_INCR
     * 	jz @F
     * 	mov dh,10h
     * @@:
     * 	shr edi, 16
     * 	and edi, (PC_WRITEABLE or PC_USER or PC_CACHEWT or PC_CACHEDIS) shr 16
     * 	or edi, 401h		;mark this PTE as a "mapped" page (400h)
     * 	shl ebx, 12
     * @@nextitem2:
     * 	lodsd
     * 	and ah,0Fh
     * 	movzx eax,ax
     * 	or eax, edi
     * 	or eax, ebx
     * 	mov [esi-4], eax
     * 	add ebx, edx
     * 	loop @@nextitem2
     * 	pop ebx
     * 	pop edi
     * 	pop esi
     * 	ret
     * @@error2:
     * 	pop esi
     * @@error:
     * 	xor eax, eax
     * 	ret
     * 	align 4
     * _PageCommitPhys endp
     */
}


// --- notifications from Jemm (SYSTEM_EXIT)
// --- eax, ebx, esi, edi, ebp must not be changed.

void call_control_procs(void)
{
    /*
     * Mapped logic from call_control_procs in VMM.ASM:
     * call_control_procs proc
     * 	mov edx,offset ddb_list
     * nextdevice:
     * 	mov ecx, [edx].VxD_Desc_Block.DDB_Control_Proc
     * 	jecxz @F
     * 	pushad
     * 	call ecx
     * 	popad
     * @@:
     * 	mov edx,[edx].VxD_Desc_Block.DDB_Next
     * 	and edx, edx
     * 	jnz nextdevice
     * 	ret
     * call_control_procs endp
     */
}


// --- in: eax->ID to find, if 000, edi=name
// --- out: eax = hModule

void FindDevice(void)
{
    /*
     * Mapped logic from FindDevice in VMM.ASM:
     * FindDevice proc public
     * 	mov edx,offset ddb_list
     * @@:
     * 	cmp ax, 0
     * 	jz cmpname
     * 	cmp ax, [edx].VxD_Desc_Block.DDB_Req_Device_Number
     * 	jz found
     * nextdev:
     * 	mov edx,[edx].VxD_Desc_Block.DDB_Next
     * 	and edx, edx
     * 	jnz @B
     * 	stc
     * 	ret
     * cmpname:
     * 	pushad
     * 	lea esi, [edx].VxD_Desc_Block.DDB_Name
     * 	mov ecx, sizeof VxD_Desc_Block.DDB_Name
     * 	repz cmpsb
     * 	popad
     * 	jnz nextdev
     * found:
     * 	mov eax, edx
     * 	ret
     * 	align 4
     * 
     * FindDevice endp
     */
}


// --- esi -> DDB
// --- C set on failure
// --- new v5.74: alloc v86 breakpoint if v86 api is to be installed

void VMM_Add_DDB(void)
{
    /*
     * Mapped logic from VMM_Add_DDB in VMM.ASM:
     * VMM_Add_DDB proc public
     * 	movzx eax, [esi].VxD_Desc_Block.DDB_Req_Device_Number
     * 	@dprintf ?JLMDBG, <"VMM_Add_DBB, ID=%X",10>, eax
     * 	mov edx, offset ddb_list
     * @@nextitem:
     * 	and eax,eax
     * 	jz @F
     * 	cmp ax, [edx].VxD_Desc_Block.DDB_Req_Device_Number
     * 	jz error
     * @@:
     * 	mov ecx, edx
     * 	mov edx,[edx].VxD_Desc_Block.DDB_Next
     * 	and edx, edx
     * 	jnz @@nextitem
     * 	and eax,eax
     * 	jz done
     * 	cmp [esi].VxD_Desc_Block.DDB_V86_API_Proc,0		;v86 API?
     * 	jz done
     * 	@dprintf ?JLMDBG, <"VMM_Add_DBB: JLM with API",10>
     * 	push ecx
     * 	push esi
     * 	mov esi,[esi].VxD_Desc_Block.DDB_V86_API_Proc
     * 	xor edx,edx
     * 	call Allocate_V86_Call_Back
     * 	pop esi
     * 	pop ecx
     * 	jc error
     * 	@dprintf ?JLMDBG, <"VMM_Add_DBB: callback allocated",10>
     * 	mov [esi].VxD_Desc_Block.DDB_V86_API_CSIP, eax
     * 	cmp [Int2fHooked],0
     * 	jnz done
     * 	inc [Int2fHooked]
     * 	pushad
     * 	mov eax, ?APIINT
     * 	mov esi, offset Int2fHook
     * 	call Hook_V86_Int_Chain
     * 	@dprintf ?JLMDBG, <"VMM_Add_DBB: Int 2F hooked, addr=%X",10>, esi
     * 	popad
     * done:
     * 	mov [ecx].VxD_Desc_Block.DDB_Next, esi
     * 	ret
     * error:
     * 	stc
     * 	ret
     * 	align 4
     * VMM_Add_DDB endp
     */
}


// ifdef _DEBUG
// dd offset trueprevint2f
void finalint2f(void)
{
    /*
     * Mapped logic from finalint2f in VMM.ASM:
     * finalint2f proc
     * 	cmp word ptr [ebp].Client_Reg_Struc.Client_EAX, 168Fh
     * 	jz done
     * 	@dprintf ?INT2FHOOKDBG, <"finalint2f: ax=%X",10>, word ptr [ebp].Client_Reg_Struc.Client_EAX
     * done:
     * 	stc
     * 	ret
     * finalint2f endp
     */
}

// endif

// --- a hook proc must be prefixed by the flat address
// --- of the variable where the previous value is stored
// --- AX=???

// dd offset prevint2f
void Int2fHook(void)
{
    /*
     * Mapped logic from Int2fHook in VMM.ASM:
     * Int2fHook proc
     * 
     * ifdef _DEBUG
     * 	cmp word ptr [ebp].Client_Reg_Struc.Client_EAX, 168Fh	;that happens pretty often, so don't log it
     * 	jz @F
     * endif
     * 	@dprintf ?INT2FHOOKDBG, <"Int2fHook: ax=%X, client_ax=%X",10>, ax, word ptr [ebp].Client_Reg_Struc.Client_EAX
     * @@:
     * 	cmp word ptr [ebp].Client_Reg_Struc.Client_EAX, 1684h
     * 	jnz not_ours
     * 	@dprintf ?INT2FHOOKDBG, <"Int2FHook: ax=1684h detected, device scan, BX=ID=%X",10>, word ptr [ebp].Client_Reg_Struc.Client_EBX
     * 	mov edx, offset ddb_list
     * 	mov ax, word ptr [ebp].Client_Reg_Struc.Client_EBX 	;v5.85 added
     * @@nextitem:
     * 	cmp ax, [edx].VxD_Desc_Block.DDB_Req_Device_Number
     * 	jz found
     * 	mov edx,[edx].VxD_Desc_Block.DDB_Next
     * 	and edx, edx
     * 	jnz @@nextitem
     * 	xor eax, eax
     * 	jmp @F
     * found:
     * 	@dprintf ?INT2FHOOKDBG, <"Int2FHook: found Device=%X",10>, word ptr [edx].VxD_Desc_Block.DDB_Req_Device_Number
     * 	mov byte ptr [ebp].Client_Reg_Struc.Client_EAX, 0	;v5.82: set AL to 0 if ok
     * 	mov eax, [edx].VxD_Desc_Block.DDB_V86_API_CSIP
     * @@:
     * 	mov word ptr [ebp].Client_Reg_Struc.Client_EDI, ax
     * 	shr eax, 16
     * 	mov word ptr [ebp].Client_Reg_Struc.Client_ES, ax
     * 
     * 	clc
     * 	ret
     * not_ours:
     * 	stc
     * 	ret
     * 	align 4
     * Int2fHook endp
     */
}


// --- edi -> DDB
// --- C set on failure
// --- new v5.74: free v86 breakpoint if v86 api exists

void VMM_Remove_DDB(void)
{
    /*
     * Mapped logic from VMM_Remove_DDB in VMM.ASM:
     * VMM_Remove_DDB proc public
     * 	xor eax, eax
     * 	and edi, edi
     * 	jz @@error
     * 	mov edx, offset ddb_list
     * @@nextitem:
     * 	cmp edx, edi
     * 	jz @@found
     * 	mov eax, edx
     * 	mov edx,[edx].VxD_Desc_Block.DDB_Next
     * 	and edx, edx
     * 	jnz @@nextitem
     * @@error:
     * 	stc
     * 	ret
     * @@found:
     * 	and eax, eax	;dont remove the first device
     * 	jz @@error
     * 	mov ecx,[edx].VxD_Desc_Block.DDB_Next
     * 	mov [eax].VxD_Desc_Block.DDB_Next, ecx
     * 	mov eax, [edi].VxD_Desc_Block.DDB_V86_API_CSIP
     * 	and eax, eax
     * 	jz @F
     * 	call Free_V86_Call_Back
     * @@:
     * 	clc
     * 	ret
     * 	align 4
     * VMM_Remove_DDB endp
     */
}


// end
