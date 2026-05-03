

#ifndef __ARM_KVM_HOST_H__
#define __ARM_KVM_HOST_H__

#include <linux/types.h>
#include <linux/kvm_types.h>
#include <asm/cputype.h>
#include <asm/kvm.h>
#include <asm/kvm_asm.h>
#include <asm/kvm_mmio.h>
#include <asm/fpstate.h>
#include <kvm/arm_arch_timer.h>

#define __KVM_HAVE_ARCH_INTC_INITIALIZED

#define KVM_USER_MEM_SLOTS 32
#define KVM_HAVE_ONE_REG
#define KVM_HALT_POLL_NS_DEFAULT 500000

#define KVM_VCPU_MAX_FEATURES 2

#include <kvm/arm_vgic.h>


#ifdef CONFIG_ARM_GIC_V3
#define KVM_MAX_VCPUS VGIC_V3_MAX_CPUS
#else
#define KVM_MAX_VCPUS VGIC_V2_MAX_CPUS
#endif

#define KVM_REQ_SLEEP \
	KVM_ARCH_REQ_FLAGS(0, KVM_REQUEST_WAIT | KVM_REQUEST_NO_WAKEUP)
#define KVM_REQ_IRQ_PENDING	KVM_ARCH_REQ(1)

u32 *kvm_vcpu_reg(struct kvm_vcpu *vcpu, u8 reg_num, u32 mode);
int __attribute_const__ kvm_target_cpu(void);
int kvm_reset_vcpu(struct kvm_vcpu *vcpu);
void kvm_reset_coprocs(struct kvm_vcpu *vcpu);

struct kvm_arch {
	
	u64    vttbr;

	
	int __percpu *last_vcpu_ran;

	

	
	u64    vmid_gen;
	u32    vmid;

	
	pgd_t *pgd;

	
	struct vgic_dist	vgic;
	int max_vcpus;

	
	u32 psci_version;
};

#define KVM_NR_MEM_OBJS     40


struct kvm_mmu_memory_cache {
	int nobjs;
	void *objects[KVM_NR_MEM_OBJS];
};

struct kvm_vcpu_fault_info {
	u32 hsr;		
	u32 hxfar;		
	u32 hpfar;		
};


enum vcpu_sysreg {
	__INVALID_SYSREG__,
	c0_MPIDR,		
	c0_CSSELR,		
	c1_SCTLR,		
	c1_ACTLR,		
	c1_CPACR,		
	c2_TTBR0,		
	c2_TTBR0_high,		
	c2_TTBR1,		
	c2_TTBR1_high,		
	c2_TTBCR,		
	c3_DACR,		
	c5_DFSR,		
	c5_IFSR,		
	c5_ADFSR,		
	c5_AIFSR,		
	c6_DFAR,		
	c6_IFAR,		
	c7_PAR,			
	c7_PAR_high,		
	c9_L2CTLR,		
	c10_PRRR,		
	c10_NMRR,		
	c12_VBAR,		
	c13_CID,		
	c13_TID_URW,		
	c13_TID_URO,		
	c13_TID_PRIV,		
	c14_CNTKCTL,		
	c10_AMAIR0,		
	c10_AMAIR1,		
	NR_CP15_REGS		
};

struct kvm_cpu_context {
	struct kvm_regs	gp_regs;
	struct vfp_hard_struct vfp;
	u32 cp15[NR_CP15_REGS];
};

typedef struct kvm_cpu_context kvm_cpu_context_t;

struct kvm_vcpu_arch {
	struct kvm_cpu_context ctxt;

	int target; 
	DECLARE_BITMAP(features, KVM_VCPU_MAX_FEATURES);

	
	u32 midr;

	
	u32 hcr;

	
	u32 irq_lines;		

	
	struct kvm_vcpu_fault_info fault;

	
	kvm_cpu_context_t *host_cpu_context;

	
	struct vgic_cpu vgic_cpu;
	struct arch_timer_cpu timer_cpu;

	

	
	bool power_off;

	 
	bool pause;

	
	struct kvm_decode mmio_decode;

	
	struct kvm_mmu_memory_cache mmu_page_cache;

	
	bool has_run_once;
};

struct kvm_vm_stat {
	ulong remote_tlb_flush;
};

struct kvm_vcpu_stat {
	u64 halt_successful_poll;
	u64 halt_attempted_poll;
	u64 halt_poll_invalid;
	u64 halt_wakeup;
	u64 hvc_exit_stat;
	u64 wfe_exit_stat;
	u64 wfi_exit_stat;
	u64 mmio_exit_user;
	u64 mmio_exit_kernel;
	u64 exits;
};

#define vcpu_cp15(v,r)	(v)->arch.ctxt.cp15[r]

int kvm_vcpu_preferred_target(struct kvm_vcpu_init *init);
unsigned long kvm_arm_num_regs(struct kvm_vcpu *vcpu);
int kvm_arm_copy_reg_indices(struct kvm_vcpu *vcpu, u64 __user *indices);
int kvm_arm_get_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *reg);
int kvm_arm_set_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *reg);
unsigned long kvm_call_hyp(void *hypfn, ...);
void force_vm_exit(const cpumask_t *mask);

#define KVM_ARCH_WANT_MMU_NOTIFIER
int kvm_unmap_hva(struct kvm *kvm, unsigned long hva);
int kvm_unmap_hva_range(struct kvm *kvm,
			unsigned long start, unsigned long end);
void kvm_set_spte_hva(struct kvm *kvm, unsigned long hva, pte_t pte);

unsigned long kvm_arm_num_regs(struct kvm_vcpu *vcpu);
int kvm_arm_copy_reg_indices(struct kvm_vcpu *vcpu, u64 __user *indices);
int kvm_age_hva(struct kvm *kvm, unsigned long start, unsigned long end);
int kvm_test_age_hva(struct kvm *kvm, unsigned long hva);

struct kvm_vcpu *kvm_arm_get_running_vcpu(void);
struct kvm_vcpu __percpu **kvm_get_running_vcpus(void);
void kvm_arm_halt_guest(struct kvm *kvm);
void kvm_arm_resume_guest(struct kvm *kvm);

int kvm_arm_copy_coproc_indices(struct kvm_vcpu *vcpu, u64 __user *uindices);
unsigned long kvm_arm_num_coproc_regs(struct kvm_vcpu *vcpu);
int kvm_arm_coproc_get_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *);
int kvm_arm_coproc_set_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *);

int handle_exit(struct kvm_vcpu *vcpu, struct kvm_run *run,
		int exception_index);

static inline void __cpu_init_hyp_mode(phys_addr_t pgd_ptr,
				       unsigned long hyp_stack_ptr,
				       unsigned long vector_ptr)
{
	

	kvm_call_hyp((void*)hyp_stack_ptr, vector_ptr, pgd_ptr);
}

static inline void __cpu_init_stage2(void)
{
	kvm_call_hyp(__init_stage2_translation);
}

static inline int kvm_arch_dev_ioctl_check_extension(struct kvm *kvm, long ext)
{
	return 0;
}

int kvm_perf_init(void);
int kvm_perf_teardown(void);

void kvm_mmu_wp_memory_region(struct kvm *kvm, int slot);

struct kvm_vcpu *kvm_mpidr_to_vcpu(struct kvm *kvm, unsigned long mpidr);

static inline void kvm_arch_hardware_unsetup(void) {}
static inline void kvm_arch_sync_events(struct kvm *kvm) {}
static inline void kvm_arch_vcpu_uninit(struct kvm_vcpu *vcpu) {}
static inline void kvm_arch_sched_in(struct kvm_vcpu *vcpu, int cpu) {}
static inline void kvm_arch_vcpu_block_finish(struct kvm_vcpu *vcpu) {}

static inline void kvm_arm_init_debug(void) {}
static inline void kvm_arm_setup_debug(struct kvm_vcpu *vcpu) {}
static inline void kvm_arm_clear_debug(struct kvm_vcpu *vcpu) {}
static inline void kvm_arm_reset_debug_ptr(struct kvm_vcpu *vcpu) {}

int kvm_arm_vcpu_arch_set_attr(struct kvm_vcpu *vcpu,
			       struct kvm_device_attr *attr);
int kvm_arm_vcpu_arch_get_attr(struct kvm_vcpu *vcpu,
			       struct kvm_device_attr *attr);
int kvm_arm_vcpu_arch_has_attr(struct kvm_vcpu *vcpu,
			       struct kvm_device_attr *attr);

static inline bool kvm_arm_harden_branch_predictor(void)
{
	switch(read_cpuid_part()) {
#ifdef CONFIG_HARDEN_BRANCH_PREDICTOR
	case ARM_CPU_PART_BRAHMA_B15:
	case ARM_CPU_PART_CORTEX_A12:
	case ARM_CPU_PART_CORTEX_A15:
	case ARM_CPU_PART_CORTEX_A17:
		return true;
#endif
	default:
		return false;
	}
}

#define KVM_SSBD_UNKNOWN		-1
#define KVM_SSBD_FORCE_DISABLE		0
#define KVM_SSBD_KERNEL		1
#define KVM_SSBD_FORCE_ENABLE		2
#define KVM_SSBD_MITIGATED		3

static inline int kvm_arm_have_ssbd(void)
{
	
	return KVM_SSBD_UNKNOWN;
}

#endif 
