
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/shm.h>
#include <cstdlib>
#include "shared_memory.hpp"
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
bool is_multiple(long x, long y){
	return (x%y) == 0;
}




int main(int argc, char* argv[]) {

    int semid = atoi(argv[2]);
    int shmid = atoi(argv[1]);

    auto* p = (ShMData*)shmat(shmid, nullptr, 0);

//dont do anything till counter reaches 100
    while (true) {
	lock(semid);
	int counter2 =  p->counter;
	unlock(semid);
	if (counter2  >= 100){
		break;
	}
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

        for (;;) {
        	lock(semid);
        	int counter2 = p->counter;
        	int multiple2 = p->multiple;
        	unlock(semid);

        	if (is_multiple(counter2, multiple2)) {
            		std::cout << "Process 2: " << counter2<< " – " << counter2 << " is a multiple of " << multiple2 << "\n";
        	} else {
            		std::cout << "Process 2: " << counter2 << "\n";
        	}

        	std::this_thread::sleep_for(std::chrono::milliseconds(500));

        	lock(semid);
        	int curr_count = p->counter;
        	unlock(semid);

        	if (curr_count > 500){
                	break;
        	}
    	}

    	shmdt(p);
    	return 0;

}

