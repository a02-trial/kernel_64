

#ifndef __S2MU005_H__
#define __S2MU005_H__
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/notifier.h>

#define MFD_DEV_NAME "s2mu005"
#define M2SH(m) ((m) & 0x0F ? ((m) & 0x03 ? ((m) & 0x01 ? 0 : 1) : ((m) & 0x04 ? 2 : 3)) : \
		((m) & 0x30 ? ((m) & 0x10 ? 4 : 5) : ((m) & 0x40 ? 6 : 7)))

struct s2mu005_haptic_platform_data {
	u16 max_timeout;
	u16 duty;
	u16 period;
	u16 reg2;
	char *regulator_name;
	unsigned int pwm_id;

	void (*init_hw) (void);
	void (*motor_en) (bool);
};
extern int s2m_acok_notify_call_chain(void);

struct s2mu005_regulator_data {
	int id;
	struct regulator_init_data *initdata;
	struct device_node *reg_node;
};

struct s2mu005_platform_data {
	
	int irq_base;
	int irq_gpio;
	bool wakeup;

	int num_regulators;
	struct s2mu005_regulator_data *regulators;
	
	struct s2mu005_haptic_platform_data *haptic_data;
	struct mfd_cell *sub_devices;
	int num_subdevs;
};

struct s2mu005
{
	struct regmap *regmap;
};

#endif 
