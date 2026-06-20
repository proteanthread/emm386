/*
 * macros.inc - Interface mapping for MACROS.INC (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing MACROS.INC.
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

// CStr macro text:VARARG
// local sym
// .const
// sym db text,0
// .code
// exitm <offset sym>
// endm

