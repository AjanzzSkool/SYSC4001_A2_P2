#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>



bool is_multiple3(long x) {
	return (x % 3) == 0;
}

int main() {
    pid_t child = fork();
    if (child < 0) {
	perror("fork");
	return 1;
    }
	//execute p2
    if (child == 0) {
        execl("./process2", "process2", (char*)nullptr);
    }

    long counter1 = 0;


    for (;;) {
		//counter 1 is multiple of 3
        if (is_multiple3(counter1)) {
            std::cout << "Cycle number: " << counter1<< " – " << counter1 << " is a multiple of 3 " << "\n";
        } else {  // counter 1 isn't a multiple if 3
            std::cout << "Cycle number: " << counter1 << "\n";
        }
        counter1++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
