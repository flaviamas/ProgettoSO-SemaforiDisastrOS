#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
    int fd = running->syscall_args[0];
    SemDescriptor * desc = SemDescriptorList_byFd(&running->sem_descriptors,fd);
    if (desc == NULL){
        running->syscall_retvalue = DSOS_ERESOURCENOFD;
        printf("Error, semaphore not owned by the Application\n");
        return;
    }
    Semaphore * sem = desc->semaphore;
    SemDescriptorPtr * ptr = desc->ptr;

    sem->count-=1;
    if(sem->count < 0) {
       SemDescriptorPtr *aux =  List_detach(&sem->descriptors,(ListItem*) ptr);
       if(!aux){
           printf("List detach error in semwait");
        running->syscall_retvalue =DSOS_ER_DETACH;
    }
        aux = List_insert(&sem->waiting_descriptors,sem->waiting_descriptors.last,(ListItem*) ptr);
        if(!aux){
            printf("List insert error in semwait");
        running->syscall_retvalue =DSOS_ER_INSERT;
    }
        running->status = Waiting;
        List_insert(&waiting_list, waiting_list.last,(ListItem*) running);
        PCB *process = (PCB*)List_detach(&ready_list,(ListItem*)ready_list.first);
        if(!process){
            printf("List detach error in semwait");
        running->syscall_retvalue =DSOS_ER_DETACH;
    }
        running = process;
    }
    running->syscall_retvalue = 0;
    return;

}
