#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>


bool is_multiple3(long x){
	return (x%3) == 0;
}
int main() {
    long counter2 = 0;
    for (;;) {
        if (is_multiple3(counter2)) {
            std::cout << "Cycle number: " << counter2<< " – " << counter2 << " is a multiple of 3 " << "\n";
        } else {
            std::cout << "Cycle number: " << counter2 << "\n";
        }
        counter2--;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (counter2 == -501){
		return 0;
	}
    }

}
