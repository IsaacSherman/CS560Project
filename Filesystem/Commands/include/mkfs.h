/*
 * mkfs.h written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#ifndef MKFS_H
#define MKFS_H





#include <stdio.h>
#include <stdlib.h>

FS_t mkfs(char *);

void write_fs(char *, FS_t);

void dsfs(FS_t *);





#endif