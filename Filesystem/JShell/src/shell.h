
#ifndef __ISHERMANS_SHELL__
#define __ISHERMANS_SHELL__

#include "fields.h"
#include "jrb.h"
#include "fs.h"

typedef struct shellstruc {
char * prompt;
JRB pids;
IS input;
int waitFlag;
FS_t * fs;
}* Shell;


#ifdef __cplusplus
extern "C"
{
#endif

Shell newShell(/*char * prompt*/);//creates a new shell with objects
int forkToExec(/*Shell, char *, argv*/);//Takes newArgv and cmd name
int getCommand(/*Shell*/); //returns number of words, 0 for empty line, EOF on ctrl-d
void freeShell(Shell);//frees everything associated with the shell, including vals of tree

#ifdef __cplusplus
}
#endif

#endif

