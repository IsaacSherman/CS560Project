//Augmentations for libfdr.a, policy wrappers etc.,
#include "jrb.h"
#include "dllist.h"
#include <string.h>
#include <stdlib.h>
#include "myLib.h"
#include <stdio.h>

/*char * 
appendChar(char * base, char add)
{
	if(base == NULL) return NULL;
	int len = strlen(base);
	char toAdd[2];
	toAdd[0] = add; toAdd[1] ='\0';
	char * ret = malloc(len+1);
	strcpy(ret, base);
	strcat(ret, toAdd);
	return ret;
}*/
	
void
addTokensToList(Dllist list, char * buffer, char * tokens)
{

	char string[1000];
	char * token;
	strcpy(string, buffer);
	token = strtok(string, tokens);
	while (token != NULL) {
		dll_append(list, new_jval_s(strdup(token)));
		token = strtok(NULL, tokens);
	}
}

void addToList(Dllist list, char * buffer)//Tokenizes buffer (by spaces and \n) then 
	//Adds it piecewise to list
{
	addTokensToList(list, buffer, " \n");
}

void addPathComponentsToList(Dllist list, char * buffer)//Tokenizes buffer (by spaces and \n) then 
	//Adds it piecewise to list
{
	addTokensToList(list, buffer, "/");
}

void freeStrList(Dllist list)//For string lists only!
{
	Dllist ptr;
	dll_traverse(ptr, list)
	{
		SafeFree(ptr->val.s);
	}
	if(list!=NULL) 	free_dllist(list);
}

void freeStrJRB(JRB tree)//Frees the keys only!
{
	JRB ptr;
	jrb_traverse(ptr, tree)
	{
		SafeFree(ptr->key.s);
	}
	if(tree!=NULL) 	jrb_free_tree(tree);
}


int 
addUniqueVToTree_str(JRB tree,char * str,  void * addV) //Returns true on successful add
{
	JRB found = jrb_find_str(tree, str);
	if(found == NULL) 
	{
		jrb_insert_str(tree, strdup(str), new_jval_v(addV));
		return true;
	}
	return false;
}

int 
addUniqueVToTree_int(JRB tree, int  key,  void * addV)//Returns true on successful add
{
	JRB found = jrb_find_int(tree, key);
	if(found == NULL)
	{
		jrb_insert_int(tree, key, new_jval_v(addV));
		return true;
	}
	return false;
}

char * 
subDirStr(char * indir, char * file)
{
	char * dir = strdup(indir);
	char * ret;
	if (dir == NULL || file == NULL) return NULL;
	int len = strlen(file)+strlen(dir)+1;
	if(dir[strlen(dir)-1] == '/'){
		//Shorten ret, since the / is already there
		dir[strlen(dir)-1] = '\0';
	}
	else  ++len;
		ret  = malloc(len);

	sprintf(ret, "%s/%s", dir, file);			
	free(dir);
	return ret;
}
 char * 
extractFileName(char * path)//returns a pointer to the last component of path; no need to free
{
	if(path == NULL) return NULL;
	int index = strlen(path);
	char * ret = &path[index];
	
	while (*ret != '/' && index >= 0)
	{
		--ret;
		--index;
	}
	if(*ret == '/') ++ret;
	return ret;
}

int
intMin(int a, int b)
{
	if (a < b) return a;
	return b;
}
