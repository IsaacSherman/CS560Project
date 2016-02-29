/*
 * read.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "read.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly


/*
 *
 */
void read(FS_t fs, int fd, int size) {
	if (index < FD_SIZE) {
		if (offset < fs->fd[index].file->size + fs->fd[index].offset)
			fs->fd[index].offset += offset;
	}
	
	return;
}