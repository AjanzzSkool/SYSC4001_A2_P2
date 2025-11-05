#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>




int main() {
    pid_t child = fork();
    int exit_status;
    if (child < 0) {
        perror("fork");
        return 1;
    }
        //execute p2
    if (child == 0) {
        execl("./process2p3", "process2p3", (char*)nullptr);
    }


// wait for p2 to end then exit
    wait(&exit_status);
    return 0;

}
