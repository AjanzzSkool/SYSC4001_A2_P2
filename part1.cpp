#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
	perror("fork");
	return 1;
    }
	//p2 (fork returns a  0  so its a child)
    if (pid == 0) {
        long counter2 = 0;
        for (;;) {
            std::cout << "P2 " << getpid() << ": " << counter2++ << "\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    } else { //P1 since fork returns child pid
        unsigned long counter1 = 0;
        for (;;) {
            std::cout << "P1 " << getpid() << ": " << counter1++ << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}
