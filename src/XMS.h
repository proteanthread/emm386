/*
 * xms.inc - --- XMS definitions (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing XMS.INC.
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

#define XMS_GETVERSION 0h
#define XMS_ENABLEA20 5h
#define XMS_DISABLEA20 6h
#define XMS_V2_QUERYMEM 8h
#define XMS_V2_ALLOCEMB 9h
#define XMS_FREEEMB 0Ah
#define XMS_LOCKEMB 0Ch
#define XMS_UNLOCKEMB 0Dh
#define XMS_ALLOCUMB 10h
#define XMS_V3_QUERYMEM 88h
#define XMS_V3_ALLOCEMB 89h

// XMS_HANDLE struct
// xh_flags    DB  ?
// xh_locks    DB  ?
// xh_baseK    DD  ?
// xh_sizeK    DD  ?
// XMS_HANDLE ends

// LPXMS_HANDLE typedef ptr XMS_HANDLE

// --- XMS handle flags

#define XMSF_FREE 1 // handle describes a free EMB
#define XMSF_USED 2 // handle describes a used EMB
#define XMSF_INPOOL 4 // handle is free


// XMS_HANDLETABLE struct
// xht_sig         DB  ?
// xht_sizeof      DB  ?
// xht_numhandles  DW  ?
// xht_pArray      DD  ? // converted to linear address for 32bit code!
// XMS_HANDLETABLE ends

// XMS_MOVE struct
// len           dd  ? // +0 block length in bytes
// src_handle    dw  ? // +4 source handle
// src_offset    dd  ? // +6 offset into source
// dest_handle   dw  ? // +10 destination handle
// dest_offset   dd  ? // +12 offset into destination
// XMS_MOVE ends

// if ?INTEGRATED
// if ?XMS35
// XMS_MOVEX struct
// XMS_MOVE <>
// src_hi        db  ? // bits 32-39 of src
// dest_hi       db  ? // bits 32-39 of dst
// XMS_MOVEX ends
// endif
// endif

// --- XMS error codes

#define XMS_NOT_IMPLEMENTED 80h
#define XMS_VDISK_DETECTED 81h
#define XMS_A20_FAILURE 82h
#define XMS_DRIVER_FAILURE 8eh
#define XMS_DRIVER_FATAL 8fh
#define XMS_HMA_NOT_THERE 90h
#define XMS_HMA_IN_USE 91h
#define XMS_HMAREQ_TOO_SMALL 92h
#define XMS_HMA_NOT_USED 93h
#define XMS_A20_STILL_ENABLED 94h
#define XMS_ALL_MEM_ALLOCATED 0a0h
#define XMS_NO_HANDLE_LEFT 0a1h
#define XMS_INVALID_HANDLE 0a2h
#define XMS_INVALID_SOURCE_HANDLE 0a3h
#define XMS_INVALID_SOURCE_OFFSET 0a4h
#define XMS_INVALID_DESTINATION_HANDLE 0a5h
#define XMS_INVALID_DESTINATION_OFFSET 0a6h
#define XMS_INVALID_LENGTH 0a7h
#define XMS_INVALID_OVERLAP 0a8h
#define XMS_PARITY_ERROR 0a9h
#define XMS_BLOCK_NOT_LOCKED 0aah
#define XMS_BLOCK_LOCKED 0abh
#define XMS_LOCK_COUNT_OVERFLOW 0ach
#define XMS_LOCK_FAILED 0adh
#define XMS_ONLY_SMALLER_UMB 0b0h
#define XMS_NO_UMB_AVAILABLE 0b1h
#define XMS_UMB_SEGMENT_NR_INVALID 0b2h

