#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "fields.h"
#include "jrb.h"
#include "myLib.h"
#include "shell.h"
#include <string.h>
#include "fslib.h"
Shell globalJShell=NULL;

extern killZombies();
extern void exit(int);


void
exitCleanup(void){
		killZombies(globalJShell);
		freeShell(globalJShell);
}
void
cntl_c_handler(int dummy)
{
	/*static int once = 0;
	printf("once = %d", once);
	if(!once++){
	fprintf(stderr, "Caught signal 11\n");
	    exit(dummy);
		signal(11, cntl_c_handler);
	}*/
	exit(1);
}

int
main(int argc, char ** argv){
	 signal(11, cntl_c_handler);
	Shell jsh;
	int words=0, lastChar=0, status=0, pid=0, doWait=true, i=0;
	char ** newArgv= NULL;
	char * lastWord, * prompt;
	int port;
	
	if(argc > 1) {
	port = atoi(argv[1]);
  if (port < 5000) {
    fprintf(stderr, "usage: serve1 port\n");
    fprintf(stderr, "       port must be > 5000\n");
    //exit(1);
	port = -1;
	}
  else{
		if(port >= 5000){
		int sock = serve_socket(port);
		int fd = accept_connection(sock);
		dupsockin(fd);
		dupsockout(fd);
		}
	}
	}
	prompt = strdup("jsh:");
	atexit(exitCleanup);
	jsh = newShell(prompt);
			if (VERBOSE) printf("s->fs = %d", (int)jsh->fs);

	globalJShell = jsh;//give me the shell so I can free it later
	SafeFree(prompt);
	for(words = getCommand(jsh); words != EOF; words=getCommand(jsh)){
		if(forkToExec(jsh) < 0) return 1;
	}
		SafeFree(prompt);
	}

