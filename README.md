# ProgettoSO-SemaforiDisastrOS
Implementing the following system calls
   // creates a semaphore in the system, having num semnum
   // the semaphore is accessible throughuot the entire system
   // by its id.
   // on success, the function call returns semnum (>=0);
   // in failure the function returns an error code <0
   - int s=DisastrOS_semOpen(int semnum)

   //releases from an application the given
   // returns 0 on success
   // returns an error code if the semaphore is not owned by the application
   - int DisastrOS_semClose(int semnum)

   //decrements the given semaphore
   //if the semaphore is 0, the caller is put onto wait
   //returns an error code
   - int DisastrOS_semWait(int semnum);

   //increments the given semaphore
   //if the semaphore was at 0, and some other thread was waiting
   //the thread is resumed
   //returns 0 on success, an error code on failure 
   int DisastrOS_semPost(int semnum);

