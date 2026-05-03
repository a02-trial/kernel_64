
#ifndef __ASM_ARM_MEMORY_H
#define __ASM_ARM_MEMORY_H

#include <linux/compiler.h>
#include <linux/const.h>
#include <linux/types.h>
#include <linux/sizes.h>

#ifdef CONFIG_NEED_MACH_MEMORY_H
#include <mach/memory.h>
#endif


#define UL(x) _AC(x, UL)


#define PAGE_OFFSET		UL(CONFIG_PAGE_OFFSET)

#ifdef CONFIG_MMU


#define TASK_SIZE		(UL(CONFIG_PAGE_OFFSET) - UL(SZ_16M))
#define TASK_UNMAPPED_BASE	ALIGN(TASK_SIZE / 3, SZ_16M)


#define TASK_SIZE_26		(UL(1) << 26)


#ifndef CONFIG_THUMB2_KERNEL
#define MODULES_VADDR		(PAGE_OFFSET - SZ_16M)
#else

#define MODULES_VADDR		(PAGE_OFFSET - SZ_8M)
#endif

#if TASK_SIZE > MODULES_VADDR
#error Top of user space clashes with start of module space
#endif


#ifdef CONFIG_HIGHMEM
#define MODULES_END		(PAGE_OFFSET - PMD_SIZE)
#else
#define MODULES_END		(PAGE_OFFSET)
#endif


#define XIP_VIRT_ADDR(physaddr)  (MODULES_VADDR + ((physaddr) & 0x000fffff))

#if !defined(CONFIG_SMP) && !defined(CONFIG_ARM_LPAE)

#define IOREMAP_MAX_ORDER	24
#endif

#define VECTORS_BASE		UL(0xffff0000)

#else 

#ifndef __ASSEMBLY__
extern unsigned long vectors_base;
#define VECTORS_BASE		vectors_base
#endif


#define TASK_SIZE		UL(0xffffffff)

#ifndef TASK_UNMAPPED_BASE
#define TASK_UNMAPPED_BASE	UL(0x00000000)
#endif

#ifndef END_MEM
#define END_MEM     		(UL(CONFIG_DRAM_BASE) + CONFIG_DRAM_SIZE)
#endif


#define MODULES_END		(END_MEM)
#define MODULES_VADDR		PAGE_OFFSET

#define XIP_VIRT_ADDR(physaddr)  (physaddr)

#endif 

#ifdef CONFIG_XIP_KERNEL
#define KERNEL_START		_sdata
#else
#define KERNEL_START		_stext
#endif
#define KERNEL_END		_end


#ifdef CONFIG_HAVE_TCM
#define ITCM_OFFSET	UL(0xfffe0000)
#define DTCM_OFFSET	UL(0xfffe8000)
#endif


#define page_to_phys(page)	(__pfn_to_phys(page_to_pfn(page)))
#define phys_to_page(phys)	(pfn_to_page(__phys_to_pfn(phys)))


#define PLAT_PHYS_OFFSET	UL(CONFIG_PHYS_OFFSET)

#ifdef CONFIG_XIP_KERNEL

#define PHYS_RELATIVE(v_data, v_text) \
	(((v_data) - PAGE_OFFSET + PLAT_PHYS_OFFSET) - \
	 ((v_text) - XIP_VIRT_ADDR(CONFIG_XIP_PHYS_ADDR) + \
          CONFIG_XIP_PHYS_ADDR))
#else
#define PHYS_RELATIVE(v_data, v_text) ((v_data) - (v_text))
#endif

#ifndef __ASSEMBLY__



#if defined(CONFIG_ARM_PATCH_PHYS_VIRT)


#define __PV_BITS_31_24	0x81000000
#define __PV_BITS_7_0	0x81

extern unsigned long __pv_phys_pfn_offset;
extern u64 __pv_offset;
extern void fixup_pv_table(const void *, unsigned long);
extern const void *__pv_table_begin, *__pv_table_end;

#define PHYS_OFFSET	((phys_addr_t)__pv_phys_pfn_offset << PAGE_SHIFT)
#define PHYS_PFN_OFFSET	(__pv_phys_pfn_offset)

#define __pv_stub(from,to,instr,type)			\
	__asm__("@ __pv_stub\n"				\
	"1:	" instr "	%0, %1, %2\n"		\
	"	.pushsection .pv_table,\"a\"\n"		\
	"	.long	1b\n"				\
	"	.popsection\n"				\
	: "=r" (to)					\
	: "r" (from), "I" (type))

#define __pv_stub_mov_hi(t)				\
	__asm__ volatile("@ __pv_stub_mov\n"		\
	"1:	mov	%R0, %1\n"			\
	"	.pushsection .pv_table,\"a\"\n"		\
	"	.long	1b\n"				\
	"	.popsection\n"				\
	: "=r" (t)					\
	: "I" (__PV_BITS_7_0))

#define __pv_add_carry_stub(x, y)			\
	__asm__ volatile("@ __pv_add_carry_stub\n"	\
	"1:	adds	%Q0, %1, %2\n"			\
	"	adc	%R0, %R0, #0\n"			\
	"	.pushsection .pv_table,\"a\"\n"		\
	"	.long	1b\n"				\
	"	.popsection\n"				\
	: "+r" (y)					\
	: "r" (x), "I" (__PV_BITS_31_24)		\
	: "cc")

static inline phys_addr_t __virt_to_phys_nodebug(unsigned long x)
{
	phys_addr_t t;

	if (sizeof(phys_addr_t) == 4) {
		__pv_stub(x, t, "add", __PV_BITS_31_24);
	} else {
		__pv_stub_mov_hi(t);
		__pv_add_carry_stub(x, t);
	}
	return t;
}

static inline unsigned long __phys_to_virt(phys_addr_t x)
{
	unsigned long t;

	
	__pv_stub((unsigned long) x, t, "sub", __PV_BITS_31_24);
	return t;
}

#else

#define PHYS_OFFSET	PLAT_PHYS_OFFSET
#define PHYS_PFN_OFFSET	((unsigned long)(PHYS_OFFSET >> PAGE_SHIFT))

static inline phys_addr_t __virt_to_phys_nodebug(unsigned long x)
{
	return (phys_addr_t)x - PAGE_OFFSET + PHYS_OFFSET;
}

static inline unsigned long __phys_to_virt(phys_addr_t x)
{
	return x - PHYS_OFFSET + PAGE_OFFSET;
}

#endif

#define virt_to_pfn(kaddr) \
	((((unsigned long)(kaddr) - PAGE_OFFSET) >> PAGE_SHIFT) + \
	 PHYS_PFN_OFFSET)

#define __pa_symbol_nodebug(x)	__virt_to_phys_nodebug((x))

#ifdef CONFIG_DEBUG_VIRTUAL
extern phys_addr_t __virt_to_phys(unsigned long x);
extern phys_addr_t __phys_addr_symbol(unsigned long x);
#else
#define __virt_to_phys(x)	__virt_to_phys_nodebug(x)
#define __phys_addr_symbol(x)	__pa_symbol_nodebug(x)
#endif


#define virt_to_phys virt_to_phys
static inline phys_addr_t virt_to_phys(const volatile void *x)
{
	return __virt_to_phys((unsigned long)(x));
}

#define phys_to_virt phys_to_virt
static inline void *phys_to_virt(phys_addr_t x)
{
	return (void *)__phys_to_virt(x);
}


#define __pa(x)			__virt_to_phys((unsigned long)(x))
#define __pa_symbol(x)		__phys_addr_symbol(RELOC_HIDE((unsigned long)(x), 0))
#define __va(x)			((void *)__phys_to_virt((phys_addr_t)(x)))
#define pfn_to_kaddr(pfn)	__va((phys_addr_t)(pfn) << PAGE_SHIFT)

extern long long arch_phys_to_idmap_offset;


static inline bool arm_has_idmap_alias(void)
{
	return IS_ENABLED(CONFIG_MMU) && arch_phys_to_idmap_offset != 0;
}

#define IDMAP_INVALID_ADDR ((u32)~0)

static inline unsigned long phys_to_idmap(phys_addr_t addr)
{
	if (IS_ENABLED(CONFIG_MMU) && arch_phys_to_idmap_offset) {
		addr += arch_phys_to_idmap_offset;
		if (addr > (u32)~0)
			addr = IDMAP_INVALID_ADDR;
	}
	return addr;
}

static inline phys_addr_t idmap_to_phys(unsigned long idmap)
{
	phys_addr_t addr = idmap;

	if (IS_ENABLED(CONFIG_MMU) && arch_phys_to_idmap_offset)
		addr -= arch_phys_to_idmap_offset;

	return addr;
}

static inline unsigned long __virt_to_idmap(unsigned long x)
{
	return phys_to_idmap(__virt_to_phys(x));
}

#define virt_to_idmap(x)	__virt_to_idmap((unsigned long)(x))


#ifndef __virt_to_bus
#define __virt_to_bus	__virt_to_phys
#define __bus_to_virt	__phys_to_virt
#define __pfn_to_bus(x)	__pfn_to_phys(x)
#define __bus_to_pfn(x)	__phys_to_pfn(x)
#endif


#define ARCH_PFN_OFFSET		PHYS_PFN_OFFSET

#define virt_to_page(kaddr)	pfn_to_page(virt_to_pfn(kaddr))
#define virt_addr_valid(kaddr)	(((unsigned long)(kaddr) >= PAGE_OFFSET && (unsigned long)(kaddr) < (unsigned long)high_memory) \
					&& pfn_valid(virt_to_pfn(kaddr)))

#endif

#include <asm-generic/memory_model.h>

#endif
