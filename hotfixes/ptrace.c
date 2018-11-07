/*
 * Disable ptrace() for users outside group `root'.
 *
 * (C) Copyright 2001-2004 Wojtek Kaniewski <wojtekka@irc.pl>
 *     GPLv2, NO WARRANTY OF ANY KIND.
 *
 * gcc -Wall -O3 -fomit-frame-pointer -c ptrace.c
 */

#include <linux/autoconf.h>
#ifdef CONFIG_SMP
#define __SMP__
#endif

#define MODULE
#define __KERNEL__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/smp_lock.h>
#include <asm/unistd.h>

#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

extern void *sys_call_table[];
static int (*old_ptrace)(long request, long pid, long addr, long data);

static int new_ptrace(long request, long pid, long addr, long data)
{
	int ret, i;
	
	lock_kernel();
	for (i = 0; i < current->ngroups; i++)
		if (!current->groups[i])
			goto call;
	printk("ptrace() attempt. uid=%d, comm=\"%.100s\"\n", current->uid, current->comm);
	ret = -EPERM;
	goto exit;
call:			
	ret = old_ptrace(request, pid, addr, data);
exit:
	unlock_kernel();
	return ret;
}

int init_module()
{
	unsigned long flags;

	save_flags(flags);
	cli();

	old_ptrace = sys_call_table[__NR_ptrace];
	sys_call_table[__NR_ptrace] = new_ptrace;
  
	restore_flags(flags);

	return 0;
}

void cleanup_module()
{
	unsigned long flags;

	save_flags(flags);
	cli();

	sys_call_table[__NR_ptrace] = old_ptrace;

	restore_flags(flags);
}


