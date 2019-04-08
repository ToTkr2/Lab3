#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
 
 sem_t *a;
 sem_t *b;
 sem_t *c;
 sem_t *d;
 sem_t *f;
 sem_t *e;
 
void *pp1(void *ptr)
{
	sem_wait(a);	
   // fork();
    printf("I am the process P1\n");
    sleep(1);
    sem_post(b);
    sem_post(d);
//
   
    pthread_exit(NULL);
}

void *pp2(void *ptr)
{
	sem_wait(b);	
	sem_wait(e);
	
  //  fork();
    printf("I am the process P2\n");
    sleep(1);
    sem_post(e);
    sem_post(c);
   
  
    pthread_exit(NULL);
}

void *pp3(void *ptr)
{
	sem_wait(d);
	sem_wait(e);	
   // fork();
    printf("I am the process P3\n");
    sem_post(e);
    sem_post(f);
  
  
    pthread_exit(NULL);
}

void *pp4(void *ptr)
{
	sem_wait(f);
	sem_wait(c);	
  //  fork();
    printf("I am the process P4\n");
    //sem_post(a);
    
    pthread_exit(NULL);
}

 

int main()
{



 a=sem_open("a",O_CREAT,0666,1);
 b=sem_open("b",O_CREAT,0666,0);
 c=sem_open("c",O_CREAT,0666,0);
 d=sem_open("d",O_CREAT,0666,0);
 e=sem_open("e",O_CREAT,0666,1);
 f=sem_open("f",O_CREAT,0666,0);

  printf("sem_init\n");
  pthread_t t1,t2,t3,t4;
//    printf( "Initial value : %d\n", counter );

    pthread_create( &t1, NULL, pp1, NULL );
   
    pthread_create( &t2, NULL, pp2, NULL );
 
    pthread_create( &t3, NULL, pp3, NULL );
   
    pthread_create( &t4, NULL, pp4, NULL );
 

    pthread_join( t1, NULL );

    pthread_join( t2, NULL );

    pthread_join( t3, NULL );

    pthread_join( t4, NULL );

    sem_unlink("a");
    sem_unlink("b");
    sem_unlink("c");
    sem_unlink("d");
    sem_unlink("e");
    sem_unlink("f");
}



