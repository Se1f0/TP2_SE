#include "tp2_functions.c"

void prod(){
  for (int i = 1; i <= n; i++)
  {

    message.mesg_type = 1;
    message.mesg_text = i;
    msgsnd(fillMess, &message, sizeof(message), 0);
    
    P(screen);
    printf("\033[0;36mData send by prod : %d \n", message.mesg_text);
    V(screen);
  }
  t->finProd = 1;
  P(screen);
  printf("\033[0;36mEOF Prod\n");
  V(screen);
  exit(0);
}

int consProd(){
  if (nbMsg() == 0 && t->finProd == 1)
  {
    return 0;
  }
  if((msgrcv(fillMess, &message, sizeof(message), 1, 0) != -1))
  {
  
    P(screen);
    printf("\033[0;32mData recieved by ConsProd : %d \n", message.mesg_text);
    V(screen);
    
    P(vide);
    deposer(message.mesg_text);
    V(plein);
    AfficherSem(2,message.mesg_text);
 
    V(affichage);
  }
  return 1;
}

void cons(){
  do
  {
  
    P(affichage);
    P(plein);
    int product = prelever();
    V(vide);
    AfficherSem(3,product);
  } while (t->nv < m || t->count != 20);
  P(screen);
  printf("\033[0;31mEOF Cons\n");
  V(screen);
  exit(0);
}

int main(){
    key = ftok(path,1238);

    create_sem();
    create_Seg();
    creeFileMess();

    t = shmat(segmid,t,0);
    if (t == (tempon*)-1)
    {
        perror("Erreur ...");
		exit(-1);
    }

    t->np = 0;
    t->nv = m;
    t->pp = 0;
    t->pv = 0;
    t->count = 0;
    t->finProd = 0;

    unsigned short int val[nbsems];
    val[vide] = m;
    val[plein] = 0;
    val[screen] = 1;
    val[affichage] = 0;
    init_sem(val);
    AfficherSem(1,1);

    int Prod=fork();
    if(Prod==0){
        prod();
    }
    int ConsProd=fork();
    if(ConsProd==0){
      int x = 1;
      do
      {
        x = consProd();
      } while (x == 1);
      P(screen);
      printf("\033[0;33mEOF ConsProd\n");
      V(screen);
       exit(0);
    }
    int Cons = fork();
    if (Cons == 0)
    {
      cons();
    }

    wait(0);
    wait(0);
    wait(0);

    semctl(semid,0,IPC_RMID,0);
    msgctl(fillMess, IPC_RMID, NULL);
    shmdt(t);
    shmctl(segmid,IPC_RMID,0);
    return 0;
}
