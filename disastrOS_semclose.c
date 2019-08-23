#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){
 int id = running->syscall_args[0];
Semaphore * sem = SemaphoreList_byId(&semaphores_list,id);
if (sem == NULL){
    running->syscall_retvalue = DSOS_ERESOURCENOFD ;
    perror("Error, semaphore not owned by the Application");
    errno = DSOS_ERESOURCENOFD;
    return;
    }


SemDescriptor * desc = SemDescriptorPtr_free(running->last_sem_fd);
SemDescriptorPtr * ptr = List_find(&sem->descriptors, running);
sem = List_detach(&semaphores_list,(ListItem *) sem);


}
