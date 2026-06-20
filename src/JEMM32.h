/*
 * jemm32.inc - --- these files contains things for 32-bit ONLY (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing JEMM32.INC.
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

#define ?RING0EXC 1 // 1=display more infos on exc in ring 0
#define ?ROMRO 1 // 1=make ROM page FF000 r/o
// ?ALTBOOT       EQU 1       ; 1=support ALTBOOT option
#define ?MMASK 1 // 1=trap DMA master mask port (0F, DE)
#define ?INVLPG 1 // 1=use INVLPG opcode on 80486+
#define ?CLEARHIGHESP 1 // 1=clear hiword(esp) for buggy VCPI clients
// ifndef ?AUXIO
#define ?AUXIO 0 // 1=use COMx for I/O, 0=use INT 10h/29h and INT 16h
// endif
#define ?USEINT10 1 // 1=use int 10h for output, 0=use int 29h
#define ?SERVTABLE 1 // 1=expose service table (for JLoad)
#define ?FASTMON 1 // 1=faster monitor jmp for int 00-0F
#define ?SHAREDGDT 1 // 1=place GDT in shared address space
#define ?EXC10 1 // 1=detect exception 10h (floating point)
#define ?I41SUPP 1 // 1=support Int 41h in protected-mode
#define ?ALT_TRAPHDL 1 // v5.85: 1=call alternate trap handler for system ports
// ifndef ?SHAREDIDT
#define ?SHAREDIDT 0 // std=?, 1=place IDT in shared address space
// endif

#define ?CODEIN2PAGE 0 // 1=move code away from first page
#define ?USETRIPLEFAULT 0 // 1=use triple fault to reboot
// ifndef ?SAFEKBD            ; v5.87: obsolete
// ?SAFEKBD        equ 0       ; 1=use original vector for int 16h kbd poll
// endif
#define ?DYNTRAPP60 1 // 1=trap port 60h conditionally only
// ifndef ?MAPEXTMEM          ; v5.86: obsolete; now covered by SB option
// ?MAPEXTMEM      equ 0       ; 1="identity map" extended memory (SBEINIT.COM/386SWAT may need this)
// endif

#define GDT_SIZE 200h // size of GDT

// --- the following equates define Jemm's memory layout:
// --- Jemm starts at 110000h with data and code (4-5 pages)
// --- system space is F8000000h-F83FFFFFh:
// ---  + 0000h: reserved (uncommitted)
// ---  + 1000h: page directory
// ---  + 2000h: page table 0
// ---  + 3000h: system page table
// ---  + 4000h: ring 0 stack (4k)
// ---  + 5000h: task state segment
// ---  + a: page pool descriptors, EMS handle/status tables
// ---  + b: shadowed HMA (if XMS handle array is in HMA)
// ---  + c: DMA buffer
// ---  + d: scratch region for physical page mapping

#define ?BASE 110000h // base of Jemm 32bit code
#define ?SYSBASE 0F8000000h // system space
#define ?SYSLINEAR ?SYSBASE+1000h
#define ?PAGEDIR ?SYSBASE+1000h // linear address page dir
#define ?PAGETAB0 1000h // offset for page table 0
#define ?PAGETABSYS 2000h // offset for system page table
#define ?TOS ?SYSLINEAR + 3000h + 1000h // top of ring 0 stack
#define ?TSSLEN 68h+32+2000h+8 // size of TSS "segment"

// --- other constants

#define @XBDA 40Eh // segment of XBDA
#define @MEM_SIZE 413h // base memory size in KB
#define @KB_FLAG 417H // Status SHIFT/CTRL/ALT etcetera.
#define @RESET_FLAG 472H // Flag for Warmboot (=1234h)

// if ?DMAPT or ?VDS
#define ?DMABUFFMAX 128 // max DMA-Buffer size in kB
// endif

#define ?FASTENTRIES 32-1

#include <x86.h>

// --- EMS constants

// -- 32M max EMS memory (in 16K EMS page units), always even
// -- this constant is also defined in Jemm386c.c

#define EMS_MAX_HANDLES 255 // std=255, the EMS handle is a byte value
#define EMS_MAX_PAGES_ALLOWED 800h // 2048*16 = 32 MB
#define EMS_MAXSTATE 64 // std=64, number of EMS save state items

// --- EMS handle descriptor

// EMSHD struct
// ehd_wIdx    dw ? // index of EMSPD for first page
// ehd_bFlags  db ?
// ehd_bSS     db ? // saved state index
// EMSHD ends

// --- values for flags

#define EMSH_USED 1

// EMSSTAT struct
// dPg0    dd ?
// dPg1    dd ?
// dPg2    dd ?
// dPg3    dd ?
// EMSSTAT ends

// --- flags for pool descriptor block

#define PBF_DONTEXPAND 1 // don't try to expand this block
#define PBF_DONTFREE 2 // don't try to free this block

// --- selector definitions
// --- FLAT_CODE_SEL must be first descriptor in GDT

#define FLAT_CODE_SEL 1 * sizeof DESCRIPTOR
#define FLAT_DATA_SEL 2 * sizeof DESCRIPTOR
#define V86_TSS_SEL 3 * sizeof DESCRIPTOR
#define REAL_CODE_SEL 4 * sizeof DESCRIPTOR
#define REAL_DATA_SEL 5 * sizeof DESCRIPTOR
// if ?CLEARHIGHESP
#define V86_STACK_SEL 6 * sizeof DESCRIPTOR
// endif
// if ?KD
#define GDT_SEL 48h
// endif

// --- equates from JLM.INC
// --- OUT_INSTR, WORD_IO and DWORD_IO are used as offset in tables
// --- and hence cannot be changed; see DMA.ASM!

#define OUT_INSTR 04h
#define WORD_IO 08h
#define DWORD_IO 10h
#define STRING_IO 20h
#define REP_IO 40h

// DMAREQ struct
// bMode       db ? // saved "mode" value (register 0Bh or D6h)
// bFlags      db ? // flags (DMAF_ENABLED)
// BlockLen    dw ? // saved "block length" value
// BaseAdr     dw ? // saved "block address" value
// PageReg     db ? // saved "page register" value
// cDisable    db ? // is automatic translation disabled? (VDS)
// DMAREQ ends

// BPTABLE struct
// pInt06  dd ?
// pInt19  dd ?
// pInt67  dd ?
// if ?VDS
// pInt4B  dd ?
// endif
// pBack   dd ?
// pI1587  dd ?
// if ?DMAPT
// p1340   dd ?
// endif
// pXMS    dd ?
// pStrat  dd ?
// pDev    dd ?
// if ?UNLOAD
// pUnload dd ?
// endif
// BPTABLE ends

// --- client register structure in V86 monitor.
// --- consists of:
// --- 1. PUSHAD
// --- 2. Int#
// --- 3. v86-exception frame with error code

// Client_Reg_Struc  struct
// Client_EDI  dd ? // +0
// Client_ESI  dd ? // +4
// Client_EBP  dd ? // +8
// Client_res0 dd ? // +12
// Client_EBX  dd ? // +16
// Client_EDX  dd ? // +20
// Client_ECX  dd ? // +24
// Client_EAX  dd ? // +28
// Client_Int  dd ? // +32
// Client_Error dd ? // +36
// Client_EIP  dd ? // +40
// Client_CS   dd ? // +44
// Client_EFlags dd ? // +48
// Client_ESP  dd ? // +52
// Client_SS   dd ? // +56
// Client_ES   dd ? // +60
// Client_DS   dd ? // +64
// Client_FS   dd ? // +68
// Client_GS   dd ? // +72
// Client_Reg_Struc ends

// --- entry in IO trap table
// --- this is for the default port trap handler;
// --- it handles byte ports only.

// IOTRAPENTRY struct
// bStart  db ?
// bEnd    db ?
// dwProc  dd ?
// IOTRAPENTRY ends

// --- Jemm Service Table
// --- this structure is filled and returned by
// --- IoCtl call 8. Used by JLoad.

// if ?SERVTABLE
// VMM_SERV_TABLE struct
// pSimulate_Int           dd ?
// pSimulate_Iret          dd ?
// pSimulate_Far_Call      dd ?
// pSimulate_Far_Ret       dd ?
// pBegin_Nest_Exec        dd ?
// pExec_Int               dd ?
// pResume_Exec            dd ?
// pEnd_Nest_Exec          dd ?
// pSimulate_IO            dd ?
// pYield                  dd ?
// pVDS_Call_Table         dd ?
// pVCPI_Call_Table        dd ?
// pIO_Trap_Table          dd ?
// pV86_Monitor            dd ? // linear address of V86_Monitor() (obsolete with ppV86Hooks)
// pStackCurr              dd ? // linear address of dwStackCurr (obsolete with ppV86Hooks)
// pMoveMemory             dd ? // linear address of MoveMemory()
// ppV86Hooks              dd ? // v5.85: address of pV86Hooks variable
// pV86Faults              dd ? // v5.86: address of V86Faults
// pControlProc            dd ? // v5.86: address of ControlProc (called by Jemm on certain events)
// VMM_SERV_TABLE ends
// endif

// --- messages for ControlProc callout (matches Win9x VMM values)
#define SYSTEM_EXIT 5
#define DEVICE_REBOOT_NOTIFY 17h

// --- macros

// @mov_eax_cr4 macro
// db 0fh, 20h, 0E0h
// endm
// @mov_ecx_cr4 macro
// db 0fh, 20h, 0E1h
// endm

// @mov_cr4_eax macro
// db 0fh, 22h, 0E0h
// endm
// @mov_cr4_ecx macro
// db 0fh, 22h, 0E1h
// endm

// @cpuid  macro
// cpuid
// db 0Fh,0A2h
// endm

// @wrmsr  macro
// db 0Fh,030h
// endm

// @rdtsc  macro
// db 0Fh,031h
// endm

// @rdmsr  macro
// db 0Fh,032h
// endm

// --- macro to get a pointer to PTEs
// --- reg: register to set
// --- ofs: offset in page dir/page table
// --- bIsConst: if blank, then ofs is/contains a register

// @GETPTEPTR macro reg, ofs, bIsConst
// if 0
// mov reg,[PAGEDIR]
// ifnb <ofs>
// lea reg, [reg+ofs]
// endif
// else
// ifnb <ofs>
// ifnb <bIsConst>
// mov reg,?PAGEDIR+ofs
// else
// lea reg,[?PAGEDIR+ofs]
// endif
// else
// mov reg, ?PAGEDIR
// endif
// endif
// endm

// @GETPTE macro reg, ofs, bIsConst
// if 0
// mov reg,[PAGEDIR]
// mov reg,[reg+ofs]
// else
// ifnb <bIsConst>
// mov reg,ds:[?PAGEDIR+ofs]
// else
// mov reg,[?PAGEDIR+ofs]
// endif
// endif
// endm

// --- macro to convert a pointer to PTEs in system page table
// --- to the according linear address

// @SYSPTE2LINEAR macro reg, pPT
// mov reg, pPT
// sub reg, ?SYSLINEAR+?PAGETABSYS
// shl reg, 10
// add reg, ?SYSBASE
// endm
