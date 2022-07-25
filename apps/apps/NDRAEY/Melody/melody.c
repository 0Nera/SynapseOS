/* Melody app by Andrey Pavlenko aka NDRAEY */
#include <ports.h>

#define A4  440 // A4 note is 440Hz
#define Ad4 466 // A#4 note is 466Hz
#define B4	493 // ...
#define C5	523
#define Cd5	554
#define D5	587
#define Dd5	622
#define E5	659
#define F5	698
#define Fd5	739
#define G5	783
#define Gd5	830
#define A5	880
#define Ad5	932
#define B5	987
#define C6	1046
#define Cd5	1108

typedef struct Note {
    unsigned short freq;
    unsigned short duration;
} Note;

int main(){
    Note notes[64] = {
        {C4, 100},
        {Dd5, 100},
        {C4, 100},
        {C4, 50},
        {F4, 100},
        {C4, 100},
        {Ad4, 100},
        {C4, 100},
        {G4, 100},
        {C4, 100},
        {C4, 100},
        {Gd4, 100},
        {Gd4, 100},
        
    };

    for(int i = 0; i < 64; i++) {
        if(notes[i]==0) {continue;}
        play_sound(notes[i][0], notes[i][1]);
    }
    /*
    for (int i = 10000; i != 500; i--) {
        play_sound(i);
        sleep(1);
    }
    nosound();
    */
    return 0;
}