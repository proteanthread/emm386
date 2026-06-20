/*
 * debug16.inc - Interface mapping for DEBUG16.INC (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing DEBUG16.INC.
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

// ifndef ?INITRMDBG
#define ?INITRMDBG 0 // log real-mode init
// endif
// ifndef ?XMSRMDBG
#define ?XMSRMDBG 0 // log XMS calls
// endif
// ifndef ?EMXRMDBG
#define ?EMXRMDBG 0 // log EMMXXXX0 calls
// endif
// ifndef ?UNLRMDBG
#define ?UNLRMDBG 0 // log unload
// endif
#define ?RMDBG ?INITRMDBG + ?XMSRMDBG + ?EMXRMDBG + ?UNLRMDBG // debug displays in real-mode

#define ?USEMONO 0 // 1=use monochrome monitor for dbg displays

// ifdef _DEBUG

// dprintf proto c :ptr, :vararg

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
