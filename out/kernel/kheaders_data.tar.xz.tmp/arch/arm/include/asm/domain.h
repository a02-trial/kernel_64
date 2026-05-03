
#ifndef __ASM_PROC_DOMAIN_H
#define __ASM_PROC_DOMAIN_H

#ifndef __ASSEMBLY__
#include <asm/barrier.h>
#include <asm/thread_info.h>
#endif


#ifndef CONFIG_IO_36
#define DOMAIN_KERNEL	0
#define DOMAIN_USER	1
#define DOMAIN_IO	2
#else
#define DOMAIN_KERNEL	2
#define DOMAIN_USER	1
#define DOMAIN_IO	0
#endif
#define DOMAIN_VECTORS	3


#define DOMAIN_NOACCESS	0
#define DOMAIN_CLIENT	1
#ifdef CONFIG_CPU_USE_DOMAINS
#define DOMAIN_MANAGER	3
#else
#define DOMAIN_MANAGER	1
#endif

#define domain_mask(dom)	((3) << (2 * (dom)))
#define domain_val(dom,type)	((type) << (2 * (dom)))

#ifdef CONFIG_CPU_SW_DOMAIN_PAN
#define DACR_INIT \
	(domain_val(DOMAIN_USER, DOMAIN_NOACCESS) | \
	 domain_val(DOMAIN_KERNEL, DOMAIN_MANAGER) | \
	 domain_val(DOMAIN_IO, DOMAIN_CLIENT) | \
	 domain_val(DOMAIN_VECTORS, DOMAIN_CLIENT))
#else
#define DACR_INIT \
	(domain_val(DOMAIN_USER, DOMAIN_CLIENT) | \
	 domain_val(DOMAIN_KERNEL, DOMAIN_MANAGER) | \
	 domain_val(DOMAIN_IO, DOMAIN_CLIENT) | \
	 domain_val(DOMAIN_VECTORS, DOMAIN_CLIENT))
#endif

#define __DACR_DEFAULT \
	domain_val(DOMAIN_KERNEL, DOMAIN_CLIENT) | \
	domain_val(DOMAIN_IO, DOMAIN_CLIENT) | \
	domain_val(DOMAIN_VECTORS, DOMAIN_CLIENT)

#define DACR_UACCESS_DISABLE	\
	(__DACR_DEFAULT | domain_val(DOMAIN_USER, DOMAIN_NOACCESS))
#define DACR_UACCESS_ENABLE	\
	(__DACR_DEFAULT | domain_val(DOMAIN_USER, DOMAIN_CLIENT))

#ifndef __ASSEMBLY__

#ifdef CONFIG_CPU_CP15_MMU
static inline unsigned int get_domain(void)
{
	unsigned int domain;

	asm(
	"mrc	p15, 0, %0, c3, c0	@ get domain"
	 : "=r" (domain)
	 : "m" (current_thread_info()->cpu_domain));

	return domain;
}

static inline void set_domain(unsigned val)
{
	asm volatile(
	"mcr	p15, 0, %0, c3, c0	@ set domain"
	  : : "r" (val) : "memory");
	isb();
}
#else
static inline unsigned int get_domain(void)
{
	return 0;
}

static inline void set_domain(unsigned val)
{
}
#endif

#ifdef CONFIG_CPU_USE_DOMAINS
#define modify_domain(dom,type)					\
	do {							\
		unsigned int domain = get_domain();		\
		domain &= ~domain_mask(dom);			\
		domain = domain | domain_val(dom, type);	\
		set_domain(domain);				\
	} while (0)

#else
static inline void modify_domain(unsigned dom, unsigned type)	{ }
#endif


#ifdef CONFIG_CPU_USE_DOMAINS
#define TUSER(instr)	#instr "t"
#else
#define TUSER(instr)	#instr
#endif

#else 


#ifdef CONFIG_CPU_USE_DOMAINS
#define TUSER(instr)	instr ## t
#else
#define TUSER(instr)	instr
#endif

#endif 

#endif 
