/*
 * jlmw.asm - --- helpers for (Watcom) C (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing JLMW.ASM.
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

#include <jlm.h>

// .code

void Get_Cur_VM_Handle(void)
{
    /*
     * Mapped logic from Get_Cur_VM_Handle in JLMW.ASM:
     * Get_Cur_VM_Handle proc stdcall public uses ebx
     *     @VMMCall Get_Cur_VM_Handle
     *     mov eax, ebx
     *     ret
     *     align 4
     * Get_Cur_VM_Handle endp
     */
}


void Begin_Nest_Exec(void)
{
    /*
     * Mapped logic from Begin_Nest_Exec in JLMW.ASM:
     * Begin_Nest_Exec proc stdcall public uses ebp pcl:ptr
     *     mov ebp,pcl
     *     @VMMCall Begin_Nest_Exec
     *     ret
     *     align 4
     * Begin_Nest_Exec endp
     */
}


void End_Nest_Exec(void)
{
    /*
     * Mapped logic from End_Nest_Exec in JLMW.ASM:
     * End_Nest_Exec proc stdcall public uses ebp pcl:ptr
     *     mov ebp,pcl
     *     @VMMCall End_Nest_Exec
     * 	ret
     *     align 4
     * End_Nest_Exec endp
     */
}


void Exec_Int(void)
{
    /*
     * Mapped logic from Exec_Int in JLMW.ASM:
     * Exec_Int proc stdcall public uses ebp intno:dword, pcl:ptr
     *     mov eax, intno
     *     mov ebp,pcl
     *     @VMMCall Exec_Int
     *     ret
     *     align 4
     * Exec_Int endp
     */
}


// end
