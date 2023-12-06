The steps taken to alter the kernel did not deviate from what the
tutorial shows. In short:
- Added the syscall numbers to `unistd_32.h`
- Added the assembly datatypes for the syscalls to `syscall_table_32.S`
- Added the function prototypes to the `syscalls.h`
- Added the function definitions with `set_period_params.h` and `get_period_params.h`
- Added the `period_params.h`
- Extended the `task_struct` by using the above struct
- Added the get/set function object files to `obj-y`
- Ran `make ARCH=i386 bzImage`
- Fired up the Guest OS
- Added the wrapper functions and the period_param struct to `/usr/include/unistd.h`
- Wrote and ran the test file

The "loop" of working for this assignment was:
	understand -> make compile-able changes -> boot qemu -> test -> repeat

