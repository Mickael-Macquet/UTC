#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "iniobj.h"

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

void handlerfinfils()
{
    int status;
    //Note: Dans le cas ou le fil est fini ( en etat zombi du coup ).
    //Les ressources seront liberé par le waitpid.
    //On se met en attente de n'importe quel fils.
    waitpid(-1, &status, WUNTRACED);
    printf("Serveur : Un des fils vient de se terminer avec le status : %d\n", status);
}

int main(int argc, char *argv[]){

    if(argc <2){
        printf("Serveur : Usage : %s [port]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Serveur : Lancement du serveur\n");
    //On execute handlerfinfils a la reception d'un signal de type SIGCHLD
    //Note: Lorsqu'un fils se termine, il envoit un SIGCHLD au pere.
    signal(SIGCHLD, handlerfinfils);

    SOCKADDR_IN saddr;
    SOCKET saddrl, sd, sdctl;
    pid_t pid;
    obj message;

    //On initialise la socket (SOCK_STREAM ==> TCP/IP)
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Serveur : Impossible de créer la socket.");
        exit(errno);
    }

    //On paramettre la socket.
    //Le socket peut etre associé a n'importe quelle adresse.
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    saddr.sin_family = AF_INET;
    //On specifie le port (passé en argument dans notre cas)
    saddr.sin_port = htons(atoi(argv[1]));
    
    //On bind la socket (associe les infos de la structure precedent a la socket)
    if(bind(sd, (SOCKADDR *)&saddr, sizeof(saddr)) == -1)
    {
        perror("Serveur : Impossible de bind la socket.");
        exit(errno);
    }
    
    //On defini le nombre max de connection pouvant etre misent en attente.
    if(listen(sd, 2) == -1)
    {
        perror("Serveur : listen()");
        exit(errno);
    }
    while(1)
    {
        printf("Serveur : Attente de nouvelle connection.\n");
        //On attend la connection d'un client.
        sdctl = accept(sd, (SOCKADDR*) 0, 0);

        //Si connection, on crée un nouveau pocessus
        pid = fork();
        
        switch(pid){
        
            case -1:
                perror("Serveur : Echec du fork");
                exit(errno);
            case 0:
                //Le fils 
                printf("Serveur : Reception d'un flux de données.\n");
                while(message.fndesc != -1){
                    //On lit les données jusqu'a ce que plus rien ne soit lut par recv
                    while(recv(sdctl, &message, sizeof(message), 0)<0)
                        perror("recv()");  

                    printobj(message);
                    sleep(1);
                } 
                printf("Serveur : Fin de la reception du flux de données\n");
                close(sdctl);
                exit(EXIT_SUCCESS);
                
            default:
                //Le pere attend la fin du fils
                //waitpid(pid, NULL, WUNTRACED);
                close(sdctl);
        }
    }
    close(sd);
    exit(EXIT_SUCCESS);

}