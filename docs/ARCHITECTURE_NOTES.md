# EMM386 Memory Manager Architecture Notes

This document provides a low-level look at the internal architecture of EMM386 / Jemm, covering boot steps, CPU modes, paging table hierarchies, and context switches.

---

## 1. System Boot & Initialization Sequence

EMM386 initializes in two distinct phases:

```
[System Boot] -> [INIT16 (Real Mode)] -> [Switch to Protected Mode] -> [JEMM32 Kernel]
```

1. **INIT16 Real-Mode Entry**:
   - Scans system RAM layout via BIOS `INT 15h` (sub-functions `AX=E820h`, `AX=E801h`, and `AH=88h`) to map available physical memory.
   - Detects the Upper Memory Area (UMA) configurations (`I=`, `X=`, `FRAME=`).
   - Reserves physical memory blocks for the page directory tables.
2. **Transition to 32-Bit Protected Mode**:
   - Configures the global descriptor table (GDT) and interrupt descriptor table (IDT).
   - Initializes the Page Directory (CR3 register) to identity-map the first 1MB of memory and map extended memory.
   - Enables the PE (Protection Enable) and PG (Paging) bits in the `CR0` register.
3. **Establish V86 Mode Context**:
   - Sets up the execution context for Virtual-86 mode, permitting MS-DOS and standard DOS applications to execute as V86 guests.
   - Hooks software interrupt vectors (`INT 67h`, `INT 2Fh`, `INT 15h`) to capture memory management API requests.

---

## 2. 64-Bit DPMI Flat Memory Paging (PML4)

To support 64-bit clients executing in x86-64 long mode, EMM386 sets up a 4-level paging hierarchy:

```
Virtual Address
  |--> PML4 Index (Bits 47-39) -> PML4 Entry -> Points to PDPT
        |--> PDPT Index (Bits 38-30) -> PDPT Entry -> Points to PD
              |--> PD Index (Bits 29-21) -> PD Entry -> Points to PT
                    |--> PT Index (Bits 20-12) -> PTE -> Physical Page Base Address
```

- **PML4 Root**: The root directory is pointed to by the CPU's `CR3` register when running in 64-bit client mode.
- **Dynamic Paging Table Walk**:
  - The function `get_pte` in `LMS64.c` walks this hierarchy.
  - If a directory entry along the path does not have the present bit (`PTE_PRESENT`) set, it dynamically allocates a new physical page, clears it, writes the address to the parent entry, and sets permission flags (`PTE_PRESENT | PTE_WRITE | PTE_USER`).
- **TLB Invalidation**:
  - Whenever page entries are modified (during `lms64_munmap` or `lms64_mprotect`), the host executes the `invlpg` assembly instruction. This clears the CPU's Translation Lookaside Buffer (TLB) for the target virtual address, ensuring cache coherency.

---

## 3. Sub-Mode Context Switching

EMM386 manages transitions between three CPU modes during execution:

1. **V86 Mode**: Runs the MS-DOS kernel, real-mode drivers, and standard TSRs.
2. **64-Bit Long Mode (DPMI Client)**: Runs the 64-bit flat DPMI application above the 4GB boundary.
3. **Compatibility / Protected Mode**: Runs host interrupt handlers and manages the DPMI supervisor.

When a 64-bit client calls a real-mode interrupt:
- The CPU switches context, saving the 64-bit state into `Client_Reg_Struc64`.
- The processor drops from 64-bit long mode to compatibility mode.
- EMM386 translates virtual buffer pointers below the 1MB barrier, switches to V86 mode, executes the DOS service, and restores 64-bit execution upon completion.

---

## 4. Customization & Control (What We Can Change)

- **Virtual / Physical Allocations**: You can change the base offset for virtual memory allocations (e.g. changing `g_next_free_virt` from `0x100000000ULL`) or adjust physical simulation boundaries.
- **Paging Optimization**: You can modify page directory caching rules to speed up page directory allocation lookups.
- **Region Tracking Limits**: The size of the `g_regions` tracking array (currently `128`) can be changed to support more concurrent mappings.

---

## 5. Architectural Rules (What We Cannot Change)

- **x86 Page Table Structure**: The 4-level paging directory layouts, offsets, and bit structures (PML4/PDPT/PD/PT) are dictated by CPU hardware and cannot be changed.
- **Paging Bitflags**: The hardware meaning of bits in PTEs (bit 0 = Present, bit 1 = Read/Write, bit 63 = No-Execute) is fixed by the processor architecture.
- **CPU Control Registers**: CR0, CR3, and CR4 register structures and loading protocols are fixed by the CPU architecture.

---

## 6. What to Expect (System Behavior)

- **Paging Latency**: Dynamically walking page tables and allocating intermediate directories can introduce microsecond-scale latency upon the first write to a virtual page.
- **Isolation Security**: Memory mappings allocated via `mmap` are isolated from other processes. A process trying to access memory outside its mapped regions will trigger a page fault.
- **Switching Overhead**: Moving between 64-bit long mode and V86 mode involves loading descriptor registers and segment bases, which incurs a CPU cycle overhead.

---

## 7. BSD Virtual Memory Integration

LMS64 incorporates BSD-style virtual memory management primitives directly onto x86 hardware paging structures:
- **Anonymous Allocations**: When a client requests `MAP_ANONYMOUS`, LMS64 simulates physical memory allocation above the 8GB boundary and links it to the page directory tables.
- **Page Access Control**: Mappings set with `PROT_READ` or `PROT_WRITE` translate directly to CPU paging flags (`PTE_WRITE` disabled or enabled), and `PROT_EXEC` modifications manage the CPU `PTE_NX` (No-Execute) hardware protection bit.

---

## 8. Troubleshooting & Recovery (What to Do If Something Breaks)

- **Triple Faults (CPU Reset)**:
  1. This occurs when the IDT or GDT is misconfigured, causing a double-fault that fails to handle.
  2. Inspect GDT selectors and ensure that the descriptors are aligned and that segment limits are correct.
- **Page Translation Failures**:
  - Trace address translations using the `g_regions` tracking array. Verify that the virtual address is within mapped boundaries and that the PML4 tables match the mapping.
- **TLB Coherency Issues**:
  - If memory contents do not update immediately, check if the `invlpg` instruction was bypassed or if the target virtual address was misaligned.
