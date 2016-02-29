/*
 * open.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "open.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly
Inode_t *create_open(char *dir_name, int tag);

/*
 *
 */
int open(FS_t fs, char *dir_name, char *mode) {
	int flag1 = 0;
	int flag2 = 0;
	int fd=-1;
	int i, j;
	
	if (VERBOSE) printf("open: checking for filename\n");

	// Check children for matching filename
	for (i=2; i<fs->cd->size+2; i++) {
		if (!strncmp(dir_name, fs->cd->children[i]->name, 16)) {
			flag1 = 1;
			break;
		}
	}
	
	if (VERBOSE && !flag1) printf("open: filename doesn't exist\n");

	if (flag1 && fs->cd->children[i]->itype == F) {
		for (j=0; j<FD_SIZE; j++) {
			if (fs->fd[j].active == 0) {
				flag2 = 1;
				break;
			}
		}
		
		if (flag2) {
			fs->fd[j].active = 1;
			fs->fd[j].file = fs->cd->children[i];
			fs->fd[j].offset = 0;
			fd = j;
			printf("SUCCESS, ");
			
			if (!strcmp(mode, "r")) {
				fs->fd[j].mode = R;
			} else {
				fs->fd[j].mode = W;
			}
		}
	// If no matching filename is found then create an inode in CD
	} else if (!strcmp(mode, "w") && strlen(dir_name)<15) {
		if (VERBOSE) printf ("open: file does not exist but we'll create a new one\n");
		int freePages=-1;

		// Make sure enough free pages
		for (int i=0; i<(fs->fs_size/fs->page_size); i++) {
			if (fs->free_list[i] == 0) {
				freePages = i;
				flag2 = 1;
				break;
			}
		}
		
		// If free inode
		if (fs->num_inodes+1<(fs->fs_size/fs->page_size) && flag2) {
			// mark page as used
			flag2 = 0;

			for (j=0; j<FD_SIZE; j++) {
				if (fs->fd[j].active == 0) {
					flag2 = 1;
					break;
				}
			}

			fs->free_list[freePages]=1;	
		
			// increase count of cd size and number inodes
			fs->num_inodes++;
			fs->cd->size++;
			
			// reallocat cd->children
			fs->cd->children = realloc((void *)fs->cd->children, (fs->cd->size+2)*sizeof(Inode_t *));
			
			// create new node
			fs->cd->children[fs->cd->size+1] = create_open(dir_name, fs->num_inodes);
			fs->cd->children[fs->cd->size+1]->children[0] = fs->cd;
		
			// create disk offset to data
			fs->cd->children[fs->cd->size+1]->children[1] = (Inode_t *)(freePages*fs->page_size+fs->header_size);
			if (VERBOSE) printf("open: offset of new file is %d\n", (int)fs->cd->children[fs->cd->size+1]->children[1]);

			if (flag2) {
                fs->fd[j].active = 1;
                fs->fd[j].file = fs->cd->children[fs->cd->size+1];
                fs->fd[j].offset = 0;
                fd = j;
                printf("SUCCESS, ");
                fs->fd[j].mode = W;
            }
		}
	}
	
	return fd;
}


Inode_t *create_open(char *dir_name, int tag) {
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
