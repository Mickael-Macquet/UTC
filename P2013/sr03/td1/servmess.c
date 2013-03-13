#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct{
    long type; //2 = serveur, 3 = client
    int question;
    int nucli;
    char product_list[100];
} message;

int main(void)
{
	key_t clef= ftok("./sr03p025", 0);
	int id_msg;
	int nombre_cli = 0;
	message msg;
	int longmsg = sizeof(message) - sizeof(long);
	int last_id = 1;
	int nombre_cli_simult = 0;
  
	//On crée notre file de message ( on crée et on s'assure qu'aucun n'est crée )
	id_msg = msgget(clef, IPC_CREAT|IPC_EXCL|0666);
	
	if(id_msg == -1)
	{
		perror("Creation file impossible\n");
		exit(0);
	}
	
	//On traite un nombre limité de demande ( 4 ).
    while(nombre_cli < 4 | nombre_cli_simult > 0)
    {
		printf("nombre client  : %d\n",nombre_cli);
		//On attend la reception de messages venant d'un client(le type est set a 2 quand on envoi vers le serveur)
		msgrcv(id_msg,(void*)&msg,longmsg, 2,0);
		
		switch(msg.question)
		{
			//affectation id client
			case -1:
			{	
				if (nombre_cli < 4 & nombre_cli_simult < 2 )
				{
					//on incremente le nombre de client en train de demander des choses.
					nombre_cli++;
					
					//on incremente le nombre de client identifié en meme temps.
					nombre_cli_simult++;
					msg.type = 3;
					msg.nucli = last_id++;//le numero que l'on veut filer au client
					msgsnd(id_msg,(void*)&msg, longmsg, 0);
					printf("Je vien d'identifier le client: %d\n", msg.nucli);
				}
				else
				{
					msg.type = 3;
					if(nombre_cli >= 4)
						//On ferme le client ( plus possible de s'identifier )
						msg.nucli = -1;
					else
						//On le met en attente ( ya juste 2 client simult )
						msg.nucli = -2;
					msgsnd(id_msg,(void*)&msg, longmsg, 0);
					printf("Je vien d'envoyer balader un client\n", msg.nucli);	
				}
			}break;
			//demande de la liste
			case 0:
			{
					//On utilise le numero du client actuel comme type.
					msg.type = msg.nucli;
					printf("numero du client : %d\n",msg.type);
					
					//On envoi une chaine faisant etat des stocks;
					strcpy(msg.product_list, "1,poireaux,25,0,5\n2,carottes,36,0,8\n3,navets,15,0,3\n");
					
					msgsnd(id_msg,(void*)&msg, longmsg, 0);
					printf("coucou j'ai envoyé (serv) %s\n", msg.product_list);

			}break;
			
			//Un client s'en va.
			case 1:
			{
				//On utilise le numero du client actuel comme type.
				msg.type = msg.nucli;
				printf("le client : %d se casse\n",msg.type);
				msgsnd(id_msg,(void*)&msg, longmsg, 0);
				nombre_cli_simult--;
				
			}break;
			
		}
	}
	msgctl(id_msg,IPC_RMID,NULL);
}
