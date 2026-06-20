# EMM386 / JEMM Memory Manager Flowcharts & Architecture

This document maps out the core execution paths of the Jemm memory manager (JEMM16/JEMM32) and the loading procedure of Jemm Loadable Modules (JLMs).

---

## 1. JEMM Initialization Flow

The diagram below shows the system boot and initialization process when JEMM is loaded (either as a device driver in `config.sys` or dynamically via `JLOAD.EXE`).

```mermaid
graph TD
    A["System Boot / Loader Launch"] --> B["INIT16 Real-Mode Entry"]
    B --> C["Validate CPU Level & Memory Size"]
    C --> D["Identify E820 / Int 15h Memory Map"]
    D --> E["Allocate Upper Memory Blocks & Page Tables"]
    E --> F["Switch to Protected Mode"]
    F --> G["Initialize JEMM32 Kernel"]
    G --> H["Setup GDT, IDT, and Page Directory"]
    H --> I["Map Extended Memory & EMS Page Frame"]
    I --> J["Establish V86 Mode Context"]
    J --> K["Install Interrupt Hooks: Int 67h, Int 2Fh, Int 15h"]
    K --> L["Return to V86 Real-Mode Supervisor"]
```

---

## 2. JLM Loading & Linking Flow

This flowchart describes how `JLOAD` loads a Jemm Loadable Module (JLM) and registers its services using the List of Lists and VMM service tables.

```mermaid
graph TD
    A["JLOAD.EXE Invocation"] --> B["Load PE executable JLM file"]
    B --> C["Resolve Relocations & Imports"]
    C --> D["Retrieve VMM Service Table from JLM.H"]
    D --> E["Call JLM Entry Point: DllMain / main"]
    E --> F{"Is Load Request?"}
    F -- "Yes" --> G["Register Device Description Block DDB"]
    G --> H["Hook Interrupts / Traps via VMM Services"]
    H --> I["Return Success (suppress JLoad message via Bit 31)"]
    F -- "No / Unload" --> J["Unhook Interrupts & Trap Handlers"]
    J --> K["Remove DDB from VMM Chain"]
    K --> L["Free JLM Memory & Terminate"]
```

---

## 3. 64-Bit DPMI & BSD Memory Mapping Flow (LMS64)

This flowchart details how a 64-bit DPMI client executes in x86-64 long mode, mapping memory above the 4GB physical boundary using standard BSD virtual memory specifications.

```mermaid
graph TD
    A["64-Bit DPMI Client Request: lms64_mmap"] --> B["Parse Virtual Address, Length, and Permissions"]
    B --> C["Align Request to 4KB Page Boundaries"]
    C --> D["Scan Global Host Page Pool above 4GB"]
    D --> E["Walk Client PML4 -> PDPT -> PD -> PT Structures"]
    E --> F["Allocate Intermediate Page Directory Tables if Missing"]
    F --> G["Write Page Table Entries PTEs with Physical Address & Attributes"]
    G --> H["Invalidate TLB Cache via invlpg"]
    H --> I["Track Region Descriptor in g_regions Array"]
    I --> J["Return 64-Bit Virtual Address Pointer to Client"]
```

---

## 4. Maintenance Guidelines

### A. What We Can Change
- **Wrappers & Interface Helpers**: You can add logging wrappers, output debug hooks, and custom printf telemetry loops inside `.c` files.
- **Diagnostics**: You can modify screen rendering logic or register output formatters in debugging functions.
- **Local Helper Variables**: You can add internal local state flags or temporary arrays inside standard C functions that are not shared with assembly code.
- **Paging Policy**: You can modify the virtual address allocation base (e.g. `g_next_free_virt`) or adjust mapping region limit checks.

### B. What We Cannot Change
- **Struct Order & Alignment**: You cannot modify the member ordering or alignment constraints in shared structs (e.g. `Client_Reg_Struc`, `Client_Reg_Struc64`, `cb_s`, `Pushad_Struc` in `JLM.H` or `LMS64.H`) as their layout must align exactly with binary offsets expected by Assembly modules.
- **Hardware Register Mapping**: The register inputs and outputs (e.g. `RAX`, `RDX`, `RSP`) mapped to standard MS-DOS interrupts (e.g. Int 21h, Int 67h, Int 15h) must remain unmodified.
- **Service Ordinals**: The VxD/VMM service ordinal positions defined in `JLM.H` cannot be reordered or deleted because they match binary API indexes compiled into the memory manager kernel.

### C. What to Expect
- **V86 Memory Constraints**: Linear addresses used during file or device access must reside within a 64 KB offset distance from the V86 DS segment register.
- **Protected Mode Trapping**: Accessing ports or instructions outside normal privileges will trigger a VM monitor trap, routing control to the emulated handlers in `EMU.c` and `VDMA.c`.
- **4-Level Paging Latency**: Accessing unmapped virtual segments triggers a page fault exception, resolved dynamically by allocating intermediate paging directories.

### D. What to Do If Something Breaks / Troubleshooting
- **Compiler Options**: If Watcom C (`wcc386`) rejects options, verify the include directory paths are passed with double quotes (e.g. `"-I..\Include"`).
- **Binary Offsets Verification**: If the system locks up, dump structure sizes and trace alignment boundaries using map files (`*.map`) to ensure C structures align exactly with their Assembly definitions.
- **Check Git Status**: Run `git diff` or `git status` to verify that original tracked repository files (`JLM.H`, `HELLO2.C`) were not modified in a way that breaks existing toolchains.
