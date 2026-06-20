/*
 * jemm.inc - --- common definitions for both Jemm32 and Jemm16 (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing JEMM.INC.
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

// option casemap:none

#define LF 0AH
#define CR 0DH

#define ?VERSIONHIGH 5
#define ?VERSIONLOW 87

// ifndef ?INTEGRATED
#define ?INTEGRATED 0 // 1=integrated version (XMS + EMM)
// endif
// ifndef ?KD
#define ?KD 0 // 1=support for PL0 debugger
// endif
#define ?DMAPT 1 // 1=enable DMA port trapping support
#define ?VDS 1 // 1=enable VDS support
#define ?VCPI 1 // 1=enable VCPI support
#define ?EMX 1 // 1=EMX compat switch supported
#define ?SB 1 // 1=SB compat switch supported (useful?)
#define ?EMMXXXX0 1 // 1=implement EMMXXXX0 IOCTL
#define ?A20XMS 1 // 1=emu A20 by trapping XMS functions
#define ?A20PORTS 1 // 1=emu A20 by trapping ports (92, 64, 60)
#define ?VME 1 // 1=support P1+ VME extension
#define ?LOAD 1 // 1=support LOAD command line option
#define ?PGE 1 // 1=support PGE on P3+ (requires ?INVLPG!)
#define ?MOVEHIGH 1 // 1=support moving in first UMB
#define ?UNLOAD 1 // 1=support UNLOAD option
#define ?FASTBOOT 1 // 1=support FASTBOOT option
#define ?SPLIT 1 // 1=support SPLIT option
#define ?HOOK13 1 // 1=hook int 13h/40h for DMA access trapping
#define ?V86EXC0D 1 // 1=support V86EXC0D option
#define ?ADDCONV 1 // 1=add A000h to conv memory if I=A000-XXXX

// if ?INTEGRATED
#define ?MOVEXBDA 1 // 1=support MOVEXBDA option; move XBDA into UMB
// else
#define ?MOVEXBDA 1
// endif
#define ?MOVEXBDAPM 1 // 1=move XBDA in protected-mode

#define ?STKSIZE 200H // Size monitor stack for non-reentrant execution
#define ?HLPSTKSIZE 80h // size of help stack for VCPI

// ?GDTOFS         equ ?HLPSTKSIZE
// if ?KD
#define GDT_SEL 48h
// endif

// --- define v86-monitor breakpoint ( used to switch back to protected-mode )
// --- the advantage of ARPL is that it's causing an "invalid opcode" exception,
// --- so it won't have to be dealt with in the rather "longish" GPF handler chain.
#define ?BPOPC 0F4h // default is HLT
// ?BPOPC         equ 063h    ; alternately ARPL

#define ?XMS35COMPAT 1 // 1=monitor is XMS v3.5 compatible (is aware of memory blocks beyond 4GB)

#define V86IOPL 3 // IOPL for v86-mode

#define SMAP 534d4150h

// E820MAP struct
// baselow  dd ?
// basehigh dd ?
// lenlow   dd ?
// lenhigh  dd ?
// type_    dd ?
// E820MAP ends

// if ?INTEGRATED

// ifndef ?XMS35
#define ?XMS35 1 // std=1, 1=support super-extended memory beyond 4GB
// endif

// --- A20 switch methods (must match order in "methods" table)

#define A20_KBC 0
#define A20_PS2 1
#define A20_BIOS 2
#define A20_ALWAYSON 3
#define A20_FAST 4
#define A20_PORT92 5

// if ?XMS35
#define DRIVER_VER 350h+2
#define INTERFACE_VER 351h
#define INTERFACE_VER_MAXSEXT0 300h // xms version if MAXSEXT=0 is set
// else
#define DRIVER_VER 300h+29
#define INTERFACE_VER 300h
// endif

// endif

// if ?EMMXXXX0

// --- ioctl read functions
#define EMMDEV_GETAPI 0
#define EMMDEV_GEMMIS 1
#define EMMDEV_VERSION 2
#define EMMDEV_GETRES 4
#define EMMDEV_SYSVARS 6
#define EMMDEV_GETUMBS 7
#define EMMDEV_GETSTAB 8 // get VMM info
// --- ioctl write functions
#define EMMDEV_UPDATE 15

// --- structure returned by Jemm386 if a SYSVARS request was made
// --- for device "EMMXXXX0"

// EMX06 struct
// e06_NoEMS       db ? // +0
// e06_Frame       dw ? // +1 segment
// e06_NoVCPI      db ? // +3
// e06_DMABuff     dd ? // +4 physical address DMA buffer
// e06_NoPGE       db ? // +8
// db ?
// db ?
// db ?
// e06_DMASize     dw ? // +12 in KB
// e06_NoVME       db ? // +14
// e06_NoA20       db ? // +15
// e06_VCPITotal   dd ? // +16 VCPI pages total (def 120 MB)
// e06_VCPIUsed    dd ? // +20 VCPI pages allocated
// EMX06 ends

// --- this is the structure for a "Get VMM info" request

// EMX08 struct
// e08_ServiceTable    dd ?
// e08_BPTable         dd ?
// e08_BPTableRM       dd ?
// e08_GDTR            df ?
// e08_IDTR            df ?
// e08_TR              dw ?
// e08_FlatCS          dw ?
// EMX08 ends

// --- this is the structure for an "UPDATE" request with IoctlWrite

// EMX15W struct
// e15_bVME    db ?
// e15_bA20    db ?
// e15_bVCPI   db ?
// e15_bPGE    db ?
// EMX15W ends

// endif

#define DMABUFFDEFAULT 64 // /* DMA buffer default size in kB */
#define MINMEM16K_DEFAULT 0 // /* MIN= default (in 16 kB units) */

// --- data for monitor initialization

// JEMMINIT struct 4
// MonitorStart      dd 0 // memory block start address (XMS/I15)
// MonitorEnd        dd 0 // memory block end address (XMS/I15)
// MaxPhysMem        dd 0 // highest physical memory address (XMS/I15)
// MaxMem16k         dd -1 // MAX mem in 16 kB units (default 7680)
// MinMem16k         dd MINMEM16K_DEFAULT // MIN mem in 16 kB units (default 0)
// XMSHandleTable    dd 0 // XMS handle table (FAR16)
// PageMap           dd 0 // conventional memory page map (FLAT!)
// MaxEMSPages       dw 0 // EMS max 16 kB pages (default 2048)
// XMSControlHandle  dw 0 // XMS memory block handle (both Jemm386 & JemmEx)
// DMABufferSize     dw DMABUFFDEFAULT // DMA buffer size in kB
// Frame             dw 0E000h // EMS page frame
// Border            dw 04000h // EMS border for mappable pages
// ResUMB            dw 0 // UMB para where the resident part was moved to
// if ?INTEGRATED
// HmaMin            dw 0 // min request in KB for HMA
// X2Max             dw -1
// if ?XMS35
// xms_version       dw INTERFACE_VER
// endif
// endif
// if ?KD
// kdofs             dd 0
// kdseg             dw 0
// endif
// NoEMS             db 0
// NoFrame           db 0
// NoPool            db 0
// AltBoot           db 0   ;v5.80: has become a bit in V86Flags
// NoVME             db 1
// NoVDS             db 0
// NoPGE             db 1
// NoA20             db 0
// NoVCPI            db 0
// NoInvlPg          db -1
// V86Flags          db 0
// NoRAM             db 0
// NoHigh            db 0
// NumUMBs           db 0 // number of UMBs installed
// if ?INTEGRATED
// A20Method         db -1
// endif
// JEMMINIT ends

// --- V86Flags equates

#define V86F_SB 1 // soundblaster driver compat
#define V86F_NOCHECK 2 // flag NOCHECK option
// if ?EMX
#define V86F_EMX 4 // EMX compat
// endif
// if ?FASTBOOT
#define V86F_FASTBOOT 8 // fastboot active
#define V86F_FASTBOOT_B 3 // bit 3
// endif
// if ?V86EXC0D
#define V86F_V86EXC0D 16 // V86EXC0D active
// endif
// if ?MOVEXBDA
#define V86F_MOVEXBDA 32 // MOVEXBDA active
// endif
#define V86F_ALTBOOT 64 // ALTBOOT active

// -- 120 MB max VCPI memory (in 16 kB units)
// -- keep this value low for buggy VCPI clients
// -- that fail with large free amounts

#define MAXMEM16K_DEFAULT 1E00h // 120M in 16K blocks

#define MAX_EMS_PAGES_POSSIBLE 8000h

// --- this is the table of RSEG offsets
// --- the values are offsets in the RSEG segment
// --- there is just one instance defined in jemm16.asm.

// RSOFS struct
// wSizeRes    dw ? // size resident part
// wBpTab      dw ? // offset of BP table
// if ?DMAPT
// wRFlags     dw ? // offset of DMA flags
// endif
// RSOFS ends

// --- bRFlags values
#define RFL_COPYBUFFER 1 // 1=copy out of DMA buffer
#define RFL_DMAOP 2 // 1=DMA op started

// --- max number of UMB blocks
// --- this differs significantly from how UMBs are handled by
// --- MS Himem. With MS Himem, each UMB has a 16-byte header
// --- (quite similarly to MCBs), and there's no UMB block limit.

#define UMB_MAX_BLOCKS 8

// UMBBLK struct
// wSegm   dw ? // segment address
// wSize   dw ? // size in paras, high bit used as flag free/allocated
// UMBBLK ends

#define UMB_ALLOCATED 80h // flag in wSize+1

// --- macros

// @BPOPC macro
// db ?BPOPC
// endm

