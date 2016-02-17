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
	FS_t test_fs = mkfs();
	
	// write out to file and close
	
	// reopen the fs from file
		
	printf("test_fs.fs_size: %d\ntest_fs.page_size: %d\ntest_fs.num_inodes: %d\n", test_fs.fs_size, test_fs.page_size, test_fs.num_inodes);
	printf("test_fs.root: %x\n\n", (int)test_fs.root);
	printf("root.name: %s\nroot.tag: %d\nroot.itype: %d\nroot.size: %d\n", test_fs.root->name, test_fs.root->tag, (int)test_fs.root->itype, test_fs.root->size);
	
	return 0;
}
