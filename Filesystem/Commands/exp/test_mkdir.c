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
	mkdir(&test_fs, "whalsey");
	mkdir(&test_fs, "isherman");

	tree(test_fs, true, true);
	tree(test_fs, false, true);
	
	write_fs(name, test_fs);
	dsfs(&test_fs);
	
	printf("done freeing\n");
	
	// reopen the fs from file
	test_fs = mkfs(name);
	
	tree(test_fs, true, true);

	dsfs(&test_fs);
	
	printf("done freeing\n");
	
	return 0;
}
