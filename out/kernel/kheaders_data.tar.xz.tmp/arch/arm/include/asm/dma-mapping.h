/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ASMARM_DMA_MAPPING_H
#define ASMARM_DMA_MAPPING_H

#ifdef __KERNEL__

#include <linux/mm_types.h>
#include <linux/scatterlist.h>
#include <linux/dma-debug.h>

#include <asm/memory.h>
#include <asm/dma-iommu.h>

#include <xen/xen.h>
#include <asm/xen/hypervisor.h>

#ifdef CONFIG_MTK_BOUNCING_CHECK
#include "../../../../drivers/misc/mediatek/include/mt-plat/aee.h"
#endif

extern const struct dma_map_ops arm_dma_ops;
extern const struct dma_map_ops arm_coherent_dma_ops;

static inline const struct dma_map_ops *get_arch_dma_ops(struct bus_type *bus)
{
	return IS_ENABLED(CONFIG_MMU) ? &arm_dma_ops : &dma_noop_ops;
}

#ifdef __arch_page_to_dma
#error Please update to __arch_pfn_to_dma
#endif


#ifndef __arch_pfn_to_dma
static inline dma_addr_t pfn_to_dma(struct device *dev, unsigned long pfn)
{
	if (dev)
		pfn -= dev->dma_pfn_offset;
	return (dma_addr_t)__pfn_to_bus(pfn);
}

static inline unsigned long dma_to_pfn(struct device *dev, dma_addr_t addr)
{
	unsigned long pfn = __bus_to_pfn(addr);

	if (dev)
		pfn += dev->dma_pfn_offset;

	return pfn;
}

static inline void *dma_to_virt(struct device *dev, dma_addr_t addr)
{
	if (dev) {
		unsigned long pfn = dma_to_pfn(dev, addr);

		return phys_to_virt(__pfn_to_phys(pfn));
	}

	return (void *)__bus_to_virt((unsigned long)addr);
}

static inline dma_addr_t virt_to_dma(struct device *dev, void *addr)
{
	if (dev)
		return pfn_to_dma(dev, virt_to_pfn(addr));

	return (dma_addr_t)__virt_to_bus((unsigned long)(addr));
}

#else
static inline dma_addr_t pfn_to_dma(struct device *dev, unsigned long pfn)
{
	return __arch_pfn_to_dma(dev, pfn);
}

static inline unsigned long dma_to_pfn(struct device *dev, dma_addr_t addr)
{
	return __arch_dma_to_pfn(dev, addr);
}

static inline void *dma_to_virt(struct device *dev, dma_addr_t addr)
{
	return __arch_dma_to_virt(dev, addr);
}

static inline dma_addr_t virt_to_dma(struct device *dev, void *addr)
{
	return __arch_virt_to_dma(dev, addr);
}
#endif


static inline unsigned long dma_max_pfn(struct device *dev)
{
	return dma_to_pfn(dev, *dev->dma_mask);
}
#define dma_max_pfn(dev) dma_max_pfn(dev)

#define arch_setup_dma_ops arch_setup_dma_ops
extern void arch_setup_dma_ops(struct device *dev, u64 dma_base, u64 size,
			       const struct iommu_ops *iommu, bool coherent);

#define arch_teardown_dma_ops arch_teardown_dma_ops
extern void arch_teardown_dma_ops(struct device *dev);


static inline bool is_device_dma_coherent(struct device *dev)
{
	return dev->archdata.dma_coherent;
}

static inline dma_addr_t phys_to_dma(struct device *dev, phys_addr_t paddr)
{
	unsigned int offset = paddr & ~PAGE_MASK;
	return pfn_to_dma(dev, __phys_to_pfn(paddr)) + offset;
}

static inline phys_addr_t dma_to_phys(struct device *dev, dma_addr_t dev_addr)
{
	unsigned int offset = dev_addr & ~PAGE_MASK;
	return __pfn_to_phys(dma_to_pfn(dev, dev_addr)) + offset;
}

static inline bool dma_capable(struct device *dev, dma_addr_t addr, size_t size)
{
	u64 limit, mask;

	if (!dev->dma_mask) {
#ifdef CONFIG_MTK_BOUNCING_CHECK
		aee_kernel_warning("Bounce Buffering", "NULL dma_mask");
#endif
		return 0;
	}

	mask = *dev->dma_mask;

	limit = (mask + 1) & ~mask;
	if (limit && size > limit) {
#ifdef CONFIG_MTK_BOUNCING_CHECK
		aee_kernel_warning("Bounce Buffering",
				"Incorrect dma_mask(%llx): limit(%llx), size(%llx)",
				mask, limit, size);
#endif
		return 0;
	}

	if ((addr | (addr + size - 1)) & ~mask) {
#ifdef CONFIG_MTK_BOUNCING_CHECK
		aee_kernel_warning("Bounce Buffering",
				"Incorrect dma_mask(%llx): addr(%llx), size(%llx)",
				mask, addr, size);
#endif
		return 0;
	}

	return 1;
}

static inline void dma_mark_clean(void *addr, size_t size) { }


extern void *arm_dma_alloc(struct device *dev, size_t size, dma_addr_t *handle,
			   gfp_t gfp, unsigned long attrs);


extern void arm_dma_free(struct device *dev, size_t size, void *cpu_addr,
			 dma_addr_t handle, unsigned long attrs);


extern int arm_dma_mmap(struct device *dev, struct vm_area_struct *vma,
			void *cpu_addr, dma_addr_t dma_addr, size_t size,
			unsigned long attrs);


extern void __init init_dma_coherent_pool_size(unsigned long size);




extern int dmabounce_register_dev(struct device *, unsigned long,
		unsigned long, int (*)(struct device *, dma_addr_t, size_t));


extern void dmabounce_unregister_dev(struct device *);




extern int arm_dma_map_sg(struct device *, struct scatterlist *, int,
		enum dma_data_direction, unsigned long attrs);
extern void arm_dma_unmap_sg(struct device *, struct scatterlist *, int,
		enum dma_data_direction, unsigned long attrs);
extern void arm_dma_sync_sg_for_cpu(struct device *, struct scatterlist *, int,
		enum dma_data_direction);
extern void arm_dma_sync_sg_for_device(struct device *, struct scatterlist *, int,
		enum dma_data_direction);
extern int arm_dma_get_sgtable(struct device *dev, struct sg_table *sgt,
		void *cpu_addr, dma_addr_t dma_addr, size_t size,
		unsigned long attrs);



int arm_dma_reserve(struct dma_iommu_mapping *mapping, dma_addr_t addr,
		size_t size);	


extern int dma_map_sg_within_reserved_iova(struct device *dev,
					struct scatterlist *sg, int nents,
					int prot, dma_addr_t dma_addr);
extern void dma_unmap_sg_within_reserved_iova(struct device *dev,
				       struct scatterlist *sg, int nents,
				       int prot, size_t size);
extern void *dma_alloc_coherent_fix_iova(struct device *dev,
					dma_addr_t dma_addr,
					size_t size, gfp_t flag);
extern void dma_free_coherent_fix_iova(struct device *dev, void *cpu_addr,
				dma_addr_t dma_addr, size_t size);


#endif 
#endif
