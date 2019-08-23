  #include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"
#include "disastrOS_globals.h"

void internal_semOpen(){
    // do stuff :)
    int id = running->syscall_args[0]; //utilizzo syscall perché è la struttura dati adatta a contenere i valori relativi ad una syscall
    int count = running->syscall_args[1];

    Semaphore* sem = SemaphoreList_byId(&semaphores_list,id);
    if(!sem) {  //non ho trovato il semaforo nella lista, lo posso creare e inserire nella lista
        sem = Semaphore_alloc(id,count);
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
