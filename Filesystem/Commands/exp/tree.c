/*
 * tree.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "tree.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly
void print_inode(Inode_t *, bool, int);


/*
 *
 */
void tree(FS_t fs, bool verbose, bool root) {
	if (verbose) {
		printf("\n========== FILE SYSTEM INFO ==========\n");
		printf("FILE SYSTEM SIZE: %d bytes\n", fs.fs_size);
		printf("PAGE SIZE: %d bytes\n", fs.page_size);
		printf("METADATA SIZE: %d bytes\n", fs.header_size);
		printf("NUMBER OF INODES: %d\n", fs.num_inodes);
	}
	
	if (root) {
		print_inode(fs.root, verbose, 0);
	} else {
		print_inode(fs.cd, verbose, 0);
	}
	return;
}


/*
 *
 */
void print_inode(Inode_t *node, bool verbose, int lvl) {
	for (int i=0; i<lvl; i++) {
		printf("|    ");
	}
	printf("+ ");
	
	printf("%s\n", node->name);
	
	if (verbose) {
		for (int i=0; i<lvl; i++) {
			printf("|    ");
		}
		printf("| ");
		printf("%s\n", (node->itype == F) ? "File" : "Dir");
		
		for (int i=0; i<lvl; i++) {
			printf("|    ");
		}
		printf("| ");
		printf("Size: %d\n", node->size);
	}
	
	for (int j=2; j<node->size; j++) {
		print_inode(node->children[j], verbose, lvl+1);
	}
	
	return;
}