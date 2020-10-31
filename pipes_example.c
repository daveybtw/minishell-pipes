#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#define READ  0
#define WRITE 1
#define max_args 13
#define maxln_Com_Amb 105
char *args1[max_args];
char *args2[max_args];


void separaArgs(char comando1[], char comando2[]){
  int i;
  
  for(i=0;i<(max_args-1);i++) args1[i]=NULL; /*Borrar argumento previo que pudiera existir*/
  strtok(comando1," "), i=0; /*separar palabras individuales usando tokens (espacio vacio)*/
  args1[i]=comando1; /*El 1er argumento sera comando por un requerimiento de execvp*/
  while((args1[++i]=strtok(NULL," "))!=NULL && i<(max_args-2));

  int j;
  
  for(j=0;j<(max_args-1);j++) args2[j]=NULL; /*Borrar argumento previo que pudiera existir*/
  strtok(comando2," "), j=0; /*separar palabras individuales usando tokens (espacio vacio)*/
  args2[j]=comando2; /*El 1er argumento sera comando por un requerimiento de execvp*/
  while((args2[++j]=strtok(NULL," "))!=NULL && j<(max_args-2));
}



int main(int argc, char *argv[]) {
	// ------------------------------
	char destino1[strlen(argv[1])+1];
    strcpy(destino1, argv[1]);
	char destino2[strlen(argv[2])+1];
    strcpy(destino2, argv[2]);
    separaArgs(destino1, destino2);
	puts(args1[0]);
	puts(args2[0]);
	// --------------------------
	pid_t pid;
	int fd[2];

	if (pipe(fd) == -1) {
		perror("Creating pipe");
		exit(EXIT_FAILURE);
	}

	switch(pid = fork()) {

	case 0:
		// The child process will execute wc.
		// Close the pipe write descriptor.
		close(fd[WRITE]);
		// Redirect STDIN to read from the pipe.
		dup2(fd[READ], STDIN_FILENO);
		// Execute wc
		execvp(args2[0], args2);

	case -1:
		perror("fork() failed)");
		exit(EXIT_FAILURE);

	default:
		// Here you can execute the first argument entered
		// Close the pipe read descriptor
		close(fd[READ]);
		// Redirect STDOUT to write to the pipe.
			dup2(fd[WRITE], STDOUT_FILENO);
		// Execute ls -l
		execvp(args1[0], args1);
		
	}
  
}