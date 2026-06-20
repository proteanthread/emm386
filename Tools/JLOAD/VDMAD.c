/*
 * vdmad.asm - --- VDMAD support --- best viewed with TAB size 4 (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing VDMAD.ASM.
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
void Get_VDMAD_Version(void)
{
    /*
     * Mapped logic from Get_VDMAD_Version in VDMAD.ASM:
     * Get_VDMAD_Version proc
     * 	mov eax,100h
     * 	clc
     * 	ret
     * 	align 4
     * 
     * Get_VDMAD_Version endp
     */
}


// --- esi = start region (linear address)
// --- ecx = size region (bytes)
// --- dl = flags (bit 0: check for 64 kB crossing, bit 1: check for 128 kB)
// --- returns phys address in EDX if ok
// --- returns size in ECX on error, in AL error code

// --- Jemm is unable to handle addresses > 110000h
// --- therefore do it here.

void Lock_DMA_Region(void)
{
    /*
     * Mapped logic from Lock_DMA_Region in VDMAD.ASM:
     * Lock_DMA_Region proc uses edi ebx
     * 
     * 	@dprintf ?DMADBG, <"Lock_DMA_Region, addr=%X, size=%X",10>, esi, ecx
     * 	push esi
     * 	lea ebx, [esi+ecx-1]	;let EBX point to the last byte
     * 	mov eax, esi
     * 	shr eax, 12
     * 	shr ebx, 12
     * 	sub ebx, eax
     * 	jc @@error2
     * 
     * 	shr esi, 12
     * 	lea esi, [esi*4 + (?PAGEMAP shl 22)]
     * 	mov ecx, ebx
     * 	lodsd
     * 	and ax, 0F000h
     * 	mov bl, dl
     * 	mov edx, eax
     * 	jecxz @@done
     * 	mov edi, edx	; save first phys page in EDX
     * 	.while ecx
     * 		lodsd
     * 		and eax, eax
     * 		jz @@error
     * 		and ax, 0F000h
     * 		add edi,1000h
     * 		cmp eax, edi
     * 		jnz @@error	; not contiguous
     * 		dec ecx
     * 	.endw
     * 	test bl, 1		; 64 kB border crossing test?
     * 	jz @@done
     * 	mov eax, edx
     * 	shr eax, 16
     * 	shr edi, 16
     * 	cmp eax, edi
     * 	jnz @@error2
     * @@done:
     * 	pop esi
     * 	mov ax, si
     * 	and ax, 0FFFh
     * 	or dx, ax
     * 	@dprintf ?DMADBG, <"Lock_DMA_Region, returns edx=%X",10>, edx
     * 	ret
     * @@error:
     * 	pop esi
     * 	mov ax, si
     * 	and ax, 0FFFh
     * 	or dx, ax
     * 	sub ebx, ecx
     * 	mov ecx, ebx
     * 	shl ecx, 12
     * 	stc
     * 	ret
     * @@error2:
     * 	pop esi
     * 	xor ecx, ecx
     * 	stc
     * 	ret
     * 	align 4
     * Lock_DMA_Region endp
     */
}


// --- esi = start region
// --- ecx = size region
// --- dl = flags

void Unlock_DMA_Region(void)
{
    /*
     * Mapped logic from Unlock_DMA_Region in VDMAD.ASM:
     * Unlock_DMA_Region proc uses esi edi ebx
     * 	sub esp, size DDS
     * 	mov edi, esp
     * 	mov [edi].DDS.dwSize, ecx
     * 	mov [edi].DDS.dwOfs, esi
     * 	mov dword ptr [edi].DDS.wSeg, 0		;clear wSeg and wID
     * 	mov dh,0
     * 	shr dl,4
     * 	mov eax,[VDS_Call_Table]
     * 	call [eax].VDSCALL.pUnlock			;call vds_unlock
     * 	lea esp,[esp+size DDS]
     * 	ret
     * 	align 4
     * 
     * Unlock_DMA_Region endp
     */
}


// --- edi = DDS
// --- al = flags

void Scatter_Lock(void)
{
    /*
     * Mapped logic from Scatter_Lock in VDMAD.ASM:
     * Scatter_Lock proc uses esi ebx
     * 	xor edx,edx
     * 	test al,1
     * 	jz @F
     * 	or dl,VDSF_PTE
     * @@:
     * 	mov eax,[VDS_Call_Table]
     * 	call [eax].VDSCALL.pScLock
     * 	ret
     * 	align 4
     * Scatter_Lock endp
     */
}


// --- edi = DDS
// --- al = flags

void Scatter_Unlock(void)
{
    /*
     * Mapped logic from Scatter_Unlock in VDMAD.ASM:
     * Scatter_Unlock proc
     * 	xor edx,edx
     * 	mov eax,[VDS_Call_Table]
     * 	call [eax].VDSCALL.pScUnlock	;this is a dummy
     * 	ret
     * 	align 4
     * Scatter_Unlock endp
     */
}


// --- esi = region
// --- ecx = size
// --- out: ebx=id, edx=phys. addr

void Request_Buffer(void)
{
    /*
     * Mapped logic from Request_Buffer in VDMAD.ASM:
     * Request_Buffer proc uses esi edi
     * 	sub esp, size DDS
     * 	mov edi, esp
     * 	mov [edi].DDS.dwSize, ecx
     * 	mov [edi].DDS.dwOfs, esi
     * 	xor edx,edx
     * 	mov dword ptr [edi].DDS.wSeg, edx
     * 	mov eax,[VDS_Call_Table]
     * 	call [eax].VDSCALL.pReqBuffer
     * 	jc @F
     * 	movzx ebx,[edi].DDS.wID
     * 	mov edx,[edi].DDS.dwPhys
     * @@:
     * 	lea esp,[esp+size DDS]
     * 	ret
     * 	align 4
     * Request_Buffer endp
     */
}


// --- ebx = id

void Release_Buffer(void)
{
    /*
     * Mapped logic from Release_Buffer in VDMAD.ASM:
     * Release_Buffer proc uses ebx edi
     * 	sub esp, size DDS
     * 	mov edi, esp
     * 	mov [edi].DDS.wID, bx
     * 	xor edx,edx					;no NOT copy out of DMA buffer!
     * 	mov eax,[VDS_Call_Table]
     * 	call [eax].VDSCALL.pRelBuffer
     * 	lea esp,[esp+size DDS]
     * 	ret
     * 	align 4
     * Release_Buffer endp
     */
}


// --- ebx = id
// --- esi = offset src
// --- edi = buffer offset
// --- ecx = buffer size

void Copy_To_Buffer(void)
{
    /*
     * Mapped logic from Copy_To_Buffer in VDMAD.ASM:
     * Copy_To_Buffer proc uses ebx esi edi
     * 
     * 	sub esp, size DDS
     * 	mov [esp].DDS.dwSize, ecx
     * 	mov ecx, edi
     * 	mov edi, esp
     * 	mov [edi].DDS.dwOfs, esi
     * 	xor edx,edx
     * 	mov [edi].DDS.wSeg, dx
     * 	mov [edi].DDS.wID, bx
     * 	mov eax,[VDS_Call_Table]
     * 	call [eax].VDSCALL.pCopyInBuf
     * 	lea esp,[esp+size DDS]
     * 	ret
     * 	align 4
     * Copy_To_Buffer endp
     */
}


// --- ebx = id
// --- esi = offset src
// --- edi = buffer offset
// --- ecx = buffer size

void Copy_From_Buffer(void)
{
    /*
     * Mapped logic from Copy_From_Buffer in VDMAD.ASM:
     * Copy_From_Buffer proc uses ebx esi edi
     * 	sub esp, size DDS
     * 	mov [esp].DDS.dwSize, ecx
     * 	mov ecx, edi
     * 	mov edi, esp
     * 	mov [edi].DDS.dwOfs, esi
     * 	xor edx,edx
     * 	mov [edi].DDS.wSeg, dx
     * 	mov [edi].DDS.wID, bx
     * 	mov eax,[VDS_Call_Table]
     * 	call [eax].VDSCALL.pCopyOutBuf
     * 	lea esp,[esp+size DDS]
     * 	ret
     * 	align 4
     * Copy_From_Buffer endp
     */
}


// end


