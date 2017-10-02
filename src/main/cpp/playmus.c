#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#ifdef unix
#include <unistd.h>
#endif

#include "SDL.h"
#include "SDL_mixer.h"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif


static int audio_open = 0;
static Mix_Music *music = NULL;
static int next_track = 0;

void CleanUp(int exitcode) {
    if( Mix_PlayingMusic() ) {
        Mix_FadeOutMusic(1500);
        SDL_Delay(1500);
    }
    if ( music ) {
        Mix_FreeMusic(music);
        music = NULL;
    }
    if ( audio_open ) {
        Mix_CloseAudio();
        audio_open = 0;
    }
    SDL_Quit();
    exit(exitcode);
}

void Menu(void) {
    char buf[10];
    LOG_E("Available commands: (p)ause (r)esume (h)alt volume(v#) > ");
    fflush(stdin);
    if (scanf("%s",buf) == 1) {
        switch(buf[0]){
        case 'p': case 'P':
            Mix_PauseMusic();
            break;
        case 'r': case 'R':
            Mix_ResumeMusic();
            break;
        case 'h': case 'H':
            Mix_HaltMusic();
            break;
        case 'v': case 'V':
            Mix_VolumeMusic(atoi(buf+1));
            break;
        }
    }
    LOG_E("Music playing: %s Paused: %s\n", Mix_PlayingMusic() ? "yes" : "no",
           Mix_PausedMusic() ? "yes" : "no");
}

int main(int argc, char *argv[])
{
    int audio_rate;
    Uint16 audio_format;
    int audio_channels;
    int audio_buffers;
    int audio_volume = MIX_MAX_VOLUME;
    int looping = 0;
    int interactive = 0;
    int rwops = 0;

    /* Initialize variables */
    audio_rate = 44100;
    audio_format = MIX_DEFAULT_FORMAT;
    audio_channels = 2;
    audio_buffers = 2048;
    /* Initialize the SDL library */
    if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) {
        LOG_E(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
        return(255);
    }
    /* Open the audio device */
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
        LOG_E(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return(2);
    } else {
        Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
        LOG_E("Opened audio at %d Hz %d bit %s (%s), %d bytes audio buffer\n", audio_rate,
            (audio_format&0xFF),
            (audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo" : "mono",
            (audio_format&0x1000) ? "BE" : "LE",
            audio_buffers );
    }
    audio_open = 1;
    /* Set the music volume */
    Mix_VolumeMusic(audio_volume);
    /* Set the external music player, if any */
    Mix_SetMusicCMD(SDL_getenv("MUSIC_CMD"));
        next_track = 0;
        /* Load the requested music file */
        if ( rwops ) {
            music = Mix_LoadMUS_RW(SDL_RWFromFile("/storage/emulated/0/err.wav", "rb"), SDL_TRUE);
        } else {
            music = Mix_LoadMUS("/storage/emulated/0/err.wav");
        }
        if ( music == NULL ) {
            LOG_E(stderr, "Couldn't load err.wav\n", SDL_GetError());
            CleanUp(2);
        }
        /* Play and then exit */
        LOG_E("Playing /storage/emulated/0/err.wav\n");
        Mix_FadeInMusic(music,looping,2000);
        while ( !next_track && (Mix_PlayingMusic() || Mix_PausedMusic()) ) {
            if(interactive)
                Menu();
            else
                SDL_Delay(100);
        }
        Mix_FreeMusic(music);
        music = NULL;
        /* If the user presses Ctrl-C more than once, exit. */
        SDL_Delay(500);

    CleanUp(0);
    /* Not reached, but fixes compiler warnings */
    return 0;
}
