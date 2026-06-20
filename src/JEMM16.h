/*
 * jemm16.inc - --- globals for jemm16.asm and init16.asm (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing JEMM16.INC.
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

// if ?INTEGRATED
#define NAMEEXE <"JEMMEX">
#define NAMEMOD <"JemmEx">
// else
#define NAMEEXE <"JEMM386">
#define NAMEMOD <"Jemm386">
// endif

#define @XBDA 40Eh
#define @MEM_SIZE 413h

// --- modes for mainex()
#define EXECMODE_SYS 0 // launched as dos device driver
#define EXECMODE_EXE 1 // launched as application

// InitJemm proto c
// IsJemmInstalled proto c
// EmmInstallcheck proto c
// XMSinit proto c
// CheckIntHooks proto stdcall :WORD
// UnloadJemm proto c
// if ?INTEGRATED
#define ?XMS_STATICHDLS 10 // free xms handles to be used for int 15h, ax=e820h
// I15SetHandle proto c
// I15AllocMemory proto stdcall :WORD, :DWORD
// GetEMBBase proto stdcall: word
// endif

// mainex proto c :WORD, :ptr BYTE

// printf proto c :ptr byte, :VARARG
// _memicmp proto c :ptr BYTE, :ptr BYTE, :WORD

// externdef c jemmini:JEMMINIT
// externdef c XMSdriverAddress:far16 ptr
// externdef c sig1:BYTE
// externdef c sig2:BYTE
// externdef bVerbose:byte

// if ?INTEGRATED
// externdef c xms_num_handles:WORD
// externdef c xms_max:dword
// if ?XMS35
// externdef c xms_smax:dword
// externdef c xms_maxhigh:dword
// externdef c xms_smax_noe820:byte
// externdef c xms_smem_used:byte
#define ?XMS_DEFHDLS 48
// else
#define ?XMS_DEFHDLS 32
// endif
#define ?XMS_MAXHDLS 128
// externdef c xms_mem_free:dword
// externdef c xms_mem_largest:dword
// externdef c xms_mem_highest:dword
// endif
