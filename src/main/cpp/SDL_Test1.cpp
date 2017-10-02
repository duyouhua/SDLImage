#include <jni.h>
#include <android/log.h>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_mixer.h"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)
int main(int argc,char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    //参数二、三，窗口出现位置，可以为固定值，参数四、五窗口大小
    SDL_Window* window = SDL_CreateWindow("SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                          800,600,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        LOG_E("SDL_CreateWindow failed %s", SDL_GetError());
    }
    SDL_Rect rect;
    rect.x=100;
    rect.y=100;
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    char* fp="/storage/emulated/0/background.bmp";//非bmp格式只能通过SDL_image库的SDL_Texture来渲染
    SDL_Surface* image =SDL_LoadBMP(fp);
    if (image == NULL) {
        LOG_E("SDL_LoadBMP failed %s", SDL_GetError());
    }
    //将image附着到surface上
//    SDL_BlitSurface(image,NULL,surface,NULL);//以(0,0)作为左上角
    SDL_BlitSurface(image,NULL,surface,&rect);//人为控制显示位置
    SDL_UpdateWindowSurface(window);
    //暂停3秒，防止一闪而过
    SDL_Delay(5000);
    SDL_FreeSurface(image);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}