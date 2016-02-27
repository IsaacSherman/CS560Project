/*
 * test_mkfs.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "mkfs.h"
#include <stdio.h>
#include <stdlib.h>

// unit tests for functions in mkfs.c
int main(int argc, char *argv[]) {
	// create an initial file system
	char *name = "disk.disk";
	FS_t test_fs = mkfs(name);
	
	// write out to file and close
	
	// reopen the fs from file
	printf("test_fs.fs_size: %d\ntest_fs.page_size: %d\ntest_fs.num_inodes: %d\n", test_fs.fs_size, test_fs.page_size, test_fs.num_inodes);
	printf("test_fs.header_size: %d\n", (int)test_fs.header_size);
	printf("test_fs.free_list: %x\n", (int)test_fs.free_list);
	printf("test_fs.root: %x\n\n", (int)test_fs.root);
	
	printf("root.name: %s \nroot.tag: %d\nroot.itype: %d\nroot.size: %d\n", test_fs.root->name, test_fs.root->tag, (int)test_fs.root->itype, test_fs.root->size);
	
	printf("total number of files/directories is %d\n", test_fs.num_inodes);

	dsfs(&test_fs);
	
	printf("done freeing\n");
	
	printf("test_fs.fs_size: %d\ntest_fs.page_size: %d\ntest_fs.num_inodes: %d\n", test_fs.fs_size, test_fs.page_size, test_fs.num_inodes);
	printf("test_fs.root: %x\n\n", (int)test_fs.root);
	
	
	
	test_fs = mkfs(name);
	
	// reopen the fs from file
	printf("test_fs.fs_size: %d\ntest_fs.page_size: %d\ntest_fs.num_inodes: %d\n", test_fs.fs_size, test_fs.page_size, test_fs.num_inodes);
	printf("test_fs.header_size: %d\n", (int)test_fs.header_size);
	printf("test_fs.free_list: %x\n", (int)test_fs.free_list);
	printf("test_fs.root: %x\n\n", (int)test_fs.root);
	
	printf("root.name: %s \nroot.tag: %d\nroot.itype: %d\nroot.size: %d\n", test_fs.root->name, test_fs.root->tag, (int)test_fs.root->itype, test_fs.root->size);
	
	printf("total number of files/directories is %d\n", test_fs.num_inodes);

	dsfs(&test_fs);
	
	printf("done freeing\n");
	
	printf("test_fs.fs_size: %d\ntest_fs.page_size: %d\ntest_fs.num_inodes: %d\n", test_fs.fs_size, test_fs.page_size, test_fs.num_inodes);
	printf("test_fs.root: %x\n\n", (int)test_fs.root);
	
	return 0;
}
