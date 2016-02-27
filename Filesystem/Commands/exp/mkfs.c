/*
 * mkfs.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "mkfs.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Users will never call these directly

// Used by mkfs()
FS_t create_fs(FILE *);
Inode_t *create_root(int);

FS_t open_fs(FILE *);
Inode_t *reconstruct_tree(FILE *);

// Used by write_fs()
void write_inode(FILE *, Inode_t *);

// Used by dsfs()
void destroy_inode(Inode_t *, bool);


/*
 *
 */
FS_t mkfs(char *diskName) {
	FS_t fs;
	FILE *f;
	
	if (!fopen(diskName, "r")) {
		if(VERBOSE) fprintf(stderr, "\nmkfs: No Existing File System by that name\n");
		
		f = fopen(diskName, "w");
		fs = create_fs(f);
	} else {
		if(VERBOSE) fprintf(stderr, "\nmkfs: Attempting to open existing FS\n");
		f = fopen(diskName, "r");
		fs = open_fs(f);
	}
	
	fclose(f);
	
	return fs;
}


/*
 *
 */
FS_t create_fs(FILE *f) {
	FS_t fs;
	
	// Initialize the FS
	fs.fs_size = DEFAULT_FS_SIZE;
	fs.page_size = DEFAULT_PAGE_SIZE;
	fs.header_size = (4*sizeof(int)) + (fs.fs_size/fs.page_size*sizeof(char)) + (fs.fs_size/fs.page_size*(18*sizeof(char)+3*sizeof(int)));
	fs.num_inodes = 0;
	fs.free_list = (char *)calloc(1,fs.fs_size/fs.page_size*sizeof(char));
	fs.root = create_root(++fs.num_inodes);
	fs.cd = fs.root;
		
	return fs;
}


/*
 *
 */
Inode_t *create_root(int tag) {
	Inode_t *node = (Inode_t *)malloc(sizeof(Inode_t));
	
	node->name = (char *)malloc(16*sizeof(char));
	node->name[0] = '/';
	node->name[1] = '\0';
	node->tag = tag;
	node->itype = D;
	node->size = 0;
	node->levels = DIRECT;
	node->children = (Inode_t**)malloc(2*sizeof(Inode_t *));
	node->children[0] = node;
	node->children[1] = node;
	
	return node;
}


/*
 *
 */
FS_t open_fs(FILE *f) {
	FS_t fs;
	int parent;
	Inode_t *hold;
	Inode_t *search;
	
	// Read in FS data
	if(VERBOSE) fprintf(stderr, "\nopen_fs: Attempting to read in FS header\n");
	fread(&fs.fs_size, sizeof(int), 1, f);
	fread(&fs.page_size, sizeof(int), 1, f);
	fread(&fs.header_size, sizeof(int), 1, f);
	fread(&fs.num_inodes, sizeof(int), 1, f);
	fs.free_list = (char *)calloc(1,fs.fs_size/fs.page_size*sizeof(char));
	fread(fs.free_list, sizeof(char), fs.fs_size/fs.page_size, f);
	
	// Reconstruct directory tree
	if(VERBOSE) fprintf(stderr, "\nopen_fs: Attempting to reconstruct tree\n"); 
	fs.root = reconstruct_tree(f);
	fs.root->children[0] = fs.root;
	fs.cd = fs.root;
	
	return fs;
}


/*
 *
 */
Inode_t *reconstruct_tree(FILE *f) {
	Inode_t *node = (Inode_t *)malloc(sizeof(Inode_t));
	node->name = (char *)malloc(16*sizeof(char));
	long int data;
	
	if(VERBOSE) fprintf(stderr, "\nreconstruct_tree: Attempting to read in Inode info\n");
	fread(node->name, sizeof(char), 16, f);
	node->name[15] = '\0';
	fread(&node->tag, sizeof(int), 1, f);
	fread(&node->itype, sizeof(char), 1, f);
	fread(&node->size, sizeof(int), 1, f);
	fread(&node->levels, sizeof(char), 1, f);
	fread((int *)&data, sizeof(int), 1, f);
	
	if(VERBOSE) fprintf(stderr, "\nreconstruct_tree: size = %d\n", node->size);
	
	if(VERBOSE) fprintf(stderr, "\nreconstruct_tree: At switch statement\n");
	switch (node->itype) {
		case(F): 
			if(VERBOSE) fprintf(stderr, "\nreconstruct_tree: case(F)\n");
			node->children = (Inode_t **)malloc(2*sizeof(Inode_t *));
			node->children[1] = (Inode_t *)data;
			break;
		case(D):
			if(VERBOSE) fprintf(stderr, "\nreconstruct_tree: case(D)\n");
			node->children = (Inode_t **)malloc((node->size+2)*sizeof(Inode_t *));
			node->children[1] = node;
			
			// For each child of directory
			for (int i=2; i<node->size+2; i++) {
				node->children[i] = reconstruct_tree(f);
				node->children[i]->children[0] = node;
			}
			break;
		default: 
			break;
	}
	
	return node;
}


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


/*
 *
 */
void dsfs(FS_t *fs) {
	free(fs->free_list);
	if (fs->num_inodes)
		destroy_inode(fs->root, true);
	
	fs->fs_size = -1;
	fs->page_size = -1;
	fs->header_size = -1;
	fs->num_inodes = -1;
	fs->free_list = NULL;
	fs->root = NULL;
	fs->cd = NULL;
	
	return;
}


/*
 *
 */
void destroy_inode(Inode_t *inode, bool recursive) {
	// If a file or a directory with no children - simple case
	if ((inode->itype == D && inode->size == 0) || inode->itype == F) {
		free(inode->name);
		free(inode->children);
		free(inode);
	
	// If a - recursive case
	} else if (inode->itype == D && inode->size > 0 && recursive) {
		// TODO
		for (int i=2; i<inode->size; i++) {
			destroy_inode(inode->children[i], true);
		}
		
	// If - error case
	} else {
		printf("Error: deleting a non-empty directory!\n");
	}
	
	return;
}