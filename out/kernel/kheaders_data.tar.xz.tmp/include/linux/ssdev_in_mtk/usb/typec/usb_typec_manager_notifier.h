

#ifndef __USB_TYPEC_MANAGER_NOTIFIER_H__
#define __USB_TYPEC_MANAGER_NOTIFIER_H__

#include <linux/muic/muic.h>
#include <linux/muic/muic_notifier.h>
#ifdef CONFIG_VBUS_NOTIFIER
#include <linux/vbus_notifier.h>
#endif
#include <linux/usb/typec/common/pdic_notifier.h>





typedef enum {

	MANAGER_NOTIFY_MUIC_NONE = 0,
	MANAGER_NOTIFY_MUIC_DOCK,
	MANAGER_NOTIFY_MUIC_MHL,
	MANAGER_NOTIFY_MUIC_USB,
	MANAGER_NOTIFY_MUIC_TSP,
	MANAGER_NOTIFY_MUIC_CHARGER,
	MANAGER_NOTIFY_MUIC_CPUIDLE,
	MANAGER_NOTIFY_MUIC_CPUFREQ,
	MANAGER_NOTIFY_MUIC_TIMEOUT_OPEN_DEVICE,
	MANAGER_NOTIFY_MUIC_UART,


	MANAGER_NOTIFY_PDIC_INITIAL = 20,
	MANAGER_NOTIFY_PDIC_WACOM, 
	MANAGER_NOTIFY_PDIC_SENSORHUB,
	MANAGER_NOTIFY_PDIC_USBDP,
	MANAGER_NOTIFY_PDIC_DP,
	MANAGER_NOTIFY_PDIC_SUB_BATTERY,
	MANAGER_NOTIFY_PDIC_BATTERY,
	MANAGER_NOTIFY_PDIC_USB,
	MANAGER_NOTIFY_PDIC_MUIC, 


	MANAGER_NOTIFY_VBUS_USB = 30,
	MANAGER_NOTIFY_VBUS_CHARGER,
} manager_notifier_device_t;

typedef enum {
	PDIC_NOTY,
	MUIC_NOTY,
	VBUS_NOTY,
	USB_STATE_NOTY,
} manager_notify_t;

typedef enum {
	PD_USB_TYPE,
	PD_TA_TYPE,
} pd_usb_state_t;

#if defined(CONFIG_VBUS_NOTIFIER)
typedef enum {
	EVENT_LOAD = 0,
	EVENT_CANCEL,
} manager_muic_event;
#endif

typedef struct
{
	uint64_t src:4;
	uint64_t dest:4;
	uint64_t id:8;
	uint64_t sub1:16;
	uint64_t sub2:16;
	uint64_t sub3:16;
	void *pd;
} MANAGER_NOTI_TYPEDEF;

struct typec_manager_event_work
{
	struct work_struct typec_manager_work;
	int src;
	int dest;
	int id;
	int sub1;
	int sub2;
	int sub3;
};

typedef struct _manager_data_t
{
	struct blocking_notifier_head manager_muic_notifier;
	struct blocking_notifier_head manager_notifier;
	struct notifier_block pdic_nb;
	struct notifier_block muic_nb;
#if defined(CONFIG_VBUS_NOTIFIER)
	struct notifier_block vbus_nb;
#endif

	struct delayed_work manager_init_work;
	struct workqueue_struct *typec_manager_wq;
	struct workqueue_struct *typec_manager_muic_wq;
	struct delayed_work cable_check_work;
	struct delayed_work usb_event_work;
#if defined(CONFIG_USB_HW_PARAM)
	struct delayed_work rtctime_update_work;
#endif
#if defined(CONFIG_VBUS_NOTIFIER)
	struct delayed_work muic_event_work;
#endif
	struct mutex mo_lock;

	int muic_attach_state;
	int muic_cable_type;
	int muic_attach_state_without_pdic;
#if defined(CONFIG_VBUS_NOTIFIER)
	int muic_fake_event_wq_processing;
#endif
	int vbus_state;

#ifdef CONFIG_USE_SECOND_MUIC
	int second_muic_attach_state;
	int second_muic_cable_type;
#endif

	int pdic_attach_state;	// USB_STATUS_NOTIFY_DETACH, UFP, DFP, DRP, NO_USB
	int pdic_drp_state;
	int pdic_rid_state;
	int cable_type;
	int usb_enum_state;
	bool usb_enable_state;
	unsigned long otg_stamp;
	int vbus_by_otg_detection;
	int pd_con_state;
	int water_det;
	int wVbus_det;
	int is_MPSM;
	void *pd;
#if defined(CONFIG_USB_HW_PARAM)
	int water_count;
	int dry_count;
	int usb_highspeed_count;
	int usb_superspeed_count;
	int waterChg_count;
	unsigned long waterDet_duration;
	unsigned long waterDet_time;
	unsigned long dryDet_time;
	unsigned long wVbus_duration;
	unsigned long wVbusHigh_time;
	unsigned long wVbusLow_time;
#endif
	int water_cable_type;
	int alt_is_support;
	int dp_is_support;
	int dp_attach_state;
	int dp_cable_type;
	int dp_hpd_state;
	int dp_is_connect;
	int dp_hs_connect;
	int dp_check_done;
	struct notifier_block manager_external_notifier_nb; 
}manager_data_t;


#define PDIC_BATTERY	(1<<0)
#define PDIC_USB	(1<<1)
#define PDIC_DP		(1<<2)

#define MANAGER_NOTIFIER_BLOCK(name)	\
	struct notifier_block (name)

extern void manager_notifier_usbdp_support(void);
extern void manager_notifier_test(void *);



extern int manager_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, manager_notifier_device_t listener);
extern int manager_notifier_unregister(struct notifier_block *nb);

#endif 
