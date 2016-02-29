/*
 * open.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "open.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly


/*
 *
 */
int open(FS_t fs, char *dir_name, char *mode) {
	int flag1 = 0;
	int flag2 = 0;
	int fd=-1;
	int i, j;
	
	// Check children for matching filename
	for (i=2; i<fs->cd->size+2; i++) {
		if (!strncmp(dir_name, fs->cd->children[i]->name, 16)) {
			flag1 = 1;
			break;
		}
	}
	
	if (flag1 && fs->cd->children[i]->itype == F) {
		for (j=0; j<FD_SIZE; j++) {
			if (fs->fd[j].active == 0) {
				flag2 = 1;
				break;
			}
		}
		
		if (flag2) {
			fs->fd[j].active = 1;
			fs->fd[j].file = fs->cd->children[i];
			fs->fd[j].offset = 0;
			fd = j;
			printf("SUCCESS, ");
			
			if (!strcmp(mode, "r")) {
				fs->fd[j].mode = R;
			} else {
				fs->fd[j].mode = W;
			}
		}
	}
	
	return fd;
}