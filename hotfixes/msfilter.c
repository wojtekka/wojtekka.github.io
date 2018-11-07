/*
 * Hotfix for setsockopt() vulnerability in <=2.4.26 and other kernels.
 * Disables MCAST_MSFILTER that isn't needed for most of installations.
 *
 * (C) Copyright 2004 Wojtek Kaniewski <wojtekka@irc.pl>
 *     GPLv2, NO WARRANTY OF ANY KIND.
 *
 * gcc -Wall -O3 -fomit-frame-pointer -c msfilter.c
 */

#include <linux/autoconf.h>
#ifdef CONFIG_SMP
#define __SMP__
#endif

#define MODULE
#define __KERNEL__

#include <linux/module.h>
#include <linux/in.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>

#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

extern void *sys_call_table[];
int (*old_socketcall)(int call, unsigned long *args);

static unsigned long new_socketcall(int call, unsigned long *args)
{
	int res;
	
	if (call == SYS_SETSOCKOPT) {
		unsigned long a[5];

		if (copy_from_user(a, args, 5 * sizeof(a[0]))) {
			res = -EFAULT;
			goto exit;
		}

		if (a[1] == SOL_IP && a[2] == MCAST_MSFILTER) {
			printk("possible MCAST_MSFILTER exploit attempt. uid=%d, comm=\"%.100s\"\n", current->uid, current->comm);
			res = -EPERM;
			goto exit;
		}
	}

	res = old_socketcall(call, args);
	
exit:
	return res;	
}

int init_module()
{
	unsigned long flags;

	save_flags(flags);
	cli();

	old_socketcall = sys_call_table[__NR_socketcall];
	sys_call_table[__NR_socketcall] = new_socketcall;
  
	restore_flags(flags);

	return 0;
}

void cleanup_module()
{
	unsigned long flags;

	save_flags(flags);
	cli();

	sys_call_table[__NR_socketcall] = old_socketcall;

	restore_flags(flags);
}


