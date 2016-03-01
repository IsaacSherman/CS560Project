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
	if (index >= 0 && index < FD_SIZE && fs->fd[index].active == 1 && fs->fd[index].mode == W) {
		if (VERBOSE) printf("\nwrite: I have to write something!\n");
	int size = strlen(string);
		if (size + fs->fd[index].offset >= fs->page_size)
			size = fs->page_size - fs->fd[index].offset;
		
		if (VERBOSE) printf("\nwrite: opening fs\n");
		FILE *f = fopen(fs->path, "r+");

		if (VERBOSE) printf("\nwrite: seeking\n");
		fseek(f, (long)fs->fd[index].file->children[1] + (long)fs->fd[index].offset, SEEK_SET);
		
		if (VERBOSE) printf("\nfile size is %d before, and string size is %d\n", fs->fd[index].file->size, size);
		fs->fd[index].file->size = (size + fs->fd[index].offset > fs->fd[index].file->size) ? (size+fs->fd[index].offset) : fs->fd[index].file->size;
		if (VERBOSE) printf("\nfile size is %d after\n", fs->fd[index].file->size);
		fs->fd[index].offset += size;

		if (VERBOSE) printf("\nwrite: writing\n");
		fwrite(string, size, sizeof(char), f);
		
		fclose(f);
		
	} else
		printf("\nread: Invalid file descriptor request\n");
	
	return;
}
