#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "shared_memory.hpp"
#include <sys/shm.h>
#include <cstdio>
#include <sys/wait.h>



bool is_multiple(long x, long y) {
	return (x % y) == 0;
}

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(ShMData), 0666);
    auto* p = (ShMData*)shmat(shmid, nullptr, 0);

 //Set the counter and multiple in the shared memory
    p->counter = 0;
    p->multiple = 3;
    pid_t child = fork();
    if (child < 0) {
	perror("fork");
	return 1;
    }
	//execute p2
    if (child == 0) {
	char shmid_str[20];
	sprintf(shmid_str, "%d", shmid);
        execl("./process2p4", "process2p4", shmid_str, (char*)nullptr);
    }

    


    for (;;) {
		
        if (is_multiple(p->counter, p->multiple)) {
            std::cout << "Process 1: " << p->counter<< " – " << p->counter << " is a multiple of " << p->multiple << "\n";
        } else {  
            std::cout << "Process 1: " << p->counter << "\n";
        }
        (p->counter)++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (p->counter > 500){
		break;
	}
    }
    

    int exit_status;
    wait(&exit_status);
    shmdt(p);
    shmctl(shmid, IPC_RMID, nullptr);
    
}
