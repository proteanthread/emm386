# EMM386 Memory Manager Users Guide

This document guides you through installing, configuring, and optimizing the **EMM386 / Jemm** memory manager in the LibreDOS operating system context.

---

## 1. Overview & Quick Start

EMM386 functions as a V86-mode memory manager that manages:
- **Upper Memory Blocks (UMBs)**: Allocates high memory regions (640KB to 1MB) to load device drivers and TSRs using `DEVICEHIGH` and `LOADHIGH`.
- **High Memory Area (HMA)**: Manages the first 64KB block above 1MB (segments `FFFF:0010` to `FFFF:FFFF`), allowing LibreDOS to load its kernel buffers high.
- **LIM EMS 4.0 (Expanded Memory)**: Simulates expanded memory using physical extended memory (XMS), exposing a 64KB page frame in the Upper Memory Area.
- **64-Bit DPMI Flat Memory (LMS64)**: Dynamically loads the flat memory helper to run 64-bit long-mode programs with access to memory above 4GB.

### Installing in CONFIG.SYS
Add the following line to your `CONFIG.SYS` file (or `FDCONFIG.SYS` / `LCONFIG.SYS`):
```ini
DEVICE=C:\DOS\EMM386.EXE RAM I=E000-EFFF FRAME=D000
```
Alternatively, if you do not require EMS page frame mapping, use the `NOEMS` switch to gain 64KB more UMB space:
```ini
DEVICE=C:\DOS\EMM386.EXE NOEMS I=C800-EFFF
```

---

## 2. Command-Line Options & Switches

| Switch | Description | Example |
| :--- | :--- | :--- |
| `FRAME=seg` | Base segment of the 64KB EMS Page Frame. | `FRAME=D000` |
| `I=start-end` | Force include segment range for UMB usage. | `I=C800-DFFF` |
| `X=start-end` | Force exclude range to prevent adapter overlaps. | `X=D000-D3FF` |
| `RAM` | Enable both UMB allocation and EMS page frame mapping. | `RAM` |
| `NOEMS` | Enable UMB allocation but disable the EMS page frame. | `NOEMS` |
| `/HMAMIN=k` | Set minimum HMA space (in KB) for a program to load high. | `/HMAMIN=16` |
| `/NUMHANDLES=n`| Configure maximum XMS handles. | `/NUMHANDLES=64` |

---

## 3. Customization & Control (What We Can Change)

- **UMB Scan Boundaries**: You can alter the starting and ending scan segments using `I=` and `X=` switches to map around sound cards, network cards, and SCSI adapters.
- **HMA Allocation Threshold**: You can adjust `/HMAMIN` to prioritize which TSRs get HMA access.
- **EMS Page Frame Segment**: You can relocate the 64KB window anywhere in the UMA (`C000h` to `E000h`) that does not conflict with BIOS ROMs.
- **Dynamic Module Loading**: You can dynamically load and unload modules (such as `LMS64` for 64-bit flat paging support) using `JLOAD.EXE`.

---

## 4. Architectural Rules (What We Cannot Change)

- **Real Mode Segment-Offset Boundaries**: Virtual-86 mode cannot bypass standard 16-bit segmented address translation below the 1MB barrier. All real-mode calls use `Segment:Offset` format.
- **HMA Size Limit**: The HMA is physically limited to `65,520` bytes (64KB - 16 bytes). It cannot be expanded beyond this limit due to 8086 segment-wrapping rules.
- **x86 CPU Modes**: EMM386 requires a minimum of an Intel 80386 CPU to run in protected V86 mode. It will not load on an 80286 or 8086.

---

## 5. What to Expect (System Behavior)

- **Boot Messages**: Upon boot, EMM386 prints a memory status report showing conventional memory backfilling, HMA status, available UMB size, and EMS handle count.
- **Interrupt Redirections**: Software interrupts `INT 67h` (EMS), `INT 2Fh` (XMS), and `INT 15h` (V86 control) are intercepted by EMM386, resulting in a microscopic interrupt dispatch latency (nanoseconds).
- **Driver Relocations**: Running `LH` or `DEVICEHIGH` relocates programs to the UMA, freeing conventional memory up to 620KB+ for standard DOS programs.

## 6. BSD Virtual Memory Integration

EMM386 itself does not need to access physical memory above 4GB directly (which is handled natively and automatically by the LibreDOS kernel). However, EMM386 provides the compatibility interfaces to allow legacy/DPMI applications to access these memory pools (above 1MB to 4GB or more):
- **Host Page Mapping**: When a 64-bit DPMI client is executed, EMM386 hooks it into the **LMS64 Module**. The client requests allocations using standard BSD-style virtual memory mappings (e.g. `PROT_READ`, `PROT_WRITE`, `MAP_ANONYMOUS`).
- **Paging Bridge**: These allocations are mapped dynamically using a 4-level PML4 paging table structure managed by LMS64 and routed directly to the underlying LibreDOS kernel page allocator.

---

## 7. Troubleshooting & Recovery (What to Do If Something Breaks)

- **System Freezes During Boot**:
  1. A device conflict is likely occurring in the UMA. Restart the machine and hold `F5` to bypass `CONFIG.SYS`.
  2. Use the `X=` switch to exclude the adapter regions (e.g. `X=C800-CFFF` or `X=D000-DFFF`).
- **"EMM386 not active" Error**:
  - Verify that `HIMEM.SYS` (or equivalent XMS manager) is loaded *before* `EMM386.EXE` in `CONFIG.SYS`.
- **"HMA not available" Message**:
  - Another driver has hooked the HMA first. Ensure `DOS=HIGH` is specified and that no other memory manager is active.
- **64-Bit DPMI App Fails to Load**:
  - Ensure the `LMS64` helper module has been loaded using `JLOAD LMS64.JLM`. Check system logs using `MEMSTAT` to verify that memory above 4GB has not been fully exhausted.
