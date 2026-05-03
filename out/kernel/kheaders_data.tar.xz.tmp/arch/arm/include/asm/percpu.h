
#ifndef _ASM_ARM_PERCPU_H_
#define _ASM_ARM_PERCPU_H_


#if defined(CONFIG_SMP) && !defined(CONFIG_CPU_V6)
static inline void set_my_cpu_offset(unsigned long off)
{
	
	asm volatile("mcr p15, 0, %0, c13, c0, 4" : : "r" (off) : "memory");
}

static inline unsigned long __my_cpu_offset(void)
{
	unsigned long off;

	
	asm("mrc p15, 0, %0, c13, c0, 4" : "=r" (off)
		: "Q" (*(const unsigned long *)current_stack_pointer));

	return off;
}
#define __my_cpu_offset __my_cpu_offset()
#else
#define set_my_cpu_offset(x)	do {} while(0)

#endif 

#include <asm-generic/percpu.h>

#endif 
