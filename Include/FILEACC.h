/*
 * fileacc.inc - --- dos file access for JLMs --- although addresses are linear, they must --- be within 64 kB distance of v86 DS register. (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing FILEACC.INC.
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

// w	textequ <word ptr>

// --- open file (for reading)
// --- edx = file name
// --- out: ax=handle

static inline void OpenFile(void)
{
    /*
     * Mapped logic from OpenFile in FILEACC.INC:
     * OpenFile proc
     * 	movzx eax,word ptr [ebp].Client_Reg_Struc.Client_DS
     * 	shl eax, 4
     * 	sub edx, eax
     * 	mov w [ebp].Client_Reg_Struc.Client_EDX, dx
     * 	mov w [ebp].Client_Reg_Struc.Client_EAX, 3D00h
     * 	mov eax,21h
     * 	@VMMCall Exec_Int
     * 	mov ax,w [ebp].Client_Reg_Struc.Client_EAX
     * 	bt [ebp].Client_Reg_Struc.Client_EFlags,0
     * 	ret
     * OpenFile endp
     */
}


// --- close file
// --- ebx = file handle

static inline void CloseFile(void)
{
    /*
     * Mapped logic from CloseFile in FILEACC.INC:
     * CloseFile proc
     * 	mov w [ebp].Client_Reg_Struc.Client_EBX, bx
     * 	mov w [ebp].Client_Reg_Struc.Client_EAX, 3E00h
     * 	mov eax,21h
     * 	@VMMCall Exec_Int
     * 	bt [ebp].Client_Reg_Struc.Client_EFlags,0
     * 	ret
     * CloseFile endp
     */
}


// --- get file size
// --- ebx = file handle
// --- out: eax=file size

static inline void GetFileSize(void)
{
    /*
     * Mapped logic from GetFileSize in FILEACC.INC:
     * GetFileSize proc
     * 	mov w [ebp].Client_Reg_Struc.Client_EBX, bx
     * 	mov w [ebp].Client_Reg_Struc.Client_EAX, 4202h   ;goto EOF
     * 	mov w [ebp].Client_Reg_Struc.Client_ECX, 0
     * 	mov w [ebp].Client_Reg_Struc.Client_EDX, 0
     * 	mov eax,21h
     * 	@VMMCall Exec_Int
     * 	bt [ebp].Client_Reg_Struc.Client_EFlags,0
     * 	jc exit
     * 	mov ax, w [ebp].Client_Reg_Struc.Client_EDX
     * 	shl eax, 16
     * 	mov ax, w [ebp].Client_Reg_Struc.Client_EAX
     * 	push eax
     * 	mov w [ebp].Client_Reg_Struc.Client_EAX, 4200h	 ;goto start
     * 	mov w [ebp].Client_Reg_Struc.Client_ECX, 0
     * 	mov w [ebp].Client_Reg_Struc.Client_EDX, 0
     * 	mov eax,21h
     * 	@VMMCall Exec_Int
     * 	pop eax
     * 	bt [ebp].Client_Reg_Struc.Client_EFlags,0
     * exit:
     * 	ret
     * GetFileSize endp
     */
}


// --- Read file into buffer
// --- bx = file handle
// --- cx = bytes to read
// --- edx = buffer (in conv. memory)
// --- out: eax=bytes read

static inline void ReadFile(void)
{
    /*
     * Mapped logic from ReadFile in FILEACC.INC:
     * ReadFile proc
     * 	movzx eax,w [ebp].Client_Reg_Struc.Client_DS
     * 	shl eax, 4
     * 	sub edx, eax
     * 	mov w [ebp].Client_Reg_Struc.Client_EBX, bx
     * 	mov w [ebp].Client_Reg_Struc.Client_ECX, cx
     * 	mov w [ebp].Client_Reg_Struc.Client_EDX, dx
     * 	mov w [ebp].Client_Reg_Struc.Client_EAX, 3F00h
     * 	mov eax,21h
     * 	@VMMCall Exec_Int
     * 	movzx eax, w [ebp].Client_Reg_Struc.Client_EAX
     * 	bt [ebp].Client_Reg_Struc.Client_EFlags,0
     * 	ret
     * ReadFile endp
     */
}


