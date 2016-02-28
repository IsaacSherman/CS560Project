/*
 * rmdir.c written by William Halsey
 * whalsey@vols.utk.edu
 */

#include "fs.h"
#include "rmdir.h"

void rmdir(FS_t fs, char *dir_name) {
	int flag = 0;
	int i;
	
	// Check children for matching filename
	for (i=2; i<fs->cd->size+2; i++) {
		if (!strncmp(dir_name, fs->cd->children[i], 16)) {
			flag = 1;
			break;
		}
	}

	// Check flag for success
	if (flag) {
		// Make sure not a file
		if (fs->cd->children[i]->itype == F) {
			printf("\nrmdir: Cannot delete file\n");
		
		// Make sure is empty
		} else if (fs->cd->children[i]->size > 0) {
			printf("\nrmdir: Directory not empty\n");
		} else {
			fs->cd->size--;
			// Allocate new children array
			Inode_t **newTemp = (Inode_t **)malloc((fs->cd->size+2)*sizeof(Inode_t *));
		
			// Catch the old child array
			Inode_t **oldTemp = fs->cd->children;

			// Copy old pointers to new array
			for (int j=0; j<fs->cd->size+2; j++) {
				if (j<i) {
					newTemp[j] = oldTemp[j];
				}
				if (j>=i) {
					newTemp[j] = oldTemp[j+1];
				}
			}
		fs->cd->children = newTemp;
		free(oldTemp);
		}

		// Set childrent to new array


		// Free old array


	} else {
		printf("\nrmdir: Directory does not exist\n");
	}

	return;
}
