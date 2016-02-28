/*
 * fs.h written by William Halsey
 * whalsey@vols.utk.edu
 *
 * TODO
 *		- 
 */

#ifndef FS_H
#define FS_H





#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define F 	1						// Used to identify Files
#define D 	0						// Used to identify Directories
#define DEFAULT_FS_SIZE 100*1024*1024	// 100 MB
#define DEFAULT_PAGE_SIZE 4*1024		// 4 KB

// For debugging
#define VERBOSE true

typedef enum Level_tag {
	DIRECT, 
	ONE_INDIRECT,
	TWO_INDIRECT
} Level_e;

// Data structure definition for an inode
typedef struct Inode_tag {
	char *name;				// Text name of the file/directory
	int tag;				// Numerical identifier of file/directory
	char itype;				// Type - file/directory
	int size;				// Files: size in bytes; Directories: number of children
	Level_e levels;			// Levels of indirection to data
	struct Inode_tag **children;	// Pointer to array of child directories and files 	[0] - points to parent
									//													[1] - points to itself
} Inode_t;

// Data structure definition for the file system or FS
typedef struct FS_tag {
	int fs_size;		// Size of the file system in bytes
	int page_size;		// Page size of the file system in bytes
	int header_size;	// Header size of file system metadata - includes inode information (also can be considered as the offset in fs to get to the data)
	int num_inodes;		// Total number of inodes present in the FS (will be at least 1)
	char *free_list;	// Boolean list of free FS pages
	Inode_t *root;		// Pointer to the root directory in the FS
	Inode_t *cd;		// Pointer to the current directory in the FS
} FS_t;

void write_fs(char *, FS_t);






#endif
