/*
 * dev.asm - *** implements EMMXXXX0 device --- public domain --- to be assembled with JWasm or Masm v6.1+ (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing DEV.ASM.
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

// .486P
// .model FLAT
void EMMXXXX0_Strategy(void)
{
    /*
     * Mapped logic from EMMXXXX0_Strategy in DEV.ASM:
     * EMMXXXX0_Strategy proc public
     *     call Simulate_Far_Ret                ; do a RETF in V86
     *     movzx eax,word ptr [ebp].Client_Reg_Struc.Client_ES
     *     movzx ecx,word ptr [ebp].Client_Reg_Struc.Client_EBX
     *     shl eax, 4
     *     add eax, ecx
     *     mov [dwReqPtr], eax
     *     ret
     * EMMXXXX0_Strategy endp
     */
}


// --- EMMXXXX0 interrupt routine

void EMMXXXX0_Interrupt(void)
{
    /*
     * Mapped logic from EMMXXXX0_Interrupt in DEV.ASM:
     * EMMXXXX0_Interrupt proc public
     *     call Simulate_Far_Ret                ; do a RETF in V86
     *     mov ecx, [dwReqPtr]
     * if ?EMMXXXX0
     *     mov al, [ecx].request_hdr.rhCmd
     *     @dprintf ?EMXDBG,<"EMMXXXX0 request %X",10>,al
     *     cmp al, 3           ;IOCTL input?
     *     jz @@ioctl_read
     *     cmp al, 8           ;write?
     *     jz @@resp_writeerr
     *     cmp al, 9           ;write+verify?
     *     jz @@resp_writeerr
     *     cmp al, 10          ;write status
     *     jz @@resp_ok
     *     cmp al, 11          ;write+flush?
     *     jz @@resp_writeerr
     *     cmp al, 12          ;IOCTL output?
     *     jz @@ioctl_write
     *     cmp al, 13          ;open device?
     *     jz @@resp_ok
     *     cmp al, 14          ;close device?
     *     jz @@resp_ok
     *     mov ax, 8103h
     *     jmp @@device_done
     * @@ioctl_read:
     *     push ecx
     *     call IoctlRead
     *     pop ecx
     *     jnc @@resp_ok
     * @@resp_readerr:
     *     mov ax, 810Bh
     *     jmp @@device_done
     * @@ioctl_write:
     *     push ecx
     *     call IoctlWrite
     *     pop ecx
     *     jnc @@resp_ok
     * @@resp_writeerr:
     *     mov ax, 810Ah
     *     jmp @@device_done
     * @@resp_ok:
     *     mov ax, 100h
     * @@device_done:
     * else
     *     mov ax, 8103h
     * endif
     *     mov [ecx].request_hdr.rhStatus, ax
     *     ret
     *     align 4
     * EMMXXXX0_Interrupt endp
     */
}


// if ?EMMXXXX0

// --- read ioctl EMMXXXX0 device
// --- inp: ECX=request header
// --- modifies eax,ebx,ecx,edx,esi,edi

void IoctlRead(void)
{
    /*
     * Mapped logic from IoctlRead in DEV.ASM:
     * IoctlRead proc
     *     mov eax, [ecx].request_hdr.rhBuffer
     *     movzx ebx, ax
     *     shr eax, 12
     *     and al, 0F0h
     *     add ebx, eax        ;ebx=buffer linear address
     * 
     *     movzx edx, [ecx].request_hdr.rhCount
     *     mov al, [EBX+0]
     *     cmp al, EMMDEV_GETAPI  ;get "API"
     *     jz  @@func00
     * if 0
     *     cmp al, EMMDEV_GEMMIS  ;GEMMIS not supported
     *     jz @@func01
     * endif
     *     cmp al, EMMDEV_VERSION ;get version
     *     jz @@func02
     *     cmp al, EMMDEV_GETRES  ;get Emm386 resident segment/size?
     *     jz @@func04
     *     cmp al, EMMDEV_SYSVARS ;get system vars
     *     jz @@func06
     *     cmp al, EMMDEV_GETUMBS ;get UMBs
     *     jz @@func07
     * if ?SERVTABLE
     *     cmp al, EMMDEV_GETSTAB ;get VMM service table info
     *     jz @@func08
     * endif
     *     jmp @@error
     * @@func00:
     *     cmp edx,6   ;bytes to read
     *     jb @@error
     *     mov word ptr [ebx+0], 0028h
     *     mov dword ptr [ebx+2], 0    ;API entry
     *     jmp @@ok
     * @@func02:
     *     cmp edx,2   ;bytes to read
     *     jb @@error
     *     mov word ptr [ebx+0], ?VERSIONHIGH + ?VERSIONLOW * 256
     *     jmp @@ok
     * @@func04:
     *     cmp edx,4   ;bytes to read
     *     jb @@error
     *     mov eax,[dwRSeg]
     *     mov [ebx+0], eax
     *     jmp @@ok
     * @@func06:
     * 
     *     cmp edx,16  ;bytes to read
     *     jb @@error
     *     mov al, [bNoEMS]
     *     mov [ebx].EMX06.e06_NoEMS, al
     *     xor eax, eax
     *     cmp [bNoFrame],0
     *     jnz @@nopf
     *     mov ah, [EMSPage2Segm]      ;get mapping of phys page 0
     * @@nopf:
     *     mov [ebx].EMX06.e06_Frame, ax
     *     mov al, [bNoVCPI]
     *     mov [ebx].EMX06.e06_NoVCPI, al
     *     cmp edx,24                  ;to get VCPI memory info, we need 24 byte
     *     jb @@novcpimem
     *     mov eax, [dwMaxMem4K]       ;VCPI 4 kB pages
     *     mov [ebx].EMX06.e06_VCPITotal, eax
     *     mov eax, [dwUsedMem4K]
     *     mov [ebx].EMX06.e06_VCPIUsed, eax
     * @@novcpimem:
     * if 1;?DMAPT
     *     mov eax, [DMABuffStartPhys]
     * else
     *     xor eax, eax
     * endif
     *     mov [ebx].EMX06.e06_DMABuff, eax
     * if 1;?DMAPT
     *     mov eax, [DMABuffSize]
     *     shr eax, 10
     * else
     *     xor eax, eax
     * endif
     *     mov [ebx].EMX06.e06_DMASize, ax
     * if ?VME
     *     mov al,1
     *     test byte ptr [dwFeatures],2
     *     jz @@novme
     *     @mov_eax_cr4
     *     and al,1
     *     xor al,1
     * @@novme:
     *     mov [ebx].EMX06.e06_NoVME,al
     * endif
     * if ?PGE
     *     mov al,1
     *     test byte ptr [dwFeatures+1],20h
     *     jz @@nopge
     *     @mov_eax_cr4
     *     shr al,7
     *     xor al,1
     * @@nopge:
     *     mov [ebx].EMX06.e06_NoPGE,al
     * endif
     * if ?A20PORTS or ?A20XMS
     *     mov al,[bNoA20]
     *     mov [ebx].EMX06.e06_NoA20,al
     * endif
     * @@ok:
     *     clc
     *     ret
     * @@error:
     *     stc
     *     ret
     * @@func07:
     *     cmp edx, UMB_MAX_BLOCKS * size UMBBLK   ;buffer large enough to get the UMB entries?
     *     jb @@error
     *     mov edi, ebx
     *     mov esi, offset UMBsegments
     *     mov ecx, UMB_MAX_BLOCKS
     *     cld
     *     rep movsd
     *     clc
     *     ret
     * if ?SERVTABLE
     * @@func08:
     *     cmp edx, size EMX08
     *     jb  @@error
     *     mov [ebx].EMX08.e08_ServiceTable, offset vmm_service_table
     *     mov [ebx].EMX08.e08_BPTable, offset bptable
     *     mov eax, [dwRSeg]
     *     shl eax, 16
     *     mov ecx, [bpstart]
     *     sub ecx, [dwRes]
     *     mov ax, cx
     *     mov [ebx].EMX08.e08_BPTableRM, eax
     *     mov word ptr [ebx].EMX08.e08_GDTR, GDT_SIZE-1
     *     mov dword ptr [ebx].EMX08.e08_GDTR+2, offset V86GDT
     *     mov eax,dword ptr [IDT_PTR+2]
     *     mov word ptr [ebx].EMX08.e08_IDTR, 7FFh
     *     mov dword ptr [ebx].EMX08.e08_IDTR+2, eax
     *     mov [ebx].EMX08.e08_TR, V86_TSS_SEL
     *     mov [ebx].EMX08.e08_FlatCS, FLAT_CODE_SEL
     *     ret
     * endif
     *     align 4
     * 
     * IoctlRead endp
     */
}


// --- all registers may be modified!
// --- inp: ECX=request header
// --- the "update" command is EMMDEV_UPDATE

void IoctlWrite(void)
{
    /*
     * Mapped logic from IoctlWrite in DEV.ASM:
     * IoctlWrite proc
     * 
     *     mov eax, [ecx].request_hdr.rhBuffer
     *     movzx esi, ax
     *     shr eax, 12
     *     and al, 0F0h
     *     add esi, eax
     * 
     *     lods byte ptr [esi]  ;function to call
     *     cmp al, EMMDEV_UPDATE
     *     jz @@func15
     * @@error:
     *     stc
     *     ret
     * @@func15:
     * 
     * ;--- esi -> EMX15W variable
     * 
     *     movzx eax, [ecx].request_hdr.rhCount  ;buffer size
     *     cmp eax, 5
     *     jb @@error
     * 
     *     lods byte ptr [esi]  ;e15_bVME
     * if ?VME
     *     cmp al,-1
     *     jz @@novme
     *     xor al,1
     *     call SetVME
     * @@novme:
     * endif
     *     lods byte ptr [esi]  ;e15_bA20
     * if ?A20PORTS or ?A20XMS
     *     cmp al,-1
     *     jz @@noa20
     *     and al, al
     *     jz @@a20emuon
     *     push eax
     *     mov al,1
     *     call A20_Set     ;enable A20 gate
     *     pop eax
     * @@a20emuon:
     *     mov [bNoA20], al
     * @@noa20:
     * endif
     *     lods byte ptr [esi]  ;e15_bVCPI
     *     cmp al,-1
     *     jz @@novcpi
     *     mov [bNoVCPI],al
     * @@novcpi:
     *     lods byte ptr [esi]  ;e15_bPGE
     * if ?PGE
     *     cmp al,-1
     *     jz @@nopge
     *     test byte ptr [dwFeatures+1], 20h   ;PGE supported?
     *     jz @@nopge
     *     and al,1
     *     xor al,1
     *     mov [bPageMask],al
     *  if 0
     *     @GETPTEPTR edi, ?PAGETAB0, 1;start of pagetab 0
     *     mov ecx,110h+1          ;00000000-00110FFF
     * @@FILL_PAGETAB0:
     *     mov edx, [edi]
     *     and dh,not 1        ;mask out G
     *     or dh,al
     *     MOV [EDI],EDX
     *     ADD EDI,4
     *     loop @@FILL_PAGETAB0
     *  endif
     *     @mov_ecx_cr4
     *     shl al,7
     *     and cl,not 80h
     *     or cl,al
     *     @mov_cr4_ecx
     * @@nopge:
     * endif
     *     clc
     *     ret
     *     align 4
     * IoctlWrite endp
     */
}


// endif

// .text$03    ends

// END
