#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "shared_memory.hpp"
#include <sys/shm.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/sem.h>

union semun{int val;};

int lock(int semid){ 
	sembuf op{0,-1,SEM_UNDO};
	return semop(semid,&op,1); 
}

int unlock(int semid){
	sembuf op{0,+1,SEM_UNDO};
	return semop(semid,&op,1);
}

bool is_multiple(long x, long y) {
	return (x % y) == 0;
}

int main() {
    
    int shmid = shmget(IPC_PRIVATE, sizeof(ShMData), 0666);
    auto* p = (ShMData*)shmat(shmid, nullptr, 0);

    int semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    union semun arg;
    arg.val = 1; 
    semctl(semid, 0, SETVAL, arg);

    lock(semid);
    p->counter = 0;
    p->multiple = 3;
    unlock(semid);
    pid_t child = fork();
    if (child < 0) {
	perror("fork");
	return 1;
    }
	//execute p2
    if (child == 0) {
	char shmid_str[20];
	sprintf(shmid_str, "%d", shmid);
        char semid_str[20];
        sprintf(semid_str, "%d", semid);
        execl("./process2p4", "process2p4", shmid_str, semid_str, (char*)nullptr);
    }

    


    for (;;) {
	lock(semid);
	int counter1 = p->counter;
	int multiple1 = p->multiple;
	unlock(semid);

        if (is_multiple(counter1, multiple1)) {
            std::cout << "Process 1: " << counter1<< " – " << counter1 << " is a multiple of " << multiple1 << "\n";
        } else {
            std::cout << "Process 1: " << counter1 << "\n";
        }
	lock(semid);
        (p->counter)++;
	unlock(semid);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

	lock(semid);
	int curr_count = p->counter;
	unlock(semid);

	if (curr_count > 500){
		break;
	}
    }


    int exit_status;
    wait(&exit_status);
    shmdt(p);
    shmctl(shmid, IPC_RMID, nullptr);
    semctl(semid, 0, IPC_RMID);
}
