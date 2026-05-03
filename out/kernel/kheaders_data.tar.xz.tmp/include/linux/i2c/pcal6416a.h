

#ifndef _PCAL6416A_H_
#define _PCAL6416A_H_

#define DRV_NAME	"pcal6416a-gpio"

#define PCAL6416A_INPUT			0x00  
#define PCAL6416A_DAT_OUT		0x02  
#define PCAL6416A_POLARITY		0x04  
#define PCAL6416A_CONFIG		0x06  
#define PCAL6416A_DRIVE0		0x40  
#define PCAL6416A_DRIVE1		0x42  
#define PCAL6416A_INPUT_LATCH		0x44  
#define PCAL6416A_EN_PULLUPDOWN		0x46  
#define PCAL6416A_SEL_PULLUPDOWN	0x48  
#define PCAL6416A_INT_MASK		0x4A  
#define PCAL6416A_INT_STATUS		0x4C  
#define PCAL6416A_OUTPUT_CONFIG		0x4F  

#define NO_PULL				0x00
#define PULL_DOWN			0x01
#define PULL_UP				0x02

#define POWER_ON			1
#define POWER_OFF			0

#define NUM_PCAL6416A_CHIP		1   // MTK_TEST


enum {
	GPIO_CFG_6_25MA,
	GPIO_CFG_12_5MA,
	GPIO_CFG_18_75MA,
	GPIO_CFG_25MA,
};

struct pcal6416a_platform_data {
	
	unsigned gpio_base;
	int gpio_start;
	int ngpio;
	int irq_base;
	int reset_gpio;
	int irq_gpio;
	uint16_t support_init;
	uint16_t init_config;
	uint16_t init_data_out;
	uint16_t init_en_pull;
	uint16_t init_sel_pull;
	struct regulator *vdd;
};

struct pcal6416a_chip {
	struct i2c_client *client;
	struct gpio_chip gpio_chip;
	struct dentry	*dentry;
	struct mutex lock;
	struct pcal6416a_platform_data *pdata;
	unsigned gpio_start;

	uint16_t reg_output;
	uint16_t reg_polarity;
	uint16_t reg_config;
	uint16_t reg_drive0;
	uint16_t reg_drive1;
	uint16_t reg_inputlatch;
	uint16_t reg_enpullupdown;
	uint16_t reg_selpullupdown;
	uint16_t reg_intmask;
	uint16_t reg_outputconfig;
};

#ifdef CONFIG_SEC_PM_DEBUG
int expander_print_all(void);
#endif

#endif
