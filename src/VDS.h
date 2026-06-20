/*
 * vds.inc - --- VDS definitions --- flags in DX used by various VDS functions (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing VDS.INC.
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

#define VDSF_COPY 02h // bit 1 - copy in/out of DMA buffer (8107/8108)
#define VDSF_NOBUFFER 04h // bit 2 - don't alloc DMA buffer (8103)
#define VDSF_NOREMAP 08h // bit 3 - change PTEs; Jemm ignores this flag!
#define VDSF_64KALIGN 10h // bit 4
#define VDSF_128KALIGN 20h // bit 5
#define VDSF_PTE 40h // bit 6 - 1=scatter_lock returns PTEs
#define VDSF_NPPTE 80h // bit 7 - allow non-present pages for scatter/gather remap

// --- error codes returned in AL by lock (8103)

#define VDSERR_NOTCONTIGUOUS 1 // region not in contiguous memory
#define VDSERR_CROSSEDBOUNDS 2 // region crossed alignemnt boundary
#define VDSERR_CANTLOCKPAGES 3
#define VDSERR_NOBUFFER 4 // 8107
#define VDSERR_BUFFTOOSMALL 5 // 8107
#define VDSERR_BUFFINUSE 6

// --- other VDS error codes

#define VDSERR_07 7 // invalid memory region (8107)
#define VDSERR_REG_NOTLOCKED 8 // region wasn't locked (8106)
#define VDSERR_NAVL_TOOSMALL 9 // # of phys pages was > than table length (8105)
#define VDSERR_INVAL_BUFFID 10 // invalid buffer ID (8108,8109,810A)
#define VDSERR_BNDVIOLATION 11 // buffer boundary violated (8109,810A)
#define VDSERR_INVAL_DMACHN 12 // invalid DMA channel # (810B, 810C)
#define VDSERR_DISCNTOVFL 13 // disable cnt overflow (810B)
#define VDSERR_DISCNTUNFL 14 // disable cnt underflow (810C)
#define VDSERR_FUNC_NOTSUPP 15 // function not supported
#define VDSERR_DXRSVDBITSSET 16 // reserved flag bits set in DX (8102,8106,8108,8109,810A,810B,810C)

// --- DDS, used by 03-04, 07-08, 09-0A

// DDS struct
// dwSize  dd ? // +0  size of region
// dwOfs   dd ? // +4  offset virtual start address
// wSeg    dw ? // +8  segment/selector virtual start address (or 0000)
// wID     dw ? // +10 buffer ID
// dwPhys  dd ? // +12 physical address
// DDS ends

// --- EDDS, used by 05-06

// EDDS struct
// dwSize  dd ? // +0
// dwOfs   dd ? // +4
// wSeg    dw ? // +8
// wRes    dw ? // +10
// wNumAvail   dw ? // +12
// wNumUsed    dw ? // +14
// EDDS ends

// --- EDDS suffix for regions

// EDDSRG struct
// dwPhysAddr  dd ? // +16
// dwSizeRg    dd ? // +20
// EDDSRG ends

// --- EDDS suffix for PTEs

// EDDSPT struct
// dwPTE       dd ? // +16
// EDDSPT ends

