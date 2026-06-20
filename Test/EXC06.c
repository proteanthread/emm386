/*
 * exc06.asm - *** EXC 06 - requires a P6+ cpu (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing EXC06.ASM.
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
// .686

// .code

// start:
// ud2
// mov ah, 4Ch
// int 21h

// END start
