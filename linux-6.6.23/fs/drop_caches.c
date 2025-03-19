// SPDX-License-Identifier: GPL-2.0
/*
 * Implement the manual drop-all-pagecache function
 */

#include <linux/pagemap.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/writeback.h>
#include <linux/sysctl.h>
#include <linux/gfp.h>
#include <linux/swap.h>
#include "internal.h"

#include "rdma_fs.h" // @pty

/* A global variable is a bit ugly, but it keeps the code simple */
int sysctl_drop_caches;

static void drop_pagecache_sb(struct super_block *sb, void *unused)
{
	struct inode *inode, *toput_inode = NULL;

	spin_lock(&sb->s_inode_list_lock);
	list_for_each_entry(inode, &sb->s_inodes, i_sb_list) {
		spin_lock(&inode->i_lock);
		/*
		 * We must skip inodes in unusual state. We may also skip
		 * inodes without pages but we deliberately won't in case
		 * we need to reschedule to avoid softlockups.
		 */
		if ((inode->i_state & (I_FREEING|I_WILL_FREE|I_NEW)) ||
		    (mapping_empty(inode->i_mapping) && !need_resched())) {
			spin_unlock(&inode->i_lock);
			continue;
		}
		__iget(inode);
		spin_unlock(&inode->i_lock);
		spin_unlock(&sb->s_inode_list_lock);

		invalidate_mapping_pages(inode->i_mapping, 0, -1);
		iput(toput_inode);
		toput_inode = inode;

		cond_resched();
		spin_lock(&sb->s_inode_list_lock);
	}
	spin_unlock(&sb->s_inode_list_lock);
	iput(toput_inode);
}

static void drop_pagecache_sb2(struct super_block *sb, void *unused) //@pty
{
	struct inode *inode, *toput_inode = NULL;
	
	spin_lock(&sb->s_inode_list_lock);
	list_for_each_entry(inode, &sb->s_inodes, i_sb_list) {
		spin_lock(&inode->i_lock);
		/*
		 * We must skip inodes in unusual state. We may also skip
		 * inodes without pages but we deliberately won't in case
		 * we need to reschedule to avoid softlockups.
		 */

		if ((inode->i_state & (I_FREEING|I_WILL_FREE|I_NEW)) ||
		    (mapping_empty(inode->i_mapping) && !need_resched())) {
			spin_unlock(&inode->i_lock);
			continue;
		}

		__iget(inode);
		spin_unlock(&inode->i_lock);
		spin_unlock(&sb->s_inode_list_lock);

		invalidate_inode_pages2(inode->i_mapping);
		iput(toput_inode);
		toput_inode = inode;

		cond_resched();
		spin_lock(&sb->s_inode_list_lock);
	}
	spin_unlock(&sb->s_inode_list_lock);
	iput(toput_inode);
}

void invalidate_inode_cache(struct super_block *sb) //@pty
{
	struct inode *inode;
	
	spin_lock(&sb->s_inode_list_lock);
	list_for_each_entry(inode, &sb->s_inodes, i_sb_list) {
		spin_lock(&inode->i_lock);
		pr_info("invalidate inode %lu\n", inode->i_ino);
		inode->i_state |= I_INVALID;
		spin_unlock(&inode->i_lock);
	}
	spin_unlock(&sb->s_inode_list_lock);
}
EXPORT_SYMBOL(invalidate_inode_cache);

int drop_caches_sysctl_handler(struct ctl_table *table, int write,
		void *buffer, size_t *length, loff_t *ppos)
{
	int ret;

	ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
	if (ret)
		return ret;
	if (write) {
		static int stfu;

		if (sysctl_drop_caches & 1) {
			lru_add_drain_all();
			iterate_supers(drop_pagecache_sb, NULL);
			count_vm_event(DROP_PAGECACHE);
		}
		if (sysctl_drop_caches & 2) {
			drop_slab();
			count_vm_event(DROP_SLAB);
		}
		if (!stfu) {
			pr_info("%s (%d): drop_caches: %d\n",
				current->comm, task_pid_nr(current),
				sysctl_drop_caches);
		}
		stfu |= sysctl_drop_caches & 4;
	}
	return 0;
}

void drop_cache_sb(struct super_block *sb) //@pty
{
	ktime_t start_time, end_time;
	start_time = ktime_get();
	lru_add_drain_all();
	end_time = ktime_get();
    pr_info("lru_add_drain_all in %lld us\n", ktime_us_delta(end_time, start_time));

	start_time = ktime_get();
	drop_pagecache_sb2(sb, NULL);
	end_time = ktime_get();
    pr_info("drop_pagecache_sb2 in %lld us\n", ktime_us_delta(end_time, start_time));

	start_time = ktime_get();
	invalidate_bdev(sb->s_bdev);
	end_time = ktime_get();
    pr_info("invalidate_bdev in %lld us\n", ktime_us_delta(end_time, start_time));

	start_time = ktime_get();
	drop_slab();
	end_time = ktime_get();
    pr_info("drop_slab in %lld us\n", ktime_us_delta(end_time, start_time));
}
EXPORT_SYMBOL(drop_cache_sb);

void drop_cache_sb_all(struct super_block *sb) //@pty
{
	ktime_t start_time, end_time;
	start_time = ktime_get();
	lru_add_drain_all();
	end_time = ktime_get();
    pr_info("lru_add_drain_all in %lld us\n", ktime_us_delta(end_time, start_time));

	// sb->s_bdev->bd_super? 없는데..

	start_time = ktime_get();
	iterate_supers(drop_pagecache_sb, NULL);
	end_time = ktime_get();
    pr_info("drop_pagecache_sb in %lld us\n", ktime_us_delta(end_time, start_time));

	start_time = ktime_get();
	drop_slab();
	end_time = ktime_get();
    pr_info("drop_slab in %lld us\n", ktime_us_delta(end_time, start_time));
}
EXPORT_SYMBOL(drop_cache_sb_all);

// @pty
void (*lock_ptr)(int, unsigned long, const char *) = NULL;
EXPORT_SYMBOL(lock_ptr); 
void (*sync_ptr)(unsigned long, const char *) = NULL;
EXPORT_SYMBOL(sync_ptr);

void (*invalidate_ptr)(unsigned long, const char *) = NULL; 
EXPORT_SYMBOL(invalidate_ptr);
void (*unlock_ptr)(int, unsigned long, const char *) = NULL; 
EXPORT_SYMBOL(unlock_ptr);