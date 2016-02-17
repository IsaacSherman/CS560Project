/*
 * mkfs.h written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#ifndef MKFS_H
#define MKFS_H





FS_t mkfs();
FS_t create_fs();
Inode_t *create_inode(char *, int);
FS_t open_fs(char *);





#endif