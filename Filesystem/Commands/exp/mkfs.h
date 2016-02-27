/*
 * mkfs.h written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#ifndef MKFS_H
#define MKFS_H





#include <stdio.h>

FS_t mkfs(char *);
void dsfs(FS_t *);
FS_t create_fs(FILE *);
FS_t open_fs(FILE *);
Inode_t *create_root(int);
void destroy_inode(Inode_t *, bool);
Inode_t *reconstruct_tree(FILE *);
void write_fs(char *, FS_t);





#endif