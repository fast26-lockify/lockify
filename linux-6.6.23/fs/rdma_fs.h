// 파일: fs/rdma_fs.h

#ifndef _RDMA_FS_H
#define _RDMA_FS_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/atomic.h>
#include <linux/blk-mq.h>
#include <linux/blk-integrity.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/scatterlist.h>
#include <linux/nvme.h>
#include <asm/unaligned.h>
#include <linux/inet.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
#include <linux/nvme-rdma.h>
#include <linux/fsnotify.h>
#include <linux/fsnotify_backend.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/highuid.h>
#include <linux/pagemap.h>
#include <linux/dax.h>
#include <linux/quotaops.h>
#include <linux/buffer_head.h>
#include <linux/writeback.h>
#include <linux/pagevec.h>
#include <linux/mpage.h>
#include <linux/uio.h>
#include <linux/bio.h>
#include <linux/workqueue.h>
#include <linux/printk.h>
#include <linux/bitops.h>
#include <linux/iomap.h>
#include <linux/iversion.h>
#include <rdma/mr_pool.h>
#include <linux/mount.h>
#include <linux/workqueue.h>
#include <linux/jbd2.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/dcache.h>
#include <linux/wait.h>
#include <linux/fdtable.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/random.h>
#include <linux/blkdev.h>
#include <linux/ktime.h>

#define BUFFER_SIZE 1024

enum fs_state_type {
	FS_STATE_NONE,
	FS_STATE_MODIFY,
	FS_STATE_SHARED,
	FS_STATE_INVALID,
	FS_STATE_ERROR,
};

enum fs_send_type {
	FS_SEND_NONE,
	FS_SEND_INIT, // not used
	FS_SEND_SYNC_REQ,
	FS_SEND_SYNC_ACK,
	FS_SEND_ERROR,
};

struct fs_write {
	enum fs_state_type state;

	// (TODO)
    char file_path[BUFFER_SIZE][256];
	ino_t inode_no[BUFFER_SIZE];
};

struct fs_send {
	enum fs_send_type send_flag;

	u64 state_addr;
	u32 state_rkey;
};

/* drop_caches.c */
void drop_cache_sb(struct super_block *sb);
void drop_cache_sb_all(struct super_block *sb);
void invalidate_inode_cache(struct super_block *sb);

extern void (*lock_ptr)(int, unsigned long, const char *);
extern void (*sync_ptr)(unsigned long, const char *);
extern void (*invalidate_ptr)(unsigned long, const char *);
extern void (*unlock_ptr)(int, unsigned long, const char *);

/* truncate.c */
extern void truncate_inode_pages2(struct address_space *mapping);

/* ext4/super.c */
extern int ext4_reload_metadata(struct super_block *sb);
extern int ext4_sync_super(struct super_block *sb);
extern int ext4_simple_reload(struct super_block *sb);
extern void ext4_drop_metadata(struct super_block *sb);
extern int ext4_load_metadata(struct super_block *sb);
extern unsigned long ext4_ipg(struct super_block *sb);

/* ext4/inode.c */ 
extern struct inode *ext4_iget_remote(struct super_block *sb);


#endif /* _RDMA_FS_H */