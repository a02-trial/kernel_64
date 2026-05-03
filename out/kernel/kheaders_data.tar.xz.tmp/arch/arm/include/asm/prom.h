
#ifndef __ASMARM_PROM_H
#define __ASMARM_PROM_H

#ifdef CONFIG_OF

extern const struct machine_desc *setup_machine_fdt(unsigned int dt_phys);
extern void __init arm_dt_init_cpu_maps(void);

#else 

static inline const struct machine_desc *setup_machine_fdt(unsigned int dt_phys)
{
	return NULL;
}

static inline void arm_dt_init_cpu_maps(void) { }

#endif 
#endif 
