#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
  // do stuff :)
      int fd = running->syscall_args[0];
    SemDescriptor * desc = SemDescriptorList_byFd(&running->sem_descriptors,fd);
    if( desc == NULL){
        perror("Error, semaphore not owned by the Application");
        running->syscall_retvalue = DSOS_ERESOURCENOFD;
        return;
    }
    Semaphore * sem = desc->semaphore;
    SemDescriptorPtr * ptr;
    sem->count+=1;
    if(sem->count <= 0) {
        List_insert(&ready_list,ready_list.last,(ListItem*)running);
       ptr = (SemDescriptorPtr*)List_detach(&sem->waiting_descriptors,sem->waiting_descriptors.first);
       List_insert (&sem->descriptors, sem->descriptors.last,(ListItem*)ptr);
       List_detach(&waiting_list, (ListItem*) ptr->descriptor->pcb);
       running->status = Ready;
       running = ptr->descriptor->pcb;
    }

    running->syscall_retvalue = 0;
    return;

}
