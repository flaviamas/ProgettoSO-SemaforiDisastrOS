#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){

    int fd = running->syscall_args[0];

    SemDescriptor * desc = SemDescriptorList_byFd(&running->sem_descriptors,fd); //mi faccio passare l'id del descrittore e cerco se sta nella lista

    if (!desc){
        running->syscall_retvalue = DSOS_ERESOURCENOFD ;
        perror("Error, semaphore not owned by the Application");
        return;
        }

    List_detach(&running->sem_descriptors,(ListItem *)desc); //elimino il descrittore dalla lista dei descrittori del processo

    Semaphore * sem = desc->semaphore; //risalgo al semaforo dal descrittore

    SemDescriptorPtr * ptr = (SemDescriptorPtr*) List_detach(&sem->descriptors, (ListItem*) desc->ptr); //prendo il puntatore del descrittore


    if (!ptr){
        running->syscall_retvalue = DSOS_ERESOURCENOFD ;
        perror("Error, semaphore not owned by the Application");
        return;
        }
        if(sem->descriptors.size == 0 && sem->waiting_descriptors.size == 0){
        List_detach(&semaphores_list, (ListItem*) sem);
        Semaphore_free(sem);
        }

  int ret = SemDescriptorPtr_free(ptr);

    ret = SemDescriptor_free(desc); //libero la memoria del descrittore

    running->syscall_retvalue = 0;
    return;
}
