# EMM386 / LibreDOS Kernel Integration Verification Guide

This document presents the official verification and validation of **EMM386** against the **LibreDOS Memory Specification (LMS)** as defined in [LMS.md](file:///C:/Users/rtdos/GitHub/kernel/docs/LMS.md) in the kernel repository. It analyzes system compliance across all physical memory scales: 1GB, 2GB, 4GB, 8GB, 16GB, 32GB, 64GB, and higher.

---

## 1. Compliance Matrix by Physical Memory Capacity

| Physical RAM | Memory Mode | Paging Hierarchy | LMS64 Activation | Architectural Behavior |
| :--- | :--- | :--- | :--- | :--- |
| **1GB - 2GB** | 32-Bit Protected / V86 | 2-Level Paging (4KB/4MB) | Disabled | Fully identity-mapped real-mode space. Memory allocations are locked under the 2GB boundary. |
| **4GB** | 32-Bit Protected / V86 | 2-Level Paging (4KB/4MB) | Disabled | Max physical limit for standard 32-bit memory management. Access above 4GB is blocked. |
| **8GB - 16GB** | x86-64 Long Mode | 4-Level PML4 Paging | Enabled | PML4 maps memory above the 4GB boundary. Physical page pool starts at `0x200000000ULL` (8GB). |
| **32GB - 64GB+**| x86-64 Long Mode | 4-Level PML4 Paging | Enabled | Supports large scale host physical allocations. Allocates memory up to hardware limits. |

---

## 2. Kernel Integration Mechanics

### A. Dynamic Intercept Override
As specified in [LMS.md](file:///C:/Users/rtdos/GitHub/kernel/docs/LMS.md), when EMM386 loads (as a character device named `EMMXXXX0`), it hooks:
- `INT 67h` (EMS services)
- `INT 2Fh` (XMS/UMB services)
- `INT 15h` (BIOS system memory queries)

Upon detecting these hooks, the LibreDOS kernel yields built-in EMS and XMS emulation routines, routing all segment mapping and page translations directly to EMM386.

### B. Segment Address Translation & MCB Linkage
- **Real-Mode Wrapper**: EMM386 respects standard `Segment:Offset` address wrapping inside virtualized 1MB spaces (`dos_chunk_t`).
- **UMB Injection**: Upper memory blocks allocated by EMM386 are linked directly into the DOS Memory Control Block (MCB) chain, permitting load-high capabilities (`LH`, `DEVICEHIGH`).

### C. LMS64 DPMI Virtualization
For host configurations **above 4GB** (8GB, 16GB, 32GB, 64GB+):
- Standard 32-bit paging cannot address memory beyond 4GB due to 32-bit register and Page Table Entry limits.
- EMM386 loads the **LMS64 Flat Memory Module**, transitioning execution into x86-64 long mode.
- LMS64 uses a 4-level PML4 paging table to map host virtual spaces above the 4GB boundary (`0x100000000ULL`), giving 64-bit clients direct access to high physical RAM.

---

## 3. Customization & Control (What We Can Change)

- **Physical Paging Limits**: You can configure the simulated starting physical address (currently `0x200000000ULL`) or memory pools allocated above 4GB inside [LMS64.c](file:///C:/Users/rtdos/GitHub/emm386/src/LMS64.c).
- **Scan Ranges**: You can customize UMB inclusion (`I=`) and exclusion (`X=`) ranges in `CONFIG.SYS` to adjust around system devices.
- **Control Handover**: You can adjust EMM386 initialization parameters to control how dynamically loaded JLMs hook interrupt registers.

---

## 4. Architectural Rules (What We Cannot Change)

- **Interrupt Vector Numbers**: Standard vectors (`INT 67h`, `INT 2Fh`, `INT 15h`) are hardcoded into PC BIOS/DOS specifications and cannot be changed.
- **x86 Paging Specifications**: The PML4 hierarchy, PDPT, PD, and PT structures and page entry bit flags are enforced by CPU hardware.
- **1MB Sandbox Wrapping**: All V86 guest allocations must wrap inside their isolated 1MB Segment Chunks to prevent guest code from corrupting supervisor registers.

---

## 5. What to Expect (System Behavior)

- **Performance Overhead**: Transitioning between 64-bit client mode (long mode) and V86 mode (DOS kernel) involves swapping CPU registers and page table roots (CR3), causing a microscopic context-switch delay.
- **Upper Memory Layout**: Boot logs will report active UMB segments mapped in the Upper Memory Area, showing exactly where ROM/RAM buffers reside.
- **Paging Walk Delay**: Dynamic page directory allocation lookups on the first access of a 64-bit virtual memory page can cause a minor delay as intermediate tables are allocated.

---

## 6. BSD Virtual Memory Integration

EMM386 integrates BSD-style virtual memory mappings directly into its 64-bit flat memory organization:
- **BSD APIs**: Standard BSD memory operations (`mmap`, `munmap`, `mprotect`) are supported for 64-bit client execution.
- **Permissions Translation**: BSD protection parameters (`PROT_READ`, `PROT_WRITE`, `PROT_EXEC`) map directly to page table entries (`PTE_WRITE` and `PTE_NX` flags), securing memory regions above the 4GB boundary.

---

## 7. Troubleshooting & Recovery (What to Do If Something Breaks)

- **System Crashes on High Memory Allocation**:
  1. If allocations above 4GB fail or cause double faults, check if the system RAM is correctly reported via BIOS `INT 15h` `AX=E820h`.
  2. Verify that the physical page simulator (`alloc_phys_page`) is not exceeding the physical RAM installed.
- **UMB Conflicts**:
  - If load-high operations freeze, exclude the video BIOS region (`X=C000-C7FF`) or SCSI card spaces (`X=D000-D3FF`) in EMM386 configurations.
- **Interrupt Conflicts**:
  - If another memory manager hooks `INT 2Fh`, ensure EMM386 is loaded first to allow the LibreDOS kernel to detect its override and yield control correctly.
