/*
 * cd.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "cd.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly


/*
 *
 */
void cd(FS_t fs, char *dir_name) {
	int flag = 1;
	int oldi = 0;
	int i;
	
	Inode_t *searcher = fs->cd;
	
	for (i=0; i<strlen(dir_name); i++) {
		// If delimiter or root
		if (dir_name[i] == '/') {
			if (VERBOSE) printf("\ncd: / at %d and fn begins at %d\n", i, oldi);	
			// Set searcher to root
			if (i == 0) {
				searcher = fs->root;
				oldi = 1;
				continue;
			}
		
			// Invalid filename length
			if (i-oldi > 15) {
				fprintf(stderr, "\ncd: Invalid filename %.*s\n", i-oldi, dir_name+i);
				return;
			}
				
			// if . or ..
			if (!strncmp(".", dir_name+oldi, i-oldi)) {
				if (VERBOSE) fprintf(stderr, "I FOUND A .\n");
				searcher = searcher->children[1];
				oldi = i+1;
				
			} else if (!strncmp("..", dir_name+oldi, i-oldi)) {
				if (VERBOSE) fprintf(stderr, "I FOUND A ..\n");
				searcher = searcher->children[0];
				oldi = i+1;
				
			} else {
				for (int j=2; j<searcher->size+2; j++) {
					if (searcher->children[j]->itype != F && !strncmp(searcher->children[j]->name, dir_name+oldi, i-oldi)) {
						searcher = searcher->children[j];
						if (VERBOSE) printf("\ncd: i found a file in the path\n");
						flag = 0;
						oldi = i+1;
						break;
					}
				}
				flag = 1-flag;
			}
		}
	}
	
	if (i != oldi) {
		// Invalid filename length
        if (i-oldi > 15) {
            fprintf(stderr, "\ncd: Invalid filename %.*s\n", i-oldi, dir_name+i);
            return;
        }
                                              
        // if . or ..
        if (!strncmp(".", dir_name+oldi, i-oldi)) {
            if (VERBOSE) fprintf(stderr, "I FOUND A .\n");
            searcher = searcher->children[1];
            oldi = i+1;
                                                                                                                                         
        } else if (!strncmp("..", dir_name+oldi, i-oldi)) {
            if (VERBOSE) fprintf(stderr, "I FOUND A ..\n");
            searcher = searcher->children[0];
            oldi = i+1;
                                                                                                                                                                                                       
        } else {
            for (int j=2; j<searcher->size+2; j++) {
                if (searcher->children[j]->itype != F && !strncmp(searcher->children[j]->name, dir_name+oldi, i-oldi)) {
                    searcher = searcher->children[j];
                    if (VERBOSE) printf("\ncd: i found a file in the path\n");
                    flag = 0;
                    oldi = i+1;
                    break;
                }
            }
            flag = 1-flag;
		}
	}

	if (flag)
		fs->cd = searcher;
	
	return;
}
