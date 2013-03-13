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
	key_t clef;
	int id_msg,choix;
	int num_cli = 0;
	int longmsg = sizeof(message) - sizeof(long);
	message msg;
	clef= ftok("./sr03p025", 0);

	//On recupere l'identifiant de la file du message
	id_msg = msgget(clef,0);

	if(id_msg == -1){
		perror("recuperation file impossible\n");
		exit(1);
	}

    while(choix!=0)
    {
        printf("1 : Demander un identifiant.\n");
        printf("2 : Demander la liste des produits.\n");
        printf("0 : Quitter.\n");
        printf("Quel est votre choix ? : \n");
        scanf("%d",&choix);
		switch(choix)
		{
			//demande d'identifiant
			case 1: 
			{
				if(num_cli)
					printf("Je suis le client identifié: %d. Pas besoin de le refaire :p\n", msg.nucli);
				else
				{
					// Type 2 car on demande au serveur.
					msg.type= 2;
					
					//Question -1 ( voir serveur demande d'id )
					msg.question = -1;
					
					//On envoi le message.
					msgsnd(id_msg,(void*)&msg,longmsg,0);
					
					//On attend la reception de message de type 3 (pour client non identifié)
					msgrcv(id_msg,(void*)&msg,longmsg,3,0);
					
					if (msg.nucli == -1)
					{
						printf("Le serveur ne prend plus de client Bye: %d\n", msg.nucli);
						exit(0);
					}
					else
						if(msg.nucli == -2)
							printf("Le serveur a trop de client reesai plus tard: %d\n", msg.nucli);
						else
						{
							num_cli = msg.nucli;
							printf("Je suis le client identifié: %d\n", msg.nucli);
						}
				}
				
			}break;
			//demande de liste de produit
			case 2: 
			{
				if(num_cli)
				{
					// Type 2 car on demande au serveur.
					msg.type=2;
					
					//Question 2 ( voir serveur, demande de la liste )
					msg.question=0;
					
					//On envoi le message.
					msgsnd(id_msg,(void*)&msg,longmsg,0);
					
					//On attend la reception de message du type num_cli ( seul le client qui a fait la demande pourra donc le lire ).
					msgrcv(id_msg,(void*)&msg,longmsg,num_cli,0);
					printf("Je suis le client: %d\n Voici la liste des produits \n %s\n", num_cli,msg.product_list);
				}
				else
					printf("Il faut que je m'identifie d'abors\n");
			}break;
			case 0:
			{
				if(num_cli)
				{
					// Type 2 car on demande au serveur.
					msg.type=2;
					
					//Question 2 ( voir serveur, demande de la liste )
					msg.question=1;
					
					//On envoi le message.
					msgsnd(id_msg,(void*)&msg,longmsg,0);
					
					//On attend la reception de message du type num_cli ( seul le client qui a fait la demande pourra donc le lire ).
					msgrcv(id_msg,(void*)&msg,longmsg,num_cli,0);
					
					printf("J'ai dis au serveur Bye :p\n");
				}
				else
					printf("J'ai rien fait du tout.Bye :p\n");
			}break;
		}
	}
}
