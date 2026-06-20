/*
 * exc00.asm - *** EXC 00 (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing EXC00.ASM.
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
// .model small
// .stack 2048
// .dosseg
// .386

// .code

// start:
// mov dx, -1
// mov cx, 8000h
// div cx
// mov ah, 4Ch
// int 21h

// END start
