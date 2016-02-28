/*
 * import.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "import.h"



void import(FS_t fs, char *dir_name) {
	int flag = 1;
	
	// Check to see if name already exists
	for (int i=2; i<fs->cd->size+2; i++) {
		if (strncmp(dir_name, fs->cd->children[i]->name, 16))
			flag = 0;
	}

	// If the name does not exist in CD
	if (flag) {
		
	} else {
		printf("\nimport: Name already exists in this directory\n");
	}

	return;
}
