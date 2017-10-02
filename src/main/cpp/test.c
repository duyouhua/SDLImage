#include "SDL.h"
#include "audio.h"

int main(int argc,char* argv[]){
    /* Initialize only SDL Audio on default device */
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        return 1; 
    }
    /* Init Simple-SDL2-Audio */
    initAudio();
    /* Play music and a sound */
    playMusic("/storage/emulated/0/err.wav", SDL_MIX_MAXVOLUME);
    playSound("/storage/emulated/0/Note1.wav", SDL_MIX_MAXVOLUME / 2);
    /* While using delay for showcase, don't actually do this in your project */
    SDL_Delay(1000);
    /* Override music, play another sound */
    playMusic("/storage/emulated/0/Note2.wav", SDL_MIX_MAXVOLUME);
    SDL_Delay(2000);
    pauseAudio();
    SDL_Delay(2000);
    unpauseAudio();
    playSound("sounds/door2.wav", SDL_MIX_MAXVOLUME / 2);
    SDL_Delay(7000);
    /* End Simple-SDL2-Audio */
    endAudio();
    SDL_Quit();
    return 0;
}
