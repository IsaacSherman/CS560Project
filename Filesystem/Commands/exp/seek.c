/*
 * seek.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "seek.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly


/*
 *
 */
void seek(FS_t fs, int index, int offset) {
	if (index < FD_SIZE && fs->fd[index].active == 1) {
		if (offset + fs->fd[index].offset < fs->fd[index].file->size)
			fs->fd[index].offset += offset;
		else
			printf("\nseek: Offset is too large\n");
	} else
		printf("\nseek: Invalid file descriptor\n");
	
	return;
}