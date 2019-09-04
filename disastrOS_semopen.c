#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"
#include "disastrOS_globals.h"
#include "disastrOS_constants.h"

void internal_semOpen(){
    int id = running->syscall_args[0];
    int count = running->syscall_args[1];

    Semaphore* sem = SemaphoreList_byId(&semaphores_list,id);
    if(!sem) {

        sem = Semaphore_alloc(id,count);
        List_insert(&semaphores_list,semaphores_list.last,(ListItem *) sem);
    }

    SemDescriptor * desc = SemDescriptor_alloc(running->last_sem_fd, sem, running); //alloco il descrittore
    running->last_sem_fd ++; //aumento l'ultimo fd dei semafori
    List_insert(&running->sem_descriptors,running->sem_descriptors.last, (ListItem *) desc);

    SemDescriptorPtr * ptr = SemDescriptorPtr_alloc(desc);
    desc->ptr = ptr;
    List_insert(&sem->descriptors, sem->descriptors.last, (ListItem*) ptr);
    running->syscall_retvalue = desc->fd;
    return;
}
