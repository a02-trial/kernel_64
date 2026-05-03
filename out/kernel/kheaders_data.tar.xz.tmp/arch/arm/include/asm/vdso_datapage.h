
#ifndef __ASM_VDSO_DATAPAGE_H
#define __ASM_VDSO_DATAPAGE_H

#ifdef __KERNEL__

#ifndef __ASSEMBLY__

#include <asm/page.h>


struct vdso_data {
	u32 seq_count;		
	u16 tk_is_cntvct;	
	u16 cs_shift;		
	u32 xtime_coarse_sec;	
	u32 xtime_coarse_nsec;

	u32 wtm_clock_sec;	
	u32 wtm_clock_nsec;
	u32 xtime_clock_sec;	
	u32 cs_mult;		

	u64 cs_cycle_last;	
	u64 cs_mask;		

	u64 xtime_clock_snsec;	
	u32 tz_minuteswest;	
	u32 tz_dsttime;
};

union vdso_data_store {
	struct vdso_data data;
	u8 page[PAGE_SIZE];
};

#endif 

#endif 

#endif 
