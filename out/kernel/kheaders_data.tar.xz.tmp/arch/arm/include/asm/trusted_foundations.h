



#ifndef __ASM_ARM_TRUSTED_FOUNDATIONS_H
#define __ASM_ARM_TRUSTED_FOUNDATIONS_H

#include <linux/printk.h>
#include <linux/bug.h>
#include <linux/of.h>
#include <linux/cpu.h>
#include <linux/smp.h>

struct trusted_foundations_platform_data {
	unsigned int version_major;
	unsigned int version_minor;
};

#if IS_ENABLED(CONFIG_TRUSTED_FOUNDATIONS)

void register_trusted_foundations(struct trusted_foundations_platform_data *pd);
void of_register_trusted_foundations(void);

#else 

static inline void register_trusted_foundations(
				   struct trusted_foundations_platform_data *pd)
{
	
	pr_err("No support for Trusted Foundations, continuing in degraded mode.\n");
	pr_err("Secondary processors as well as CPU PM will be disabled.\n");
#if IS_ENABLED(CONFIG_SMP)
	setup_max_cpus = 0;
#endif
	cpu_idle_poll_ctrl(true);
}

static inline void of_register_trusted_foundations(void)
{
	
	if (of_find_compatible_node(NULL, NULL, "tlm,trusted-foundations"))
		register_trusted_foundations(NULL);
}
#endif 

#endif
