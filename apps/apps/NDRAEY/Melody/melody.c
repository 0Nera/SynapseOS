/* Melody app by Andrey Pavlenko aka NDRAEY */
#include <ports.h>
#include <stdio.h>

#define A2 110
#define Ad2 116
#define B2 123
#define C3 130
#define Cd3 138
#define D3 146
#define Dd3 155
#define E3 164
#define F3 174
#define Fd3 185
#define G3 196
#define Gd3 207
#define A3 220
#define Ad3 233
#define B3 246
#define C4 261
#define Cd4 277
#define D4 293
#define Dd4 311
#define E4 329
#define F4 349
#define Fd4 370
#define G4 392
#define Gd4 415
#define A4 440
#define Ad4 466
#define B4 493
#define C5 523
#define Cd5 554
#define D5 587
#define Dd5 622
#define E5 659
#define F5 698
#define Fd5 740
#define G5 784
#define Gd5 831
#define A5 880
#define Ad5 932
#define B5 988
#define C6 1046
#define Cd6 1108
#define D6 1174
#define Dd6 1244
#define E6 1318
#define F6 1396
#define Fd6 1479
#define G6 1567
#define Gd6 1661
#define A6 1760
#define Ad6 1864
#define B6 1975
#define C7 2093

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
    return 0;
}
