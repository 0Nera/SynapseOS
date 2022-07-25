/* Melody app by Andrey Pavlenko aka NDRAEY */
#include <ports.h>
#include <stdio.h>

#define A2 110.00
#define Ad2 116.54
#define B2 123.47
#define C3 130.81
#define Cd3 138.59
#define D3 146.83
#define Dd3 155.56
#define E3 164.81
#define F3 174.61
#define Fd3 185.00
#define G3 196.00
#define Gd3 207.65
#define A3 220.00
#define Ad3 233.08
#define B3 246.94
#define C4 261.63
#define Cd4 277.18
#define D4 293.66
#define Dd4 311.13
#define E4 329.63
#define F4 349.23
#define Fd4 369.99
#define G4 392.00
#define Gd4 415.30
#define A4 440.00
#define Ad4 466.16
#define B4 493.88
#define C5 523.25
#define Cd5 554.37
#define D5 587.33
#define Dd5 622.25
#define E5 659.25
#define F5 698.46
#define Fd5 739.99
#define G5 783.99
#define Gd5 830.61
#define A5 880.00
#define Ad5 932.33
#define B5 987.77
#define C6 1046.50
#define Cd6 1108.73
#define D6 1174.66
#define Dd6 1244.51
#define E6 1318.51
#define F6 1396.91
#define Fd6 1479.98
#define G6 1567.98
#define Gd6 1661.22
#define A6 1760.00
#define Ad6 1864.66
#define B6 1975.53
#define C7 2093.00

typedef struct Note {
    unsigned short freq;
    unsigned short duration;
} Note;

int main(){
    Note notes[64] = {
        {C4, 150},
        {Dd4, 100},
        {C4, 100},
        {C4, 50},
        {F4, 100},
        {C4, 100},
        {Ad3, 100},
        {C4, 250},
        {G4, 100},
        {C4, 80},
        {C4, 80},
        {Gd4, 100},
        {G4, 100},
        {Dd4, 100},
        {C4, 100},
        {G4, 100},
        {C5, 100},
        {C4, 70},
        {Ad3, 90},
        {Ad3, 90},
        {Fd3, 100},
        {D4, 100},
        {C4, 500},
    };

    printf("Beverly Hills Theme - Axel F by NDRAEY\n");

    for(int i = 0; i < 23; i++) {
        //if(notes[i]==0) {continue;}
        play_sound(notes[i].freq);
        printf("%d ", notes[i].freq);
        sleep((notes[i].duration/8));
        nosound();
        sleep(2);
    }
    nosound();
    /*
    for (int i = 10000; i != 500; i--) {
        play_sound(i);
        sleep(1);
    }
    nosound();
    */
    return 0;
}