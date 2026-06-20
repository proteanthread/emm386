/*
 * x86.inc - --- page table flags (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing X86.INC.
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

#define PTF_PRESENT 001h
#define PTF_RW 002h
#define PTF_USER 004h
#define PTF_PWT 008h // page write through
#define PTF_PCD 010h // page cache disable
#define PTF_ACCESS 020h // page accessed
#define PTF_DIRTY 040h // page dirty
#define PTF_4MB 080h // PDEs: 4MB PDE
#define PTF_GBL 100h // page global (P3+)

// --- features, returned by CPUID in EDX
#define CF_FPU 0001h
#define CF_VME 0002h
#define CF_DE 0004h // debugging ext
#define CF_PSE 0008h // Page Size Ext (4MB)
#define CF_TSC 0010h // Time Stamp Counter
#define CF_MSR 0020h // RDMSR/WRMSR supported
#define CF_PAE 0040h // Physical Address Ext
#define CF_MCE 0080h // Machine Check Exceptions
#define CF_CX8 0100h // CMPXCHG8B supported
#define CF_APIC 0200h // APIC exists
// rsvd       equ 0400h;
#define CF_SEP 0800h // SYSENTER/SYSEXIT supported
#define CF_MTRR 1000h
#define CF_PGE 2000h

// --- flags in CR4
#define CR4_VME 01h
#define CR4_PVI 02h
#define CR4_TSD 04h // 1=time stamp disable
#define CR4_DE 08h
#define CR4_PSE 10h // 1=page size extensions enabled
#define CR4_PAE 20h
#define CR4_MCE 40h
#define CR4_PGE 80h

// --- descriptor

// DESCRIPTOR struct
// wLimit  dw ? // +0
// wA0015  dw ? // +2
// bA1623  db ? // +4
// bAttr   db ? // +5
// bLimEx  db ? // +6
// bA2431  db ? // +7
// DESCRIPTOR ends

// GATE struct
// wOfsLo dw ?
// wSeg   dw ?
// wAttr  dw ?
// wOfsHi dw ?
// GATE ends

// --- TSS structure
// --- the only fields in the TSS which are needed are tsEsp0, tsSS0
// --- and tsOfs. Jemm386 will never switch tasks.

// TSSSEG  struct
// dd ? // +00 selector
// tsEsp0  dd ? // +04
// tsSS0   dd ? // +08
// dq ? // +0C
// dq ? // +14
// tsCR3   dd ? // +1C
// tsEip   dd ? // +20
// tsEfl   dd ? // +24
// tsEax   dd ? // +28
// tsEcx   dd ? // +2C
// tsEdx   dd ? // +30
// tsEbx   dd ? // +34
// tsEsp   dd ? // +38
// tsEbp   dd ? // +3C
// tsEsi   dd ? // +40
// tsEdi   dd ? // +44
// tsES    dd ? // +48
// tsCS    dd ? // +4C
// tsSS    dd ? // +50
// tsDS    dd ? // +54
// tsFS    dd ? // +58
// tsGS    dd ? // +5C
// tsLDTR  dd ? // +60
// tsFlags dw ? // +64
// tsOfs   dw ? // +66
// TSSSEG  ends

// --- stack frame for PUSHAD

// PUSHADS struct
// rEDI    dd ?
// rESI    dd ?
// rEBP    dd ?
// dd ? // reserved
// rEBX    dd ?
// rEDX    dd ?
// rECX    dd ?
// rEAX    dd ?
// PUSHADS ends

// --- RETF stack frame for 16bit

// RETFWS struct
// union
// struct
// _Ip     dw ?
// _Cs     dw ?
// ends
// _CsIp   dd ?
// ends
// RETFWS ends

// --- IRET stack frame for 16bit

// IRETWS struct
// RETFWS <?>
// _Fl     dw ?
// IRETWS ends

// --- IRET stack frame for 32bit protected-mode

// IRETDS struct
// _Eip    dd ? // +0
// _Cs     dd ? // +4
// _Efl    dd ? // +8
// IRETDS ends

// --- stack frame expected by ring0-IRETD to switch to v86-mode

// IRETDV86 struct
// _Eip    dd ? // +0
// _Cs     dd ? // +4
// _Efl    dd ? // +8
// _Esp    dd ? // +12
// _Ss     dd ? // +16
// _Es     dd ? // +20
// _Ds     dd ? // +24
// _Fs     dd ? // +28
// _Gs     dd ? // +32
// IRETDV86 ends

// --- bit positions of CPUID edx
#define CPUID_VME 1 // =2
#define CPUID_PGE 13 // =2000h
#define CPUID_PAT 16 // =10000h

