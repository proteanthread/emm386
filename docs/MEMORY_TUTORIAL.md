# LibreDOS Memory Management Tutorial

This tutorial explains the historical and modern memory management models used in x86 architectures, detailing how LibreDOS handles memory from real-mode segments up to 64-bit flat virtual addresses.

---

## 1. The DOS Memory Map Layout

In standard IBM PC-compatible systems, the first 1MB of address space is structured as follows:

```
+-----------------------------------+ 1088KB (1MB + 64KB - 16B)
|      High Memory Area (HMA)       |
+-----------------------------------+ 1024KB (1MB Boundary)
|      System BIOS & ROM Area       | F000h - FFFFh (64KB)
+-----------------------------------+
|      EMS Page Frame / UMBs        | C000h - EFFFh (192KB)
+-----------------------------------+
|      Video RAM & Graphics ROM     | A000h - BFFFh (128KB)
+-----------------------------------+ 640KB Boundary
|                                   |
|       Conventional Memory         | 0000h - 9FFFh (640KB)
|                                   |
+-----------------------------------+ 0KB
```

### A. Conventional Memory
The first 640KB of memory. Real-mode programs, DOS kernels, stacks, and device drivers must reside here by default.

### B. Upper Memory Area (UMA) & Upper Memory Blocks (UMBs)
The 384KB region between 640KB and 1MB. Traditionally reserved for video buffers and BIOS ROMs. EMM386 scans this range for unused space and backfills it with physical RAM, creating UMBs that DOS can use to load TSRs high.

### C. High Memory Area (HMA)
The first 64KB (minus 16 bytes) above 1MB. Under real mode, enabling the A20 address line allows the CPU to address up to `10FFEFh` without switching modes, allowing DOS to load its kernel buffers high.

---

## 2. Advanced Paging & Historical Models

### A. x86 Memory Segmentation
Real-mode x86 CPUs use a segment-offset addressing scheme (`Segment * 16 + Offset`). The segment registers (`CS`, `DS`, `SS`, `ES`) hold 16-bit base addresses, limiting direct addressing to 1MB.

### B. Overlays
Before virtual memory, programs larger than conventional memory used overlays. The compiler divided the program into segments; a small overlay manager loaded these segments from disk into the same physical memory space on-demand.

### C. Sideways Address Space
Popular in 8-bit computers (like BBC Micro), sideways RAM/ROM mapped multiple memory banks into the same logical window. EMS replicates this bank-switching mechanism on PC architectures.

### D. Expanded Memory (EMS)
A bank-switching scheme (LIM EMS 4.0). Maps logical 16KB pages from a larger memory pool into a 64KB page frame in the UMA, enabling real-mode programs to access megabytes of memory.

### E. Extended Memory (XMS)
Memory above the 1MB boundary. Access is managed by an Extended Memory Manager (like HIMEM) using global handles.

### F. GEMMIS (GEMM Import Structure)
A communication protocol used by Microsoft Windows 3.x to import memory layout details and take over protected-mode execution from EMM386.

### G. Physical Address Extension (PAE) & Address Windowing Extensions (AWE)
PAE is a 32-bit x86 paging feature mapping 32-bit virtual addresses to 36-bit physical addresses (up to 64GB). AWE is a Windows API that lets 32-bit applications allocate physical memory beyond 4GB and map it into their virtual address window, acting as a modern equivalent to EMS.

---

## 3. Modern 64-Bit Flat Paging (PML4)

In x86-64 long mode, the CPU discards segment limit checks and uses a flat 64-bit address space. Paging uses a 4-level PML4 tree mapping 48 bits of virtual address space to physical memory. This allows programs to address terabytes of RAM without segment bounds.

---

## 4. Customization & Control (What We Can Change)

- **Memory Segment Mappings**: You can configure UMB sizes, page frame addresses, and backfill boundaries in the memory map.
- **Paging Limits**: You can adjust virtual memory allocations and simulation physical pools.
- **Driver Load Strategy**: You can prioritize loading kernel buffers into the HMA vs. UMBs.

---

## 5. Architectural Rules (What We Cannot Change)

- **1MB Address Wall**: Real-mode execution limits direct hardware access to 1MB (plus the HMA if A20 is enabled).
- **A20 Line Mechanics**: Enabling and disabling the 21st address line (A20) is dictated by hardware controllers (Keyboard Controller or Port 92h) and cannot be bypassed.
- **PML4 Hardware Hierarchy**: The 4-level paging structure (PML4 -> PDPT -> PD -> PT) is enforced by x86-64 processors.

---

## 6. What to Expect (System Behavior)

- **UMB Availability**: The amount of free UMB space depends on installed video cards and motherboard BIOS sizes.
- **EMS Overhead**: Accessing pages mapped through EMS is instantaneous once mapped, but swapping pages requires calling `INT 67h`, which incurs a short interrupt trap overhead.
- **Flat 64-Bit Addressing**: When executing 64-bit DPMI clients, memory above 4GB can be accessed directly using standard 64-bit registers (`RAX`, `RBX`) without segment mapping overhead.

---

## 7. BSD Virtual Memory Integration

EMM386 incorporates BSD-style virtual memory mappings within 64-bit DPMI:
- **Virtual Paging Pools**: Maps virtual allocations using BSD primitives (`mmap`, `munmap`, `mprotect`).
- **Page Directory Allocations**: Translates anonymous mappings into standard PML4 directories, isolating 64-bit process space while maintaining compatibility with the low-level real-mode DOS operating system.

---

## 8. Troubleshooting & Recovery (What to Do If Something Breaks)

- **A20 Line Toggle Fails**:
  1. A20 switching might fail on specific hardware.
  2. Use alternative A20 options (e.g. `A20CONTROL` switches) to select a different hardware method.
- **Conventional Memory Bottlenecks**:
  - If conventional memory drops below 580KB, ensure `DOS=HIGH,UMB` is set in `CONFIG.SYS` and check that EMM386 has allocated UMBs.
- **EMS Frame Overlap**:
  - If EMS functions fail, ensure the page frame (`FRAME=D000` or similar) does not conflict with system BIOS or video memory.
