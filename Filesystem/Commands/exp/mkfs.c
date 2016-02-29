/*
 * mkfs->c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "mkfs.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly

// Used by mkfs()
FS_t create_fs(FILE *, char *);
Inode_t *create_root(int);

FS_t open_fs(FILE *, char *);
Inode_t *reconstruct_tree(FILE *);

// Used by dsfs()
void destroy_inode(Inode_t *, bool);


/*
 *
 */
FS_t mkfs(char *diskName) {
	FS_t fs;
	FILE *f;
	
	if (!fopen(diskName, "r")) {
		if(VERBOSE) fprintf(stdout, "\nmkfs: No Existing File System by that name\n");
		f = fopen(diskName, "w");
		if(f == 0) perror("fopen");
		fs = create_fs(f, diskName);
		write_fs(fs);
	} else {
		if(VERBOSE) fprintf(stdout, "\nmkfs: Attempting to open existing FS\n");
		f = fopen(diskName, "r");
		if(f == 0) perror("fopen");
		fs = open_fs(f, diskName);
	}
	
	fclose(f);
	
	return fs;
}


/*
 *
 */
FS_t create_fs(FILE *f, char * diskName) {
	FS_t fs = mallocFS();
	printf("Entering create_fs\n");
	// Initialize the FS
	fs->fs_size = DEFAULT_FS_SIZE;
	fs->page_size = DEFAULT_PAGE_SIZE;
	fs->header_size = (4*sizeof(int)) + (fs->fs_size/fs->page_size*sizeof(char)) + (fs->fs_size/fs->page_size*(18*sizeof(char)+3*sizeof(int)));
	fs->num_inodes = 0;
	fs->free_list = (char *)calloc(1,fs->fs_size/fs->page_size*sizeof(char));
	fs->root = create_root(++fs->num_inodes);
	fs->cd = fs->root;
	fs->path = strdup(diskName);
	printf("Exiting create_fs\n");
	
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
FS_t open_fs(FILE *f, char * diskName) {
	FS_t fs = mallocFS();
	int parent;
	Inode_t *hold;
	Inode_t *search;
	
	// Read in FS data
	if(VERBOSE) fprintf(stdout, "\nopen_fs: Attempting to read in FS header\n");
	fread(&(fs->fs_size), sizeof(int), 1, f);
	fread(&(fs->page_size), sizeof(int), 1, f);
	fread(&(fs->header_size), sizeof(int), 1, f);
	fread(&(fs->num_inodes), sizeof(int), 1, f);
	fs->free_list = (char *)calloc(1,fs->fs_size/fs->page_size*sizeof(char));
	fread(fs->free_list, sizeof(char), fs->fs_size/fs->page_size, f);
	
	// Reconstruct directory tree
	if(VERBOSE) fprintf(stdout, "\nopen_fs: Attempting to reconstruct tree\n"); 
	fs->root = reconstruct_tree(f);
	fs->root->children[0] = fs->root;
	fs->cd = fs->root;
	fs->path = strdup(diskName);
	return fs;
}


/*
 *
 */
Inode_t *reconstruct_tree(FILE *f) {
	Inode_t *node = (Inode_t *)malloc(sizeof(Inode_t));
	node->name = (char *)malloc(16*sizeof(char));
	int data;
	int test = 0;
	
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: Attempting to read in Inode info\n");
	test = fread(node->name, sizeof(char), 16, f);
	node->name[15] = '\0';
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: %d items read\n", test);
	test = fread(&node->tag, sizeof(int), 1, f);
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: %d items read\n", test);
	test = fread(&node->itype, sizeof(char), 1, f);
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: %d items read\n", test);
	test = fread(&node->size, sizeof(int), 1, f);
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: %d items read\n", test);
	test = fread(&node->levels, sizeof(char), 1, f);
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: %d items read\n", test);
	test = fread(&data, sizeof(int), 1, f);
	
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: %d items read\n", test);
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: name = %s\n", node->name);
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: size = %d\n", node->size);
	
	if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: At switch statement\n");
	switch (node->itype) {
		case(F): 
			if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: case(F)\n");
			node->children = (Inode_t **)malloc(2*sizeof(Inode_t *));
			node->children[1] = (Inode_t *)data;
			if(VERBOSE) printf("reconstructTree: Disk offset = %d", (int)node->children[1]);
			break;
		case(D):
			if(VERBOSE) fprintf(stdout, "\nreconstruct_tree: case(D)\n");
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
void dsfs(FS_t fs) {
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
		if (VERBOSE) fprintf(stdout, "\ndestroy_inode: deleting directory with no children or file\n");
		free(inode->name);
		free(inode->children);
		free(inode);
	
	// If a - recursive case
	} else if (inode->itype == D && inode->size > 0 && recursive) {
		if (VERBOSE) fprintf(stdout, "\ndestroy_inode: deleting directory with children\n");
		for (int i=2; i<inode->size+2; i++) {
			destroy_inode(inode->children[i], true);
		}
		
	// If - error case
	} else {
		printf("destroy_inode: Can not delete a non-empty directory!\n");
	}
	
	return;
}