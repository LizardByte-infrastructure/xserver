
/*
 *                   XFree86 int10 module
 *   execute BIOS int 10h calls in x86 real mode environment
 *                 Copyright 1999 Egbert Eich
 */

#ifndef _XF86INT10_H
#define _XF86INT10_H

#include <X11/Xmd.h>
#include <X11/Xdefs.h>
#include "xf86Pci.h"

#define SEG_ADDR(x) (((x) >> 4) & 0x00F000)
#define SEG_OFF(x) ((x) & 0x0FFFF)

#define SET_BIOS_SCRATCH     0x1
#define RESTORE_BIOS_SCRATCH 0x2

/* int10 info structure */
typedef struct {
    int entityIndex;
    uint16_t BIOSseg;
    uint16_t inb40time;
    ScrnInfoPtr pScrn;
    void *cpuRegs;
    char *BIOSScratch;
    int Flags;
    void *private;
    struct _int10Mem *mem;
    int num;
    int ax;
    int bx;
    int cx;
    int dx;
    int si;
    int di;
    int es;
    int bp;
    int flags;
    int stackseg;
    struct pci_device *dev;
    struct pci_io_handle *io;
} xf86Int10InfoRec, *xf86Int10InfoPtr;

typedef struct _int10Mem {
    uint8_t (*rb) (xf86Int10InfoPtr, int);
    uint16_t (*rw) (xf86Int10InfoPtr, int);
    uint32_t (*rl) (xf86Int10InfoPtr, int);
    void (*wb) (xf86Int10InfoPtr, int, uint8_t);
    void (*ww) (xf86Int10InfoPtr, int, uint16_t);
    void (*wl) (xf86Int10InfoPtr, int, uint32_t);
} int10MemRec, *int10MemPtr;

typedef struct {
    uint8_t save_msr;
    uint8_t save_pos102;
    uint8_t save_vse;
    uint8_t save_46e8;
} legacyVGARec, *legacyVGAPtr;

/* OS dependent functions */
extern _X_EXPORT xf86Int10InfoPtr xf86InitInt10(int entityIndex);
extern _X_EXPORT xf86Int10InfoPtr xf86ExtendedInitInt10(int entityIndex,
                                                        int Flags);
extern _X_EXPORT void xf86FreeInt10(xf86Int10InfoPtr pInt);
extern _X_EXPORT void *xf86Int10AllocPages(xf86Int10InfoPtr pInt, int num,
                                           int *off);
extern _X_EXPORT void xf86Int10FreePages(xf86Int10InfoPtr pInt, void *pbase,
                                         int num);
extern _X_EXPORT void *xf86int10Addr(xf86Int10InfoPtr pInt, uint32_t addr);

/* x86 executor related functions */
extern _X_EXPORT void xf86ExecX86int10(xf86Int10InfoPtr pInt);

#ifdef _INT10_PRIVATE

#define I_S_DEFAULT_INT_VECT 0xFF065
#define SYS_SIZE 0x100000
#define SYS_BIOS 0xF0000
#if 1
#define BIOS_SIZE 0x10000
#else                           /* a bug in DGUX requires this - let's try it */
#define BIOS_SIZE (0x10000 - 1)
#endif
#define LOW_PAGE_SIZE 0x600
#define V_RAM 0xA0000
#define VRAM_SIZE 0x20000
#define V_BIOS_SIZE 0x10000
#define V_BIOS 0xC0000
#define BIOS_SCRATCH_OFF 0x449
#define BIOS_SCRATCH_END 0x466
#define BIOS_SCRATCH_LEN (BIOS_SCRATCH_END - BIOS_SCRATCH_OFF + 1)
#define HIGH_MEM V_BIOS
#define HIGH_MEM_SIZE (SYS_BIOS - HIGH_MEM)
#define SEG_ADR(type, seg, reg)  type((seg << 4) + (X86_##reg))
#define SEG_EADR(type, seg, reg) type((seg << 4) + (X86_E##reg))

#define X86_TF_MASK		0x00000100
#define X86_IF_MASK		0x00000200
#define X86_IOPL_MASK		0x00003000
#define X86_NT_MASK		0x00004000
#define X86_VM_MASK		0x00020000
#define X86_AC_MASK		0x00040000
#define X86_VIF_MASK		0x00080000      /* virtual interrupt flag */
#define X86_VIP_MASK		0x00100000      /* virtual interrupt pending */
#define X86_ID_MASK		0x00200000

#define MEM_RB(name, addr)      (*name->mem->rb)(name, addr)
#define MEM_RW(name, addr)      (*name->mem->rw)(name, addr)
#define MEM_RL(name, addr)      (*name->mem->rl)(name, addr)
#define MEM_WB(name, addr, val) (*name->mem->wb)(name, addr, val)
#define MEM_WW(name, addr, val) (*name->mem->ww)(name, addr, val)
#define MEM_WL(name, addr, val) (*name->mem->wl)(name, addr, val)

/* OS dependent functions */
extern _X_EXPORT Bool MapCurrentInt10(xf86Int10InfoPtr pInt);

/* x86 executor related functions */
extern _X_EXPORT Bool xf86Int10ExecSetup(xf86Int10InfoPtr pInt);

/* int.c */
extern _X_EXPORT xf86Int10InfoPtr Int10Current;

#if defined (_PC)
extern _X_EXPORT void xf86Int10SaveRestoreBIOSVars(xf86Int10InfoPtr pInt,
                                                   Bool save);
#endif

extern _X_EXPORT void *xf86HandleInt10Options(ScrnInfoPtr pScrn,
                                              int entityIndex);
extern _X_EXPORT BusType xf86int10GetBiosLocationType(const xf86Int10InfoPtr
                                                      pInt);
extern _X_EXPORT Bool xf86int10GetBiosSegment(xf86Int10InfoPtr pInt,
                                              void *base);

#endif                          /* _INT10_PRIVATE */
#endif                          /* _XF86INT10_H */
