#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main (void)
{

  pid_t pid1,pid2;
  char machaine[100];
  
  int taille,ret;
  FILE *fichier = NULL;  
  int p[2];
  if (pipe(p) == -1)
  { 
      perror ("creation pipe impossible\n");
      exit(0);
  }
  else
  {

    pid1 = fork();
    switch(pid1)
    {
      case -1:	perror("Creation impossible");
		break;
      
      case 0:	//code du fils1
		printf("fils 1");
		close(p[0]);
		fichier = fopen("input.txt","r");
		do
		{
		    ret = fgets(machaine,100,fichier);
		    taille = strlen(machaine);
		    sprintf(machaine,"[%03d][%s]",taille);
		    write(p[1], machaine, strlen(machaine));
		}
		while(ret != EOF);
		fclose();
		close(p[1]);
      default:
		pid2 = fork();
		switch(pid2)
		{
		    case -1: 	perror("Creation impossible");
				break;
		    
		    case 0:	//code du fil2
				printf("fils 2");
				close(p[1]);	


				close(p[0]);
				break;
		    default:
				printf("pere");
				close(p[0]);
				close(p[1]);
				waitpid(pid1);
				waitpid(pid2);
				exit(0)
		  
				
		}
    }
}