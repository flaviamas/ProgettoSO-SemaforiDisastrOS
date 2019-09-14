#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
    int fd = running->syscall_args[0];
    SemDescriptor * desc = SemDescriptorList_byFd(&running->sem_descriptors,fd);
    if(!desc){
        printf("Error, semaphore not owned by the Application\n");
        running->syscall_retvalue = DSOS_ERESOURCENOFD;
        return;
    }
    Semaphore * sem = desc->semaphore;
    SemDescriptorPtr * ptr;
    sem->count+=1;
    if(sem->count <= 0) {
    ptr = (SemDescriptorPtr*)List_detach(&sem->waiting_descriptors,sem->waiting_descriptors.first);

    SemDescriptorPtr *aux = (SemDescriptorPtr*) List_insert (&sem->descriptors, sem->descriptors.last,(ListItem*)ptr);
    if(!aux){

        printf("List insert Error sempost \n");
        running->syscall_retvalue = DSOS_ER_INSERT;
    }
    PCB* proc = ptr->descriptor->pcb;
    proc->status = Ready;
    PCB* process=(PCB*) List_detach(&waiting_list, (ListItem*) proc);
    List_insert(&ready_list,ready_list.last,(ListItem*)process);
    }

    running->syscall_retvalue = 0;
    return;

}
