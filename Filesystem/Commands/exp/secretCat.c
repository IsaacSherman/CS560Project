/*
 * secretCat.c written by William Halsey
 * whalsey@vols.utk.edu
 *
 */

#include "fs.h"
#include "secretCat.h"

// ========== PROTOTYPES OF "PRIVATE" FUNCTIONS ==========
// Never call these directly


/*
 *
 */
void secretCat(FS_t fs, char *dir_name, char *dest_name) {
	int flag = 0;
	int i;
	
	// Check children for matching filename
	for (i=2; i<fs->cd->size+2; i++) {
		if (!strncmp(dir_name, fs->cd->children[i]->name, 16)) {
			flag = 1;
			break;
		}
	}
	
	if (flag && fs->cd->children[i]->itype == F) {
		int size; 
		FILE *f;
		FILE *fout;
		char *DATA;
		
		// TODO - FIX THIS!!
		if (fs->cd->children[i]->levels == DIRECT) {
			size = fs->cd->children[i]->size;
			f = fopen(fs->path, "r");
			fout = fopen(dest_name, "w");
			fseek(f, (long)fs->cd->children[i]->children[1], SEEK_SET);
			
			DATA = (char *)malloc((size+1)*sizeof(char));
			fread(DATA, sizeof(char), size, f);
			DATA[size] = '\0';
			
			fprintf(fout, "%s", DATA);
			
			free(DATA);
			fclose(f);
			fclose(fout);
		}
	}
	
	return;
}