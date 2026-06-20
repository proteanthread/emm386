/*
 * jlmfasm.inc - --- this is an assembler include file in FASM format --- to be used for JLMs (Jemm Loadable Modules). --- it defines the API exposed by Jemm/JLoad. --- parameters, calling convention and return values are the same as --- for Win9x. --- the register usage usually is the same as for the Win32 stdcall --- convention: registers EAX, ECX and EDX may be changed inside a function, --- registers EBX, ESI, EDI, EBP are preserved. --- the exception of this rule is Get_Cur_VM_Handle, which returns the --- "handle" in EBX. -------------------------------------------------------------------------- (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing JLMFASM.INC.
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

// macro @VMMCall name_ {
// int 20h
// dw VMM_#name_
// dw 1
// }

// macro VMMJmp name_ {
// int 20h
// dw VMM_#name_ + 8000h
// dw 1
// }

// macro VxDCall name_ {
// int 20h
// dd name_
// }

// --- macro to define a hook proc (Hook_V86_Int_Chain)

// macro HookProc name_, oldvect {
// jmp $+8
// jmp dword [oldvect]
