#include "doomkeys.h"

#include "doomgeneric.h"

#include <stdio.h>




#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned char key){
    switch (key){
        case 0:
            key = KEY_ENTER;
            break;
        case 1:
            key = KEY_ESCAPE;
            break;
        case 2:
            key = KEY_LEFTARROW;
            break;
        case 3:
            key = KEY_RIGHTARROW;
            break;
        case 4:
            key = KEY_UPARROW;
            break;
        case 5:
            key = KEY_DOWNARROW;
            break;
        case 6:
            key = KEY_FIRE;
            break;
        case 7:
            key = KEY_USE;
            break;
        case 8:
            key = KEY_RSHIFT;
            break;
        default:
            key = tolower(key);
            break;
    }

    return key;
}

static void addKeyToQueue(int pressed, unsigned char keyCode){
    unsigned char key = convertToDoomKey(keyCode);

    unsigned short keyData = (pressed << 8) | key;

    s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
    s_KeyQueueWriteIndex++;
    s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}



void DG_Init(){
    // window creation
    const char windowClassName[] = "DoomWindowClass";
    const char windowTitle[] = "Doom";

}

void DG_DrawFrame(){
}

void DG_SleepMs(uint32_t ms){
    Sleep(ms);
}

uint32_t DG_GetTicksMs(){
    return GetTickCount();
}

int DG_GetKey(int* pressed, unsigned char* doomKey){
    if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex) {
        //key queue is empty

        return 0;
    }
    else {
        unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
        s_KeyQueueReadIndex++;
        s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

        *pressed = keyData >> 8;
        *doomKey = keyData & 0xFF;

        return 1;
    }
}

void DG_SetWindowTitle(const char * title) {
        
}