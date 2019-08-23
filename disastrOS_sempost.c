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

    Semaphore * sem = desc->semaphore;


    if(sem->count < 0) {

    }
    sem->count+=1;
    running->syscall_retvalue = 0;
    return;

}
