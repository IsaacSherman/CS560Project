#ifndef __ISHERMANS_LIB__
#define __ISHERMANS_LIB__

#define SafeFree(ToFree) { free(ToFree); ToFree = NULL;}//ensures it won't get freed again
#define true 1
#define false 0


#ifdef __cplusplus
extern "C"
{
#endif

extern void addToList(/*Dllist list, char * buffer*/);//Tokenizes buffer (by spaces and \n) then 
extern void freeStrList(/*Dllist list*/);//For string lists only!
extern void freeStrJRB(/*Dllist list*/);//str keys only!
extern int addUniqueVToTree_int(/*JRB tree, int  key,  void * addV*/);
extern int addUniqueVToTree_str(/*JRB tree,char * str,  void * addV*/);
extern char * subDirStr(/*char * dir,char * file*/);//Mallocs a new string in format "dir/file"
extern char * extractFileName(/* char * path*/);
extern char * appendChar(/*char * dest, char append*/); //Reallocs dest and appends char
extern int intMin(/*int, int*/);//returns the minimum of two integers
#ifdef __cplusplus
}
#endif

#endif
