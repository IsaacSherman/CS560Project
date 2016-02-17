/*
 * fs.h written by William Halsey
 * whalsey@vols.utk.edu
 *
 * TODO
 *		- 
 */

#ifndef FS_H
#define FS_H





#include <stdbool.h>

#define F true
#define D false
#define DEFAULT_FS_SIZE 104857600	// 100 MB
#define DEFAULT_PAGE_SIZE 4096		// 4 KB

typedef enum Level_tag {
	DIRECT, 
	ONE_INDIRECT,
	TWO_INDIRECT
} Level_e;

// Data structure definition for an inode
typedef struct Inode_tag {
	char *name;				// Text name of the file/directory
	int tag;				// Numerical identifier of file/directory
	bool itype;				// Type - file/directory
	int size;				// Size in bytes
	Level_e levels;			// Levels of indirection to data
	struct Inode_tag **children;	// Pointer to array of child directories and files 	[0] - points to itself
									//													[1] - points to its parent
} Inode_t;

// Data structure definition for the file system or FS
typedef struct FS_tag {
	int fs_size;		// Size of the file system in bytes
	int page_size;		// Page size of the file system in bytes
	int header_size;	// Header size of file system metadata - includes inode information
	int num_inodes;		// Total number of inodes present in the FS (will be at least 1)
	bool *free_list;	// Boolean list of free FS pages
	Inode_t *root;		// Pointer to the root directory in the FS
} FS_t;





#endif