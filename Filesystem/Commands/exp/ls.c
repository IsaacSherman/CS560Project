/*
 * ls.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "ls.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly


/*
 *
 */
void ls(FS_t fs) {
	for (int i=2; i<fs->cd->size+2; i++) {
		printf("%s\n", fs->cd->children[i]->name);
	}
	return;
}