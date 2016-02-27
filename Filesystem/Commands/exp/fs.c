#include "fs.h"


// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly

// Used by write_fs()
void write_inode(FILE *, Inode_t *);

/*
 *
 */
void write_fs(char * diskName, FS_t fs) {
	FILE *f;
	
	f = fopen(diskName, "w");
	
	// Write initial FS to file
	fwrite(&fs.fs_size, sizeof(int), 1, f);
	fwrite(&fs.page_size, sizeof(int), 1, f);
	fwrite(&fs.header_size, sizeof(int), 1, f);
	fwrite(&fs.num_inodes, sizeof(int), 1, f);
	fwrite(fs.free_list, sizeof(char), fs.fs_size/fs.page_size, f);
	
	// Write out inodes
	write_inode(f, fs.root);
	
	fclose(f);
	
	return;
}


/*
 *
 */
void write_inode(FILE *f, Inode_t *inode) {
	fwrite(inode->name, sizeof(char), 16, f);
	fwrite(&inode->tag, sizeof(int), 1, f);
	fwrite(&inode->itype, sizeof(char), 1, f);
	fwrite(&inode->size, sizeof(int), 1, f);
	fwrite(&inode->levels, sizeof(char), 1, f);
	if (inode->itype == F) {
		fwrite(inode->children[1], sizeof(int), 1, f);
	} else {
		putw(-1, f);
	}
	
	if (inode->itype == D) {
		for (int i=2; i<inode->size; i++) {
			write_inode(f, inode->children[i]);
		}
	}
	return;
}


