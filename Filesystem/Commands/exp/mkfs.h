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
FS_t create_fs();
FS_t open_fs(FILE *);
Inode_t *create_inode(char *, int);
void destroy_inode(Inode_t *, bool);





#endif