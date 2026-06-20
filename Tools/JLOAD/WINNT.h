/*
 * winnt.inc - Interface mapping for WINNT.INC (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing WINNT.INC.
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

// ifndef WINNT_INCLUDED

// pushcontext listing
// .nolist
// .xcref

// ifdef @list_on
// .list
// .cref
// endif

#define WINNT_INCLUDED 1

// HANDLE		typedef ptr
// ifndef HINSTANCE
// HINSTANCE	typedef HANDLE
// endif

#define MINCHAR 80h
#define MAXCHAR 7fh
#define MINSHORT 8000h
#define MAXSHORT 7fffh
#define MINLONG 80000000h
#define MAXLONG 7fffffffh
#define MAXBYTE 0ffh
#define MAXWORD 0ffffh
#define MAXDWORD 0ffffffffh

#define SECTION_QUERY 00001h
#define SECTION_MAP_WRITE 00002h
#define SECTION_MAP_READ 00004h
#define SECTION_MAP_EXECUTE 00008h
#define SECTION_EXTEND_SIZE 00010h

#define DUPLICATE_CLOSE_SOURCE 00000001h
#define DUPLICATE_SAME_ACCESS 00000002h

#define PAGE_NOACCESS 1h
#define PAGE_READONLY 2h
#define PAGE_READWRITE 4h
#define PAGE_WRITECOPY 8h
#define PAGE_EXECUTE 10h
#define PAGE_EXECUTE_READ 20h
#define PAGE_EXECUTE_READWRITE 40h
#define PAGE_EXECUTE_WRITECOPY 80h
#define PAGE_GUARD 100h
#define PAGE_NOCACHE 200h
#define MEM_COMMIT 1000h
#define MEM_RESERVE 2000h
#define MEM_DECOMMIT 4000h
#define MEM_RELEASE 8000h
#define MEM_FREE 10000h
#define MEM_PRIVATE 20000h
#define MEM_MAPPED 40000h
#define MEM_RESET 80000h
// #define MEM_TOP_DOWN       0x100000
// #define SEC_FILE           0x800000
// #define SEC_IMAGE         0x1000000
// #define SEC_RESERVE       0x4000000
// #define SEC_COMMIT        0x8000000
// #define SEC_NOCACHE      0x10000000

#define HEAP_NO_SERIALIZE 00000001h
#define HEAP_GROWABLE 00000002h
#define HEAP_GENERATE_EXCEPTIONS 00000004h
#define HEAP_ZERO_MEMORY 00000008h
#define HEAP_REALLOC_IN_PLACE_ONLY 00000010h

// MEMORY_BASIC_INFORMATION STRUCT
// BaseAddress       DWORD      ?
// AllocationBase    DWORD      ?
// AllocationProtect DWORD      ?
// RegionSize        DWORD      ?
// State             DWORD      ?
// Protect           DWORD      ?
// Type_             DWORD      ?
// MEMORY_BASIC_INFORMATION ENDS



// RTL_CRITICAL_SECTION struct
// DebugInfo       dd ?
// LockCount       sdword ?
// RecursionCount  sdword ?
// OwningThread    dd ?
// LockSemaphore   dd ?
// SpinCount		dd ?
// RTL_CRITICAL_SECTION ends

#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH 2
#define DLL_THREAD_DETACH 3
#define DLL_PROCESS_DETACH 0

#define LANG_NEUTRAL 0
#define SUBLANG_NEUTRAL 0
#define SUBLANG_DEFAULT 1
#define SUBLANG_SYS_DEFAULT 2
#define SORT_DEFAULT 0

// MAKELANGID	macro p:req, s:req
// exitm <(s shl 10) or p>
// endm
// MAKELCID macro lgid:req, srtid:req
// exitm <(srtid shl 16) or lgid>
// endm

#define CSTR_LESS_THAN 1 // // string 1 less than string 2
#define CSTR_EQUAL 2 // // string 1 equal to string 2
#define CSTR_GREATER_THAN 3 // // string 1 greater than string 2

#define NORM_IGNORECASE 000000001h // /* ignore case */



#define LANG_SYSTEM_DEFAULT MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT)
#define LANG_USER_DEFAULT MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
#define LOCALE_SYSTEM_DEFAULT MAKELCID(LANG_SYSTEM_DEFAULT, SORT_DEFAULT)
#define LOCALE_USER_DEFAULT MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT)


#define FILE_ATTRIBUTE_READONLY 00000001h
#define FILE_ATTRIBUTE_HIDDEN 00000002h
#define FILE_ATTRIBUTE_SYSTEM 00000004h
#define FILE_ATTRIBUTE_DIRECTORY 00000010h
#define FILE_ATTRIBUTE_ARCHIVE 00000020h
#define FILE_ATTRIBUTE_NORMAL 00000080h
#define FILE_ATTRIBUTE_TEMPORARY 00000100h
#define FILE_ATTRIBUTE_COMPRESSED 00000800h
#define FILE_ATTRIBUTE_OFFLINE 00001000h

#define FILE_SHARE_READ 01
#define FILE_SHARE_WRITE 02
#define FILE_SHARE_DELETE 04


#define STATUS_WAIT_0 000000000h
#define STATUS_ABANDONED_WAIT_0 000000080h
#define STATUS_USER_APC 0000000C0h
#define STATUS_TIMEOUT 000000102h
#define STATUS_PENDING 000000103h
#define STATUS_SEGMENT_NOTIFICATION 040000005h
#define STATUS_GUARD_PAGE_VIOLATION 080000001h
#define STATUS_DATATYPE_MISALIGNMENT 080000002h
#define STATUS_BREAKPOINT 080000003h
#define STATUS_SINGLE_STEP 080000004h
#define STATUS_ACCESS_VIOLATION 0C0000005h
#define STATUS_IN_PAGE_ERROR 0C0000006h
#define STATUS_INVALID_HANDLE 0C0000008h
#define STATUS_NO_MEMORY 0C0000017h
#define STATUS_ILLEGAL_INSTRUCTION 0C000001Dh
#define STATUS_NONCONTINUABLE_EXCEPTION 0C0000025h
#define STATUS_INVALID_DISPOSITION 0C0000026h
#define STATUS_ARRAY_BOUNDS_EXCEEDED 0C000008Ch
#define STATUS_FLOAT_DENORMAL_OPERAND 0C000008Dh
#define STATUS_FLOAT_DIVIDE_BY_ZERO 0C000008Eh
#define STATUS_FLOAT_INEXACT_RESULT 0C000008Fh
#define STATUS_FLOAT_INVALID_OPERATION 0C0000090h
#define STATUS_FLOAT_OVERFLOW 0C0000091h
#define STATUS_FLOAT_STACK_CHECK 0C0000092h
#define STATUS_FLOAT_UNDERFLOW 0C0000093h
#define STATUS_INTEGER_DIVIDE_BY_ZERO 0C0000094h
#define STATUS_INTEGER_OVERFLOW 0C0000095h
#define STATUS_PRIVILEGED_INSTRUCTION 0C0000096h
#define STATUS_STACK_OVERFLOW 0C00000FDh
#define STATUS_CONTROL_C_EXIT 0C000013Ah

#define THREAD_BASE_PRIORITY_LOWRT 15
#define THREAD_BASE_PRIORITY_MAX 2
#define THREAD_BASE_PRIORITY_MIN - 2
#define THREAD_BASE_PRIORITY_IDLE - 15

#define REG_NONE 0
#define REG_SZ 1
#define REG_EXPAND_SZ 2
#define REG_BINARY 3
#define REG_DWORD 4
#define REG_DWORD_LITTLE_ENDIAN 4
#define REG_DWORD_BIG_ENDIAN 5
#define REG_LINK 6
#define REG_MULTI_SZ 7
#define REG_RESOURCE_LIST 8
#define REG_FULL_RESOURCE_DESCRIPTOR 9
#define REG_RESOURCE_REQUIREMENTS_LIST 10
#define REG_QWORD 11
#define REG_QWORD_LITTLE_ENDIAN 11

#define CONTEXT_i386 00010000h
#define CONTEXT_i486 00010000h

#define CONTEXT_CONTROL CONTEXT_i386 or 00000001h // // SS:SP, CS:IP, FLAGS, BP
#define CONTEXT_INTEGER CONTEXT_i386 or 00000002h // // AX, BX, CX, DX, SI, DI
#define CONTEXT_SEGMENTS CONTEXT_i386 or 00000004h // // DS, ES, FS, GS
#define CONTEXT_FLOATING_POINT CONTEXT_i386 or 00000008h // // 387 state
#define CONTEXT_DEBUG_REGISTERS CONTEXT_i386 or 00000010h // // DB 0-3,6,7

#define CONTEXT_FULL CONTEXT_CONTROL or CONTEXT_INTEGER or CONTEXT_SEGMENTS

#define SIZE_OF_80387_REGISTERS 80

// FLOATING_SAVE_AREA struct
// ControlWord    dd ?
// StatusWord     dd ?
// TagWord        dd ?
// ErrorOffset    dd ?
// ErrorSelector  dd ?
// DataOffset     dd ?
// DataSelector   dd ?
// RegisterArea   db SIZE_OF_80387_REGISTERS dup (?)
// Cr0NpxState    dd ?
// FLOATING_SAVE_AREA ends

// /
// / Context Frame
// /
// /  This frame has a several purposes: 1) it is used as an argument to
// /  NtContinue, 2) is is used to constuct a call frame for APC delivery,
// /  and 3) it is used in the user level thread creation routines.
// /
// /  The layout of the record conforms to a standard call frame.
// /

// CONTEXT struct

// //
// // The flags values within this flag control the contents of
// // a CONTEXT record.
// //
// // If the context record is used as an input parameter, then
// // for each portion of the context record controlled by a flag
// // whose value is set, it is assumed that that portion of the
// // context record contains valid context. If the context record
// // is being used to modify a threads context, then only that
// // portion of the threads context will be modified.
// //
// // If the context record is used as an IN OUT parameter to capture
// // the context of a thread, then only those portions of the thread's
// // context corresponding to set flags will be returned.
// //
// // The context record is never used as an OUT only parameter.
// //

// ContextFlags dd ? // +0

// //
// // This section is specified/returned if CONTEXT_DEBUG_REGISTERS is
// // set in ContextFlags.  Note that CONTEXT_DEBUG_REGISTERS is NOT
// // included in CONTEXT_FULL.
// //

// rDr0 dd ? // +4
// rDr1 dd ?
// rDr2 dd ?
// rDr3 dd ?
// rDr6 dd ?
// rDr7 dd ?

// //
// // This section is specified/returned if the
// // ContextFlags word contains the flag CONTEXT_FLOATING_POINT.
// //

// FloatSave FLOATING_SAVE_AREA {} // +28

// //
// // This section is specified/returned if the
// // ContextFlags word contains the flag CONTEXT_SEGMENTS.
// //

// SegGs dd ? // +140
// SegFs dd ?
// SegEs dd ?
// SegDs dd ?

// //
// // This section is specified/returned if the
// // ContextFlags word contains the flag CONTEXT_INTEGER.
// //

// rEdi dd ? // +156
// rEsi dd ?
// rEbx dd ?
// rEdx dd ?
// rEcx dd ?
// rEax dd ?

// //
// // This section is specified/returned if the
// // ContextFlags word contains the flag CONTEXT_CONTROL.
// //

// rEbp   dd ? // +180
// rEip   dd ?
// SegCs  dd ?
// EFlags dd ?
// rEsp   dd ?
// SegSs  dd ?

// CONTEXT ends // +204

// IMAGE_DOS_HEADER STRUCT
// e_magic           WORD      ? // +0
// e_cblp            WORD      ? // +2
// e_cp              WORD      ? // +4
// e_crlc            WORD      ? // +6		number of relocation records
// e_cparhdr         WORD      ? // +8
// e_minalloc        WORD      ? // +10
// e_maxalloc        WORD      ? // +12
// e_ss              WORD      ? // +14
// e_sp              WORD      ? // +16
// e_csum            WORD      ? // +18
// e_ip              WORD      ? // +20
// e_cs              WORD      ? // +22
// e_lfarlc          WORD      ? // +24	begin relocation records
// e_ovno            WORD      ? // +26
// e_res             WORD   4 dup(?) // +28
// e_oemid           WORD      ? // +36
// e_oeminfo         WORD      ? // +38
// e_res2            WORD  10 dup(?) // +40
// e_lfanew          DWORD      ? // +60
// IMAGE_DOS_HEADER ENDS


// IMAGE_FILE_HEADER struct // size=20
// Machine				dw ? // 0
// NumberOfSections	dw ?
// TimeDateStamp		dd ? // 4
// PointerToSymbolTable dd ? // 8
// NumberOfSymbols		dd ? // 12
// SizeOfOptionalHeader dw ? // 16
// Characteristics		dw ? // flags
// IMAGE_FILE_HEADER ends

// *** flags ***

// 0400: If Image is on removable media, copy and run from the swap file.
// 0800: If Image is on Net, copy and run from the swap file.

#define IMAGE_FILE_RELOCS_STRIPPED 0001h
#define IMAGE_FILE_EXECUTABLE_IMAGE 0002h
#define IMAGE_FILE_LINE_NUMS_STRIPPED 0004h
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED 0008h
#define IMAGE_FILE_16BIT_MACHINE 0040h
#define IMAGE_FILE_BYTES_REVERSED_LO 0080h // Bytes of machine word are reversed.
#define IMAGE_FILE_32BIT_MACHINE 0100h
#define IMAGE_FILE_DEBUG_STRIPPED 0200h // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP 0400h
#define IMAGE_FILE_NET_RUN_FROM_SWAP 0800h
#define IMAGE_FILE_SYSTEM 1000h // System File.
#define IMAGE_FILE_DLL 2000h
#define IMAGE_FILE_UP_SYSTEM_ONLY 4000h // File should only be run on a UP machine
#define IMAGE_FILE_BYTES_REVERSED_HI 8000h // Bytes of machine word are reversed.

#define IMAGE_FILE_MACHINE_UNKNOWN 0h
#define IMAGE_FILE_MACHINE_I386 14ch // // Intel 386.
#define IMAGE_FILE_MACHINE_R3000 162h // // MIPS little-endian, 0x160 big-endian
#define IMAGE_FILE_MACHINE_R4000 166h // // MIPS little-endian
#define IMAGE_FILE_MACHINE_R10000 168h // // MIPS little-endian
#define IMAGE_FILE_MACHINE_ALPHA 184h // // Alpha_AXP
#define IMAGE_FILE_MACHINE_POWERPC 1F0h // // IBM PowerPC Little-Endian


// IMAGE_DATA_DIRECTORY struct
// VirtualAddress    DWORD   ?
// Size_			  DWORD   ?
// IMAGE_DATA_DIRECTORY ends

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16


// IMAGE_OPTIONAL_HEADER  struct // size = 28 + 68 + 128 = 224
// standard, size=28
// Magic					dw ? // 0
// MajorLinkerVersion		db ?
// MinorLinkerVersion		db ?
// SizeOfCode				dd ? // 4
// SizeOfInitializedData	dd ? // 8
// SizeOfUninitializedData dd ? // 12
// AddressOfEntryPoint     dd ? // 16
// BaseOfCode				dd ? // 20
// BaseOfData				dd ? // 24
// NT specific, size = 68
// ImageBase				dd ? // 28
// SectionAlignment		dd ? // 32
// FileAlignment			dd ? // 36
// MajorOperatingSystemVersion   dw ? // 40
// MinorOperatingSystemVersion   dw ?
// MajorImageVersion		dw ? // 44
// MinorImageVersion		dw ?
// MajorSubsystemVersion	dw ? // 48
// MinorSubsystemVersion	dw ?
// Win32VersionValue		dd ? // 52
// SizeOfImage				dd ? // 56
// SizeOfHeaders			dd ? // 60
// CheckSum				dd ? // 64
// Subsystem				dw ? // 68
// DllCharacteristics		dw ?
// SizeOfStackReserve		dd ? // 72
// SizeOfStackCommit		dd ? // 76
// SizeOfHeapReserve		dd ? // 80
// SizeOfHeapCommit		dd ? // 84
// LoaderFlags				dd ? // 88
// NumberOfRvaAndSizes     dd ? // 92
// DataDirectory IMAGE_DATA_DIRECTORY	IMAGE_NUMBEROF_DIRECTORY_ENTRIES dup (<>)
// IMAGE_OPTIONAL_HEADER  ends

// Subsystem Values

#define IMAGE_SUBSYSTEM_UNKNOWN 0 // / Unknown subsystem.
#define IMAGE_SUBSYSTEM_NATIVE 1 // / Image doesn't require a subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_GUI 2 // / Image runs in the Windows GUI subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_CUI 3 // / Image runs in the Windows character subsystem.
#define IMAGE_SUBSYSTEM_OS2_CUI 5 // / image runs in the OS/2 character subsystem.
#define IMAGE_SUBSYSTEM_POSIX_CUI 7 // / image run  in the Posix character subsystem.
#define IMAGE_SUBSYSTEM_RESERVED8 8 // / image run  in the 8 subsystem.


// Directory Entries

#define IMAGE_DIRECTORY_ENTRY_EXPORT 0 // // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT 1 // // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE 2 // // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION 3 // // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY 4 // // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC 5 // // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG 6 // // Debug Directory
#define IMAGE_DIRECTORY_ENTRY_COPYRIGHT 7 // // Description String
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE 7 // // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR 8 // // Machine Value (MIPS GP)
#define IMAGE_DIRECTORY_ENTRY_TLS 9 // // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG 10 // // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT 11 // // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT 12 // // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT 13 // // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14 // // COM Runtime descriptor


// IMAGE_NT_HEADERS struct
// Signature		dd ? // 00 "PE"
// FileHeader		IMAGE_FILE_HEADER <>
// OptionalHeader	IMAGE_OPTIONAL_HEADER <>
// IMAGE_NT_HEADERS ends

// --- section table

// IMAGE_SECTION_HEADER struct
// Name_			db 8 dup (?) // 0
// union Misc
// PhysicalAddress dd ? // 8
// VirtualSize     dd ?
// ends
// VirtualAddress  dd ? // 12
// SizeOfRawData   dd ? // 16
// PointerToRawData dd ? // 20
// PointerToRelocations dd ? // 24	points to array of IMAGE_RELOCATIONs (.OBJ)
// PointerToLinenumbers  dd ? // 28
// NumberOfRelocations dw ? // 32
// NumberOfLinenumbers  dw ?
// Characteristics  dd ? // 36
// IMAGE_SECTION_HEADER ends

// //
// // Section characteristics.
// //

#define IMAGE_SCN_TYPE_NO_PAD 00000008h // Reserved.
// 
#define IMAGE_SCN_CNT_CODE 00000020h // Section contains code.
#define IMAGE_SCN_CNT_INITIALIZED_DATA 00000040h // Section contains initialized data.
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA 00000080h // Section contains uninitialized data.
// 
#define IMAGE_SCN_LNK_OTHER 00000100h // Reserved.
#define IMAGE_SCN_LNK_INFO 00000200h // Section contains comments or some other type of information.
// IMAGE_SCN_TYPE_OVER             equ 00000400h ; Reserved.
#define IMAGE_SCN_LNK_REMOVE 00000800h // Section contents will not become part of image.
#define IMAGE_SCN_LNK_COMDAT 00001000h // Section contents comdat.
// //                                  00002000h ; Reserved.
// 
// IMAGE_SCN_MEM_PROTECTED - Obsolete  00004000h ;
#define IMAGE_SCN_MEM_FARDATA 00008000h
// IMAGE_SCN_MEM_SYSHEAP  - Obsolete   00010000h ;
#define IMAGE_SCN_MEM_PURGEABLE 00020000h
#define IMAGE_SCN_MEM_16BIT 00020000h
#define IMAGE_SCN_MEM_LOCKED 00040000h
#define IMAGE_SCN_MEM_PRELOAD 00080000h
// 
#define IMAGE_SCN_ALIGN_1BYTES 00100000h
#define IMAGE_SCN_ALIGN_2BYTES 00200000h
#define IMAGE_SCN_ALIGN_4BYTES 00300000h
#define IMAGE_SCN_ALIGN_8BYTES 00400000h
#define IMAGE_SCN_ALIGN_16BYTES 00500000h // Default alignment if no others are specified.
#define IMAGE_SCN_ALIGN_32BYTES 00600000h
#define IMAGE_SCN_ALIGN_64BYTES 00700000h
// // Unused                           00800000h ;
// 
#define IMAGE_SCN_LNK_NRELOC_OVFL 01000000h // Section contains extended relocations.
#define IMAGE_SCN_MEM_DISCARDABLE 02000000h // Section can be discarded.
#define IMAGE_SCN_MEM_NOT_CACHED 04000000h // Section is not cachable.
#define IMAGE_SCN_MEM_NOT_PAGED 08000000h // Section is not pageable.
#define IMAGE_SCN_MEM_SHARED 10000000h // Section is shareable.
#define IMAGE_SCN_MEM_EXECUTE 20000000h // Section is executable.
#define IMAGE_SCN_MEM_READ 40000000h // Section is readable.
#define IMAGE_SCN_MEM_WRITE 80000000h // Section is writeable.

// --- relocations

// IMAGE_RELOCATION struct
// union
// VirtualAddress		DWORD   ?
// RelocCount			DWORD   ?
// ends
// SymbolTableIndex	DWORD   ?
// Type_				WORD    ?
// IMAGE_RELOCATION ends

// //
// // I386 relocation types.
// //
#define IMAGE_REL_I386_ABSOLUTE 00000h // // Reference is absolute, no relocation is necessary
#define IMAGE_REL_I386_DIR16 00001h // // Direct 16-bit reference to the symbols virtual address
#define IMAGE_REL_I386_REL16 00002h // // PC-relative 16-bit reference to the symbols virtual address
#define IMAGE_REL_I386_DIR32 00006h // // Direct 32-bit reference to the symbols virtual address
#define IMAGE_REL_I386_DIR32NB 00007h // // Direct 32-bit reference to the symbols virtual address, base not included
#define IMAGE_REL_I386_SEG12 00009h // // Direct 16-bit reference to the segment-selector bits of a 32-bit virtual address
#define IMAGE_REL_I386_SECTION 0000Ah
#define IMAGE_REL_I386_SECREL 0000Bh
#define IMAGE_REL_I386_TOKEN 0000Ch // // clr token
#define IMAGE_REL_I386_SECREL7 0000Dh // // 7 bit offset from base of section containing target
#define IMAGE_REL_I386_REL32 00014h // // PC-relative 32-bit reference to the symbols virtual address


// IMAGE_BASE_RELOCATION struct
// VirtualAddress	DWORD   ?
// SizeOfBlock		DWORD   ?
// IMAGE_BASE_RELOCATION ends

#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGH 1
#define IMAGE_REL_BASED_LOW 2
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_HIGHADJ 4
#define IMAGE_REL_BASED_MIPS_JMPADDR 5
#define IMAGE_REL_BASED_MIPS_JMPADDR16 9
#define IMAGE_REL_BASED_IA64_IMM64 9
#define IMAGE_REL_BASED_DIR64 10

// --- exports

// IMAGE_EXPORT_DIRECTORY  struct
// Characteristics 		dd ?
// TimeDateStamp			dd ?
// MajorVersion			dw ?
// MinorVersion			dw ?
// nName					dd ? // name of module
// nBase					dd ? // base of ordinal
// NumberOfFunctions		dd ? // number of entries in EAT table
// NumberOfNames			dd ? // number of entries in name/ordinals table
// AddressOfFunctions		dd ? // RVA "export address table" (EAT)
// AddressOfNames			dd ? // RVA "name table" ()
// AddressOfNameOrdinals	dd ? // RVA "ordinals table" (WORDS)
// IMAGE_EXPORT_DIRECTORY  ends

// popcontext listing

// endif
