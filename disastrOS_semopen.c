  #include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"
#include "disastrOS_globals.h"
#include "disastrOS_constants.h"

void internal_semOpen(){
    // do stuff :)
    int id = running->syscall_args[0];
    int count = running->syscall_args[1];

    Semaphore* sem = SemaphoreList_byId(&semaphores_list,id);
    if(sem == 0) {
        printf("SONO NELL'IF\n");
        sem = Semaphore_alloc(id,count);
        printf("Allocato\n");
        if(sem == 0) {
            printf("ALLOCAZIONE FALLITA");
            running->syscall_retvalue = DSOS_ERSEMOPEN;
            return;
        }
        List_insert(&semaphores_list,semaphores_list.last,(ListItem *) sem);

    }
    else{ //printo errore
        perror("Error, semaphore already existing");
        errno = DSOS_ERESOURCENOFD;
        running->syscall_retvalue = DSOS_ERESOURCENOFD;
        return;
        }


    SemDescriptor * desc = SemDescriptor_alloc(running->last_sem_fd, sem, running); //alloco il descrittore
    running->last_sem_fd ++; //aumento l'ultimo fd dei semafori
    if(!desc) perror ("error in the allocation of the sem descriptor");
    List_insert(&running->sem_descriptors,running->sem_descriptors.last, (ListItem *) desc);

    SemDescriptorPtr * ptr = SemDescriptorPtr_alloc(desc);
    if(ptr) perror("error in the allocation of the sem desc ptr");
    desc->ptr = ptr;
    List_insert(&sem->descriptors, sem->descriptors.last, (ListItem*) ptr);
    running->syscall_retvalue = desc->fd;
    return;

}
