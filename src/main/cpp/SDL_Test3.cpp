#include <jni.h>
#include <android/log.h>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)

SDL_Rect rect;
bool moveRight = true;
bool moveDown = true;
void updatePosition(){
    if (moveRight == true)
    {
        rect.x++;
        if (rect.x+rect.w>=800)
        {
            moveRight=false;
        }
    }
    else{
        rect.x--;
        if (rect.x<=0)
        {
            moveRight=true;
        }
    }
    if (moveDown == true)
    {
        rect.y++;
        if (rect.y+rect.h>=600)
        {
            moveDown=false;
        }
    }
    else{
        rect.y--;
        if (rect.x<=0)
        {
            moveDown=true;
        }
    }
}

int main(int argc,char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                          800,600,
                                          SDL_WINDOW_SHOWN);
    SDL_Surface* catSurface = IMG_Load("/storage/emulated/0/test.png");
    if (catSurface == NULL) {
        LOG_E("SDL_LoadBMP failed %s", SDL_GetError());
    }
    //通过渲染器更新窗口，避免刷新时鼠标一闪一闪的,参数三：硬件加速
    SDL_Renderer * rend = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    //先清空下
    SDL_RenderClear(rend);
    //Renderer不像用Blilt填充，需要先转换为纹理
    SDL_Texture* cat = SDL_CreateTextureFromSurface(rend,catSurface);
    rect.x=0;
    rect.y=0;
    rect.w=catSurface->w;
    rect.h=catSurface->h;
    SDL_Event event;
    bool quit = false;
    while(quit == false){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){//点击关闭
                quit = true;
            }
        }
        updatePosition();
        SDL_Delay(5);//避免过度渲染,CPU耗
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend,cat,NULL,&rect);
        SDL_RenderPresent(rend);
    }
    SDL_FreeSurface(catSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}