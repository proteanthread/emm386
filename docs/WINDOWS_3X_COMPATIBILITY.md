# EMM386 Windows 3.x Compatibility Guide

This document defines how EMM386 and the LibreDOS Memory Specification (LMS) enable the installation and execution of **Microsoft Windows 3.x (3.0, 3.1, and 3.11)** in Real, Standard, and 386 Enhanced modes.

---

## 1. Windows 3.x Execution Modes & Requirements

Windows 3.x runs in three distinct operational modes, each demanding specific memory management interfaces:

### A. Real Mode (Windows 3.0 Only)
- **Command**: `WIN /R`
- **Execution**: Runs entirely below the 1MB conventional memory barrier, utilizing expanded memory bank-switching.
- **EMM386 Role**: Fulfills the **LIM EMS 4.0** specification, exposing a 64KB page frame in the Upper Memory Area (typically at `D000h`) via `INT 67h`. Windows maps pages dynamically into this frame to load multiple applications.

### B. Standard Mode (Windows 3.0 & 3.1)
- **Command**: `WIN /S`
- **Execution**: Switches the CPU to 16-bit protected mode.
- **EMM386 Role**: Yields control via standard XMS protocols. Standard mode uses the XMS driver (`HIMEM.SYS` or EMM386's built-in XMS engine) to manage the A20 address gate and allocate extended memory blocks (EMBs).
- **V86 Conflict Resolution**: Because Standard mode switches to protected mode directly, it cannot run concurrently with an active Virtual-86 monitor unless EMM386 yields. EMM386 complies with standard segment-handover protocols to permit this switch.

### C. 386 Enhanced Mode (Windows 3.0, 3.1, & 3.11)
- **Command**: `WIN /3` (or simply `WIN`)
- **Execution**: Windows runs its own Virtual Machine Manager (VMM) in 32-bit protected mode, controlling the paging directories.
- **Transition Protocols**:
  - **VCPI (Virtual Control Program Interface)**: Used primarily by Windows 3.0. EMM386 provides VCPI services via `INT 67h` (e.g. `AX=DE01h` to get protected interface and `AX=DE0Ch` to switch between protected and V86 modes).
  - **GEMMIS (Global EMM Import Structure)**: Used by Windows 3.1 and 3.11. EMM386 implements GEMMIS support, exporting memory structures to Windows during startup. This allows the Windows VMM to safely take over the GDT and page directory tables from EMM386.

---

## 2. Customization & Control (What We Can Change)

- **VCPI Page Allocation Pool**: You can configure the maximum number of physical 4KB pages allocated to VCPI clients (Windows 3.0).
- **Command Line Exclusions**: You can configure memory range exclusions (`X=`) to prevent Windows' built-in drivers from colliding with mapped EMS page frames.
- **A20 Gating Speed**: You can select alternative hardware or software methods for toggling the A20 gate to resolve incompatibilities on specific PC clones.

---

## 3. Architectural Rules (What We Cannot Change)

- **GEMMIS Offset Structures**: The byte offsets and descriptors inside the GEMMIS export structure must align exactly with the Microsoft specifications to prevent Windows from crashing on boot.
- **VCPI API Interface**: Register states and function ordinals for VCPI calls (`AX=DE00h` to `AX=DE0Ch`) are fixed by the VCPI standard.
- **CPU Control Register Layouts**: CPU registers (`CR0`, `CR3`, `CR4`) and memory paging hierarchies are determined by the x86 architecture.

---

## 4. What to Expect (System Behavior)

- **Seamless Transitions**: When launching `WIN.COM`, EMM386 hands control of the CPU to the Windows kernel. Upon exiting Windows, EMM386 re-establishes the virtual-86 supervisor environment.
- **Memory Consolidation**: Running in 386 Enhanced Mode allows Windows to create virtual DOS machines (VDMs), each virtualizing a 1MB real-mode environment.
- **Microsecond Interrupt Latency**: Transitioning interrupts between the Windows protected-mode kernel and V86 mode introduces a microscopic trap latency.

---

## 5. BSD Virtual Memory Integration

While EMM386 supports 64-bit flat DPMI clients via LMS64 (using BSD-style `mmap` calls):
- **Isolation**: These 64-bit routines reside completely above the 4GB boundary and do not interfere with Windows 3.x's internal 32-bit memory organization.
- **Resource Protection**: The LibreDOS kernel manages memory above 4GB independently, ensuring that Windows 3.x is allocated memory within its 32-bit physical address limits.

---

## 6. Troubleshooting & Recovery (What to Do If Something Breaks)

- **Windows Freezes on Boot Screen (Logo screen hang)**:
  1. This is usually caused by a conflict between Windows' virtual device drivers (VxDs) and mapped upper memory.
  2. Edit your `SYSTEM.INI` file and add the following lines to the `[386Enh]` section:
     ```ini
     EMMExclude=A000-FFFF
     ```
  3. Alternatively, restart Windows in Standard Mode using `WIN /S` to isolate protected-mode mapping issues.
- **"GEMMIS Loader Error" on Startup**:
  - Ensure that EMM386 is loaded with the `GEMMIS` option enabled and that no other resident memory manager is intercepting `INT 2Fh`.
- **NMI (Non-Maskable Interrupt) Parity Errors**:
  - Windows might conflict with system hardware NMIs. Check the motherboard configuration and ensure that the EMM386 driver command-line options do not conflict with hardware RAM parity.
