#include <jni.h>
#include <SDL_mixer.h>
#include "SDL.h"
#include <android/log.h>
#include "SDL_main.h"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)

static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play
void my_audio_callback(void *userdata, Uint8 *stream, int len);
struct
{
    SDL_AudioSpec spec;
    Uint8 *sound;               /* Pointer to wave data */
    Uint32 soundlen;            /* Length of wave data */
    int soundpos;               /* Current play position */
} wave;
static int done = 0;
/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    SDL_Quit();
    exit(rc);
}

void SDLCALL
fillerup(void *unused, Uint8 * stream, int len)
{
    Uint8 *waveptr;
    int waveleft;
    printf("callback len:%d\n",len);
    /* Set up the pointers */
    waveptr = wave.sound + wave.soundpos;
    waveleft = wave.soundlen - wave.soundpos;

    /* Go! */
    while (waveleft <= len) {
        SDL_memcpy(stream, waveptr, waveleft);
        SDL_PauseAudio(1);
        return;
    }
    SDL_memcpy(stream, waveptr, len);
    wave.soundpos += len;

}

int main(int argc,char* argv[]){
    SDL_Init(SDL_INIT_AUDIO);

    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0){
        LOG_E("Mix_OpenAudio failed %s", SDL_GetError());
    }
    Mix_Music* sound=Mix_LoadMUS("/storage/emulated/0/err.wav");
    if (sound == NULL){
        LOG_E("文件打开失败");
        return 0;
    }
    Mix_PlayMusic(sound,1);//播放1次
    SDL_Delay(10000);    //一定要有此句话，否则程序会立即关闭，而不会听到声音
//    int i;
//    char filename[4096]="/storage/emulated/0/err.wav";
//
//    /* Enable standard application logging */
//    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
//
//    /* Load the SDL library */
//    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
//        return (1);
//    }
//
//    /* Load the wave file into memory */
//    if (SDL_LoadWAV(filename, &wave.spec, &wave.sound, &wave.soundlen) == NULL) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s\n", argv[1], SDL_GetError());
//        quit(1);
//    }
//
//    wave.spec.callback = fillerup;//设置回调函数
//
//    /* Show the list of available drivers */
//    SDL_Log("Available audio drivers:");
//    for (i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
//        SDL_Log("%i: %s", i, SDL_GetAudioDriver(i));
//    }
//
//    /* Initialize fillerup() variables */
//    if (SDL_OpenAudio(&wave.spec, NULL) < 0) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open audio: %s\n", SDL_GetError());
//        SDL_FreeWAV(wave.sound);
//        quit(2);
//    }
//
//    SDL_Log("Using audio driver: %s\n", SDL_GetCurrentAudioDriver());
//    printf("SDL_AudioSpec.samples:%d",wave.spec.samples);
//    printf("SDL_AudioSpec sample size:%d",wave.spec.format);//AUDIO_S16LSB
//    printf("SDL_AudioSpec.channels:%d",wave.spec.channels);
//    printf("SDL_AudioSpec.size:%d",wave.spec.size);//跟回调函数的len相等应该是样品数*样品所占字节数
//    /* Let the audio run */
//    SDL_PauseAudio(0);
//    while ((SDL_GetAudioStatus() == SDL_AUDIO_PLAYING))//获取音频状态
//        SDL_Delay(1000);
//
//    /* Clean up on signal */
    SDL_CloseAudio();//关掉音频进程以及音频设备
//    SDL_FreeWAV(wave.sound);//释放数据由SDL_LoadWAV申请的
    SDL_Quit();
//    printf("=========over==========\n");
//    system("pause");
    return 0;
//    if( Mix_PlayingMusic() == 0 ) {
//        //Play the music
//        LOG_E("Play the music");
//        Mix_PlayMusic( sound, -1 );
//    }

//    bool quit=false;
//    while(quit == false){
//        while(SDL_PollEvent(&event)){
//            if(event.type == SDL_QUIT){//点击关闭
//                quit = true;
//            }
//        }
//        SDL_Delay(5);
//        SDL_RenderPresent(rend);
//    }
    // local variables
//    static Uint32 wav_length; // length of our sample
//    static Uint8 *wav_buffer; // buffer containing our audio file
//    static SDL_AudioSpec wav_spec; // the specs of our piece of music
//    /* Load the WAV */
//    // the specs, length and buffer of our wav are filled
//    char* MUS_PATH="/storage/emulated/0/err.wav";
//    if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL ){
//        LOG_E("Load file fail\n");
//        return 1;
//    }
//    // set the callback function
//    wav_spec.callback = my_audio_callback;
//    wav_spec.userdata = NULL;
//    // set our global static variables
//    audio_pos = wav_buffer; // copy sound buffer
//    audio_len = wav_length; // copy file length
//    /* Open the audio device */
//    if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
//        LOG_E("Couldn't open audio: %s\n", SDL_GetError());
//        exit(-1);
//    }
//    /* Start playing */
//    SDL_PauseAudio(0);
//    // wait until we're don't playing
//    while ( audio_len > 0 ) {
//        SDL_Delay(100);
//    }
//    // shut everything down
//    SDL_CloseAudio();
//    SDL_FreeWAV(wav_buffer);
}
// audio callback function
// here you have to copy the data of your audio buffer into the requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void my_audio_callback(void *userdata, Uint8 *stream, int len) {
    if (audio_len ==0)
        return;
    len = ( len > audio_len ? audio_len : len );
    //SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
    audio_pos += len;
    audio_len -= len;
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//    return 0;
}