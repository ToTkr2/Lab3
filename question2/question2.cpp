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
int ticketCount=100;

void * worker( void * arg )
{
    while(1)
    {  
   // printf("%c\n",a);
    int temp;
   sem_wait(a);
   temp=ticketCount;
   pthread_yield();
   
   temp=temp-1;

   
   pthread_yield();

   ticketCount=temp;
   printf("售票之后：%d\n",ticketCount);
   sem_post(a);
    //sem_post(a);
    }
    
    pthread_exit(NULL);

}
void * worker1( void * arg )
{
	
    int temp;
    while(1){
   sem_wait(a);
   temp=ticketCount;
   pthread_yield();
  
   temp=temp+1;
 //  sem_post(a);
  
   pthread_yield();

   ticketCount=temp;
   printf("退票之后：%d\n",ticketCount);
      sem_post(a);
}
    pthread_exit(NULL);
}

int main( int argc, char * argv[] )
{
	
    a=sem_open("a",O_CREAT,0666,1);
    
   
    pthread_t p1, p2;
   
    pthread_create( &p1, NULL, worker, NULL );
   
    pthread_create( &p2, NULL, worker1, NULL );
 
    
    pthread_join( p1, NULL );
  
    pthread_join( p2, NULL );
   
    return(0);
    
    sem_unlink("a");
   // sem_unlink("b");


}
