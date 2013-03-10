#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


int main(void):
{
  key_t clef;
  int id_msg;
  longmsg = sizeof(message) - sizeof(long);
  
  clef= ftok("sr03p025", 0)
  id_msg = msgget(clef, 0);
  if(id_msg == -1)
    perror("recuperation file impossible");
    exit();
  //demande d'identifiant
  m.type= 2;
  m.question = -1;
  msgsnd(id_msg,(void*)&msg,longmsg,0);
  msgrcv(id_msg,(void*)&msg,longmsg,3,0)
  num_cli= msg.numcli
  
  //demande de liste de produit
  msg.type=2;
  msg.question=1
  msgsnd(id_msg,(void*)&msg,longmsg,0);
  msgrcv(id_msg,(void*)&msg,longmsg,numcli,0)
  
  
  
}