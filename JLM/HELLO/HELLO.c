/*
 * hello.asm - --- a simple JLM which displays a message onto the screen --- with the help of v86-int 21h and nested execution (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing HELLO.ASM.
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

#include <jlm.h>

// ifdef FMTPE
// --- since v2.19, JWasm's -pe option knows how to interpret linker directives supplied
// --- in the .drectve info section. Thus no extra link step is required.
// option dotname
// .drectve segment info // linker directives
// db "-subsystem:native -fixed:no"
// .drectve ends
// .hdr$1 segment use16
// %       incbin <STUB>
// .hdr$1 ends
// endif

// .DATA

// szText  db "Hello, world!", 13,10,0

// .CODE

void _main(void)
{
    /*
     * Mapped logic from _main in HELLO.ASM:
     * _main   proc
     * 
     *         push esi
     *         @VMMCall Begin_Nest_Exec     ;start nested execution
     *         mov esi, offset szText
     * @@nextitem:
     *         lodsb
     *         and al,al
     *         jz @@done
     * 
     * ;--- Call int 21h, ah=2 in v86-mode.
     * ;--- Be aware that in Jemm's context is no DOS extender installed.
     * ;--- So there is no translation for DOS functions that use pointers.
     * 
     *         mov byte ptr [ebp].Client_Reg_Struc.Client_EDX,al
     *         mov byte ptr [ebp].Client_Reg_Struc.Client_EAX+1,2
     *         mov eax,21h
     *         @VMMCall Exec_Int
     * 
     *         jmp @@nextitem
     * @@done:
     *         @VMMCall End_Nest_Exec       ;end nested execution
     *         pop esi
     *         mov eax, 80000001h	;bit 31=1: suppress JLoad msg
     *         ret
     * 
     * _main   ENDP
     */
}


// END _main

