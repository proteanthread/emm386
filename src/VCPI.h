/*
 * vcpi.inc - structure for VCPI switch from V86 to protected mode (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing VCPI.INC.
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

// V86TOPM  STRUCT
// dwCR3       DD  ? // client's CR3 value
// dwGDTOFFS   DD  ? // offset of client's GDTR value
// dwIDTOFFS   DD  ? // offset of client's IDTR value
// wLDTR       DW  ? // client's LDTR value
// wTR         DW  ? // client's TR value
// dfCSEIP     DF  ? // entry point in client
// DW  ?
// V86TOPM  ENDS

