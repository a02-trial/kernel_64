#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__


#define TSK_ACTIVE_MM 1060 

#define TI_FLAGS 0 
#define TI_PREEMPT 4 
#define TI_ADDR_LIMIT 8 
#define TI_TASK 12 
#define TI_CPU 16 
#define TI_CPU_DOMAIN 20 
#define TI_CPU_SAVE 24 
#define TI_USED_CP 76 
#define TI_TP_VALUE 92 
#define TI_FPSTATE 104 
#define TI_VFPSTATE 248 
#define VFP_CPU 272 

#define S_R0 0 
#define S_R1 4 
#define S_R2 8 
#define S_R3 12 
#define S_R4 16 
#define S_R5 20 
#define S_R6 24 
#define S_R7 28 
#define S_R8 32 
#define S_R9 36 
#define S_R10 40 
#define S_FP 44 
#define S_IP 48 
#define S_SP 52 
#define S_LR 56 
#define S_PC 60 
#define S_PSR 64 
#define S_OLD_R0 68 
#define PT_REGS_SIZE 72 
#define SVC_DACR 72 
#define SVC_ADDR_LIMIT 76 
#define SVC_REGS_SIZE 80 

#define L2X0_R_PHY_BASE 0 
#define L2X0_R_AUX_CTRL 4 
#define L2X0_R_TAG_LATENCY 8 
#define L2X0_R_DATA_LATENCY 12 
#define L2X0_R_FILTER_START 16 
#define L2X0_R_FILTER_END 20 
#define L2X0_R_PREFETCH_CTRL 24 
#define L2X0_R_PWR_CTRL 28 

#define MM_CONTEXT_ID 376 

#define VMA_VM_MM 32 
#define VMA_VM_FLAGS 40 

#define VM_EXEC 4 

#define PAGE_SZ 4096 

#define SYS_ERROR0 10420224 

#define SIZEOF_MACHINE_DESC 104 
#define MACHINFO_TYPE 0 
#define MACHINFO_NAME 4 

#define PROC_INFO_SZ 52 
#define PROCINFO_INITFUNC 16 
#define PROCINFO_MM_MMUFLAGS 8 
#define PROCINFO_IO_MMUFLAGS 12 

#define CPU_SLEEP_SIZE 40 
#define CPU_DO_SUSPEND 44 
#define CPU_DO_RESUME 48 
#define SLEEP_SAVE_SP_SZ 8 
#define SLEEP_SAVE_SP_PHYS 4 
#define SLEEP_SAVE_SP_VIRT 0 

#define DMA_BIDIRECTIONAL 0 
#define DMA_TO_DEVICE 1 
#define DMA_FROM_DEVICE 2 

#define CACHE_WRITEBACK_ORDER 6 
#define CACHE_WRITEBACK_GRANULE 64 


#define VDSO_DATA_SIZE 4096 

#endif
