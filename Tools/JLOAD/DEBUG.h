/*
 * debug.inc - *** debug macros and equates (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing DEBUG.INC.
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

#define ?USEMONO 0

// ifndef ?RMDBG
// ?RMDBG   = 0 // trace real-mode
// endif
// ifndef ?INITDBG
// ?INITDBG = 0 // trace init
// endif
// ifndef ?PEDBG
// ?PEDBG   = 0 // trace PE loader
// endif
// ifndef ?IODBG
// ?IODBG   = 0 // trace IO trapping
// endif
// ifndef ?PAGEDBG
// ?PAGEDBG = 0 // trace page memory
// endif
// ifndef ?JLMDBG
// ?JLMDBG  = 0 // trace jlm handling
// endif
// ifndef ?DMADBG
// ?DMADBG  = 0 // trace DMA handling
// endif
// ifndef ?V86HOOKDBG
// ?V86HOOKDBG  = 0 // trace v86 hook chain handling
// endif
// ifndef ?INT2FHOOKDBG
// ?INT2FHOOKDBG  = 0 // trace Int2F hook handling
// endif

// ifdef _DEBUG
// ?TRACE = ?INITDBG + ?PEDBG + ?IODBG + ?PAGEDBG + ?JLMDBG + ?DMADBG + ?V86HOOKDBG + ?INT2FHOOKDBG
// else
// ?TRACE = 0
// endif

// dprintf   proto c :ptr, :vararg

// ifdef _DEBUG

// @dprintf macro bCond, fmt, args:vararg
// if bCond
// ifnb <args>
// invoke dprintf, CStr(fmt), args
// else
// invoke dprintf, CStr(fmt)
// endif
// endif
// endm

// else

// @dprintf textequ < // >

// endif

