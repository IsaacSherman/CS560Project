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
	if (index < FD_SIZE && fs->fd[index].active == 1 && fs->fd[index].mode == R) {
		if (size + fs->fd[index].offset >= fs->fd[index].file->size)
			size = fs->fd[index].file->size - fs->fd[index].offset;
			
		char *DATA = (char *)malloc((size+1)*sizeof(char));
		FILE *f = fopen(fs->path, "r");
		fseek(f, (long)fs->fd[index].file->children[1] + (long)fs->fd[index].offset, SEEK_SET);
		
		fs->fd[index].offset += size;
		
		fread(data, size, sizeof(char), f);
		data[size] = '\0';
		
		printf("%s", data);
		
		free(data);
		fclose(f);
		
	} else
		printf("\nread: Invalid file descriptor request\n");
	
	return;
}