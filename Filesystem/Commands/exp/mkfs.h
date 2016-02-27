/*
 * mkfs.h written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#ifndef MKFS_H
#define MKFS_H





#include <stdio.h>

FS_t mkfs(char *);

FS_t create_fs(FILE *);
Inode_t *create_root(int);

FS_t open_fs(FILE *);
Inode_t *reconstruct_tree(FILE *);

void write_fs(char *, FS_t);
void write_inode(FILE *, Inode_t *);

void dsfs(FS_t *);
void destroy_inode(Inode_t *, bool);





#endif