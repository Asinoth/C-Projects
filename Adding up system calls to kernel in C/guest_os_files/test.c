#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int get_period_params_check(int ret, int expected_ret,
				struct period_params *params,
				struct period_params *expected_params)
{
	if (ret != expected_ret) {
		printf("NOTOK return value expected %d got %d\n", expected_ret, ret);
		return 1;
	}
	else if(params && expected_params
		&& (params->period != expected_params->period
			|| params->execution_time != expected_params->execution_time
			|| params->num_of_periods != expected_params->num_of_periods)
		) 
	{
		printf("NOTOK period_params expected { %d %d %d } got { %d %d %d }\n",
			params->period, params->execution_time, params->num_of_periods,	
			expected_params->period, expected_params->execution_time,
			expected_params->num_of_periods);
		return 1;
	}
	else {
		printf("OK\n");
		return 0;
	}
}	

int set_period_params_check(int ret, int expected_ret,
				struct period_params *expected_params)
{
	if (ret != expected_ret) {
		printf("NOTOK return value expected %d got %d\n",
			expected_ret, ret);
		return 1;
	}
	else {
		struct period_params params;
		int get_ret = get_period_params(&params);
		return get_period_params_check(get_ret, 0,
					&params, expected_params);
	}
}

void params_set(struct period_params *params,
		int period, int exec_time, int num_of_p)
{
	params->period = period;
	params->execution_time = exec_time;
	params->num_of_periods = num_of_p;
}

struct set_period_bad_args {
	const char* const prompt;
	const int ints[3];
} SET_PERIOD_BAD_ARGS[] = {
	{
		"\tPeriod negative... ",
		{ -12345, 5, 10 }
	},
	{
		"\tExecution time negative... ",
		{ 10, -12345, 10 }
	},
	{
		"\tNum of periods negative... ",
		{ 5, 3, -1234 }
	},
	{
		"\tPeriod and exec time negative... ",
		{ -1234, -1234, 10 }
	},
	{
		"\tPeriod and num of periods negative... ",
		{ -1234, 5, -1234 }
	},
	{
		"\tExecution time and num of periods negative... ",
		{ 5, -1234, -1234 }
	},
	{
		"\tAll negative... ",
		{ -1234, -1234, -1234 }
	}
};

int main(void) {
	struct period_params params, expected_params;
	struct set_period_bad_args *bad_args;
	int ret, i, failures = 0, tests = 0;	

	/* *************************************** */
	
	++tests;	
	printf("Initial get... ");
	params_set(&expected_params, 0, 0, 0);
	ret = get_period_params(&params);
	failures += get_period_params_check(ret, 0,
					&params, &expected_params);
	
	++tests;
	printf("Initial set of 3 1 5... ");
	params_set(&expected_params, 3, 1, 5);
	ret = set_period_params(3, 1, 5);
	failures += set_period_params_check(ret, 0, &expected_params);

	/* Below the period_params must not change from 3 1 5 at the end */
	
	++tests;	
	printf("Pass NULL to get_period_params... ");
	ret = get_period_params(NULL);
	failures += get_period_params_check(ret, EINVAL, NULL, NULL);

	printf("Pass negative numbers to set_period_params...\n");
	for (i = 0;
		i < (int)(sizeof(SET_PERIOD_BAD_ARGS) / sizeof(struct set_period_bad_args));
		++i)
	{
		++tests;
		bad_args = &SET_PERIOD_BAD_ARGS[i];
		printf(bad_args->prompt);
		ret = set_period_params(bad_args->ints[0],
					bad_args->ints[1],
					bad_args->ints[2]);
		failures += set_period_params_check(ret, EINVAL, &expected_params);
	}
	
	++tests;
	printf("Values should not be changed from 3 1 5... ");
	ret = get_period_params(&params);
	failures += get_period_params_check(ret, 0,
						&params, &expected_params);

	/* *************************************** */
	
	printf("%d TESTS :: %d SUCCESSES :: %d FAILURES\n",
		tests, tests - failures, failures);
		
	return 0;
}
