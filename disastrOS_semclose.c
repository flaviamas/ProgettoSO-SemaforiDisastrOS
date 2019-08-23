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

if (desc == NULL){
    running->syscall_retvalue = DSOS_ERESOURCENOFD ;
    perror("Error, semaphore not owned by the Application");
    return;
    }

List_detach(&running->sem_descriptors,(ListItem *)desc); //elimino il descrittore dalla lista dei descrittori del processo

running->last_sem_fd --;

Semaphore * sem = desc->semaphore; //risalgo al semaforo dal descrittore

SemDescriptorPtr * ptr = desc->ptr; //prendo il puntatore del descrittore

List_detach(&sem->descriptors,(ListItem *) ptr); //lo levo dalla lista
SemDescriptorPtr_free(desc->ptr); //libero la memoria del puntatore
SemDescriptor_free(desc); //libero la memoria del descrittore
Semaphore_free(sem);
running->syscall_retvalue = 0;
return;
}
