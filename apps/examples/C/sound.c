#include <ports.h>


int main() {
    for (int i = 10000; i != 500; i--) {
        play_sound(i);
        sleep(1);
    }
    nosound();

    return 0;
}