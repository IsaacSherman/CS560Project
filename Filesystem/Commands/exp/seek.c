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
	if (index < FD_SIZE) {
		if (offset < fs->fd[index].file->size)
			fs->fd[index].offset += offset;
	}
	
	return;
}