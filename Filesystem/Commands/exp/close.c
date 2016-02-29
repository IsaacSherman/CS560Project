/*
 * close.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "close.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly


/*
 *
 */
void close(FS_t fs, int index) {
	if (index < FD_SIZE)
		fs->fd[index].active = 0;
	
	return;
}