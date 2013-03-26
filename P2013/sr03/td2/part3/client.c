#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include <strings.h>
#include <sys/wait.h>
#include "iniobj.h"

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int main(int argc, char *argv[]){
    //On verifie que l'utilisateur a bien passé au moin deux argument
    if(argc < 3){
        printf("Usage : %s [address] [port]\n", argv[0]);
        return EXIT_FAILURE;
    }
    struct hostent *hostinfo;
    SOCKADDR_IN saddrser;
    size_t saddrl;
    iniobj();
    SOCKET sd;
    int i;

    //On crée la socket.
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Impossible de créer la socket.");
        exit(EXIT_FAILURE);
    }

    hostinfo = gethostbyname(argv[1]);
    if (hostinfo == NULL)
    {
      fprintf (stderr, "Unknown host %s.\n", argv[1]);
      exit(EXIT_FAILURE);
    }

    saddrser.sin_addr = *(struct in_addr *) hostinfo->h_addr;
    saddrser.sin_port = htons(atoi(argv[2]));
    saddrser.sin_family = AF_INET;
    saddrl = sizeof(saddrser);

    bcopy(hostinfo->h_addr, &(saddrser.sin_addr.s_addr), hostinfo->h_length);
    
    //On essai de se connecter au serveur.
    if(connect(sd,(const struct sockaddr*)&saddrser, saddrl) == -1)
    {
      perror("Connection impossible.");
      exit(errno);
    }

    //On envoi les données.
    for (i = 0; i < TABLEN; ++i)
    {
        //Si l'envoi echou, on reesais.
        while(send(sd, (const void *)&tobj[i], sizeof(tobj[i]), 0)<0)
            perror("Echec de l'envoi. On retente.");
    }

    //La structure dans laquel on recupere le resultat.
    obj retour;

    int taillemsg;

    //On attend la reception de la taille du message suivant.
    if(recv(sd, &taillemsg, sizeof(int), 0)==SOCKET_ERROR)
    {
        perror("Erreur dans la reception du message");
        exit(-1);
    }
    printf("Taille du message a recevoir : %d\n", taillemsg);
    
    //On recupere le message suivant.
    if(recv(sd, &retour, taillemsg, 0)==SOCKET_ERROR)
    {
        perror("Erreur dans la reception du message");
        exit(-1);
    }
    printf("L'object recu est :\n");
    printobj(retour);

    int valeur_ret = -1;
    //On signal au serveur que la connection peut etre fermé.
    while(send(sd, &valeur_ret, sizeof(valeur_ret), 0)<0)
        perror("Echec de l'envoi. On retente.");

    exit(EXIT_SUCCESS);
}