/*
 * debugsys.inc - Real mode Debugger services: (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing DEBUGSYS.INC.
 *
 * Changeability & Constraints:
 *   - CAN BE CHANGED: Local helper functions, logging wrappers, and diagnostic outputs.
 *   - CANNOT BE CHANGED: Standard API calling conventions, hardware entry vectors, and binary structure alignments.
 *
 * Expected Behavior:
 *   - Mapped counterpart declarations and logic flow from the original assembly source.
 *
 * Diagnostics & Recovery:
 *   - Verify compiler alignment flags and register preservation states if system lockups occur.
 */

#define D386_RM_Int 68h // hooked by the debugger in real mode.

#define D386_Id 0F386h // debugger identification code

#define D386_MIN 43h // minimum INT 68 function code
#define D386_Identify 43h // returns debugger identification, if debugger
#define D386_Prepare_PMode 44h // partially prepare for protected mode operation
#define D386_Real_Mode_Init 45h // re-init fro real mode after entering pmode
#define D386_Set_Switches 46h // set debugging switches
#define D386_Execute_Cond 47h // execute conditional BP (/B option)
#define D386_Free_Segment 48h // undefine the real mode segment's symbols
#define D386_Set_Baudrate 49h // set com port baud rate
#define D386_Reinit 4ah // reinitialize debugger for protected mode
#define D386_Def_Deb_Segs 4bh // define debugger's segments
#define D386_Set_Com_Port 4ch // set com port number
#define D386_Link_Sym 4dh // link sym file map
#define D386_Unlink_Sym 4eh // unlink sym file maps
#define D386_Remove_Segs 4fh // remove any undefined segments from the
#define D386_Load_Segment 50h // defines the actual segment/selector for a
#define D386_Display_Char 51h // display a character to the debugging terminal
#define D386_Display_Str 52h // display a string to the debugging terminal
#define D386_IsVxDInstalled 53h // returns if debug VxD has been installed
#define D386_VxDInstall 54h // sets that the debug VxD installed/uninstalled
#define D386_RegisterDotCmd 55h // registers dot command
#define D386_DeRegisterDotCmd 56h // de-registers dot command
#define D386_Printf 57h // Printf
#define D386_Link_Sym_Phys 58h // link symbol file with physical address
#define D386_CheckMap 59h // DX:DI = pointer to module name
#define D386_SetAutoLoadSym 5ah // (BL) != 0, auto load symbols
#define D386_SetTeftiPort 5bh // (BX) = TEFTI port address
#define D386_ExecDebugCommand 5ch // execute debugger command script
#define D386_LoadCodeDataHigh 5dh // makes the debugger copy its code/data high
#define D386_SetWinVersion 5eh // sets Windows version number
#define D386_ScanChar 5fh // scan for character
#define D386_UnGetChar 60h // ungetchar scaned character, AL = char
#define D386_Stop 61h // stop at the CS:IP specified
#define D386_MAX 61h // maximum INT 68 function code

// D386_Load_Segment type equates:

#define ST_code_sel 0 // code selector
#define ST_data_sel 1 // data selector

#define ST_code_seg 10h // code segment
#define ST_data_seg 11h // data segment

#define ST_dual_code 40h // code segment and selector
#define ST_dual_data 41h // data segment and selector

#define ST_device_code 80h // device driver code segment
#define ST_device_data 81h // device driver data segment

// D386_Load_Segment device load parameters structure

// D386_Device_Params STRUC
// DD_logical_seg	dw  ? // logical segment # from map
// DD_actual_sel	dw  ? // actual selector value
// DD_base			dd  ? // linear address offset for start of segment
// DD_length		dd  ? // actual length of segment
// DD_name			df  ? // 16:32 ptr to null terminated device name
// DD_sym_name		df  ? // 16:32 ptr to null terminated symbolic
// DD_alias_sel	dw  ? // alias selector value (0 = none)
// D386_Device_Params ENDS

#define WDEB_INT2F_STARTING 0 // first time starting
#define WDEB_INT2F_ENDING 1 // first time ending
#define WDEB_INT2F_NESTED_STARTING 2 // start on level of nesting
#define WDEB_INT2F_NESTED_ENDING 3 // end one level of nesting

// PMINIT routine functions

#define PMINIT_INIT_IDT 0 // (ES:EDI) = pointer to PM IDT
#define PMINIT_INIT_PAGING 1 // (BX) = phys-linear selector
#define PMINIT_ENABLE_DEBUG_QUERYS 2 // enables dot commands, etc.
#define PMINIT_INIT_SPARE_PTE 3 // (EBX) = lin addr of spare PTE
#define PMINIT_SET_ENTER_EXIT_VMM 4 // (EBX) = Enter VMM routine addr
#define PMINIT_GET_SIZE_PHYS 5 // get debugger size/phys addr
#define PMINIT_SET_BASE_SPARE_PTE 6 // set debugger base/spare PTE
#define PMINIT_ENABLE_MEMORY_CONTEXT 7 // enables mem context functions
#define PMINIT_MAX 7

// if 0
// WdebVCPIInfo	  STRUCT
// fnVCPI	df	? // VCPI protect mode server entry point
// rdsVCPI	dw	? // Selector for VCPI server
// laVTP	dd	? // linear address of data structure containing
// Port67	dw	? // Qualitas magic port for emulating INT 67h
// WdebVCPIInfo	  ENDS

// VTP	struct
// zaCr3VTP	dd	0 // physical addr of page directory
// laGdtrVTP	dd	0 // linear addr in first meg of gdtr
// laIdtrVTP	dd	0 // linear addr in first meg of idtr
// selLdtVTP	dw	0 // selector of ldt
// selTrVTP	dw	0 // selector of tr
// ipVTP		dw	0 // 48-bit address of protect
// unusedVTP	dw	0 // mode entry point to xfer to
// csVTP		dw	0 // 
// VTP	ends

#define VCPI_RM_CALLOUT_INT 67h // v86 mode call to VCPI server
#define VCPI_PROT_ENTRY 0DE0CH
// endif


// Protected mode Debugger services:

#define Debug_Serv_Int 41h // Interrupt that calls Deb386 to perform

#define DS_Out_Char 0 // function to display the char in DL
#define DS_In_Char 1 // function to read a char into AL
#define DS_Out_Str 2 // function to display a NUL terminated string
#define DS_Is_Char 3 // Non blocking In_Chr
#define DS_Out_Str16 12h // function to display a NUL terminated string
#define DS_ForcedGO16 40h // enter the debugger and perform the equivalent
#define DS_LinkMap 45h // DX:(E)DI = ptr to paragraph in front of map
#define DS_UnlinkMap 46h // DX:(E)DI = ptr to paragraph in front of map
#define DS_CheckMap 47h // DX:(E)DI = pointer to module name
#define DS_IsAutoLoadSym 48h // returns AX != 0, auto load symbols
#define DS_DebLoaded 4Fh // check to see if the debugger is installed and
#define DS_DebPresent 0F386h
#define DS_LoadSeg 50h // define a segment value for the
#define DS_LoadSeg_32 0150h // Define a 32-bit segment for Windows 32
#define DS_MoveSeg 51h // notify the debugger that a segment has moved
#define DS_FreeSeg 52h // notify the debugger that a segment has been
#define DS_FreeSeg_32 0152h // notify the debugger that a segment has been
#define DS_DGH 56h // register "dump global heap" handler
#define DS_DFL 57h // register "dump free list" handler
#define DS_DLL 58h // register "dump LRU list" handler
#define DS_StartTask 59h // notify debugger that a new task is starting
#define DS_Kernel_Vars 5ah // Used by the Windows kernel to tell the
#define DS_VCPI_Notify 5bh // notify debugger that DOS extender is
#define DS_ReleaseSeg 5ch // This does the same as a DS_FreeSeg, but
#define DS_User_Vars 5dh // DS:SI = pointer to an array of offsets:
// DS_POSTLOAD		=	60h // Used by the RegisterPTrace interface
// DS_EXITCALL		=	62h // Somebody will fill these in if we ever
// DS_INT2			=	63h // figure out what they are supposed to do.
// DS_LOADDLL		=	64h
// DS_DELMODULE	=	65h

// DS_NEWTASK		=	0BH
// DS_FLUSHTASK	=	0CH
// DS_SWITCHOUT	=	0DH
// DS_SWITCHIN		=	0EH

#define DS_IntRings 20h // function to tell debugger which INT 1's & 3's
#define DS_IncludeSegs 21h // function to tell debugger to go ahead and
// MaxDebugSegs 	= 20

#define DS_CondBP 0F001h // conditional break pt, if the command line
#define DS_ForcedBP 0F002h // break pt, which accomplishes the same thing
#define DS_ForcedGO 0F003h // enter the debugger and perform the equivalent
#define DS_HardINT1 0F004h // check to see if INT 1 hooked for all rings
#define DS_Out_Symbol 0Fh // find the symbol nearest to the address in
#define DS_Disasm_Ins 10h // function to disassemble the instruction

#define DS_JumpTableStart 70h
#define DS_RegisterDotCommand 70h
#define DS_RegisterDotCommand16 71h
#define DS_DeRegisterDotCommand 72h
#define DS_Printf 73h
#define DS_Printf16 74h
#define DS_GetRegisterSet 75h
#define DS_SetAlternateRegisterSet 76h
#define DS_GetCommandLineChar 77h
#define DS_EvaluateExpression 78h
#define DS_VerifyMemory 79h
#define DS_PrintRegisters 7ah
#define DS_PrintStackDump 7bh
#define DS_SetThreadID 7ch
#define DS_ExecDebugCommand 7dh
#define DS_GetDebuggerInfo 7eh
#define DS_CheckFault 7fh
#define DS_SetBreak 80h
#define DS_RedirectExec 81h
#define DS_PassOnDebugCommand 82h
#define DS_TrapFault 83h
#define DS_SetStackTraceCallBack 84h
#define DS_RemoveSegs 85h
#define DS_DefineDebugSegs 86h
#define DS_SetBaudRate 87h
#define DS_SetComPort 88h
#define DS_ChangeTaskNum 89h
#define DS_ExitCleanup 8ah
#define DS_InstallVGAHandler 8bh
#define DS_GetComBase 8ch
#define DS_GetSymbol 8dh
#define DS_CopyMem 8eh
#define DS_JumpTableEnd 8eh

// SaveRegs_Struc	struc
// Debug_EAX		dd	?
// Debug_EBX		dd	?
// Debug_ECX		dd	?
// Debug_EDX		dd	?
// Debug_ESP		dd	?
// Debug_EBP		dd	?
// Debug_ESI		dd	?
// Debug_EDI		dd	?
// Debug_ES		dw	?
// Debug_SS		dw	?
// Debug_DS		dw	?
// Debug_FS		dw	?
// Debug_GS		dw	?
// Debug_EIP		dd	?
// Debug_CS		dw	?
// dd	?
// Debug_EFlags		dd	?
// Debug_CR0		dd	?
// Debug_GDT		dq	?
// Debug_IDT		dq	?
// Debug_LDT		dw	?
// Debug_TR		dw	?
// Debug_CR2		dd	?
// Debug_CR3		dd	?
// Debug_DR0		dd	?
// Debug_DR1		dd	?
// Debug_DR2		dd	?
// Debug_DR3		dd	?
// Debug_DR6		dd	?
// Debug_DR7		dd	?
// Debug_DR7_2		dd	?
// Debug_TR6		dd	?
// Debug_TR7		dd	?
// Debug_TrapNumber	dw	-1 // -1 means no trap number
// Debug_ErrorCode		dw	0 // 0 means no error code
// SaveRegs_Struc ends

// DebInfoBuf	struc
// DIB_MajorVersion	db	0
// DIB_MinorVersion	db	0
// DIB_Revision		db	0
// db	0 // reserved
// DIB_DebugTrap16		dd	0 // send 16 bit trap to debugger
// DIB_DebugTrap32		df	0 // send 32 bit trap to debugger
// DIB_DebugBreak16	dd	0 // 16 bit break in debugger
// DIB_DebugBreak32	df	0 // 32 bit break in debugger
// DIB_DebugCtrlC16	dd	0 // 16 bit check for ctrl C
// DIB_DebugCtrlC32	df	0 // 32 bit check for ctrl C
// DebInfoBuf	ends

// BreakStruc	struc
// BS_BreakEIP		dd	0 // CS:EIP, SS:ESP to go to
// BS_BreakCS		dw	0 // on a error or ctrlc break
// BS_BreakESP		dd	0
// BS_BreakSS		dw	0
// BreakStruc	ends

// RedirectExecStruc	struc
// RDE_fpbufDebugCommand	df	0 // debugger command script
// RDE_cbDebugCommand	dw	0 // debugger command script len
// RDE_fpszInput		df	0 // input stream pointer
// RDE_usFlags		dw	0 // reserved (must be 0)
// RDE_cbOutput		dd	0 // size of output buffer
// RDE_fpbufOutput		df	0 // output buffer pointer
// RedirectExecStruc	ends

#define REPEAT_FOREVER_CHAR 0feh // send next character until
// end of debugger command

// AddrS	struc // for printf service
// AddrOff		dd	0
// AddrSeg		dw	0
// AddrType	db	0
// AddrSize	db	0
// AddrTask	dw	0
// AddrS	ends

#define AddrTypeSize word ptr AddrType

#define EXPR_TYPE_SEG 00000001b // address type segment:offset
#define EXPR_TYPE_SEL 00001001b // address type selector:offset
#define EXPR_TYPE_LIN 00000010b // address type linear
#define EXPR_TYPE_PHY 00001010b // address type physical
#define EXPR_TYPE_LOG 00001000b // logical address (no sel yet)

#define DEBUG_FAULT_TYPE_V86 00000001b
#define DEBUG_FAULT_TYPE_PM 00000010b
#define DEBUG_FAULT_TYPE_RING0 00000100b
#define DEBUG_FAULT_TYPE_FIRST 00001000b
#define DEBUG_FAULT_TYPE_LAST 00010000b

// 
// Interrupt and services that Win386 provides to the debugger
// 

#define Win386_Query_Int 22h // interrupt for Win386 protected mode

#define Win386_Alive 0 // function 0, query Win386 installation
#define Win386_Q_Ack 0F386h // good response from func 43h, of
#define Win386_Query 1 // function 1, query Win386 state
#define Win386_PhysToLinr 2 // function 2, have Win386 convert a
#define Win386_AddrValid 3 // function 3, have Win386 check the
#define Win386_MapVM 4 // function 4, make sure that the VM's
#define Win386_UnmapVM 5 // function 5, map out the VM's low
#define Win386_GetDLAddr 6 // function 6, return offset of dyna-link
#define Win386_GetVXDName 7 // function 7, determines whether an address
#define Win386_GetPDE 8 // function 8, get pde for a context
#define Win386_GetFrame 9 // function 9, get phys addr for not pres ptes
#define Win386_GetLDTAddress 10 // function 10,
#define Win386_GetThreadID 11 // function 11, AX = Current Thread ID
#define Win386_GetTSHandler 12 // function 12, return offset of transfer-space
#define Win386_GetArplHandler 13 // function 12, return offset of ARPL fault
#define Max_Win386_Services 13
