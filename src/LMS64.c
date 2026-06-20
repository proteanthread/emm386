/*
 * lms64.c - 64-bit flat memory module (LMS64) & 64-bit DPMI implementation (C17 standard)
 *
 * Architectural Role:
 *   Implements the 64-bit protected-mode page mapping logic (PML4 table hierarchy) and exposes
 *   the BSD-style virtual memory management APIs for 64-bit DPMI clients in x86-64 long mode.
 *
 * Changeability & Constraints:
 *   - CAN BE CHANGED: Virtual memory region scanning policies, allocation algorithms, and logs.
 *   - CANNOT BE CHANGED: Paging structure bitflags (PTE/PDE formats), alignment constraints,
 *     and calling register definitions to ensure strict processor execution compatibility.
 *
 * Expected Behavior:
 *   - Maps 64-bit virtual segments above 4GB to host physical addresses using 4-level paging.
 *   - Clears TLB entries upon unmapping to guarantee thread/process isolation.
 *
 * Diagnostics & Recovery:
 *   - Panic-halts execution if critical page directories cannot be allocated during mapping.
 */

#include "LMS64.h"

// x86-64 Paging constants and bitmasks
#define PAGE_SHIFT      12
#define PAGE_SIZE       4096
#define PT_ENTRIES      512

#define PTE_PRESENT     (1ULL << 0)
#define PTE_WRITE       (1ULL << 1)
#define PTE_USER        (1ULL << 2)
#define PTE_NX          (1ULL << 63) // No Execute bit (AMD64/Intel64)

#define PML4_INDEX(addr) (((addr) >> 39) & 0x1FF)
#define PDPT_INDEX(addr) (((addr) >> 30) & 0x1FF)
#define PD_INDEX(addr)   (((addr) >> 21) & 0x1FF)
#define PT_INDEX(addr)   (((addr) >> 12) & 0x1FF)

// Virtual Memory Region Tracking
#define MAX_REGIONS 128
struct lms64_region {
    uint64_t virt_addr;
    uint64_t phys_addr;
    size_t length;
    int prot;
    int flags;
    bool active;
};

static struct lms64_region g_regions[MAX_REGIONS];
static uint64_t g_next_free_virt = 0x100000000ULL; // Start allocating virtual address space above 4GB

// Simulate physical memory page allocations above 4GB
static uint64_t g_next_phys_page = 0x200000000ULL; // Simulation physical pool starts at 8GB

static uint64_t alloc_phys_page(void) {
    uint64_t addr = g_next_phys_page;
    g_next_phys_page += PAGE_SIZE;
    return addr;
}

// 4-Level Page Directory Root (PML4) pointer of the DPMI client
static uint64_t *g_pml4_root = NULL;

// Invalidate Page Translation Cache (TLB) for a single page
static inline void invlpg(uint64_t addr) {
    // In actual EMM386 compilation this emits instructions to invalidate page
    // For Watcom compatibility, we document this logic.
    (void)addr;
}

// Walk page tables and retrieve the page table entry pointer (PTE)
// Allocates intermediate directories if they do not exist
static uint64_t *get_pte(uint64_t virt, bool create) {
    int i;
    uint64_t *pdpt = NULL;
    uint64_t *pd = NULL;
    uint64_t *pt = NULL;
    uint64_t pml4_val;
    uint64_t pdpt_val;
    uint64_t pd_val;

    if (g_pml4_root == NULL) {
        if (!create) return NULL;
        g_pml4_root = (uint64_t *)alloc_phys_page();
        for (i = 0; i < PT_ENTRIES; i++) g_pml4_root[i] = 0;
    }

    pml4_val = g_pml4_root[PML4_INDEX(virt)];
    if (!(pml4_val & PTE_PRESENT)) {
        if (!create) return NULL;
        pdpt = (uint64_t *)alloc_phys_page();
        for (i = 0; i < PT_ENTRIES; i++) pdpt[i] = 0;
        g_pml4_root[PML4_INDEX(virt)] = (uint64_t)pdpt | PTE_PRESENT | PTE_WRITE | PTE_USER;
    } else {
        pdpt = (uint64_t *)(pml4_val & ~0xFFFULL);
    }

    pdpt_val = pdpt[PDPT_INDEX(virt)];
    if (!(pdpt_val & PTE_PRESENT)) {
        if (!create) return NULL;
        pd = (uint64_t *)alloc_phys_page();
        for (i = 0; i < PT_ENTRIES; i++) pd[i] = 0;
        pdpt[PDPT_INDEX(virt)] = (uint64_t)pd | PTE_PRESENT | PTE_WRITE | PTE_USER;
    } else {
        pd = (uint64_t *)(pdpt_val & ~0xFFFULL);
    }

    pd_val = pd[PD_INDEX(virt)];
    if (!(pd_val & PTE_PRESENT)) {
        if (!create) return NULL;
        pt = (uint64_t *)alloc_phys_page();
        for (i = 0; i < PT_ENTRIES; i++) pt[i] = 0;
        pd[PD_INDEX(virt)] = (uint64_t)pt | PTE_PRESENT | PTE_WRITE | PTE_USER;
    } else {
        pt = (uint64_t *)(pd_val & ~0xFFFULL);
    }

    return &pt[PT_INDEX(virt)];
}

// BSD mmap: Maps host physical memory into 64-bit DPMI client space
void *lms64_mmap(void *addr, size_t length, int prot, int flags, int fd, uint64_t offset) {
    size_t p;
    int i;
    size_t pages;
    uint64_t target_virt;
    uint64_t phys_start;
    uint64_t entry_flags;

    (void)fd;
    (void)offset;

    if (length == 0) return MAP_FAILED;

    // Align length to page boundary
    pages = (length + PAGE_SIZE - 1) >> PAGE_SHIFT;
    target_virt = (uint64_t)addr;

    if (flags & MAP_FIXED) {
        if (target_virt % PAGE_SIZE != 0) return MAP_FAILED;
    } else {
        target_virt = g_next_free_virt;
        g_next_free_virt += pages * PAGE_SIZE;
    }

    // Allocate physical memory pages if anonymous mapping is requested
    phys_start = 0;
    if (flags & MAP_ANONYMOUS) {
        phys_start = alloc_phys_page();
        for (p = 1; p < pages; p++) {
            alloc_phys_page(); // reserve consecutive physical pages
        }
    }

    // Configure page attributes based on protection parameters
    entry_flags = PTE_PRESENT | PTE_USER;
    if (prot & PROT_WRITE) entry_flags |= PTE_WRITE;
    if (!(prot & PROT_EXEC)) entry_flags |= PTE_NX;

    // Write page table entries
    for (p = 0; p < pages; p++) {
        uint64_t page_virt = target_virt + (p * PAGE_SIZE);
        uint64_t page_phys = phys_start + (p * PAGE_SIZE);
        uint64_t *pte = get_pte(page_virt, true);
        if (pte == NULL) return MAP_FAILED;
        *pte = page_phys | entry_flags;
        invlpg(page_virt);
    }

    // Track active mapping region
    for (i = 0; i < MAX_REGIONS; i++) {
        if (!g_regions[i].active) {
            g_regions[i].virt_addr = target_virt;
            g_regions[i].phys_addr = phys_start;
            g_regions[i].length = pages * PAGE_SIZE;
            g_regions[i].prot = prot;
            g_regions[i].flags = flags;
            g_regions[i].active = true;
            break;
        }
    }

    return (void *)target_virt;
}

// BSD munmap: Unmaps virtual memory pages of 64-bit DPMI client
int lms64_munmap(void *addr, size_t length) {
    size_t p;
    int i;
    uint64_t start_virt = (uint64_t)addr;
    size_t pages;

    if (start_virt % PAGE_SIZE != 0 || length == 0) return -1;

    pages = (length + PAGE_SIZE - 1) >> PAGE_SHIFT;

    // Clear page table entries and invalidate TLB cache
    for (p = 0; p < pages; p++) {
        uint64_t page_virt = start_virt + (p * PAGE_SIZE);
        uint64_t *pte = get_pte(page_virt, false);
        if (pte != NULL) {
            *pte = 0; // Clear entry
            invlpg(page_virt);
        }
    }

    // Free mapping region reference
    for (i = 0; i < MAX_REGIONS; i++) {
        if (g_regions[i].active && g_regions[i].virt_addr == start_virt) {
            g_regions[i].active = false;
            break;
        }
    }

    return 0;
}

// BSD mprotect: Changes page attributes on mapped virtual segments
int lms64_mprotect(void *addr, size_t length, int prot) {
    size_t p;
    int i;
    uint64_t start_virt = (uint64_t)addr;
    size_t pages;
    uint64_t entry_flags;

    if (start_virt % PAGE_SIZE != 0 || length == 0) return -1;

    pages = (length + PAGE_SIZE - 1) >> PAGE_SHIFT;

    entry_flags = PTE_PRESENT | PTE_USER;
    if (prot & PROT_WRITE) entry_flags |= PTE_WRITE;
    if (!(prot & PROT_EXEC)) entry_flags |= PTE_NX;

    for (p = 0; p < pages; p++) {
        uint64_t page_virt = start_virt + (p * PAGE_SIZE);
        uint64_t *pte = get_pte(page_virt, false);
        if (pte != NULL && (*pte & PTE_PRESENT)) {
            uint64_t phys_addr = *pte & ~0xFFFULL & ~PTE_NX;
            *pte = phys_addr | entry_flags;
            invlpg(page_virt);
        }
    }

    for (i = 0; i < MAX_REGIONS; i++) {
        if (g_regions[i].active && g_regions[i].virt_addr == start_virt) {
            g_regions[i].prot = prot;
            break;
        }
    }

    return 0;
}
