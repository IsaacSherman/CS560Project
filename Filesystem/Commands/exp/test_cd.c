#include "fs.h"
#include "mkfs.h"
#include "mkdir.h"
#include "tree.h"
#include "cd.h"

// unit tests for functions in mkfs.c
int main(int argc, char *argv[]) {
	// create an initial file system
	char *name = "disk.disk";
	FS_t test_fs = mkfs(name);
	
	tree(test_fs, true, true);
	mkdir(&test_fs, "whalsey");
	mkdir(&test_fs, "isherman");
	
	cd(&test_fs, "isherman/");
	mkdir(&test_fs, "isherman2");
	cd(&test_fs, "../whalsey/");
	mkdir(&test_fs, "whalsey2");
	tree(test_fs, true, false);
	cd(&test_fs, "/");
	mkdir(&test_fs, "awesomesauce");
	cd(&test_fs, "./awesomesauce/");
	mkdir(&test_fs, "oneLastDir");
	cd(&test_fs, "oneLastDir/");
	mkdir(&test_fs, "jkOneMore");
	cd(&test_fs, "jkOneMore/");
	mkdir(&test_fs, "lastOne");
	mkdir(&test_fs, "cosc560");

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
