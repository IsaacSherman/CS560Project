#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "fields.h"
#include "jrb.h"
#include "myLib.h"
#include "shell.h"
#include <string.h>
Shell globalJShell=NULL;
extern killZombies();
void
exitCleanup(void){
		killZombies(globalJShell);
		freeShell(globalJShell);
}
void
cntl_c_handler(int dummy)
{
	//fprintf(stderr, "Caught signal 11\n");
	    //signal(11, cntl_c_handler);
}

int
main(int argc, char ** argv){
	 signal(11, cntl_c_handler);
	Shell jsh;
	int words=0, lastChar=0, status=0, pid=0, doWait=true, i=0;
	char ** newArgv= NULL;
	char * lastWord, * prompt;
	if(argc > 1) prompt = strdup(argv[1]);
	else prompt = strdup("jsh:");
	atexit(exitCleanup);
	jsh = newShell(prompt);
	globalJShell = jsh;//give me the shell so I can free it later
	SafeFree(prompt);
	for(words = getCommand(jsh); words != EOF; words=getCommand(jsh)){
		if(forkToExec(jsh) < 0) return 1;
	}
		SafeFree(prompt);
	}

