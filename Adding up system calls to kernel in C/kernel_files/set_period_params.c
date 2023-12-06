#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>

#include <linux/period_params.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>

asmlinkage long sys_set_period_params(int period, int execution_time,
					int num_of_periods) {
	struct period_params *params = &current->per_params;
	
	printk("-> Alex 3643\nSetting period params\n");
	printk("\tperiod: %d\n", period);
	printk("\texecution_time: %d\n", execution_time);
	printk("\tnum_of_periods: %d\n", num_of_periods);
	
	if (period <= 0 || execution_time <= 0 || num_of_periods <= 0) {
		printk("No argument can be <= 0\n");
		return EINVAL;
	}
	
	if (execution_time > period) {
		printk("Impossible to have execution_time > period\n");
		return EINVAL;
	}
	
	
	params->period = period;
	params->execution_time = execution_time;
	params->num_of_periods = num_of_periods;

	return 0;
}
