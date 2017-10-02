#include <jni.h>
#include <android/log.h>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)
int main(int argc,char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                          800,600,
                                          SDL_WINDOW_SHOWN);
    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    //SDL_Surface* coin = SDL_loadBMP("coin.bmp");
    //某些图片格式有白色背景，支持背景透明的格式有png，gif，需要SDL2Image函数库
    SDL_Surface* image = IMG_Load("/storage/emulated/0/test.png");
    if (image == NULL) {
        LOG_E("SDL_LoadBMP failed %s", SDL_GetError());
    }
    //将image附着到surface上
    for(int i=0;i<10;i++){
        SDL_BlitSurface(image,NULL,surface,&rect);
        rect.x=rect.x+100;
    }
    SDL_UpdateWindowSurface(window);
    SDL_Delay(3000);
    SDL_FreeSurface(image);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}