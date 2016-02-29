/*
 * write.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "write.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly


/*
 *
 */
void write(FS_t fs, int index, char *string) {
	//
	if (index < FD_SIZE && fs->fd[index].active == 1 && fs->fd[index].mode == W) {
	int size = strlen(string);
		if (size + fs->fd[index].offset >= fs->page_size)
			size = fs->page_size - fs->fd[index].offset;
			
		FILE *f = fopen(fs->path, "r+");
		fseek(f, (long)fs->fd[index].file->children[1] + (long)fs->fd[index].offset, SEEK_SET);
		
		fs->fd[index].offset += size;
		
		fwrite(string, size, sizeof(char), f);
		
		fclose(f);
		
	} else
		printf("\nread: Invalid file descriptor request\n");
	
	return;
}