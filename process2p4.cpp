
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/shm.h>
#include <cstdlib>
#include "shared_memory.hpp"

bool is_multiple(long x, long y){
	return (x%y) == 0;
}

int main(int argc, char* argv[]) {


    int shmid = atoi(argv[1]);

    auto* p = (ShMData*)shmat(shmid, nullptr, 0);

//dont do anything till counter reaches 100
    while (p->counter < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    while (p->counter <= 500) {
	if (is_multiple(p->counter, p->multiple)) {
            std::cout << "Process 2: " << p->counter<< " – " << p->counter << " is a multiple of " << p->multiple << "\n";
        } else {
            std::cout << "Process 2: " << p->counter << "\n";
        }
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    shmdt(p);
    return 0;

}
