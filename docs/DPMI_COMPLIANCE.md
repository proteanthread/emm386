# EMM386 DPMI 32-Bit & 64-Bit Specification Compliance Guide

This document validates that the built-in **DPMI32** and **DPMI64 (LMS64)** hosts in EMM386 comply with standard DPMI specifications. It demonstrates how standard DOS extenders and DPMI servers (such as `DOS4GW.EXE`, `CWSDPMI.EXE`, and `QDPMI`/`QEMM`) are rendered redundant under LibreDOS, while still allowing them to be loaded for compatibility.

---

## 1. DPMI Compliance Architecture

EMM386 implements the core **DOS Protected Mode Interface (DPMI) V0.9 / V1.0** specifications. It hooks software interrupts `INT 2Fh` and `INT 31h` to expose native protected-mode and long-mode allocations.

### A. DPMI Entry Detection (`INT 2Fh AX=1687h`)
When an application (such as a DJGPP-compiled binary or a Watcom C program) starts:
1. It queries DPMI availability by calling `INT 2Fh` with `AX=1687h`.
2. EMM386 intercept routines reply by:
   - Returning `AX = 0000h` (indicating DPMI host is present).
   - Returning flags in `BX` (indicating 32-bit and/or 64-bit client capabilities).
   - Returning the processor type in `CL` (e.g. `03h` for 80386, `06h` for x86-64 long-mode compatible).
   - Returning the protected-mode entry vector address in `ES:DI`.
3. The program calls the returned vector address to execute the mode switch. Once switched, the program runs in protected/long mode and calls DPMI services via `INT 31h`.

### B. Supported DPMI Functions (`INT 31h`)
EMM386 handles the core function groups:
- **LDT Descriptor Management (AH=00h)**: Allocates descriptors, gets/sets base addresses, sets segment limits, and configures segment access rights.
- **DOS Memory Management (AH=01h)**: Allocates and frees real-mode conventional blocks for data transfer.
- **Interrupt Vector Management (AH=02h)**: Gets/sets protected-mode and real-mode interrupt hooks.
- **Real-Mode Call Simulation (AH=03h)**: Simulates real-mode software interrupts and calls real-mode procedures from protected mode.
- **Physical Memory Management (AH=05h)**: Allocates and resizes physical extended memory blocks.
- **Page Lock Management (AH=06h)**: Locks and unlocks virtual page ranges.

---

## 2. Extender Redundancy & Compatibility

Under LibreDOS, EMM386's native host implementation alters the role of legacy extenders:

- **`DOS4GW.EXE` / Watcom Extender**:
  - Legacy binaries bound with `DOS4GW` contain a stub that checks for an active DPMI server.
  - When run under LibreDOS, `DOS4GW` detects EMM386's built-in DPMI host and runs *on top of it* without loading its own standalone protected-mode initialization tables.
  - If a program is compiled as a clean DPMI client, it executes directly without requiring the `DOS4GW.EXE` file at all.
- **`CWSDPMI.EXE` (DJGPP DPMI Server)**:
  - 32-bit and 64-bit DJGPP applications run natively on EMM386's built-in host.
  - `CWSDPMI.EXE` is not required to be present in the path or run as a resident TSR; EMM386 answers all `INT 2Fh AX=1687h` queries and manages the 32-bit/64-bit heap allocations natively.
- **`QDPMI` / `QEMM`**:
  - Legacy memory manager DPMI providers are fully redundant. EMM386 handles the hooks directly.

*Note: For compatibility, if a program explicitly launches its own bundled DPMI extender, EMM386 yields the LDT allocation ranges, allowing the custom extender to coexist safely.*

---

## 3. Customization & Control (What We Can Change)

- **LDT Descriptor Capacity**: You can configure the maximum descriptor slots allocated to the client.
- **Dynamic DPMI Heap Limit**: You can change the starting and maximum heap size limits returned to the client during query free memory calls.
- **Interrupt Redirect Routines**: You can adjust how protected-mode exception traps (such as GPFs or Page Faults) are logged or displayed for debugging.

---

## 4. Architectural Rules (What We Cannot Change)

- **Entry Vector Calling Convention**: The inputs/outputs of the `INT 2Fh AX=1687h` query must strictly follow standard specifications to prevent legacy loaders from crashing.
- **Descriptor Type Formats**: LDT descriptor structures and selector index bitmask formats are defined by x86 CPU hardware and cannot be changed.
- **INT 31h Function Numbers**: Standard function numbers (e.g. `0000h` to `0604h`) must remain mapped to their respective operations to ensure client executables run without binary modifications.

---

## 5. What to Expect (System Behavior)

- **Automatic Bypass**: Legacy applications load instantly without looking for or spawning shell messages from `DOS4GW` or `CWSDPMI`.
- **Lower Footprint**: By eliminating external TSR managers and memory extenders, conventional memory footprint is reduced, and execution speed is improved by bypassing layered loaders.
- **Execution Traps**: Real-mode calls made by protected-mode clients trap through EMM386's supervisor layer, translating segmented data buffers automatically.

---

## 6. BSD Virtual Memory Integration

For 64-bit DPMI applications running via LMS64:
- **Direct Paging Maps**: EMM386 translates DPMI64 memory requests to BSD-style virtual memory mappings.
- **Virtual Page Allocation**: Standard DPMI physical block allocations (`0501h`) use BSD `mmap` parameters to map pages above 4GB, preventing 64-bit clients from conflicting with 32-bit DOS blocks.

---

## 7. Troubleshooting & Recovery (What to Do If Something Breaks)

- **"DPMI Host Not Found" or "CWSDPMI Required" error**:
  1. The client is not executing `INT 2Fh AX=1687h` correctly or the hook is blocked.
  2. Verify that EMM386 is loaded and that the DPMI flag is enabled in the configuration file.
- **General Protection Faults (EXC0Dh) on Switch**:
  - Check client stack size. DPMI clients must configure a valid stack segment before enabling protected mode.
- **Coexistence Conflicts with Custom Loaders**:
  - If a specific legacy game crashes when trying to load its own extender, use EMM386 command-line exclusions to disable the internal DPMI server for that application session.
