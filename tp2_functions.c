#include "tp2_headers.h"

//file de messages
void creeFileMess(){
  if((fillMess=msgget(key,IPC_CREAT|IPC_EXCL|0600))==-1){
   if((fillMess=msgget(key,IPC_CREAT|0600))!=-1){
     msgctl(fillMess, IPC_RMID, NULL);
    }
    if((fillMess=msgget(key,IPC_CREAT|0600))==-1){
      perror("Error de Mssget");
      exit(1);
    }
  }
}
int nbMsg(){
    if (msgctl(fillMess,IPC_STAT,&buf) == -1){
        perror("Erreur msgctl()") ;
        exit(1) ;
    }
    else
    {
        return buf.msg_qnum;
    }
    
}
//les semaphores
void create_sem() {
    semid = semget(key,nbsems,IPC_CREAT|IPC_EXCL|0666);
    if (semid == -1)
    {

        semid = semget(key,nbsems,0);
        semctl(semid,0,IPC_RMID,0);
        semid = semget(key,nbsems,IPC_CREAT|IPC_EXCL|0666);
        if (semid == -1)
        {
            perror("SemError....");
            exit(-1);
        }
    }
}

void init_sem(unsigned short int  semvals[]){
	semval.array = semvals;
    semctl(semid,nbsems/*0*/,SETALL,semval);
}

void P(int numsem){
	operation[0].sem_num = numsem;
	operation[0].sem_op = -1;
	operation[0].sem_flg = 0;
	semop (semid, operation,1);
}

void V(int numsem){
	operation[0].sem_num = numsem;
	operation[0].sem_op = 1;
	operation[0].sem_flg = 0;
	semop (semid, operation,1);
}

void AfficherSem(int color,int val){
    int v=0;
    P(screen);

    if(color==1) printf("\033[0;37m");

    if(color==2) {
    printf("\033[0;33m");
    if(t->pv==0) v= m-1; else v=t->pv-1; 
    printf("\033[0;33mThe ConsProd deposit a data %d in index %d\n",val,v);
    }

    if(color==3) {
        printf("\033[0;31m"); 
        if(t->pp==0) v= m-1; else v=t->pp-1;
    printf("\033[0;31mThe Consumer consumed a data %d from index %d\n",val,v);
    }
	semctl(semid,nbsems/*0*/,GETALL,semval);
	printf("	╔═══════════════════════════════════════════════════════╗\n");
	printf("	║	la valeur du sémaphore vide  est  %hu		║\n",semval.array[vide]);
	printf("	║	la valeur du sémaphore plein est  %hu		║\n",semval.array[plein]);
	printf("	╚═══════════════════════════════════════════════════════╝");
    printf("\n");
    V(screen);
}
//memoire partage
void create_Seg() {
    segmid = shmget(key,sizeof(tempon),IPC_CREAT|IPC_EXCL|0666);
    if (segmid == -1)
    {
        segmid = shmget(key,sizeof(tempon),0);
        shmctl(segmid,IPC_RMID,0);
        segmid = shmget(key,sizeof(tempon),IPC_CREAT|IPC_EXCL|0666);
        if (segmid == -1)
        {
            perror("SegError.....");
            exit(-1);
        }
        
    }
}

//Prod -- Cons
void deposer(int product) {
    t->tab[t->pv] = product;
    t->pv = (t->pv + 1) % m;
}

int prelever() {
    int product = t->tab[t->pp];
    t->pp = (t->pp + 1) % m;
    t->count++;
    return product;  
}