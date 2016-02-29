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
void read(FS_t fs, int index, int size) {
	//
	if (index < FD_SIZE && fs->fd[index].active == 1 && fs->fd[index].mode == R) {
		if (size + fs->fd[index].offset >= fs->fd[index].file->size)
			size = fs->fd[index].file->size - fs->fd[index].offset;
			
		char *DATA = (char *)malloc((size+1)*sizeof(char));
		FILE *f = fopen(fs->path, "r");
		fseek(f, (long)fs->fd[index].file->children[1] + (long)fs->fd[index].offset, SEEK_SET);
		
		fs->fd[index].offset += size;
		
		fread(DATA, size, sizeof(char), f);
		DATA[size] = '\0';
		
		printf("%s\n", DATA);
		
		free(DATA);
		fclose(f);
		
	} else
		printf("\nread: Invalid file descriptor request\n");
	
	return;
}
