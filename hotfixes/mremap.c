/*
 * Hotfix for mremap() vulnerability in <=2.4.23 and other kernels.
 *
 * (C) Copyright 2004 Wojtek Kaniewski <wojtekka@irc.pl>
 *     GPLv2, NO WARRANTY OF ANY KIND.
 *
 * gcc -Wall -O3 -fomit-frame-pointer -c mremap.c
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
#include <linux/mman.h>
#include <asm/unistd.h>

#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

extern void *sys_call_table[];
static unsigned long (*old_mremap)(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);

static unsigned long new_mremap(unsigned long addr, unsigned long old_len, unsigned long new_len, unsigned long flags, unsigned new_addr)
{
	if ((flags & MREMAP_FIXED) && !new_len && (new_addr != addr)) {
		printk("possible mremap() exploit attempt. uid=%d, comm=\"%.100s\"\n", current->uid, current->comm);
		return -EPERM;
	}

	return old_mremap(addr, old_len, new_len, flags, new_addr);
}

int init_module()
{
	unsigned long flags;

	save_flags(flags);
	cli();

	old_mremap = sys_call_table[__NR_mremap];
	sys_call_table[__NR_mremap] = new_mremap;
  
	restore_flags(flags);

	return 0;
}

void cleanup_module()
{
	unsigned long flags;

	save_flags(flags);
	cli();

	sys_call_table[__NR_mremap] = old_mremap;

	restore_flags(flags);
}


