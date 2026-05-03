

#ifndef SEC_VIBRATOR_H
#define SEC_VIBRATOR_H

#include <linux/kthread.h>
#include <linux/leds.h>
#include <linux/cdev.h>

#define MAX_INTENSITY			10000
#define MAX_TIMEOUT			10000
#define PACKET_MAX_SIZE			1000

#define HAPTIC_ENGINE_FREQ_MIN		1200
#define HAPTIC_ENGINE_FREQ_MAX		3500

#define VIB_BUFSIZE                     30

#define HOMEKEY_DURATION		7

struct vib_packet {
	int time;
	int intensity;
	int freq;
	int overdrive;
};

enum {
	VIB_PACKET_TIME = 0,
	VIB_PACKET_INTENSITY,
	VIB_PACKET_FREQUENCY,
	VIB_PACKET_OVERDRIVE,
	VIB_PACKET_MAX,
};

enum {
	FREQ_ALERT = 0,	
	FREQ_ZERO,	
	FREQ_LOW,	
	FREQ_MID,	
	FREQ_HIGH,	
	FREQ_PRESS,	
	FREQ_RELEASE,	
	FREQ_MAX,
};

struct sec_vibrator_ops {
	int (*enable)(struct device *dev, bool en);
	int (*set_intensity)(struct device *dev, int intensity);
	int (*set_frequency)(struct device *dev, int frequency);
	int (*set_overdrive)(struct device *dev, bool en);
	int (*get_motor_type)(struct device *dev, char *buf);
	int (*get_num_waves)(struct device *dev);
	int (*set_cp_trigger_index)(struct device *dev, unsigned int index);
	int (*get_cp_trigger_index)(struct device *dev);
	int (*set_cp_trigger_queue)(struct device *dev, const char *buf);
	int (*get_cp_trigger_queue)(struct device *dev, char *buf);
	int (*set_force_touch_intensity)(struct device *dev, int intensity);
	int (*set_tuning_with_temp)(struct device *dev, int temperature);
	bool (*get_calibration)(struct device *dev);
	int (*get_step_size)(struct device *dev, int *step_size);
	int (*get_intensities)(struct device *dev, int *buf);
	int (*set_intensities)(struct device *dev, int *buf);
	int (*get_haptic_intensities)(struct device *dev, int *buf);
	int (*set_haptic_intensities)(struct device *dev, int *buf);
	int (*get_haptic_durations)(struct device *dev, int *buf);
	int (*set_haptic_durations)(struct device *dev, int *buf);
};

struct sec_vibrator_drvdata {
	struct class *to_class;
	struct device *to_dev;
	struct device *dev;
	struct hrtimer timer;
	struct kthread_worker kworker;
	struct kthread_work kwork;
	struct mutex vib_mutex;
	struct vib_packet vib_pac[PACKET_MAX_SIZE];
	const struct sec_vibrator_ops *vib_ops;

	bool f_packet_en;
	bool packet_running;
	int packet_size;
	int packet_cnt;
	unsigned int index;

	int force_touch_intensity;
	int intensity;
	int frequency;
	bool overdrive;

	int timeout;

	struct led_classdev cdev;
	int state;
	int duration;
};

extern int sec_vibrator_register(struct sec_vibrator_drvdata *ddata);
extern int sec_vibrator_unregister(struct sec_vibrator_drvdata *ddata);

#endif 
