#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int main (void)
{
	int taillemsg = 6;
	int pid1,pid2,finmsg;
	char machaine[100];
	char chaine[100];
	char msg[100];
	char buffer[20];
	char * ret;
	int taille, statut;
	FILE *fichier = NULL;  
	int p[2];
	printf("Debut\n");
	
	//creation du pipe
	if (pipe(p) == -1)
	{ 
		perror ("creation pipe impossible\n");
		exit(0);
	}
	else
	{
		//Creation du premier fils.
		pid1 = fork();
		switch(pid1)
		{
			//Erreur à la creation.
			case -1:	perror("Creation impossible");
						printf("souci");
						break;
      
			//code du fils1
			case 0:	
				printf("Processus fils : pid = %d\n", getpid());
				printf("fils 1\n");
				
				//on ferme le pipe coté lecture.
				close(p[0]);
				fichier = fopen("input.txt","r");
				while(fgets(machaine,100,fichier) != NULL)
				{	
					taille = strlen(machaine);
					machaine[taille-1] = '\0';
					sprintf(chaine,"[%03d][%s]",taille,machaine);
					printf("%s\n", chaine);
					write(p[1], chaine, strlen(chaine));
				}
				
				fclose(fichier);
				//on ferme le pipe coté ecriture.
				close(p[1]);
				printf("fin du fils 1\n");
				exit(0);
				
				break;
			//code du pere
			
			default:
				printf("Processus PERE : pid = %d\n", getpid() );

				//creation du deuxieme fils
				pid2 = fork();
				switch(pid2)
				{
					case -1: 	perror("Creation impossible");
					break;
		    
					case 0:	//code du fil2

						printf("fils 2\n");
						
						close(p[1]);	
						while(1)
						{
							
							read(p[0], buffer,5);
							
							buffer[5] = '\0';
							
							sscanf(buffer, "[%d]",&taillemsg);
							finmsg = 0;
							msg[0]='\0';
							strcpy(msg, buffer);
							printf("buffer debuig : %s\n", msg);
							while(!finmsg)
							{
								if (taillemsg < 18)
								{
									
									// + 1 car on n'a pas compter les [] dans le message initial.
									// ET que l'on a virer le dernier caractere ( \n ) de chaque ligne.
									read(p[0], buffer,taillemsg+1);
									
									buffer[taillemsg+1] = '\0';
									printf("buffer debuigzzz : %s\n", buffer);
									finmsg = 1;
								}
								else
								{
									read(p[0], buffer,19);
									buffer[sizeof(buffer)] = '\0';
									taillemsg = taillemsg -19;
									printf("buffer debuiga : %s : taille : %d\n", buffer,taillemsg);
								}
								
								strcat(msg, buffer);
								
							}	

							printf("fils 2 lit: %s de taille %d\n", msg, taillemsg);
							
						}	

						close(p[0]);
						break;
					default:
						printf("pere\n");
						close(p[0]);
						close(p[1]);
						waitpid(pid1, &statut, 0);
						waitpid(pid2, &statut, 0); 
						printf("Fin du pere\n");
						exit(0);	
				}
		}
	}
}
