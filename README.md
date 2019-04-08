# Lab3
---
1）通过fork的方式，产生4个进程P1,P2,P3,P4，每个进程打印输出自己的名字，例如P1输出“I am the process P1”。要求P1最先执行，P2、P3互斥执行，P4最后执行。通过多次测试验证实现是否正确。  
信号量b，d控制p2,p3在P1之后，e控制p2,p3互斥，f，c控制p4的顺序  
源代码：  
```
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
    fork();
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
	
    fork();
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
    fork();
    printf("I am the process P3\n");
    sem_post(e);
    sem_post(f);
  
  
    pthread_exit(NULL);
}

void *pp4(void *ptr)
{
	sem_wait(f);
	sem_wait(c);	
    fork();
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
```
实验结果：
 ![image text](https://github.com/ToTkr2/Lab3/blob/master/img/1.png)  
 
 ---
 2）火车票余票数ticketCount 初始值为1000，有一个售票线程，一个退票线程，各循环执行多次。添加同步机制，使得结果始终正确。要求多次测试添加同步机制前后的实验效果。  
 信号量a实现了售票退票的互斥，售票或退票时，wait（a），a为0，售票或退票后，signal（a），a为1，worker（）是售票，worker1（）是退票。  
 源代码：  
 ```
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
   printf("售票之后：%d\n",temp);
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
   printf("退票之后：%d\n",temp);
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
   // printf("售票之后:%d\n",ticketCount);
    return(0);
    
    sem_unlink("a");
   // sem_unlink("b");


}

```
实验结果：  
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/2.png)  

---
3）一个生产者一个消费者线程同步。设置一个线程共享的缓冲区， char buf[10]。一个线程不断从键盘输入字符到buf,一个线程不断的把buf的内容输出到显示器。要求输出的和输入的字符和顺序完全一致。（在输出线程中，每次输出睡眠一秒钟，然后以不同的速度输入测试输出是否正确）。要求多次测试添加同步机制前后的实验效果。 
信号量a为缓存区的数量，b为已经写入的数量，worker（）是写字符，worker（）是读字符。  
源代码：
```
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
        printf("%d:%c\n",j,*out);
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
```
实验结果：
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/3.png)

---
4）进程通信问题。阅读并运行共享内存、管道、消息队列三种机制的代码  
实验测试a）通过实验测试，验证共享内存的代码中，receiver能否正确读出sender发送的字符串？如果把其中互斥的代码删除，观察实验结果有何不同？如果在发送和接收进程中打印输出共享内存地址，他们是否相同，为什么？  
代码在question4文件夹中的Sender.cpp和Receiver.cpp中  
可以准确读出，实验结果如图：  
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/4.1.png)  
把互斥代码删掉，会变成无序重复输出，如图所示：  
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/4.4.png)  
共享不同，如图所示：  
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/4.6.png)  
因为操作系统给进程分配的是虚拟内存，相应每个进程会有自己的虚拟内存分配地址。所以，虽然我们打印出来的内存地址不一样，但是它们实际映射的物理内存地址是一样的。  


b)有名管道和无名管道通信系统调用是否已经实现了同步机制？通过实验验证，发送者和接收者如何同步的。比如，在什么情况下，发送者会阻塞，什么情况下，接收者会阻塞？  
代码在question4文件夹中的pipe.cpp，fifo_rcv.cpp和fifo_snd.cpp中  
结果如图：  
无名管道：多用于亲缘关系进程间通信，方向为单向；为阻塞读写；通信进程双方退出后自动消失  
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/4.8.png)  
有名管道：对于以只读方式打开的FIFO文件，如果open调用是阻塞的，除非有一个进程以写方式打开同一个FIFO，否则它不会返回；如果open调用是非阻塞的的，则即使没有其他进程以写方式打开同一个FIFO文件，open调用将成功并立即返回。对于以只写方式打开的FIFO文件，如果open调用是阻塞的，open调用将被阻塞，直到有一个进程以只读方式打开同一个FIFO文件为止；如果open调用是非阻塞的，open总会立即返回，但如果没有其他进程以只读方式打开同一个FIFO文件，open调用将返回-1，并且FIFO也不会被打开。  
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/4.6.png)  


c）消息通信系统调用是否已经实现了同步机制？通过实验验证，发送者和接收者如何同步的。比如，在什么情况下，发送者会阻塞，什么情况下，接收者会阻塞？  
代码在question4文件夹中的client.cpp和server.cpp中  
结果如图：  
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/4.7.png)  
验证阻塞机制：写进程在此机制中不会被阻塞，其写入的字符串会一直被添加至队列的末端，而读进程会从队列的首端一直读取消息，消息节点一旦被读取便会移除队列。当队列中不含其需要类型的消息时便会阻塞。  
![image text](https://github.com/ToTkr2/Lab3/blob/master/img/4.3.png)  

---
5）阅读Pintos操作系统，找到并阅读进程上下文切换的代码，说明实现的保存和恢复的上下文内容以及进程切换的工作流程。  
1.有新生线程生成

thread_create();           

 

在thread_creat()中，创建新线程的过程是：  

init_thread()(设置线程的状态是THREAD_BLOCKED，设置线程的初始优先级,将此线程放入all_list队列中))--->设置线程的上下文环境---> thread_unblock()（解阻塞该线程，放入ready_list队列中）  

状态转换是block->ready->running或者block->ready  
当前线程自己改变优先级

thread_set_priority()        //thread.c  



此函数是修改当前线程的优先级，那么当增高优先级时，自然还是最高优先级线程，不做处理，当优先级下降时，可能不是系统中的最高优先级，需要进行处理。  


状态转换是running或者running->block    
thread的结构体定义：  

struct thread

{

tid_t tid;   /* Thread identifier. */

enum thread_status status;   /* Thread state. */

char name[16];    /* Name (for debugging purposes). */

uint8_t *stack;    /* Saved stack pointer. */

int priority;     /* Priority. */

struct list_elem allelem;     /* List element for all threads list. */

struct list_elem elem;        /* List element. */

}  
每一个thread都会有一个专属于自己的elem成员，这个elem成员用于作为各种队列元素来代表自己  
通过使用list_entry（）函数得到了一个线程（list_entry(...)->priority）,list_entry（）的三个传入参数list_begin(&ready_list)，struct thread,elem，list_entry()函数通过elem在ready_list中查找，找到了begin元素相应的thread.其实list_entry()函数的作用就是如此，不仅可以在ready_list中查找，还可以在all_list,和waiters中查找相应的实体结构。  

参考资料https://blog.csdn.net/lhyer/article/details/44728487


