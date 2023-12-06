#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>

#include <linux/period_params.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>

asmlinkage long sys_get_period_params(struct period_params *params) {
	const struct period_params *per_params =  &current->per_params;
	
	printk("-> Alex 3643\nGetting params\n");
	
	if (params == NULL) {
		printk("\t! Got NULL as params argument\n");
		return EINVAL;
	}
	
	printk("\tperiod: %d\n", per_params->period);
	printk("\texecution_time: %d\n", per_params->execution_time);
	printk("\tnum_of_periods: %d\n", per_params->num_of_periods);
	
	params->period = per_params->period;
	params->execution_time = per_params->execution_time;
	params->num_of_periods = per_params->num_of_periods;
	
	return 0;
}
