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
FS_t mkfs(char *diskName) {
	FS_t fs;
	FILE *f;
	
	// TODO
	//	check for a file with "filename"
	
	if (!fopen(diskName, "r")) {
		printf("\n\nNo Existing File System by that name\n\n");
		fs = create_fs();
		f = fopen(diskName, "w");
		fwrite(&fs.fs_size, sizeof(int), 1, f);
		fwrite(&fs.page_size, sizeof(int), 1, f);
		fwrite(&fs.header_size, sizeof(int), 1, f);
		fwrite(&fs.num_inodes, sizeof(int), 1, f);
	} else {
		f = fopen(diskName, "r");
		fs = open_fs(f);
	}
	
	fclose(f);
	
	return fs;
}


/*
 *
 */
void dsfs(FS_t *fs) {
	free(fs->free_list);
	if (fs->num_inodes)
		destroy_inode(fs->root, true);
	
	fs->fs_size = -1;
	fs->page_size = -1;
	fs->header_size = -1;
	fs->num_inodes = -1;
	fs->free_list = NULL;
	fs->root = NULL;
	
	return;
}


/*
 *
 */
FS_t create_fs() {
	FS_t fs;
	
	fs.fs_size = DEFAULT_FS_SIZE;
	fs.page_size = DEFAULT_PAGE_SIZE;
	fs.header_size = sizeof(FS_t) + sizeof(fs.fs_size/fs.page_size*sizeof(char)) + fs.fs_size/fs.page_size*sizeof(Inode_t);
	fs.num_inodes = 0;
	fs.free_list = (char *)calloc(1,fs.fs_size/fs.page_size*sizeof(char));
	fs.root = create_inode("/", fs.num_inodes++);
	
	return fs;
}


/*
 *
 */
FS_t open_fs(FILE *f) {
	FS_t fs;
	
	fread(&fs.fs_size, sizeof(int), 1, f);
	fread(&fs.page_size, sizeof(int), 1, f);
	fread(&fs.header_size, sizeof(int), 1, f);
	fread(&fs.num_inodes, sizeof(int), 1, f);
	
	return fs;
}


/*
 *
 */
Inode_t *create_inode(char *name, int tag) {
	Inode_t *node = (Inode_t *)malloc(sizeof(Inode_t));
	
	node->name = name;
	node->tag = tag;
	node->itype = D;
	node->size = 0;
	node->levels = DIRECT;
	node->children = NULL;
	
	return node;
}


/*
 *
 */
void destroy_inode(Inode_t *inode, bool recursive) {
	// If a file or a directory with no children - simple case
	if ((inode->itype == D && inode->size == 0) || inode->itype == F) {
		free(inode);
	
	// If a - recursive case
	} else if (inode->itype == D && inode->size > 0 && recursive) {
		// TODO
		
	// If - error case
	} else {
		printf("Error: deleting a non-empty directory!\n");
	}
	
	return;
}