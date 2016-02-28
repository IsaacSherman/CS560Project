#include "fs.h"


// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly

// Used by write_fs()
void write_inode(FILE *, Inode_t *);

FS_t mallocFS(){
FS_t ret = malloc(sizeof(struct FS_tag));
ret->root = malloc(sizeof(struct Inode_tag));
ret->cd = ret->root;
return ret;
}

/*
 *
 */
void write_fs(FS_t fs) {
	FILE *f;
	char * diskName = fs->path;
	int test; 
	
	f = fopen(diskName, "r+");
	
	// Write initial FS to file
	test = fwrite(&fs->fs_size, sizeof(int), 1, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_fs: %d items written\n", test);
	test = fwrite(&fs->page_size, sizeof(int), 1, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_fs: %d items written\n", test);
	test = fwrite(&fs->header_size, sizeof(int), 1, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_fs: %d items written\n", test);
	test = fwrite(&fs->num_inodes, sizeof(int), 1, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_fs: %d items written\n", test);
	test = fwrite(fs->free_list, sizeof(char), fs->fs_size/fs->page_size, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_fs: %d items written\n", test);
	
	// Write out inodes
	write_inode(f, fs->root);
	
	fclose(f);
	
	return;
}


/*
 *
 */
void write_inode(FILE *f, Inode_t *inode) {
	int test;
	test = fwrite(inode->name, sizeof(char), 16, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_inode: %d items written\n", test);
	test = fwrite(&inode->tag, sizeof(int), 1, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_inode: %d items written\n", test);
	test = fwrite(&inode->itype, sizeof(char), 1, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_inode: %d items written\n", test);
	test = fwrite(&inode->size, sizeof(int), 1, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_inode: %d items written\n", test);
	test = fwrite(&inode->levels, sizeof(char), 1, f);
	if(VERBOSE) fprintf(stderr, "\nwrite_inode: %d items written\n", test);
	if (inode->itype == F) {
		test = fwrite(inode->children[1], sizeof(int), 1, f);
		if(VERBOSE) fprintf(stderr, "\nwrite_inode: %d items written\n", test);
	} else {
		test = putw(-1, f);
	}
	
	if (inode->itype == D) {
		for (int i=2; i<inode->size+2; i++) {
			write_inode(f, inode->children[i]);
		}
	}
	return;
}


