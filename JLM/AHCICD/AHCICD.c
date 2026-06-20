/*
 * ahcicd.asm - --- JLM to read CD/DVD in AHCI mode. --- "cooked" mode is supported only. (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing AHCICD.ASM.
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
void DevInt(void)
{
    /*
     * Mapped logic from DevInt in AHCICD.ASM:
     * DevInt proc
     * 
     * 	.const
     * 
     * 	align 4
     * 
     * ;--- cmds 00-0E
     * Vector label dword
     * 	dd Init		; 0 Init
     * 	dd Error3	; 1 media check (block devices)
     * 	dd Error3	; 2 build bpb (block devices)
     * 	dd IOCtlInp	; 3 ioctl input
     * 	dd Error3	; 4 input
     * 	dd Error3	; 5 nondestr. input
     * 	dd Error3	; 6 input status (char devices)
     * 	dd Error3	; 7 input flush (char devices)
     * 	dd Error3	; 8 output
     * 	dd Error3	; 9 output with verify
     * 	dd Error3	;10 output status (char devices)
     * 	dd Error3	;11 output flush (char devices)
     * 	dd Error3	;12 ioctl output
     * 	dd Exit		;13 device open
     * 	dd Exit		;14 device close
     * ;	dd Error3	;15 removable media (block devices)
     * LVECTOR equ ($ - offset Vector) / sizeof dword
     * 
     * ;--- cmds 128-13x
     * Vect2 label dword
     * 	dd ReadL	;128 CD read long
     * 	dd Error3	;129 CD reserved
     * 	dd Exit		;130 CD read long prefetch
     * ;	dd Error3	;131 CD seek
     * ;	dd Error3	;132 CD play audio
     * ;	dd Error3	;133 CD stop audio
     * ;	dd Error3	;134 CD write long
     * ;	dd Error3	;135 CD write long verify
     * ;	dd Error3	;136 CD resume audio
     * LVECT2 equ ($ - offset Vect2) / sizeof dword
     * 
     * 	.code
     * 
     * 	mov ebx, [ppReq]
     * 	movzx eax, word ptr [ebx+0]
     * 	movzx ebx, word ptr [ebx+2]
     * 	shl ebx, 4
     * 	add ebx, eax			;Point to DOS request packet.
     * 	mov [pRequest], ebx		;linear address request header
     * 
     * 	mov al, [ebx].RPH.bOp
     * 
     * 	@dprintf <'DevInt enter, Op=%X, flags=%X',13,10>, eax, [ebp].Client_Reg_Struc.Client_EFlags
     * 
     * 	and al, al
     * 	jz @F
     * 	mov al, [bUnits]
     * 	cmp [ebx].RPH.bSubU, al
     * 	mov ax, 8101h
     * 	jnc ExitAX
     * @@:
     * 	mov esi, offset Vector
     * 	mov cl, LVECTOR
     * 	movzx eax, [ebx].RPH.bOp
     * 	and al, al
     * 	jns @F
     * 	mov esi, offset Vect2
     * 	and al, 7Fh
     * 	mov cl, LVECT2
     * @@:
     * 	cmp al, cl
     * 	jnc Error3X
     * 	push [ebp].Client_Reg_Struc.Client_EFlags
     * 	or byte ptr [ebp].Client_Reg_Struc.Client_EFlags+1, 2	; set client IF so Yield will enable interrupts
     * 	call dword ptr [esi+eax*4]
     * 	pop [ebp].Client_Reg_Struc.Client_EFlags
     * ExitAX:
     * 	@dprintf <'DevInt exit, ax=%X',13,10>, ax
     * 	mov ebx, [pRequest]
     * 	mov [ebx].RPH.wStat, ax
     * 	@VMMCall Simulate_Far_Ret
     * 	ret
     * Error3X:
     * 	push ExitAX
     * Error3:
     * 	mov ax, 8103h
     * 	ret
     * Exit:
     * 	mov ax, RPDON
     * 	ret
     * 
     * DevInt endp
     */
}


// --- IOCtl Input dispatcher

void IOCtlInp(void)
{
    /*
     * Mapped logic from IOCtlInp in AHCICD.ASM:
     * IOCtlInp proc
     * 
     * 	.const
     * 
     * 	align 4
     * 
     * ;--- IOCTL input subcmds
     * IVect   label dword
     * 	dd IOC_GetHdr	; 0 get device driver header address
     * 	dd IOC_Error3	; 1 drive head location
     * 	dd IOC_Error3	; 2 reserved
     * 	dd IOC_Error3	; 3 error statistics
     * 	dd IOC_Error3	; 4 audio channel info
     * 	dd IOC_Error3	; 5 raw data bytes
     * 	dd IOC_DevSt	; 6 device status
     * 	dd IOC_SecSize	; 7 sector size
     * 	dd IOC_Error3	; 8 volume size
     * 	dd IOC_Media	; 9 media change status
     * ;	dd IOC_Error3	;10 audio disk info
     * ;	dd IOC_Error3	;11 audio track info
     * ;	dd IOC_Error3	;12 audio q-channel info
     * ;	dd IOC_Error3	;13 audio sub-channel info
     * ;	dd IOC_Error3	;14 UPC code
     * ;	dd IOC_Error3	;15 audio status info
     * LIVECT  equ ($ - offset IVect) shr 2
     * 
     * 	.code
     * 
     * 	movzx eax, word ptr [ebx].RPIOC.dwAddr+0
     * 	movzx esi, word ptr [ebx].RPIOC.dwAddr+2
     * 	shl esi, 4
     * 	add esi, eax
     * 	movzx eax, byte ptr [esi]
     * 	@dprintf <'DevInt, ioctl input, buffer=%X, al=%X',13,10>, esi, eax
     * 	cmp al, LIVECT
     * 	jnc IOC_Error3
     * 	jmp [eax*4][IVect]
     * IOC_Error3:
     * 	mov ax, 8103h
     * 	retn
     * 
     * IOC_GetHdr:
     * 	mov eax, [pBase]
     * 	shl eax, 12
     * 	@dprintf <'DevInt, ioctl input, gethdr, return=%X',13,10>, eax
     * 	mov [esi+1], eax
     * IOC_done:
     * 	mov ax, RPDON
     * 	retn
     * 
     * ;--- request "get device status"
     * ;--- status flag bits:
     * ;--- 001 - door opened
     * ;--- 002 - door unlocked
     * ;--- 004 - supports cooked AND raw
     * ;--- 008 - read AND write
     * ;--- 010 - data read AND play audio
     * ;--- 020 - supports interleaving
     * ;--- 040 - (res)
     * ;--- 080 - supports prefetching
     * ;--- 100 - supports audio channel manipulation
     * ;--- 200 - supports HSG AND red book addressing modes
     * 
     * IOC_DevSt:
     * 	mov dword ptr [esi+1], 2	;set "door unlocked"
     * 	jmp IOC_done
     * 
     * IOC_SecSize:
     * 	mov word ptr [esi+2], SLCOOKED
     * 	jmp IOC_done
     * 
     * IOCtlInp endp
     */
}


void IOC_Media(void)
{
    /*
     * Mapped logic from IOC_Media in AHCICD.ASM:
     * IOC_Media proc
     * 	@dprintf <'DevInt, ioctl input, media',13,10>
     * 	xor edi, edi
     * 	call Setup		; set EDI=command table, ebx=port offset
     * 
     * ;--- write packet data ( 12 bytes )
     * ;--- 0: 4A - cmd "get event status notification"
     * ;--- 1: 01 - bit 0=1 immediate
     * ;--- 4: 10 - notification class request, bit 4=1 is "media"
     * ;--- 7-8: 0008 - allocation length ( hibyte first )
     * 
     * ;--- ACMD.00: 4A,01,00,00
     * ;--- ACMD.04: 10,00,00,00
     * ;--- ACMD.08: 08,00,00,00
     * 	mov [edi].CT.Pkt0, 14AH
     * 	mov [edi].CT.Pkt1, 10H
     * 	mov [edi].CT.Pkt2, 8
     * 
     * 	call Req8
     * 	and al, al
     * 	jnz MedChg
     * 
     * ;--- request has returned 8 bytes,
     * ;--- 0-3: event header (0-1 event data length, hibyte first)
     * ;--- 4-7: event data ( 0=media event [bit 0-3], 1=media status [bit 0-1])
     * 
     * 	mov eax, [pBuff]
     * 	mov ax, [eax+4]
     * 
     * 	and al, al
     * 	jnz MedChg
     * 	test ah, 2  ; bit 1=1 if media present
     * 	jz MedChg
     * 	mov byte ptr [esi+1], 1
     * 	jmp MedEnd
     * MedChg:
     * 	mov byte ptr [esi+1], 0
     * MedEnd:
     * 	call Setup
     * 	call ReqSense
     * 
     * 	mov eax, [pBuff]
     * 	mov al, [eax+2]
     * 
     * 	and al, 15
     * 	jz @F
     * 	mov byte ptr [esi+1], 0
     * 	cmp al, 6
     * 	jz MedEnd
     * @@:
     * 	mov ax, RPDON
     * 	ret
     * IOC_Media endp
     */
}


// --- read sector(s)
// --- in: ebx=RL

void ReadL(void)
{
    /*
     * Mapped logic from ReadL in AHCICD.ASM:
     * ReadL proc
     * 	mov al, 3
     * 	cmp [ebx].RPRL.bAMode, 0; High Sierra addressing mode?
     * 	jnz ErrorAL
     * 	cmp [ebx].RPRL.bDMode, 0; 0=cooked, 1=raw
     * 	jnz ErrorAL
     * 
     * 	movzx eax, word ptr [ebx].RPRL.dwAddr+0
     * 	movzx edx, word ptr [ebx].RPRL.dwAddr+2
     * 	shl edx, 4
     * 	add eax, edx
     * 	mov [pDest], eax
     * 
     * 	mov cx, [ebx].RPRL.wSecCnt
     * 	mov eax, [ebx].RPRL.dwStart
     * 	mov [wSecCnt], cx
     * 	mov [dwSector], eax
     * 	@dprintf <'DevInt, readl, start=%X, sectors=%X, dst=%X',13,10>, eax, cx, pDest
     * 	xor edi, edi
     * ReadLP:
     * 	cmp [wSecCnt], 0
     * 	jz Exit
     * 	call Setup			; set EDI=command table, ebx=port offset
     * 
     * ;--- write packet data ( 12 bytes )
     * ;--- 0: cmd 28 ( read )
     * ;--- 1: flags
     * ;--- 2-5: LBA (high byte first)
     * ;--- 6: group no
     * ;--- 7-8: transfer length [sector count] ( high byte first )
     * ;--- 9: control
     * 
     * 	mov eax, [dwSector]
     * 	shld ecx, eax, 16
     * 
     * ;--- ax=sect[0-15]
     * ;--- cx=sect[16-31]
     * 
     * 	movzx eax, ax
     * 	xchg al, ah
     * 	xchg cl, ch
     * 
     * ;--- al=sect[8-15], ah=sect[0-7], cl=sect[24-31], ch=sect[16-23]
     * 
     * 	shl ecx, 16
     * 	mov cl, 28H
     * 
     * ;--- ACMD.00: 28h (opcode "read cooked"),00,Sector.04,Sector.03
     * ;--- ACMD.04: Sector.02, Sector.01, group#, length.02
     * ;--- ACMD.08: length.01,00,00,00
     * 	mov [edi].CT.Pkt0, ecx
     * 	mov [edi].CT.Pkt1, eax
     * 	mov [edi].CT.Pkt2, 1
     * 
     * 	mov ecx, 800007FFH	; PRDT.0C: FF,07,00,80  (PRC=7FF transfer 2048 bytes, 80=I)
     * 	call ReqECX
     * 	and al, al
     * 	jnz readerr
     * 	push edi
     * 	mov edi, [pDest]
     * 	mov esi, [pBuff]
     * 	cld
     * if ?IRQWND
     * 	mov ecx, SLCOOKED
     * 	@VMMCall MoveMemory
     * else
     * 	mov ecx, SLCOOKED / sizeof dword
     * 	rep movsd
     * 	@VMMCall Yield
     * endif
     * 	pop edi
     * 	add [pDest], SLCOOKED
     * 	inc [dwSector]
     * 	dec [wSecCnt]
     * 	jmp ReadLP
     * readerr:
     * 	call Setup
     * 	call ReqSense
     * 	and al, al
     * 	jnz ErrorAL
     * 	mov eax, [pBuff]
     * 	mov al, [eax+2]
     * 	and al, 15
     * 	mov ebx, offset ERRTAB
     * 	xlat
     * ErrorAL:
     * 	mov ah, 81h
     * 	@dprintf <'DevInt, readl, error, ax=%X',13,10>, ax
     * 	ret
     * Exit:
     * ifdef _DEBUG
     * 	mov ebx, [pRequest]
     * 	@dprintf <'DevInt, readl, ok, sector=%X',13,10>, [ebx].RPRL.dwStart
     * endif
     * 	mov ax, RPDON
     * 	ret
     * ReadL endp
     */
}


// --- in:
// --- ebx=RP if edi==0
// --- out:
// --- ebx=port offset
// --- edi=linear address ATAPI command table

void Setup(void)
{
    /*
     * Mapped logic from Setup in AHCICD.ASM:
     * Setup proc
     * 	cmp edi, 0			; first call?
     * 	jnz @F
     * 	movzx eax, byte ptr [ebx+1]
     * 	movzx ebx, [eax*2][wPort]
     * 	add ebx, [pHBA]
     * 
     * ;--- get command list/table bases.
     * ;--- those are physical addresses and hence "should" be translated to linear ones.
     * 
     * if ?OPTIONR
     * 	mov eax, [pCLB]						; get command list base
     * 	mov edi, [pCTBA]					; get command table base address
     * 	@dprintf <'DevInt, setup, cmdlist=%X/%X, cmdtable=%X/%X',13,10>, eax, dword ptr [ebx].PORT.dqCLB, edi, [eax].CLH.dwCTBA
     * else
     * 	mov eax, dword ptr [ebx].PORT.dqCLB	; get command list base
     * 	mov edi, [eax].CLH.dwCTBA			; get command table base address
     * 	@dprintf <'DevInt, setup, cmdlist=%X, cmdtable=%X',13,10>, eax, edi
     * endif
     * 
     * ;--- CLH.00: 25,00,01,00 [CFL=5 dwords, A=1, PRD table length=1]
     * ;--- CLH.04: 00,00,00,00 [PRD byte count=0]
     * 	mov dword ptr [eax].CLH.flags1, 10025H
     * 	mov [eax].CLH.PRDBC, 0
     * 
     * @@:
     * ;--- CT.00: 27,80,A0,01 [27=H2D register FIS,80=set command register,A0=ATAPI packet command,01=Features]
     * ;--- CT.04: 00,FF,FF,00 [LBA low,mid,high,device]
     * ;--- CT.08: 00,00,00,00 [LBA (exp) low,mid,high,features]
     * ;--- CT.0C: 01,00,00,00 [sector count, sector count,res,control]
     * 	mov [edi].CT.cfis0, 1A08027H
     * 	mov [edi].CT.cfis1, 0FFFF00H
     * 	mov [edi].CT.cfis2, 0
     * 	mov [edi].CT.cfis3, 1
     * 
     * ;--- set base; must be physical address
     * 
     * 	mov ecx, [dwBuffPh]
     * 	mov [edi].CT.p0.dwBase, ecx
     * 	xor ecx, ecx
     * 	mov [edi].CT.p0.dwBaseU, ecx
     * 	mov [edi].CT.p0.dwRsvd, ecx
     * 
     * 	xor ecx, ecx
     * 	mov [ebx].PORT.dwIE, ecx
     * 	or ecx, -1
     * 	mov [ebx].PORT.dwIS, ecx
     * 	mov [ebx].PORT.dwSERR, ecx
     * 	mov ecx, [ebx].PORT.dwCMD
     * 	or cl, PCMD_FRE		; enable FIS receive
     * 	mov [ebx].PORT.dwCMD, ecx
     * 	or cl, PCMD_ST		; start processing command list
     * 	mov [ebx].PORT.dwCMD, ecx
     * 	@dprintf <'DevInt, setup, port=%X, cmdtable=%X, waiting',13,10>, ebx, edi
     * @@:
     * 	mov eax, [ebx].PORT.dwCMD
     * 	test ax, PCMD_CR	; command list running?
     * if 0 ; changed 04/2023
     * 	jz @B
     * else
     * 	jnz @F
     * 	@VMMCall Yield
     * 	jmp @B
     * @@:
     * endif
     * 	ret
     * Setup endp
     */
}


// --- initiate "request sense notification" cmd

void ReqSense(void)
{
    /*
     * Mapped logic from ReqSense in AHCICD.ASM:
     * ReqSense proc
     * 
     * ;--- write packet data ( 12 bytes )
     * ;--- 0: cmd 03 ( request sense )
     * ;--- 1-3: reserved
     * ;--- 4: allocation length
     * ;--- 5: control
     * 
     * ;--- ACMD.00: 03,00,00,00
     * ;--- ACMD.04: 08,00,00,00
     * ;--- ACMD.08: 00,00,00,00
     * 	mov [edi].CT.Pkt0, 3
     * 	mov [edi].CT.Pkt1, 8
     * 	mov [edi].CT.Pkt2, 0
     * 
     * ReqSense endp
     */
}


// --- fall thru!!!

// --- Req8: read 8 bytes into buffer

// Req8:
// mov ecx, 80000007H // PRDT.12: 07,00,00,80 (PRC=7; read 8 bytes, 80=I)

// --- ReqECX:
// --- in: ecx: read ECX+1 bytes ( bit 31 = 1, indicating end of table )
// ---     edi: cmd table
// ---     ebx: port
// --- out: AL=0 ok, AL=12 error

void ReqECX(void)
{
    /*
     * Mapped logic from ReqECX in AHCICD.ASM:
     * ReqECX proc
     * 	mov [edi].CT.p0.dwCnt, ecx
     * 	mov [ebx].PORT.dwCI, 1	; bitmask, 1=slot 0 command issued
     * contwait:
     * 	cmp [ebx].PORT.dwCI, 0
     * 	jz done
     * 	test [ebx].PORT.dwIS, PIS_TFES
     * 	jnz error
     * 	@VMMCall Yield
     * 	cmp [ebx].PORT.dwSERR, 0
     * 	JZ contwait
     * error:
     * 	call Stop
     * 	mov al, 12
     * 	ret
     * done:
     * 	mov eax, [ebx].PORT.dwTFD
     * 	test al, 1
     * 	jnz error
     * 	call Stop
     * 	mov al, 0
     * 	ret
     * 
     * Stop:
     * 	mov ecx, [ebx].PORT.dwCMD
     * 	or cl, PCMD_CLO
     * 	and cl, 0EEH		; good idea to reset PCMD_ST and PCMD_FRE here?
     * 	mov [ebx].PORT.dwCMD, ecx
     * 	retn
     * 
     * ReqECX endp
     */
}


// --- read PCI config address/data ports
// --- in: edx = addr+80000000h
// --- out: eax = value

void GetPCI(void)
{
    /*
     * Mapped logic from GetPCI in AHCICD.ASM:
     * GetPCI proc
     * 	push edx
     * 	mov eax, edx
     * 	mov dx, 0cf8h
     * 	out dx, eax
     * 	mov dl, 0fch
     * 	in eax, dx
     * 	pop edx
     * 	ret
     * GetPCI endp
     */
}


void SavePCI(void)
{
    /*
     * Mapped logic from SavePCI in AHCICD.ASM:
     * SavePCI proc
     * 	mov dx, 0cf8h
     * 	in eax, dx
     * 	mov ebx, eax
     * 	ret
     * SavePCI endp
     */
}


void RestPCI(void)
{
    /*
     * Mapped logic from RestPCI in AHCICD.ASM:
     * RestPCI proc
     * 	mov dx, 0cf8h
     * 	mov eax, ebx
     * 	out dx, eax
     * 	ret
     * RestPCI endp
     */
}


// if 0

// --- in: edx = addr+80000000h, eax = value

void PutPCI(void)
{
    /*
     * Mapped logic from PutPCI in AHCICD.ASM:
     * PutPCI proc
     * 	push edx
     * 	push eax
     * 	mov eax, edx
     * 	mov dx, 0cf8h
     * 	out dx, eax
     * 	mov dl, 0fch
     * 	pop eax
     * 	out dx, eax
     * 	pop edx
     * 	ret
     * PutPCI endp
     */
}


// endif

// --- display string EDX

void dispString(void)
{
    /*
     * Mapped logic from dispString in AHCICD.ASM:
     * dispString proc
     * 	push esi
     * 	mov esi, edx
     * 	@VMMCall Begin_Nest_Exec
     * nextitem:
     * 	lodsb
     * 	cmp al,0
     * 	jz done
     * 	mov byte ptr [ebp].Client_Reg_Struc.Client_EDX, al
     * 	mov byte ptr [ebp].Client_Reg_Struc.Client_EAX+1, 2
     * 	mov eax, 21h
     * 	@VMMCall Exec_Int
     * 	jmp nextitem
     * done:
     * 	@VMMCall End_Nest_Exec
     * 	pop esi
     * 	ret
     * dispString endp
     */
}


void dispBanner(void)
{
    /*
     * Mapped logic from dispBanner in AHCICD.ASM:
     * dispBanner proc
     * 	cmp bQuiet, 0
     * 	jnz @F
     * 	cmp bBanner, 0
     * 	jnz @F
     * 	mov bBanner, 1
     * 	push edx
     * 	mov edx, CStr('AHCI Optical Disk Driver v1.2',13,10,"Inspired by Rudolph R. Loew's AHCICD",13,10)
     * 	call dispString
     * 	pop edx
     * @@:
     * 	ret
     * dispBanner endp
     */
}


// --- ebp: client reg struct
// --- esi: cmdline

void Init(void)
{
    /*
     * Mapped logic from Init in AHCICD.ASM:
     * Init proc
     * 
     * ;--- scan cmdline args
     * 
     * nextchar:
     * 	lodsb
     * 	cmp al, ' '
     * 	jz nextchar
     * 	cmp al, 9
     * 	jz nextchar
     * 	cmp al, 13
     * 	jz donecmdl
     * 	cmp al, 0
     * 	jz donecmdl
     * 	cmp al, '/'
     * 	jnz errcmdl
     * 	lodsb
     * 	mov ah, [esi]
     * 	or al, 20h
     * 	cmp ax, ':c'
     * 	jz isOptC
     * 	cmp ax, ':d'
     * 	jz isOptD
     * 	cmp AL, 'q'
     * 	jz isOptQ
     * if ?OPTIONR
     * 	cmp AL, 'r'
     * 	jz isOptR
     * endif
     * 	jmp errcmdl
     * 
     * isOptD:
     * 	inc esi
     * 	mov edi, [pBase]
     * 	add edi, DOSDRV.name_
     * 	mov dword ptr [edi+0], '    '
     * 	mov dword ptr [edi+4], '    '
     * 	mov ecx, sizeof DOSDRV.name_
     * nextdevchar:
     * 	lodsb
     * 	cmp al, ' '
     * 	jbe doneOptD
     * 	cmp al, 'a'
     * 	jb @F
     * 	cmp al, 'z'
     * 	ja @F
     * 	sub al, 20H
     * @@:
     * 	stosb
     * 	loop nextdevchar
     * 	inc esi
     * doneOptD:
     * 	dec esi
     * 	jmp nextchar
     * isOptC:
     * 	inc esi
     * 	lodsb
     * 	cmp al, '0'
     * 	jb errcmdl
     * 	cmp al, '9'
     * 	Ja errcmdl
     * 	sub al, '0'
     * 	mov [bCntOpt], al
     * 	jmp nextchar
     * isOptQ:
     * 	mov [bQuiet], 1
     * 	jmp nextchar
     * if ?OPTIONR
     * isOptR:
     * 	mov [bReloc], 1
     * 	jmp nextchar
     * endif
     * 
     * donecmdl:
     * 
     * ;--- done cmdline processing
     * 
     * 	call dispBanner
     * 
     * ;--- scan for AHCI devices
     * 
     * 	call SavePCI
     * 	mov edx, 80000008H
     * nextHBA:
     * 	call GetPCI
     * 	mov al, 0			; clear bits 0-7 (interface doesn't matter)
     * 	cmp eax, 1060100H	; AHCI device?
     * 	jnz @F
     * 	dec [bCntOpt]		; /C:x option?
     * 	js  foundHBA
     * @@:
     * 	@VMMCall Yield
     * 	add edx, 100H		; next "function"
     * 	cmp edx, 81000008H
     * 	jc nextHBA
     * 	call RestPCI
     * 	jmp errnodisk
     * 
     * foundHBA:
     * 	@dprintf <'found AHCI device at PCI=%X',13,10>, edx
     * 
     * 	mov dl, 24h			;get ABAR5
     * 	call GetPCI
     * 	mov edi, eax
     * 
     * 	call RestPCI
     * 
     * ;--- HBA does not necessarily have to start at a page boundary
     * ;--- so always reserve 2 pages.
     * 
     * 	@dprintf <'HBA physical addr=%X',13,10>, edi
     * 
     * 	push 0
     * 	push 2
     * 	push PR_SYSTEM
     * 	@VMMCall _PageReserve
     * 	add esp, 3*4
     * 	cmp eax, -1
     * 	jz errpr
     * 	mov ebx, eax
     * 
     * 	shr eax, 12		; convert linear address to page#
     * 	mov ecx, edi
     * 	shr ecx, 12
     * 	push PC_WRITEABLE
     * 	push ecx
     * 	push 2			; map 2 pages ( in theory even 3 might be needed )
     * 	push eax
     * 	@VMMCall _PageCommitPhys
     * 	add esp, 4*4
     * 
     * 	and edi, 0fffh
     * 	add ebx, edi
     * 	mov [pHBA], ebx
     * 	@dprintf <'HBA mapped at linear addr=%X',13,10>, ebx
     * 	mov eax, [ebx].HBA.dwGHC	; get Global HBA Control
     * 	@dprintf <'HBA.GHC=%X',13,10>, eax
     * 	test eax, 80000000H	; AHCI enabled?
     * 	jz errnoahci
     * 
     * 	mov ecx, [ebx].HBA.dwPI	; get ports implemented (bit mask)
     * 	@dprintf <'HBA.Ports=%X',13,10>, ecx
     * 
     * 	mov edx, 100h		; port offsets are 100h, 180h, 200h, 280h, ..., 1080h
     * 	mov edi, offset wPort
     * nextPort:
     * 	shr ecx, 1
     * 	jnc @F
     * 	mov eax, [ebx+edx].PORT.dwSIG	; get signature
     * 	cmp eax, 0EB140101H
     * 	jnz @F
     * 	@dprintf <'found ATAPI device at port=%X',13,10>, edx
     * 	mov eax, [ebx+edx].PORT.dwSSTS	; get SATA status
     * 	@dprintf <'device status=%X',13,10>, eax
     * 	and al, 0FH			; bits 0-3: device detection
     * 	cmp al, 3			; device detected and communication established?
     * 	jnz @F
     * 	mov eax, edx
     * 	stosw
     * 	cmp edi, offset wPort + sizeof wPort; table end reached?
     * 	jz donePort
     * @@:
     * 	add edx, 80H
     * 	and ecx, ecx
     * 	Jnz nextPort
     * 
     * donePort:
     * 	sub edi, offset wPort
     * 	shr edi, 1
     * 	mov ecx, edi
     * 	cmp ecx, 0
     * 	jz errnodisk
     * 	mov [bUnits], cl
     * 
     * if ?OPTIONR
     * 	mov edi, offset wPort
     * 	cmp [bReloc], 0
     * 	jz noreloc
     * 	mov esi, offset cmdlst
     * 	mov [pCLB], esi
     * 	mov [pCTBA], offset cmdtab
     * 	mov [pBuff], offset buffer
     * 	push ecx
     * 	mov ecx, sizeof cmdlst + sizeof fis + sizeof cmdtab + sizeof buffer
     * 	mov edx, 1
     * 	VxDCall VDMAD_Lock_DMA_Region
     * 	pop ecx
     * 	@dprintf <'physical/logical address CLB=%X/%X',13,10>, edx, esi
     * 	mov esi, edx
     * 	.while ecx
     * 		movzx edx, word ptr [edi]
     * 		mov eax, esi
     * 		mov dword ptr [ebx+edx].PORT.dqCLB, eax
     * 		add eax, sizeof cmdlst
     * 		mov dword ptr [ebx+edx].PORT.dqFB, eax
     * 		add edi, sizeof WORD
     * 		dec ecx
     * 	.endw
     * 	mov eax, esi
     * 	add eax, sizeof cmdlst + sizeof fis
     * 	mov edx, [pCLB]
     * 	mov [edx].CLH.dwCTBA, eax
     * 	@dprintf <'CLB.CTBA=%X',13,10>, eax
     * 	add eax, sizeof cmdtab
     * 	mov [dwBuffPh], eax
     * 	jmp buffers_done
     * noreloc:
     * 	movzx edx, word ptr [edi]
     * 	mov eax, dword ptr [ebx+edx].PORT.dqCLB
     * 	mov edx, [eax].CLH.dwCTBA
     * 	mov [pCLB], eax
     * 	mov [pCTBA], edx
     * 	@dprintf <'linear address command list/table=%X/%X',13,10>, eax, edx
     * endif
     * 	mov esi, offset buffer
     * 	mov [pBuff], esi
     * 	mov ecx, SLCOOKED
     * 	mov edx, 1
     * 	VxDCall VDMAD_Lock_DMA_Region
     * 	mov [dwBuffPh], edx
     * 	@dprintf <'physical address sector buffer=%X',13,10>, edx
     * 
     * buffers_done:
     * 
     * 	mov ebx, [pBase]
     * 	mov al, [bUnits]
     * 	mov [ebx].DOSDRV.bUnits, al
     * 	cmp [bQuiet], 0
     * 	jnz @F
     * 	add al,'0'
     * 	movzx eax, al
     * 	push eax
     * 	mov edx, esp
     * 	call dispString
     * 	pop eax
     * 	mov edx, CStr(' AHCI Optical Disk(s) Found',13,10)
     * 	call dispString
     * @@:
     * 	mov ebx, [pRequest]
     * 	mov word ptr [ebx].RPInit.dwFree+0, REQOFS+4+SIZERMCODE+5
     * 	mov ax, RPDON
     * 	mov [ebx].RPInit.wStat, ax
     * 	mov [ebx].RPInit.bUnit, 0
     * 	ret
     * 
     * errpr:
     * 	mov edx, CStr('_PageReserve() failed',13,10)
     * 	jmp @F
     * errnoahci:
     * 	mov edx, CStr('AHCI Controller not in AHCI Mode',13,10)
     * 	jmp @F
     * errcmdl:
     * if ?OPTIONR
     * 	mov edx, CStr('SYNTAX: DEVICE=JLOAD.EXE AHCICD.DLL [/C:#] /D:devname [/Q][/R]',13,10)
     * else
     * 	mov edx, CStr('SYNTAX: DEVICE=JLOAD.EXE AHCICD.DLL [/C:#] /D:devname [/Q]',13,10)
     * endif
     * 	jmp @F
     * errnodisk:
     * 	MOV edx, CStr('No AHCI Optical Disk Found',13,10)
     * @@:
     * 	call dispBanner
     * 	call dispString
     * 	mov ax, RPERR
     * 	ret
     * Init endp
     */
}


void DllMain(void)
{
    /*
     * Mapped logic from DllMain in AHCICD.ASM:
     * DllMain proc stdcall public uses esi edi hModule:dword, dwReason:dword, dwRes:dword
     * 
     * local pCmdLine:dword
     * 
     * 	mov eax, dwReason
     * 	cmp eax, 1
     * 	jnz done
     * 
     * 	mov esi, dwRes
     * 	test [esi].JLCOMM.wFlags, JLF_DRIVER
     * 	jz failed
     * 	movzx ecx, [esi].JLCOMM.wLdrCS
     * 	shl ecx, 4
     * 	mov [pBase], ecx
     * 	lea eax, [ecx+REQOFS]
     * 	mov [ppReq], eax
     * 	mov eax, [esi].JLCOMM.lpCmdLine
     * 	mov [pCmdLine], eax
     * 	mov eax, [esi].JLCOMM.lpRequest
     * 	mov [pRequest], eax
     * 
     * 	mov esi, offset DevInt
     * 	xor edx, edx
     * 	@VMMCall Allocate_V86_Call_Back
     * 	jc failed
     * 
     * 	mov edi, [pBase]
     * 	mov [edi].DOSDRV.wAttr, 0C800h		;driver attributes
     * 	mov [edi].DOSDRV.ofsStr, REQOFS+4
     * 	mov [edi].DOSDRV.ofsInt, REQOFS+4+SIZERMCODE
     * 	mov [edi].DOSDRV.wRes1, 0
     * 	mov [edi].DOSDRV.bRes2, 0
     * 
     * 	add edi, REQOFS+4
     * 	mov esi, offset rmcode
     * 	mov ecx, SIZERMCODE+1
     * 	cld
     * 	rep movsb
     * 	stosd
     * 
     * 	@VMMCall Get_Cur_VM_Handle
     * 
     * 	mov esi, [pCmdLine]
     * 
     * ;--- set EBP to the client pointer before calling I_Init
     * 
     * 	push ebp
     * 	mov ebp, [ebx].cb_s.CB_Client_Pointer
     * 	call Init
     * 	pop ebp
     * 
     * 	cmp ax,RPDON
     * 	setz al
     * 	movzx eax,al
     * done:
     * 	ret
     * failed:
     * 	xor eax, eax
     * 	ret
     * 
     * DllMain endp
     */
}


// end DllMain

