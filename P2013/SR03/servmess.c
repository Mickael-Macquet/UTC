#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define REQ_NUM_CLI = -1 blabla

typedef struct
{
    long type;
    int question;
    int numclt;
    
}message


int main(void):
{
  key_t clef;
  int id_msg;
  message msg;
  longmsg = sizeof(message) - sizeof(long);
  
  clef= ftok("sr03p025", 0)
  id_msg = msgget(clef, IPC_CREAT|IPC_EXCLT|0666);
  if(id_msg == -1)
    perror("Creation file impossible");
    exit();
    //msgsnd
    //msgrcv
    msgrcv(id_msg,(void*)&msg,long-msg, 2,p)
    switch(message.question)
      case -1:
	  //affectation id client
	  msg.type=3;
	  msg.numclt = //le numero que l'on veut filer au client
	  msgsnd(id_msg,(void*)&msg, longmsg, 0);
      case 0:
	  //list produit
	  msg.type = msg.numclt
	  
}