#include "shell.h"
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>
//#include <sys/wait.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include "myLib.h"
#include <assert.h>
#include "fslib.h"

struct stat {};

Shell 
newShell(char * prompt, FS_t fs){//creates a new shell with objects
	Shell ret;
	ret = (Shell) malloc(sizeof(struct shellstruc));
	if(ret == NULL) return NULL; //malloc error
	ret->prompt = strdup(prompt);
	if (ret->prompt ==NULL) {//malloc error
		SafeFree(ret);
		return NULL;
	}
	ret->pids = make_jrb();
	if(ret->pids == NULL){
		SafeFree(ret->prompt);
		SafeFree(ret);
		return NULL;
	}
	ret->input = new_inputstruct(NULL);
	if(ret->input == NULL){
		jrb_free_tree(ret->pids);
		SafeFree(ret->prompt);
		SafeFree(ret);
		return NULL;
	}
	ret->fs = malloc(sizeof(FS_t));
	return ret;
}


void
prompt(Shell jsh){
	fflush(stdout);
	printf("%s", jsh->prompt);
	fflush(stdout);
}

void 
addColonToPrompt(Shell jsh){
int size = strlen(jsh->prompt)+1;
char * newPrompt = malloc(strlen(prompt)+1);
for(int i = 0; i < size-1; ++i){
	newPrompt[i] = jsh->prompt[i];
	}
	newPrompt[size-1] = ':';
	newPrompt[size] = '\0';
	jsh->prompt = strdup(newPrompt);
	}

void 
freeShell(Shell ret){//frees everything associated with the shell, including vals of tree
	jettison_inputstruct(ret->input);
	JRB ptr;
	jrb_traverse(ptr, ret->pids)	SafeFree(ptr->val.s);//free all the strings
	jrb_free_tree(ret->pids);
	SafeFree(ret->prompt);
}

int 
shouldWait(Shell jsh){//returns true or false
	int words = jsh->input->NF;
	if(jsh->waitFlag == -1){//Don't know
		if(words > 0) {
			char * lastWord = jsh->input->fields[words-1];
			int lastChar = strlen(lastWord)-1;
			if(lastWord[lastChar] == '&'){
				lastWord[lastChar] = '\0';//overwrite ampersand
				jsh->waitFlag = false;
				return false;//don't wait
			}
		}
		else
			jsh->waitFlag = true;
		return true;
	}
	return jsh->waitFlag;
}



int
parseCommand(Shell S, char ** cmd, char *** argv){

	int i, words = S->input->NF;
	*cmd = strdup(S->input->fields[0]);
	int special = false;
	FS_t fs = S->fs;
	/*
	these are the commands we need to parse, with declarations in the style of C:
	Also, strings are char *, it looks like all of our files will be text
	void mkfs(void)
	fd open (string filename, char flag) where
		flag is either r or w
	string read (int fd, size) 
		fd is the file descriptor
		size is the number of bytes to read- and the amount to increase offset
	void write (int fd, string text)
	void seek (int fd, int offset)
	void close(int fd)
	void mkdir (string dirname)
	void rmdir (string dirname)
	void cd (string dirname)
	void * ls (void) no params!
	string cat (string filename)
	string tree (void) 
	import (string src, string dest)
	export (string src, string dest)
	*/
	if(strcmp(*cmd, "mkfs") == 0) {
		//make file system
		char *path;
		FS_t temp;
		if(words > 1){
			path = strdup(S->input->fields[1]);
		}
		else path = strdup("disk.disk");
		printf("s->fs = %d", (int)S->fs);
		printf("S->fs = %d, temp = %d\n", (int)S->fs, (int)temp);
		temp = mkfs(path);
		S->fs = temp;
		tree(S->fs, true, true);
		printf("S->fs = %d, temp = %d\n", (int)S->fs, (int)temp);
		
		free(path);
	}
	else if (strcmp(*cmd, "read") == 0){
		//read, 
	printf("read\n");
	}
	else if (strcmp(*cmd, "open") == 0){
		//open, 
	}
	else if (strcmp(*cmd, "write") == 0){
		//write, 
	}
	else if (strcmp(*cmd, "seek") == 0){
		//seek, 
	}
	else if (strcmp(*cmd, "close") == 0){
		//close, 
	}
	else if (strcmp(*cmd, "mkdir") == 0){
		mkdir(S->fs, S->input->fields[1]);
		write_fs(S->fs);//mkdir, 
	}
	else if (strcmp(*cmd, "rmdir") == 0){
		rmdir(S->fs, S->input->fields[1]);
		write_fs(S->fs);
		//rmdir, 
	}
	else if (strcmp(*cmd, "cd") == 0){
		//cd, 
		cd(S->fs, S->input->fields[1]);
	//	free(S->prompt);
		S->prompt = strdup(fs->cd->name);
		addColonToPrompt(S);
	}
	else if (strcmp(*cmd, "ls") == 0){
		//ls, 
		ls(S->fs);
		
	}
	else if (strcmp(*cmd, "tree") == 0){
		//tree,
		tree(S->fs, true, false); 

	}
	else if (strcmp(*cmd, "import") == 0){
		//import, 
		import(S->fs, S->input->fields[1], S->input->fields[2]); 
		write_fs(S->fs);
	}
	else if (strcmp(*cmd, "export") == 0){
		//export, 
	}
	else{
	printf("Invalid option: %s, going to try to fork and exec:", *cmd);
		
	for(i = 0; i < words; i++){

		(*argv)[i] = strdup((S->input->fields[i]));
		if((*argv)[i][0]=='>' || (*argv)[i][0]=='<' ||(*argv)[i][0]=='|' ) special = true;  
		printf("argv[%d] = %s\n", i, (*argv)[i]);
		}
	argv[i] = NULL;
	int pid = fork();
	if(pid == 0){
	execvp(cmd, argv);
	perror("exec");
	exit(1);
	}
	else return special;
	}
	
}

char **
trimGenericArgs(char ** in, char symbol, int behavior, char terminator, char ** out){
	//behavior 0 skips the symbol and the following word;
	//behavior 1 skips the symbol up to the next symbol or NULL or terminator is encountered
	//in either case, out contains the excised arguments, terminated by NULL (unless NULL is passed)
	int argc = 0, i, longest=0;
	char ** ptr, **ret, ** ptr2 = out;
	for(ptr = in; ptr != NULL && argc >= 0 && (*ptr)!=NULL; ptr++) argc++;
	if (argc < 0) {
		fprintf(stderr, "argv not NULL terminated array\n");
		return NULL;
	}
	ret = malloc(sizeof(char *) *(argc));
	if(ret == NULL) {
		perror("trimStr->malloc");
		return NULL;
	}
	ptr = in;
	for(i=0; ptr != NULL && *ptr != NULL && (*ptr)[0] != symbol; ++ptr)
		ret[i++]= strdup( *ptr); //ret is copy of ptr until symbol
	if(*ptr == NULL){ 
		ret[i] = NULL;
		return ret;} //no symbol found
	if(out != NULL){
		*ptr2++ = strdup(*ptr++);
		*ptr2++ = strdup(*ptr++);
	} else ptr+=2;

	if( behavior && ptr != NULL ) {
		while(ptr!=NULL && *ptr != NULL && !((*ptr)[0] == symbol || (*ptr)[0] == terminator)){
			if (out != NULL) *ptr2++ = strdup(*ptr++);
			else ++ptr;
		}
	}
	for(i=i; ptr != NULL && *ptr != NULL; ++ptr) ret[i++]=strdup( *ptr);//skip that symbol 
	ret[i] = NULL;//Null terminated
	if(out != NULL) *(++ptr2) = NULL;
	return ret;
}

char ** 
trimInArgs(char ** in){
	return trimGenericArgs(in, '<', 0, 0, NULL);
}
char **	
trimOutArgs(char ** in){
	return trimGenericArgs(in, '>', 0, 0, NULL);
}
char **
trimNextPipe(char ** in, char ** out){
	return trimGenericArgs(in, '|', 1, '>', out);
}



char **
trimArglist(char ** in, int mode){//I don't actually use this function, but the decs are useful
	//for explaining the above 3 trim funcs
	char ** ret=NULL;
	//mode list:
	//0 = cmd args > file more stuff BECOMES: cmd args more stuff
	//1 = cmd args stuff < file BECOMES: cmd args stuff
	//2 = cmd args |stuff 1|stuff 2 BECOMES: cmd args |stuff 2
	switch (mode){
		case 0: ret = trimOutArgs(in);
				break;
		case 1: ret = trimInArgs(in);
				break;
		case 2: ret = trimNextPipe(in, NULL);
				break;
		default :return NULL;
	}
	return ret;
}

void
printCharArray(char ** in, char * label){
	char ** ptr;
	int i = 0;
	if(in == NULL) fprintf(stderr, "Null array passed to print!\n");
	for (ptr = in; ptr!= NULL && (*ptr)!=NULL; ptr++) fprintf(stderr, "%s[%d] = %s\t",label,  i++, *ptr);
	fprintf(stderr, "\n");
}

void
free2dCharArray(char ** argv){//frees the contents of a char ** array, but not the array itself
	char ** ptr;
	for(ptr=argv; ptr!=NULL && *ptr != NULL; ++ptr)
		SafeFree(*ptr);
}

void
swap2dCharArray(char ** old, char ** out){//takes out and puts it into old; assumes old is big enough
	char ** ptr;//also frees out at the end
	free2dCharArray(old);
	for(ptr = out; ptr != NULL, *out != NULL; ++ptr){
		*old++ = strdup(*out++);
		if(*out == NULL) *old = NULL;
	}
	free2dCharArray(out);
}




testTrims(){
	char * test[] = {"cat", "<",  "pookie", ">>", "pookie.txt", NULL};//,"-5",  "|","head", "-3",  ">", "fileout", NULL};
printCharArray(test, "original");
char ** test1 = malloc(sizeof test);
char ** test2 = trimNextPipe(test, test1);

handleSpecials(NULL, test);
printCharArray(test1, "test1");
printCharArray(test2, "test2");
swap2dCharArray(test1, test2);
printCharArray(test1, "test1");
free2dCharArray(test2);
printCharArray(test1, "test1");
printCharArray(test2, "AfterFree");
}



int
redirect(Shell S, int mode, char * file, char ** argv){
	//mode 2 is >>
	//mode 1 is stdout redirect
	//mode 0 is stdin redirect
	int fd1;
	if(mode == 1){
		struct stat buf;
		int err = stat(file, &buf);
		if(err != -1) {
			fprintf(stderr, "%s already exists.\n", file);
			exit(1);
		}
		fd1 = open(file, O_CREAT | O_WRONLY, 0644);
	} else if(mode == 2) {
		fd1 = open(file, O_APPEND| O_WRONLY, 0644);
		mode = 1;//set mode to 1 for output redirect below
	} else {//mode is 0
		assert(mode == 0);//if mode isn't 0, there will be issues
		fd1 = open(file, O_RDONLY);
	}
	if (fd1<0) {//Catch all the errors
		perror(file);
		exit(1);//bail out
	}
	if(dup2(fd1, mode)!= mode){ //overwrite stdout/in
		perror("dup2");
		exit(1);
	}
	close(fd1);
	return 0;
}

int 
findPipe(char ** arglist){
	char ** ptr = arglist;
	int ret = false;
	for(; ptr != NULL && *ptr != NULL; ++ptr) ret += (*ptr[0] == '|');
	return ret;
}

char ** 
recursivePipes(char ** arglist){//Gets the most basic, pipefree command from arglist
	int numPipes = findPipe(arglist);
	if(numPipes == 0) {
		return arglist;
	}
	else return(recursivePipes(trimNextPipe(arglist, NULL)));
}

closeAllPipes(int pipefd[][2], int numPipes)
{
	int i;
	for (i = 0; i < numPipes; i++){
		close(pipefd[i][0]);
		close(pipefd[i][1]);
	}
}

//OK, we'll just make a char [][][] array and call it a day.
char ***
generate3dCharArrayFromPipes(char ** arglist, int numPipes){
	char *** ret= malloc(sizeof (char ***)*numPipes), **ptr = arglist, **nextList;
	ret[0] = recursivePipes(arglist);
	int i, j;
	for(i = 0; *ptr != NULL; ++ptr, ++i);//set i to number of args
	for(j = 1; j <= numPipes; j++){
		nextList = malloc(sizeof(char **)*i);
		arglist = trimNextPipe(arglist, nextList);//only really care about nextList now
		if(*nextList != NULL) free(nextList++[0]);//Eat the pipe character;
		ret[j] = nextList;
	}
	return ret;
}

int
handlePipes(Shell S, char ** arglist){
	int i, numPipes = findPipe(arglist), dummy, pipefd[findPipe(arglist)][2], w = shouldWait(S); 
	char *** cmds = generate3dCharArrayFromPipes(arglist, numPipes);
	if( numPipes==0) return 0; //no pipes
	for(i = 0; i < numPipes;++i) pipe(pipefd[i]);
	for(i = 0; i <= numPipes; i++){
		dummy = fork();
		if (dummy == 0){
			if(i!=0){//if not the first cmd
				if(dup2(pipefd[i-1][0], 0)<0){
					perror("dup2, read");
					exit(1);
				}
			}
			if(i < numPipes) {//if not the last
				if(dup2(pipefd[i][1], 1)<0){
					perror("dup2, write");
					exit(1);
				}
			}
			closeAllPipes(pipefd, numPipes);
			execvp(cmds[i][0], cmds[i]);
			perror(cmds[i][0]);
			exit(1);
		}
		else addUniqueVToTree_int(S->pids, dummy, NULL);
	}
	closeAllPipes(pipefd, numPipes);
	for(i=0; i < numPipes && w; i++) wait(&dummy);//wait for all procs to end
	exit(1);//Kills all zombies on exit
}
int 
handleSpecials(Shell S, char ** arglist){
	int ins=0, outs=0, pipes=0;
	char ** ptr= arglist;
	//get input and output redirects first; they only take one argument.

	if(*ptr!= NULL && *ptr[0] != '<')  
		for(ptr = arglist; *ptr != NULL && *ptr[0] != '<'; ++ptr);
	if(*ptr != NULL){
		redirect(S, 0, *(++ptr), arglist);//Finish scanning
		while(*ptr!=NULL && *ptr[0] != '<') ++ptr;
		if(*ptr != NULL) return -1; //die
		swap2dCharArray(arglist, trimInArgs(arglist));//remove '<' and file from list
	}
	for(ptr = arglist; *ptr != NULL && *ptr[0] != '>'; ++ptr) continue;
	if(*ptr != NULL) {
		//Handle >> or  >
		if(strcmp(*ptr, ">>") == 0) redirect(S, 2, *(++ptr), arglist);
		else redirect(S, 1, *(++ptr), arglist);//do the swap
		while(*ptr!=NULL && *ptr[0] != '>') ++ptr;
		if(*ptr != NULL) return -2; //die
		swap2dCharArray(arglist, trimOutArgs(arglist));//remove '>' and file from list
	}
	//resulting arglist should only have args and pipes
	return handlePipes(S, arglist);
}

int 
forkToExec(Shell jsh){
	char * cmd = NULL,  ** argv= malloc(sizeof(char *)*jsh->input->NF+1);
	int pid, w, status;
	w=shouldWait(jsh);
	fflush(stdout);
		if(parseCommand(jsh, &cmd, &argv)){//special characters found
			//int err = handleSpecials(jsh, argv);//If pipes, we'll die in here.  Otherwise, 
			//if (err < 0){
			//	fprintf(stderr, "Ambiguous %sput redirect.\n",  err==-1?"in":"out");
			//}
		}
		//execvp(cmd, argv ); //we come out here
		//perror(cmd);
	
	return 0;
}



int 
getCommand(Shell jsh){ //returns number of words, EOF on ctrl-d
	int words;
	prompt(jsh);
	jsh->waitFlag = -1;//For shouldWait calls.  Establish it for one command on the should wait call, then
	//on successive calls for the same line, return the flag
	for(words = get_line(jsh->input);words == 0; words = get_line(jsh->input));//ignore blank lines
	if(words != EOF) {
		if(strcmp(jsh->input->fields[0], "exit") == 0){ 
			words = EOF; 
		}
	}//break on "exit"

	return words;
}

void
killZombies(Shell S){
	JRB ptr; 
	jrb_traverse(ptr, S->pids){
		kill(ptr->key.i, SIGKILL);
	}
}
