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
    //colorkey将一种颜色设置为透明色
    SDL_Surface* surfacecat = IMG_Load("/storage/emulated/0/test.bmp");
    SDL_Surface* surfaceBlack = IMG_Load("/storage/emulated/0/black.bmp");
    SDL_Renderer * rend = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    SDL_Texture* cat = SDL_CreateTextureFromSurface(rend,surfacecat);
    SDL_Texture* black = SDL_CreateTextureFromSurface(rend,surfaceBlack);

    SDL_SetTextureBlendMode(cat,SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(black,SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(surfaceBlack,1);
//    SDL_SetTextureAlphaMod(black,1);//数字越小，越长尾巴
    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=surfacecat->w;
    rect.h=surfacecat->h;
    SDL_Rect rectwindow;
    rectwindow.x=0;
    rectwindow.y=0;
    rectwindow.w=800;
    rectwindow.h=600;
    bool quit=false;
    SDL_Event event;
    while(quit == false){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){//点击关闭
                quit = true;
            } else if (event.type == SDL_MOUSEMOTION) {
                rect.x=event.motion.x-rect.w/2;
                rect.y=event.motion.y-rect.h/2;
            }
        }
        SDL_RenderCopy(rend,black,NULL,&rectwindow);
        SDL_RenderCopy(rend,cat,NULL,&rect);
        SDL_RenderPresent(rend);
    }
    SDL_FreeSurface(surfacecat);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
//    SDL_Init(SDL_INIT_VIDEO);
//    SDL_Window* window = SDL_CreateWindow("SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
//                                          800,600,
//                                          SDL_WINDOW_SHOWN);
//    //colorkey将一种颜色设置为透明色
//    SDL_Surface* surface = IMG_Load("/storage/emulated/0/test.bmp");
//    SDL_Renderer * rend = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
//    SDL_Texture* cat = SDL_CreateTextureFromSurface(rend,surface);
//    int alpha=255;
//    SDL_SetTextureBlendMode(cat,SDL_BLENDMODE_BLEND);
//    SDL_Rect rect;
//    rect.x=0;
//    rect.y=0;
//    rect.w=surface->w;
//    rect.h=surface->h;
//    bool quit=false;
//    SDL_Event event;
//    while(quit == false){
//        while(SDL_PollEvent(&event)){
//            if(event.type == SDL_QUIT){//点击关闭
//                quit = true;
//            }
//        }
//        //先清空下
//        SDL_RenderClear(rend);
////        SDL_SetSurfaceAlphaMod(cat,alpha);
//        SDL_SetTextureAlphaMod(cat,alpha);//取值0~255之间
//        alpha-=10;
//        //再赋值
//        SDL_RenderCopy(rend,cat,NULL,&rect);
//        //最后显示
//        SDL_RenderPresent(rend);
//        SDL_Delay(10);
//    }
//    SDL_FreeSurface(surface);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//    return 0;
}