# EMM386 Memory Manager Programmers Guide

This guide describes how to programmatically interface with EMM386 to manage conventional, extended, expanded, and 64-bit flat memory under LibreDOS.

---

## 1. XMS 2.0 / 3.0 API Reference

The Extended Memory Specification (XMS) API is accessed via the driver's Control Function.

### A. Detecting the XMS Driver & Retrieving Control Address
To check if the manager is present, execute `INT 2Fh` with `AX=4300h`. If AL returns `80h`, the driver is present.
To retrieve the 32-bit Control Function address, execute `INT 2Fh` with `AX=4310h`. The entry point address is returned in `ES:BX`.

### B. Core XMS API Functions
All calls to the Control Function expect the function number in `AH`:

- **Get XMS Version (AH=00h)**: Returns XMS version in AX, driver revision in BX, and HMA status in DX.
- **Request HMA (AH=01h)**: Reserves the High Memory Area.
- **Release HMA (AH=02h)**: Releases the HMA.
- **Global Enable A20 (AH=03h)**: Enables the A20 address line.
- **Global Disable A20 (AH=04h)**: Disables the A20 line.
- **Local Enable A20 (AH=05h)**: Enables A20 for the calling process.
- **Local Disable A20 (AH=06h)**: Disables A20 locally.
- **Query A20 (AH=07h)**: Returns A20 status (0=disabled, 1=enabled) in AX.
- **Query Free Memory (AH=08h)**: Returns size of largest free EMB in AX and total free memory in DX (up to 64MB).
- **Allocate EMB (AH=09h)**: Allocates an Extended Memory Block. Size in DX (KB). Returns handle in DX.
- **Free EMB (AH=0Ah)**: Frees an allocated EMB. Handle in DX.
- **Move EMB (AH=0Bh)**: Moves data between conventional and extended memory. Expects `ES:SI` pointing to a `Move_Struc`.
- **Lock EMB (AH=0Ch)**: Locks a block and returns its 32-bit physical base address in `DX:BX`.
- **Unlock EMB (AH=0Dh)**: Unlocks a block.
- **Get Handle Information (AH=0Eh)**: Returns lock count in BH, free handle count in BL, and block size in DX.
- **Reallocate EMB (AH=0Fh)**: Resizes an allocated block. Handle in DX, new size in BX.
- **Request UMB (AH=10h)**: Allocates an Upper Memory Block. Size in DX. Returns segment in BX.
- **Release UMB (AH=11h)**: Frees an allocated UMB. Segment in DX.

### C. XMS 3.0 Supersized Functions (Memory > 64MB)
- **Query Any Free Memory (AH=88h)**: Returns largest block in EAX, total free memory in EDX, highest physical address in ECX.
- **Allocate Any EMB (AH=89h)**: Allocates block of size EDX (KB). Returns handle in DX.
- **Get Extended EMB Handle Info (AH=8Eh)**: Returns lock count in BH, free handles in CX, block size in EDX.
- **Reallocate Any EMB (AH=8Fh)**: Resizes block to new size EBX (KB).

---

## 2. EMS 4.0 API Reference

The LIM Expanded Memory Specification (EMS) services are accessed via Software Interrupt `INT 67h`.

- **Get Status (AH=40h)**: Checks EMS driver health.
- **Get Page Frame Address (AH=41h)**: Returns segment base of the 64KB EMS window in BX.
- **Get Page Counts (AH=42h)**: Returns unallocated pages in BX, total pages in DX.
- **Allocate Pages (AH=43h)**: Allocates BX pages. Returns handle in DX.
- **Map Handle Page (AH=44h)**: Maps logical page BX to physical page AL for handle DX.
- **Deallocate Pages (AH=45h)**: Releases handle DX.
- **Get Version (AH=46h)**: Returns version code `40h` in AL.
- **Save / Restore Page Map (AH=47h / AH=48h)**: Preserves mappings during context switches.

---

## 3. LMS64 BSD Virtual Memory API

For native 64-bit DPMI clients in x86-64 long mode, EMM386 provides BSD-style virtual memory mappings via LMS64.

### Function Signatures
```c
void *lms64_mmap(void *addr, size_t length, int prot, int flags, int fd, uint64_t offset);
int lms64_munmap(void *addr, size_t length);
int lms64_mprotect(void *addr, size_t length, int prot);
```

- **`lms64_mmap`**: Maps host memory. Passing `MAP_ANONYMOUS` in flags allocates physical pages above the 8GB boundary. Returns a 64-bit virtual pointer.
- **`lms64_munmap`**: Clears page translations and invalidates the translation lookaside buffer (TLB) for the target virtual address range.
- **`lms64_mprotect`**: Alters read/write/execute rights on the mapped page range using PML4 bitflags (`PTE_WRITE`, `PTE_NX`).

---

## 4. Customization & Control (What We Can Change)

- **API Wrappers**: You can implement custom high-level libraries (e.g. C runtime wrappers for `xms_malloc` or `ems_map`) to hide register assignments.
- **Pointer Conversion Utilities**: You can modify how 64-bit addresses are translated to V86 segment-offsets during DPMI compatibility mode transitions.
- **Debug Hooks**: You can insert tracing logs or performance telemetry inside XMS/EMS entry handlers.

---

## 5. Architectural Rules (What We Cannot Change)

- **Calling Conventions**: Register inputs (e.g. `AH` for function number, `DX` for handles) must match the XMS/EMS specifications exactly to preserve binary compatibility.
- **Structure Alignments**: Offsets inside shared structures (`Client_Reg_Struc`, `Move_Struc`, `Client_Reg_Struc64`) must not be changed, as Assembly routines and compiler binaries access members by hardcoded offsets.
- **Interrupt Vectors**: `INT 67h`, `INT 2Fh`, and `INT 15h` are hardcoded interrupt vectors reserved by DOS and BIOS standards.

---

## 6. What to Expect (System Behavior)

- **Stack Requirements**: Programs calling the XMS/EMS APIs must allocate at least 256 bytes of stack space to prevent stack overflows during supervisor mode traps.
- **TLB Latency**: Calling `lms64_munmap` or `lms64_mprotect` flushes TLB entries via `invlpg` instructions, which can cause microsecond-scale execution delays due to instruction pipelines flushing.
- **Memory Limits**: Standard XMS handles are limited by the available XMS page pool, and allocations return error code `88h` (all memory allocated) if memory is exhausted.

---

## 7. BSD Virtual Memory Integration

LMS64 integrates BSD-style virtual memory mechanics directly into the x86-64 long mode execution layout:
- **mmap Memory Mapping**: When a 64-bit application calls `lms64_mmap`, it uses standard BSD arguments (`PROT_READ`, `PROT_WRITE`, `MAP_PRIVATE`, `MAP_ANONYMOUS`).
- **4-Level Page Mappings**: The DPMI host translates these parameters into PML4, PDPT, PD, and PT page table entries, setting appropriate bits (`PTE_USER`, `PTE_WRITE`, `PTE_NX`).

---

## 8. Troubleshooting & Recovery (What to Do If Something Breaks)

- **XMS Call Returns Failure (AX=0000h)**:
  1. Inspect the error code returned in the `BL` register.
  2. Common codes:
     - `80h`: Function not implemented.
     - `81h`: VDISK detected.
     - `A2h`: Invalid handle (verify handle lifetime).
     - `A8h`: Lock count overflow (verify matching Lock/Unlock pairs).
- **Page Faults (EXC0Eh) during 64-bit Execution**:
  - The program attempted to access an unmapped page or write to a read-only page. Verify that `lms64_mmap` was executed successfully and that `lms64_mprotect` was not set to `PROT_READ` only.
- **Incorrect Struct Alignment in wcc386**:
  - Ensure `#pragma pack` directives are correctly wrapped around structure definitions to match assembly binary alignments.
