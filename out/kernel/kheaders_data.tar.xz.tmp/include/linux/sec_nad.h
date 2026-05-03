

#ifndef SEC_NAD_H
#define SEC_NAD_H

#if defined(CONFIG_SEC_FACTORY)
#define NAD_PARAM_NAME "/dev/block/by-name/up_param"
#define NAD_ENV_DATA_OFFSET 4*1024*1024 - 512

#define NAD_PARAM_READ  0
#define NAD_PARAM_WRITE 1
#define NAD_PARAM_EMPTY 2

#define NAD_BUFF_SIZE       10
#define NAD_CMD_LIST        3

#define SMD_ITEM_RESULT_LENGTH 128

#define STEP_MAIN_TIMEOUT 	8000

enum nad_enum_step {
	STEP_NONE = 0,
	STEP_SMD_DDRSCANTEST_START,
	STEP_SMD_DDRSCANTEST_DONE,
	STEP_SMD_NAD_START,
	STEP_SMD_NAD_DONE,
	STEP_CAL1,
	STEP_MAIN,
	STEP_ETC,
	//
	STEP_STEPSCOUNT,
};

enum nad_reboot_option {
	NO_REBOOT = 0,
	REBOOT,
};

struct nad_env {
	unsigned int curr_step;
    unsigned int nad_acat_remaining_count;
    unsigned int nad_dramtest_remaining_count;
	unsigned int nad_smd_result;
	unsigned int nad_acat_result;
	unsigned int nad_main_result;
	unsigned int nad_dram_test_result;
	unsigned int nad_dram_fail_data;
	volatile unsigned long long nad_dram_fail_address;
	char smd_item_result[SMD_ITEM_RESULT_LENGTH];
};

struct sec_nad_param {
	struct work_struct sec_nad_work;
	struct delayed_work sec_nad_delay_work;
	struct completion comp;
	unsigned long offset;
	int state;
	int retry_cnt;
	int curr_cnt;
};

enum {
	NAD_CHECK_NONE = 0,
	NAD_CHECK_INCOMPLETED,
	NAD_CHECK_FAIL,
	NAD_CHECK_PASS,
};

static struct sec_nad_param sec_nad_param_data;
static struct nad_env sec_nad_env;
extern unsigned int lpcharge;
static int erase_pass;
static DEFINE_MUTEX(sec_nad_param_lock);

#endif
#endif