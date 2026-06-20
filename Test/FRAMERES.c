/*
 * frameres.asm - --- create binary: --- jwasm -bin -Fo frameres.com frameres.asm (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing FRAMERES.ASM.
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

// .286
// .MODEL tiny

// .CODE

// org 100h

// start:
// mov ax,3567h
// int 21h
// mov ax,es // IV 67h set?
// or ax,bx
// jz exit
// mov ah,41h // get page frame
// int 67h
// and ah,ah // ok?
// jz exit
// mov cx,4 // cx=size mapping array
// mov dx,0 // dx=handle (0=system)
// mov si,offset map // ds:si -> mapping array
// mov ax,5000h
// int 67h
// exit:
// mov ax,4c00h
// int 21h
// align 2
// map dw -1,0,-1,1,-1,2,-1,3

// end start

