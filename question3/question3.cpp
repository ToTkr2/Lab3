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
 char m[10];
int i=0,j=0;

void * worker( void * arg )
{
	
 while(1)
    {
        sem_wait(a);
        char *in = &m[i++%10];
        scanf("%c",in);
        sem_post(b);
        if(*in == 'q') break;
    }

 }
   
       
    

void * worker1( void * arg )
{
	
      while(1)
    {
        sleep(1);
        sem_wait(b);
        char *out = &m[j++%10];
        printf("read:%c\n",*out);
        sem_post(a);
    }

}



int main( int argc, char * argv[] )
{
	 a=sem_open("a",O_CREAT,0666,10);
     b=sem_open("b",O_CREAT,0666,0);
    
   
   
    pthread_t p1, p2;
  
    pthread_create( &p1, NULL, worker, NULL );
   
    pthread_create( &p2, NULL, worker1, NULL );
 

   

    pthread_join( p2, NULL );
    pthread_join( p1, NULL ); 
    sem_unlink("a");
    sem_unlink("b");
    
    return(0);
    
    
}