/*
 * dma.inc - --- ports ISA DMA controller (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing DMA.INC.
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

#define DMA_STATUS_CMD 8 // R status / W command
#define DMA_REQUEST 9 // W b0+1: channel, b2: 1=enable?, b3-7:rsvd
#define DMA_SINGLEMASK 10 // W b0+1: channel to mask, b2: 0=clear/1=mask, b3-7:rsvd
#define DMA_MODE 11 // W b0+1: channel, b2-7:see below
#define DMA_FLIPFLOP 12 // W
#define DMA_IMM_RESET 13 // R immediate / W master reset (masks all 4 channels)
#define DMA_MASK_RESET 14 // W master enable (unmasks all 4 channels)
#define DMA_MULTIMASK 15 // W b0-3: 1=channel[0-3] masked, 0=channel[0-3] unmasked

// --- bits in DMA_MODE
#define DMA_MODE_OPERATION 1100b // "op" bits 2+3
#define DMA_MODE_OP_VERIFY 0000b
#define DMA_MODE_OP_WRITE 0100b
#define DMA_MODE_OP_READ 1000b

#define DMA_MODE_AUTO 10h // b4
#define DMA_MODE_DIRECTION 20h // b5: direction 0=increment, 1=decrement
// --- b6-7: 00=demand,01=single,10=block,11=cascade

#define DMA_BASE_16BIT 0C0h

#define DMA_STATUS_CMD16 DMA_BASE_16BIT + DMA_STATUS_CMD*2
#define DMA_SINGLEMASK16 DMA_BASE_16BIT + DMA_SINGLEMASK*2
#define DMA_MODE16 DMA_BASE_16BIT + DMA_MODE*2
#define DMA_FLIPFLOP16 DMA_BASE_16BIT + DMA_FLIPFLOP*2
#define DMA_IMM_RESET16 DMA_BASE_16BIT + DMA_IMM_RESET*2
#define DMA_MASK_RESET16 DMA_BASE_16BIT + DMA_MASK_RESET*2
#define DMA_MULTIMASK16 DMA_BASE_16BIT + DMA_MULTIMASK*2

#define MAXDMACHANNEL 8
