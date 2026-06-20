/*
 * lms64.h - 64-bit flat memory module (LMS64) & 64-bit DPMI declarations (C17 standard)
 *
 * Architectural Role:
 *   Defines the structures and interfaces supporting 64-bit DPMI client execution in x86-64 long
 *   mode. Exposes standard BSD-style virtual memory mapping (mmap) constants and APIs.
 *
 * Changeability & Constraints:
 *   - CAN BE CHANGED: Helper macros, inline translations, and local diagnostic options.
 *   - CANNOT BE CHANGED: Struct member offsets in Client_Reg_Struc64, paging flags, and BSD
 *     mmap function signatures to ensure strict ABI compatibility with 64-bit clients.
 *
 * Expected Behavior:
 *   - Serves as the include header mapping out 64-bit flat registers and page directory formats.
 *
 * Diagnostics & Recovery:
 *   - Verify struct member offsets align exactly with the CPU register state registers push/pop
 *     in protected mode.
 */

#ifndef _LMS64_H_
#define _LMS64_H_

#ifndef _STDINT_H_
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned __int64   uint64_t;
#endif

#ifndef _STDBOOL_H_
typedef int                bool;
#define true               1
#define false              0
#endif

#ifndef _SIZE_T_DEFINED
typedef unsigned int       size_t;
#define _SIZE_T_DEFINED
#endif

#ifndef NULL
#define NULL               ((void *)0)
#endif

// BSD-Style memory protection options
#define PROT_NONE       0x0             // Page cannot be accessed
#define PROT_READ       0x1             // Page can be read
#define PROT_WRITE      0x2             // Page can be written
#define PROT_EXEC       0x4             // Page can be executed

// BSD-Style mapping configuration options
#define MAP_SHARED      0x01            // Share changes across mappings
#define MAP_PRIVATE     0x02            // Changes are private to process
#define MAP_FIXED       0x10            // Interpret addr exactly as requested
#define MAP_ANONYMOUS   0x20            // Mapping is not backed by any file

// Error code returned by mmap on failure
#define MAP_FAILED      ((void *)-1)

// 64-bit DPMI register context structure
// Tracks complete register state of an x86-64 long mode client process.
struct Client_Reg_Struc64 {
    uint64_t Client_RDI;       // Client RDI register
    uint64_t Client_RSI;       // Client RSI register
    uint64_t Client_RBP;       // Client RBP register
    uint64_t Client_RSP_all;   // RSP at entry point
    uint64_t Client_RBX;       // Client RBX register
    uint64_t Client_RDX;       // Client RDX register
    uint64_t Client_RCX;       // Client RCX register
    uint64_t Client_RAX;       // Client RAX register
    uint64_t Client_R8;        // Extended register R8
    uint64_t Client_R9;        // Extended register R9
    uint64_t Client_R10;       // Extended register R10
    uint64_t Client_R11;       // Extended register R11
    uint64_t Client_R12;       // Extended register R12
    uint64_t Client_R13;       // Extended register R13
    uint64_t Client_R14;       // Extended register R14
    uint64_t Client_R15;       // Extended register R15
    uint64_t Client_IntNo;     // Hooked interrupt number
    uint64_t Client_Error;     // Error code parameter
    uint64_t Client_RIP;       // Client instruction pointer (64-bit)
    uint16_t Client_CS;        // Client Code Segment selector
    uint16_t Client_Pad1[3];   // Padding for 64-bit boundary alignment
    uint64_t Client_EFlags;    // 64-bit EFLAGS/RFLAGS register
    uint64_t Client_RSP;       // Client Stack Pointer (64-bit)
    uint16_t Client_SS;        // Client Stack Segment selector
    uint16_t Client_Pad2[3];   // Padding
    uint16_t Client_ES;        // Extra Segment selector
    uint16_t Client_Pad3[3];   // Padding
    uint16_t Client_DS;        // Data Segment selector
    uint16_t Client_Pad4[3];   // Padding
    uint16_t Client_FS;        // FS selector
    uint16_t Client_Pad5[3];   // Padding
    uint16_t Client_GS;        // GS selector
    uint16_t Client_Pad6[3];   // Padding
};

// BSD-Style virtual memory mapping interface function declarations
void *lms64_mmap(void *addr, size_t length, int prot, int flags, int fd, uint64_t offset);
int lms64_munmap(void *addr, size_t length);
int lms64_mprotect(void *addr, size_t length, int prot);

#endif // _LMS64_H_
