/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ARM_MPU_H
#define __ARM_MPU_H

#ifdef CONFIG_ARM_MPU


#define MPUIR_nU		1
#define MPUIR_DREGION		8
#define MPUIR_IREGION		16
#define MPUIR_DREGION_SZMASK	(0xFF << MPUIR_DREGION)
#define MPUIR_IREGION_SZMASK	(0xFF << MPUIR_IREGION)


#define MMFR0_PMSA		(0xF << 4)
#define MMFR0_PMSAv7		(3 << 4)


#define MPU_RSR_SZ		1
#define MPU_RSR_EN		0


#define MPU_RSR_ALL_MEM		63


#define MPU_ACR_XN		(1 << 12)
#define MPU_ACR_SHARED		(1 << 2)


#define MPU_RGN_CACHEABLE	0xB
#define MPU_RGN_SHARED_CACHEABLE (MPU_RGN_CACHEABLE | MPU_ACR_SHARED)
#define MPU_RGN_STRONGLY_ORDERED 0


#ifdef CONFIG_SMP
#define MPU_RGN_NORMAL		(MPU_RGN_CACHEABLE | MPU_ACR_SHARED)
#else
#define MPU_RGN_NORMAL		MPU_RGN_CACHEABLE
#endif


#define MPU_AP_PL1RW_PL0RW	(0x3 << 8)
#define MPU_AP_PL1RW_PL0R0	(0x2 << 8)
#define MPU_AP_PL1RW_PL0NA	(0x1 << 8)


#define MPU_PROBE_REGION	0
#define MPU_BG_REGION		1
#define MPU_RAM_REGION		2
#define MPU_VECTORS_REGION	3


#define MPU_MAX_REGIONS		16

#define MPU_DATA_SIDE		0
#define MPU_INSTR_SIDE		1

#ifndef __ASSEMBLY__

struct mpu_rgn {
	
	u32 drbar;
	u32 drsr;
	u32 dracr;
};

struct mpu_rgn_info {
	u32 mpuir;
	struct mpu_rgn rgns[MPU_MAX_REGIONS];
};
extern struct mpu_rgn_info mpu_rgn_info;

#endif 

#endif 

#endif
