
#include <linux/ioctl.h>
#include <linux/types.h>

extern struct usb_function_instance *alloc_inst_mtp_ptp(bool mtp_config);
extern struct usb_function *function_alloc_mtp_ptp(
			struct usb_function_instance *fi, bool mtp_config);

struct mtp_file_range {
    
	int         fd;
    
	loff_t      offset;
    
	int64_t     length;
    
	uint16_t    command;
    
	uint32_t    transaction_id;
};

struct mtp_event {
    
	size_t      length;
    
	void        *data;
};


#define MTP_SEND_FILE              _IOW('M', 0, struct mtp_file_range)

#define MTP_RECEIVE_FILE           _IOW('M', 1, struct mtp_file_range)

#define MTP_SEND_EVENT             _IOW('M', 3, struct mtp_event)

#define MTP_SEND_FILE_WITH_HEADER  _IOW('M', 4, struct mtp_file_range)

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

#ifdef __KERNEL__
#ifdef CONFIG_COMPAT
struct __compat_mtp_event {
	compat_size_t   length;
	compat_caddr_t  data;
};

#endif
#endif

#define COMPAT_MTP_SEND_EVENT   _IOW('M', 3, struct __compat_mtp_event)


