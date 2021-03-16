#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>

#define nbsems 4
#define n 20
#define m 5
#define vide 0
#define plein 1
#define screen 2
#define affichage 3

struct mesg_buffer { 
  long mesg_type; 
  int  mesg_text; 
} message;

union semun{
	int semval;		
	struct semid_ds *buf;
	ushort *array;
}arg_ctl;

union semun semval;	
struct sembuf operation[1];

typedef struct tempon
{
  int nv,np;
  int pv,pp;
  int count;
  int finProd;
  int tab[m];
}tempon;
tempon* t = NULL;

struct msqid_ds buf;
key_t key;
char* path="/home/mohamed/Tpse/TP2";

int fillMess,semid,segmid;
int finConsProd = 0;

//semephores
void AfficherSem(int color,int val);
void create_sem();
void init_sem(unsigned short int  semvals[]);
void P(int numsem);
void V(int numsem);

//memoire partages
void create_Seg();

//File de messages
void creeFileMess();

//Function Prod - Cons
void deposer(int product);
int prelever();
