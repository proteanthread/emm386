# EMM386 Memory Specifications - LIM EMS 4.0 & UMB Compliance Guide

This document defines how the EMM386 (Jemm) memory manager complies with the **LIM Expanded Memory Specification (EMS) Version 4.0**, the **Upper Memory Block (UMB)** specification, and how it integrates with the **LibreDOS Memory Specification (LMS)**.

---

## 1. LIM EMS 4.0 Compatibility Reference

The EMM386 implementation complies with the Lotus-Intel-Microsoft (LIM) Expanded Memory Specification V4.0. It exposes EMS services to real-mode and V86-mode applications via Software Interrupt `INT 67h`.

### A. The 64KB Page Frame & EMS Page Map
- **Page Frame Allocation**: EMM386 reserves a contiguous 64KB region in the Upper Memory Area (UMA), typically starting at segment `D000h` (or customized via the `FRAME=` switch).
- **Logical to Physical Page Mapping**: This 64KB window is divided into four physical 16KB pages (Page 0, 1, 2, 3). Function `44h` maps any logical 16KB page from allocated handles onto one of these physical pages.
- **Handles & Allocations**: Supports up to 255 handles, allowing applications to request blocks of expanded memory from the global XMS page pool.

### B. Supported EMS Functions
EMM386 implements the core LIM EMS V4.0 API:
- **Get Status (AH=40h)**: Verifies if the EMS hardware/software is functioning.
- **Get Page Frame Address (AH=41h)**: Returns the segment base address of the 64KB page frame.
- **Get Unallocated Page Count (AH=42h)**: Returns total free and available 16KB pages.
- **Allocate Pages (AH=43h)**: Creates a handle and assigns the requested count of 16KB pages.
- **Map Handle Page (AH=44h)**: Maps a logical page to a physical page within the frame.
- **Deallocate Pages (AH=45h)**: Releases pages assigned to a handle.
- **Get Version (AH=46h)**: Returns version code `40h` (indicating EMS 4.0 compliance).
- **Save / Restore Page Map (AH=47h / AH=48h)**: Preserves active mappings during interrupt routing.
- **EMS 4.0 Multi-Instance Extensions**: Implements subfunctions `50h` (Map/Unmap multiple pages), `58h` (Get/Set directory of handles), and `5Bh` (Get/Set alternate map register sets for multitasking OS environments).

---

## 2. Upper Memory Area (UMA) & UMB Management

EMM386 manages the Upper Memory Area (640KB to 1MB range, segments `A000h` to `FFF0h`) to provide Upper Memory Blocks (UMBs) for loading TSRs and device drivers high (`LH` and `DEVICEHIGH`).

### A. Memory Scanning & Backfilling
- **Scanning Upper Memory**: EMM386 scans the UMA to locate unused address spaces (excluding video RAM at `A000h`-`BFFFh` and system BIOS at `F000h`-`FFFFh` unless configured).
- **Exclusion / Inclusion**: Respects command line switches like `I=` (force include address range as UMB) and `X=` (exclude address range to prevent hardware collisions).
- **Conventional Memory Backfilling**: If conventional memory is configured to be less than 640KB, EMM386 can map physical RAM into vacant lower segments to increase the standard base memory.

### B. Linkage to the DOS MCB Chain
- Once UMB areas are allocated, EMM386 links these memory blocks into the standard DOS Memory Control Block (MCB) chain.
- This links UMBs to conventional memory, allowing the standard DOS memory allocation strategy (e.g. High-First) to allocate TSRs in upper memory.

---

## 3. Integration with LibreDOS Memory Specification (LMS)

EMM386 functions as the virtualized memory extension controller when running under the LibreDOS kernel:

- **Interrupt Intercept & Yield**: As defined in [LMS.md](file:///C:/Users/rtdos/GitHub/kernel/docs/LMS.md), when LibreDOS detects that EMM386 has hooked `INT 67h` (EMS) and `INT 2Fh` (XMS), the kernel's built-in emulator yields, passing memory translation and page mapping requests directly to EMM386.
- **INT 15h Memory Mapping**: EMM386 queries system memory layout using BIOS `INT 15h` (sub-functions `AH=88h`, `AX=E801h`, and `AX=E820h`) virtualized by the LMS layer to configure the global pool of extended memory pages.
- **A20 Gate and Segment Wrapping**: Works in tandem with LMS HMA wrapping, ensuring `FFFF:0010` to `FFFF:FFFF` wraps or enables based on A20 gate status.

---

## 4. 64-Bit DPMI & Flat Memory Module (LMS64) Specification

To support modern, resource-intensive legacy applications running under LibreDOS, EMM386 includes the **LMS64 Flat Memory Module**. This module allows native 64-bit DPMI clients to run in x86-64 long mode, mapping memory above the 4GB boundary while maintaining 100% compatibility with MS-DOS/PC-DOS.

### A. 4-Level Virtual Memory Paging (PML4)
- **Paging Hierarchy**: Rather than using real-mode or 32-bit protected-mode page tables, LMS64 implements a standard x86-64 4-level paging structure:
  - **PML4** (Page Map Level 4): Resolves bits 47-39 of the 64-bit virtual address.
  - **PDPT** (Page Directory Pointer Table): Resolves bits 38-30.
  - **PD** (Page Directory): Resolves bits 29-21.
  - **PT** (Page Table): Resolves bits 20-12.
- **Dynamic Allocation**: Intermediate page directories are allocated dynamically when writing page table entries (PTEs) if they do not already exist.
- **PTE Flag Mapping**: Maps physical pages using hardware bitflags including:
  - `PTE_PRESENT` (bit 0): Page is loaded in physical memory.
  - `PTE_WRITE` (bit 1): Page has read/write privileges.
  - `PTE_USER` (bit 2): User-mode access permitted.
  - `PTE_NX` (bit 63): No-Execute bit to prevent execution of data segments.

### B. BSD-Style Virtual Memory APIs
LMS64 exposes standard BSD-style virtual memory mapping and protection functions for 64-bit clients:
- **`lms64_mmap`**: Maps host physical memory into the 64-bit virtual space. Allocates consecutive physical pages starting above 8GB for anonymous mappings (`MAP_ANONYMOUS`). Returns the 64-bit virtual address.
- **`lms64_munmap`**: Unmaps virtual memory pages, clears their page table entries, and invalidates the translation lookaside buffer (TLB) via CPU instructions (`invlpg`).
- **`lms64_mprotect`**: Alters page table protection flags (read, write, execute permissions) for active mappings.

### C. 64-Bit DPMI Register Context (`Client_Reg_Struc64`)
When a 64-bit client triggers an interrupt or traps to supervisor mode, the CPU state is saved into a `Client_Reg_Struc64` structure. This structure tracks:
- 64-bit General Purpose Registers (`RAX`, `RBX`, `RCX`, `RDX`, `RSI`, `RDI`, `RBP`, `RSP`, `R8`-`R15`).
- 64-bit Instruction Pointer (`RIP`) and flags (`RFLAGS`).
- Segment Selectors (`CS`, `SS`, `DS`, `ES`, `FS`, `GS`) aligned to 64-bit boundaries with padding.

---

## 5. C17 Compliance Audit

This workspace has been modernized to comply strictly with the **ISO C17 standard**:
- **Comment Format**: Every single-line comment, inline annotation, and preprocessor comment utilizes modern double-slash `//` format.
- **Header Blocks**: File header block descriptions are wrapped in proper `/* ... */` multiline comments.
- **No Legacy C Code**: Function parameter lists are strictly declared (e.g., `void` for parameterless functions), removing old K&R parameters style. Non-standard trailing function semicolons have been removed.
- **Watcom C Compatibility**: The code compiles cleanly with the Open Watcom compiler (`wcc386`) yielding **0 warnings and 0 errors**.

---

## 6. Maintenance Guidelines

### A. What We Can Change
- **Page Allocation Rules**: You can modify page scanning techniques (e.g., first-fit vs. best-fit) for EMS page allocations.
- **UMB Scan Boundaries**: Default inclusion/exclusion lists for UMA segments can be modified.
- **Logging & Diagnostics**: You can add logging statements or telemetry wrappers inside C mapping functions.
- **64-bit Virtual Allocation base**: Change the starting virtual/physical simulation boundaries in `LMS64.c`.

### B. What We Cannot Change
- **INT 67h Calling Conventions**: The register interface layouts and values returned by `INT 67h` must remain strictly compliant with LIM EMS 4.0 specification.
- **Struct Alignment**: Structs mapped to BIOS variables or sharing memory blocks with Assembly routines (e.g. `Client_Reg_Struc`, `Client_Reg_Struc64`) must retain their strict layout and alignment.

### C. What to Expect
- **Segment Address Translations**: Pointers mapped to V86 memory space are translated relative to the active guest's segment boundaries.
- **Isolated Paging**: Paging operates on 4KB page bounds (XMS), 16KB bounds (EMS), and 4-level PML4 hierarchies (64-bit DPMI).

### D. What to Do If Something Breaks / Troubleshooting
- **EMS Detection Failure**: Verify that `INT 67h` vector points to the correct entry hook and that the EMM device header is correctly registered.
- **Memory Map Overlaps**: If hardware conflicts occur, audit the excluded memory regions (`X=`) in `flowchart.md` or configuration logs.
- **64-Bit Mode Faults**: Verify `Client_Reg_Struc64` padding sizes align with the expected CPU pushed stack layout in Assembly handlers.
