#include "fs.h"
#include "mkfs.h"
#include "mkdir.h"
#include "tree.h"

// unit tests for functions in mkfs.c
int main(int argc, char *argv[]) {
	// create an initial file system
	char *name = "disk.disk";
	FS_t test_fs = mkfs(name);
	
	tree(test_fs, true, true);
	
	dsfs(&test_fs);
	
	printf("done freeing\n");
	
	printf("test_fs.fs_size: %d\ntest_fs.page_size: %d\ntest_fs.num_inodes: %d\n", test_fs.fs_size, test_fs.page_size, test_fs.num_inodes);
	printf("test_fs.root: %x\n\n", (int)test_fs.root);
	
	
	
	test_fs = mkfs(name);
	
	// reopen the fs from file
	tree(test_fs, true, true);

	dsfs(&test_fs);
	
	printf("done freeing\n");
	
	printf("test_fs.fs_size: %d\ntest_fs.page_size: %d\ntest_fs.num_inodes: %d\n", test_fs.fs_size, test_fs.page_size, test_fs.num_inodes);
	printf("test_fs.root: %x\n\n", (int)test_fs.root);
	
	return 0;
}
