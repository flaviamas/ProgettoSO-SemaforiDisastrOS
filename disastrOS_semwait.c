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
    running->syscall_retvalue = DSOS_ERESOURCENOFD ;
    perror("Error, semaphore not owned by the Application");
    return;
    }
    Semaphore * sem = desc->semaphore;
    SemDescriptorPtr * ptr = desc->ptr;
    PCB * process;
    sem->count-=1;
    if(sem->count < 0) {
        List_detach(&sem->descriptors,(ListItem*) ptr);
        List_insert(&sem->waiting_descriptors,sem->waiting_descriptors.last,(ListItem*) ptr);
        running->status = Waiting;
        List_insert(&waiting_list, waiting_list.last,(ListItem*) runnning);
        process = (PCB*)List_detach(&ready_list,(ListItem*)ready_list.first);
    }
    running->syscall_retvalue = 0;
    return;

}
