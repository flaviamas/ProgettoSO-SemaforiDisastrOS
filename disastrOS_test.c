 #include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"

#define BUFF_SIZE 20
#define ITERATION  10
#define PRODUCER_ID 1
#define CONSUMER_ID 2
#define WRITE_ID 3
#define READ_ID 4
int buffer[BUFF_SIZE];
int write_idx = 0;
int read_idx = 0;
int count  = 1;
void f_producer(int consumer,int producer,int read, int write){

    disastrOS_semWait(producer);
    disastrOS_semWait(write);
    printf("PRODUCO %d NELLA CELLA %d\n",count,write_idx);
    buffer [write_idx] = count;
    count ++;
    write_idx = (write_idx+1)%BUFF_SIZE;
    disastrOS_semPost(write);
    disastrOS_semPost(consumer);

}

void f_consumer(int consumer,int producer, int read, int write){
    disastrOS_semWait(consumer);
    disastrOS_semWait(read);
    int var  = buffer[read_idx];

    printf("LEGGO %d NELLA CELLA %d\n",var,read_idx);
    read_idx = (read_idx+1) %BUFF_SIZE;

    disastrOS_semPost(read);
    disastrOS_semPost(producer);

}

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
printf("Apro i semafori\n");
int consumer = disastrOS_semOpen(CONSUMER_ID,0);
int producer = disastrOS_semOpen(PRODUCER_ID,BUFF_SIZE);
int read = disastrOS_semOpen(READ_ID,1);
int write = disastrOS_semOpen(WRITE_ID,1);




 for(int i=0; i<ITERATION; i++){

    if(disastrOS_getpid()%2==0)  f_producer(consumer,producer,read,write);
    else f_consumer(consumer,producer,read,write);
}


  printf("PID: %d, terminating\n", disastrOS_getpid());

  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iterate %d\n", disastrOS_getpid(), i);

    disastrOS_sleep(20);
  }
  disastrOS_semClose(producer);
  disastrOS_semClose(consumer);
  disastrOS_semClose(read);
  disastrOS_semClose(write);
  disastrOS_exit(disastrOS_getpid()+1);
}




void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);


  printf("I feel like to spawn 10 nice threads\n");
  int alive_children=0;
  for (int i=0; i<3; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    int fd=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd);
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }


  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }  for(int i = 0; i < BUFF_SIZE; i++)
    printf("%d\t",buffer[i]);



  printf("shutdown!");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
