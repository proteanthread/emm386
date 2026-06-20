# EMM386 Memory Manager Compilers Guide

This guide documents the compiler configurations, toolchain options, C17 standard formatting constraints, and struct alignments for building EMM386 under LibreDOS.

---

## 1. Toolchain & Compiler Options

EMM386 utilizes the **Open Watcom C/C++ x86 32-bit compiler (`wcc386`)** for compiling C17 modules.

### Compilation Command Layout
To compile modules like `LMS64.c` or helper JLMs:
```cmd
wcc386 -mf -zl -zls -s "-I..\Include" LMS64.c
```

### Compiler Flag Reference
- **`-mf`**: Flat Memory Model. Configures the compiler to generate code assuming a single flat 32-bit segment for code and data.
- **`-zl`**: Remove default library dependency references from the compiled object files. Necessary for bare-metal supervisor mode execution where standard libraries are absent.
- **`-zls`**: Set string literal alignment and character handling.
- **`-s`**: Suppress stack checking code, preventing the compiler from injecting call checks to standard library stack probes.
- **`-I`**: Specify include directories. Under PowerShell, wrap directory targets in double quotes (e.g. `"-I..\Include"`) to prevent parsing syntax splits.

---

## 2. ISO C17 Formatting & Compliance Rules

To maintain high code quality and prevent compiler issues, all C source and header files must comply with the **ISO C17 standard**:

- **Comment Modernization**:
  - Single-line comments, inline comments, and preprocessor explanations must use double-slash `//` format.
  - Multi-line file header descriptions must use C-style `/* ... */` blocks.
- **No K&R Parameter Styles**:
  - Parameterless functions must be explicitly declared with `void` (e.g., `void xms_handler(void)`).
- **Function Syntax Cleanup**:
  - Avoid non-standard trailing semicolons after function block closures.
- **Variable Declarations**:
  - Open Watcom's C compiler does not permit declaring loop counters inside `for` loops (e.g. `for (int i = 0; ...)`). Declare counters at the top of functions.
- **Hexadecimal Suffixes**:
  - Suffixes containing separators (e.g., `0xFFF_ULL`) are invalid. Use standard `0xFFFULL` syntax.

---

## 3. Structure Packing & Alignment

Because structures (like `Client_Reg_Struc64` and `Move_Struc`) share memory boundaries with Assembly code and CPU register dumps, exact alignment is critical:

- **Packing Directives**: Use `#pragma pack` directives to force 1-byte packing where binary structures map directly to register states or segment limits.
- **Padding Bytes**: Explicitly declare padding members (e.g. `uint16_t Client_Pad1[3]`) to align 64-bit boundaries and preserve compiler offsets.

---

## 4. Customization & Control (What We Can Change)

- **Compiler Options**: You can adjust compiler optimizations (e.g. `-ot` for time, `-os` for space) or change warning reporting levels (`-w4`).
- **Include Paths**: You can modify relative directory targets and include options in compile batches (`MAKEOW.BAT`).
- **Preprocessor Defines**: You can inject preprocessor flags (`-d?INTEGRATED=1`) to customize feature inclusions during build time.

---

## 5. Architectural Rules (What We Cannot Change)

- **C17 Compliance standards**: The code must compile cleanly using ISO C17 mode. Legacy K&R code or non-standard syntax extensions are prohibited.
- **Struct Offsets**: The binary ordering and byte boundaries of structures mapping to CPU registers cannot be altered, as assembly procedures expect specific offsets.
- **Bare-metal Environment**: No code can rely on standard C library features (`malloc`, `printf`, `free`) or standard header imports that assume a hosted operating system.

---

## 6. What to Expect (Compiler Outputs)

- **Zero-Warning Builds**: Compilation of modules should finish with **0 warnings and 0 errors**. Any compiler warning must be resolved.
- **Object Output**: Successful compilation yields standard 32-bit OMF object files (`*.obj`), which are then linked into JLM executables or merged into the EMM386 kernel.

---

## 7. BSD Virtual Memory Integration

To support BSD-style APIs without introducing standard C library dependencies:
- **Local Symbol Definitions**: Constants like `PROT_READ` and `MAP_ANONYMOUS` are defined locally within [LMS64.h](file:///C:/Users/rtdos/GitHub/emm386/Include/LMS64.h).
- **Type Definitions**: Basic types (`uint64_t`, `bool`, `size_t`) are defined locally using compiler preprocessor checks, preventing conflicts with Watcom system headers.

---

## 8. Troubleshooting & Recovery (What to Do If Something Breaks)

- **"Unresolved external symbol" Linker Error**:
  1. A function prototype was declared but its implementation is missing or not linked.
  2. Verify that the implementation module (e.g., `LMS64.obj`) is included in the linker command line.
- **Compiler Rejects Include Directory Path**:
  - Ensure the path uses double quotes around the include argument (e.g., `"-I..\Include"`) to bypass command-line parsing splits in the shell.
- **Structure Offset Mismatches**:
  - If context switches cause system crashes, check the compiled structure offsets using map files (`*.map`) and verify they match Assembly structures exactly.
