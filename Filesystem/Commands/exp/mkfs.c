/*
 * mkfs.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "mkfs.h"
#include <stdio.h>
#include <stdlib.h>

/*
 *
 */
FS_t mkfs() {
	return create_fs();
}


/*
 *
 */
FS_t create_fs() {
	FS_t fs;
	fs.fs_size = DEFAULT_FS_SIZE;
	fs.page_size = DEFAULT_PAGE_SIZE;
	fs.num_inodes = 0;
	fs.free_list = NULL;
	fs.root = create_inode("/", ++fs.num_inodes);
	
	return fs;
}


/*
 *
 */
Inode_t *create_inode(char *name, int tag) {
	Inode_t *node = (Inode_t *)malloc(sizeof(Inode_t));
	
	//node.name = (char *)malloc(20*sizeof(char));
	node->name = name;
	node->tag = tag;
	node->itype = D;
	node->size = 0;
	node->levels = DIRECT;
	
	return node;
}


/*
 *
 */
FS_t open_fs(char *fs_name) {
	FS_t fs;
	return fs;
}