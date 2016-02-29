/*
 * import.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "import.h"

Inode_t *create_inode(char *dir_name, int tag);

void import(FS_t fs, char *dir_name) {
	// first check to make sure that there are additional inode spaces
	if (fs->num_inodes>=(fs->fs_size/fs->page_size)) {
		printf("\nmkdir: Not enough metadata space.\n");
		return;
	}
	
	int flag1 = 1;
	int flag2 = 0;
	int count = 0;
	int size, numPages, remain;
	int *freePages;
	char *DATA;
	
	FILE *f;
	
	// Check to see if name already exists
	for (int i=2; i<fs->cd->size+2; i++) {
		if (!strncmp(dir_name, fs->cd->children[i]->name, 16))
			flag1 = 0;
	}

	// If the name does not exist in CD
	if (flag1) {
		// Make sure filename is short
		if (strlen(dir_name) < 16) {
			// Figure out size of file to import and rewind to beginning of file
			f = fopen(dir_name, "r");
			fseek(f, 0L, SEEK_END);
			size = ftell(f);
			fseek(fp, 0L, SEEK_SET);
			
			// Find total number of pages file will reside in and number of remainder bytes
			numPages = size/fs->page_size;
			remainder = size%fs->page_size;
			if (remainder)
				numPages++;
			
			// TODO - FIX THIS!!
			if (numPages>1) {
				printf("\nimport: Fatal error! indirection not supported yet\n");
				return;
			}
			
			freePages = (int *)malloc(numPages*sizeof(int));
			
			// Make sure enough free pages
			for (int i=0; i<(fs->fs_size/fs->page_size); i++) {
				if (fs->free_list[i] == 0) {
					freePages[count] = i;
					count++;
				}
				
				if (count >= numPages) {
					flag2 = 1;
					break;
				}
			}
			
			// If enough free pages
			if (flag2) {
				for (int i=0; i<count; i++) {
					fs->free_list[freePages[count]]=1;
				}
				// Create new file inode
				DATA = (char *)malloc(size*sizeof(char));
				fread(DATA, sizeof(char), size, f);
				
				FILE *f2 = fopen(fs->path, "r+");
				
				fs->num_inodes++;
				fs->cd->size++;
				fs->cd->children = realloc((void *)fs->cd->children, (fs->cd->size+2)*sizeof(Inode_t *));
				fs->cd->children[fs->cd->size+1] = create_inode(dir_name, fs->num_inodes);
				fs->cd->children[fs->cd->size+1]->children[0] = fs->cd;
				
				fs->cd->children[fs->cd->size+1]->children[1] = (Inode_t *)(freePages[0]*fs->page_size+fs->header_size);
				fs->cd->children[fs->cd->size+1]->size = size;
				// Adjust levels if necessary
				if (numPages>1)
					fs->cd->children[fs->cd->size+1]->levels = ONE_INDIRECT;
					
				// Try to write this data
				fseek(f2, (long)fs->cd->children[fs->cd->size+1]->children[1], SEEK_SET);
				fwrite(DATA, sizeof(char), size, f2);
				
				fclose(f2);
			}
			
			fclose(f);
		} else {
			printf("\nimport: Filename is too long. Limit 15 characters.\n");
		}
	} else {
		printf("\nimport: Name already exists in this directory\n");
	}

	return;
}

Inode_t *create_inode(char *dir_name, int tag) {
	Inode_t *node = (Inode_t *)malloc(sizeof(Inode_t));
	
	node->name = (char *)malloc(16*sizeof(char));
	strncpy(node->name, dir_name, 15);
	node->name[15] = '\0';
	
	node->tag = tag;
	node->itype = F;
	node->size = 0;
	node->levels = DIRECT;
	node->children = (Inode_t **)malloc(2*sizeof(Inode_t *));
	
	return node;
}