## Part 1

1. What is the basic difference between a process and a thread? (Hint: Lecture 11 Threads)
    # ANSWER: The difference between a process and a thread is that threads are mini processes that cannot exist on their own as they do not have their own data segment or heap and must exist inside a process. Threads also have cheap creation, context switching and good communication amongst themselves where as processes' actions are more expensive

2. What is a critical regions/section? (Hint: Lecture 12 Concurrency)
    # ANSWER: A critical region or section is the part of a program that needs to access shared memory, files or variables 

3. Like Gridlock in traffic flow at a four-way intersection with stop signs, every car is waiting for the car in front of it to move while none of them is movable. What is a deadlock? (Hint: Lecture 13 Concurrency II)
    # ANSWER: A deadlock is when no work can be completed because processes are all waiting to get resources or when all the processes are waiting for an event that only one process can cause  

4. What does it mean for a resource to be preemptible? What does it mean for a resource to be non-preemptible? What are example resources of each? (Hint: Lecture 13 Concurrency II)
    # ANSWER: For a resources to be preemptible it has to be able to be removed from a process without any problems but for a resource to be non-preemptible once given to a process cant be taken from it without cooperation from the process. A preemptive resource is RAM because can just swap the pages to disk without issue. A non-preemptible resource would be a mutex beacuse in order to allow a proces to use the critical region/section of a program , the mutex cant be taken away from that process to ensure rhat only one process has access to a critical section/region at a time. so until the process is done using the critical section/region of a program the mutex cant be taken away from it


5. In terms of state-change, what happens to a process which calls sleep()? What about when wakeup() is called? What happens to the lock that process was holding?
    # ANSWER: when a process calls sleep() it is put into a state of sleeping where it is not actively using the cpu allowing for other processes that are ready to have cpu time and the current lock that the process had would be released to be given to other ready processes. When a process calls wakeup() the sleeping process is changed to a ready/excecutable state and is now ready for cpu time and when the process that was previously executing is put to a different state other than running, it's lock is released, and given to the process that was woken up.

6.  Spinlock and mutex are two types of locking mechanisms. _____ relinquishes the CPU when waiting for the lock, while the other one does not. (Hint: Lecture 12 Concurrency)
    # ANSWER: Mutex

## Part 2.1

1. Locate the usages of variable num_threads. This variable indicates how many threads will be created. From what you’ve learned in argument parsing, what decides the value of variable num_threads?
# ANSWER: Within the context of arg parsing if the length of argc is 2 it will convert the value called when the program was started in the command line into an integer stored in argv[1] and it will set the number of threads to that number 

2. During put phase and get phase, some threads are created. Take insertion phase as an example: read the line
        (1) pthread_create(&threads[i], NULL, put_phase, (void *)i);.

    What is the third argument put_phase used for? What’s the relationship between the fourth argument (void *)i and put_phase’s first parameter void *arg?
    
# ANSWER: The third argument put_phase is used to represent the function the current thread will excecute which will consist of inserting into the hashtable within put_phase. The relationship between the fourth arg and put_phase's first arg void *arg by representing what information witll be passed into put_phase when ran which will be the current thread's id number within put_phase  

3. During retrieval phase, explain the line
        (2) pthread_join(threads[i], (void **)&lost_keys[i]);

    by filling the blanks. Each blank expects a word or a phrase. (Hint: Lecture 13 Concurrency II, page 31.)

    • ______ joins the i-th thread in threads table. Joining means it _____ the i-th thread
    to finish their execution before continuing its own execution. Select for the second blank from
        (A) kills; (B) wakes up; (C) waits; (D) blocks.

    # ANSWER 1: Main thread | (C) waits;

    
    • Knowing that &lost_keys[i] is the address of ______ _ upon the exit of i-th thread, before the execution of line (2), lost_keys[i] contains _____. After the execution of line (2), lost_keys[i] contains _____.

    # ANSWER 2: the i-th thread id that just exited | undefined | number of lost keys for the i-th thread


4. insert and retrieve are defined in mutex.c, mutex_bucket.c or spin.c. They are called in test.c. Will insert operation possibly co-occur with retrieve operation? "Co-occuring" means one thread
    is inserting while another is retrieving. Just answer "yes" or "no".
    # ANSWER: Yes 

## Part 2.2

1. Why are some keys lost?
    # ANSWER: Some keys are lost due to race conditions occuring where multiple threads needed the same key and so keys that were allocated in the hashtable for a corresponding thread was not found when trying to retrieve it, so therefore those keys were lost

2. Do we need to modify insert in order to not lose the keys? Why or why not? 
    # ANSWER: Yes because in order to resolve potetial race conditions with multiple threads wanting access to a critical region/section at the same time or by wanting the same key, we must implement mutexes or locks to stop the threads from accessing the critical region/section at the same time by making threads wait until the key is released before we insert the key/val into the hash table 

3. Do we need to modify retrieve in order to not lose the keys? Why or why not?
    # ANSWER: No becuase bucket_entry *retrieve(int key) job is to search in the hashtable by each key and return it and return null if its not present therefore, theres no other funcitonality to be done there to prevent keys from being lost since if its not inserted properly it cant be retrieved and the retrieval of a key doesnt not involve any race conditions 

4. For the function(s) you answered "yes" in 2 and 3, modify mutex.c so that the critical region is protected with a mutex
    # ANSWER: see modified void insert(int key, int val) in mutex.c. The critical region/section is now protected with a mutex by ensuring that the lock is only set after acquring the lock and waiting for the other threads to finish and then release the lock and then insert into the hash table

## Part 2.3

1. If two inserts or two retrieves are being done on different buckets in parallel, is a lock still needed at the moment, respectively? Why or why not?
    # ANSWER: No a lock isnt needed in this case because since either thread is inserting or retrieving on different buckets they wont ever create a race condition since they are not accessing the same critical region even tho they are doing it at the same time. so either thread wont ever override the other therefore no lock is needed to stop them from accessing the same bucket 

## Part 2.4

1. For function(s) you answer "yes" in question 2 and 3 in part 2.2, update them accordingly with spinlock APIs
    # ANSWER: Insert function in spin.c was the only function updated because race conditions only happen while inserting. the spin lock added has ensured that the threads acquire the spinlock wait for the other threads to release their lock and then insert into the hash table 

2. Does insertion time performance significantly change comparing to part 2.2? Does retrieval time
performance significantly change comparing to part 2.2? Try to test both versions of implementation
with 200 threads multiple times. Write down any significant performance differences or similarity (i.e.,
the number of seconds it takes to insert/retrieve all the keys)

# ANSWER: Inswertion term with spinlock is significantly longer than mutex insertion time. This is because in spinlock it uses up cpu time waiting for a thread to release the lock for the next thread to use, whereas mutex yields the cpu and puts a thread to sleep and waits and allows another thread to use cpu time until the lock gets released. 
# Retrieval time across both methods are nearly the same with spin lock being slightly faster than mutex. retrieval is a one step process and doesnt need locks to be done therefore the times are similar  

# Mutex: Inserted 2000000 keys in 2.510774 seconds            |  2.261268 seconds     |   2.453186 seconds
#        Retrieved 2000000/2000000 keys in 2.096907 seconds   |  2.094508 seconds     |   2.000393 seconds
#                     
# Spinlock: Inserted 2000000 keys in 63.192989 seconds           |  47.063436 seconds   |   60.206176 seconds
#           Retrieved 2000000/2000000 keys in 1.900433 seconds   |  1.903210 seconds    |   2.110330 seconds
