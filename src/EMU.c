/*
 * emu.asm - --- privileged opcode emulation --- copyright Tom Ehlert --- to be assembled with JWasm or Masm v6.1+ (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart source representing EMU.ASM.
 *
 * What Can Be Changed:
 *   - Local helper functions, logging wrappers, and diagnostic outputs.
 *   - Addition of supported opcode templates, conditional flags (e.g., EMUCLTS).
 *
 * What Cannot Be Changed:
 *   - Standard API calling conventions, hardware entry vectors, and binary structure alignments.
 *   - Self-modifying instruction template (EmuInstr location and size), exception handling vectors, and privilege level checks.
 *
 * Expected Behaviour:
 *   - Mapped counterpart declarations and logic flow from the original assembly source.
 *   - Detects and emulates privileged x86 instruction execution (like CLTS, INVD, WBINVD, WRMSR, RDTSC, RDMSR) in V86 mode.
 *
 * What To Do If Something Breaks Or Does Not Work:
 *   - Verify compiler alignment flags and register preservation states if system lockups occur.
 *   - Ensure CPU instruction decoding is correct, inspect self-modifying code execution states, and verify client register stack alignments.
 */

// EMUCLTS equ 0	; 1=emulate CLTS ( resets TS bit in CR0 )

// --- publics/externals

//   assume SS:FLAT,DS:FLAT,ES:FLAT

// --- emulate some privileged 0Fh opcodes (not HLT)
// --- ebp -> client struct
// --- ESI -> linear address of CS:EIP
// --- AL==0Fh
// --- this is not called, but jumped to

// .486P
// .model FLAT
void ExtendedOp(void)
{
    /*
     * Mapped logic from ExtendedOp in EMU.ASM:
     * ExtendedOp proc public
     *     mov al,[esi+1]
     * if EMUCLTS
     *     cmp al,6
     *     je @@clts
     * endif
     *     cmp al,9
     *     je @@wbinvd
     *     cmp al,8
     *     je @@invd
     *     cmp al,30h
     *     je @@wrmsr
     *     cmp al,31h	; whether RDTSC is privileged depends on CR4, bit 2 (TSD)
     *     je @@rdtsc
     *     cmp al,32h
     *     je @@rdmsr
     *     cmp al,20h
     *     jb V86_Exc0D ; not an opcode we emulate
     *     cmp al,23h
     *     ja V86_Exc0D
     * 
     * ; opcodes 0F 20 xx to 0F 23 xx emulated via self-modifying code
     * 
     *     mov ah,[esi+2]  ; get third byte of opcode
     *     mov WORD PTR [EmuInstr+0],ax
     *     add [ebp].Client_Reg_Struc.Client_EIP,3 ; jump over emulated instruction
     *     @v86popregX
     *     call RunEmuInstr
     *     add esp,4+4                 ; eat error code & "int#"
     *     iretd
     * if EMUCLTS
     * @@clts:
     * 	clts                ; 0f 06 is clts opcode
     *     jmp @@invdshare
     * endif
     * @@invd:
     *     invd                ; 0f 08 is invd opcode
     *     jmp @@invdshare
     * @@wbinvd:
     *     wbinvd              ; 0f 09 is wbinvd opcode
     * @@invdshare:
     *     @v86popregX
     *     jmp @@twoeat
     * 
     * @@wrmsr:
     *     @v86popregX
     *     @wrmsr
     *     jmp @@twoeat
     * 
     * ; early pentiums and such will throw an exception on rdtsc instruction in V86
     * ;  regardless of CR4 setting, later CPU versions won't
     * 
     * @@rdtsc:
     *     @v86popregX
     *     @rdtsc
     *     jmp @@twoeat
     * 
     * @@rdmsr:
     *     @v86popregX
     *     @rdmsr              ; rdmsr opcode
     * 
     * @@twoeat:
     *     add esp,4+4         ; eat return address and error code
     *     add [esp].IRETDV86._Eip,2   ; jump over instruction
     *     iretd
     * 
     *     align 4
     * 
     * ExtendedOp endp
     */
}


// .text$03 ends

// END
