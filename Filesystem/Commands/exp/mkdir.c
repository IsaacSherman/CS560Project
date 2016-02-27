/*
 * mkdir.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "mkdir.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly
Inode_t *create_inode(char *, int);

/*
 *
 */
void mkdir(FS_t fs, char *dir_name) {
	// first check to make sure that there are additional inode spaces
	if (fs.num_inodes<(fs.fs_size/fs.page_size)) {
		
		// check that dir_name is not too long
		if (strlen(dir_name) < 16) {
			fs.num_inodes++;
			fs.cd->size++;
			realloc((void *)fs.cd->children, fs.cd->size*sizeof(Inode_t *));
			fs.cd->children[fs.cd->size+1] = create_inode(dir_name, fs.num_inodes);
			fs.cd->children[fs.cd->size+1]->children[0] = fs.cd;
			
		} else {
			printf("\nmkdir: Directory name is too long. Limit 15 characters.\n");
		}
		
	} else {
		printf("\nmkdir: Not enough metadata space.\n");
	}
	
	return;
}


Inode_t *create_inode(char *dir_name, int tag) {
	Inode_t *node = (Inode_t *)malloc(sizeof(Inode_t));
	
	node->name = (char *)malloc(16*sizeof(char));
	strncpy(node->name, dir_name, 15);
	node->name[15] = '\0';
	
	node->tag = tag;
	node->itype = D;
	node->size = 0;
	node->levels = DIRECT;
	node->children = (Inode_t **)malloc(2*sizeof(Inode_t *));
	node->children[1] = node;
	
	return node;
}