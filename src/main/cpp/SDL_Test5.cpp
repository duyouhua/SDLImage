#include <jni.h>
#include <android/log.h>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)
typedef struct
{
    int x;
    int y;
    int r;
    int speed;
    int alpha;
} Spot;
void drawSpot(Spot* spot,SDL_Renderer* rend,SDL_Texture* circle){
    SDL_Rect rect;
    rect.x=spot->x;
    rect.y=spot->y;
    rect.w=spot->r;
    rect.h=spot->r;
    SDL_SetTextureAlphaMod(circle,spot->alpha);
    SDL_RenderCopy(rend,circle,NULL,&rect);
}
void moveSpot(Spot* spot){
    spot->y=spot->y+spot->speed;
    if (rand()%2==0){
        spot->x=spot->x+spot->speed;
    } else {
        spot->x=spot->x-spot->speed;
    }
    if (spot->x>600||spot->x<0){
        spot->x=0;
    }
    if (spot->y>800)
    {
        spot->y=0;
    }
}
int main(int argc,char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                          800,600,
                                          SDL_WINDOW_SHOWN);
    //colorkey将一种颜色设置为透明色
    SDL_Surface* surfacecat = IMG_Load("/storage/emulated/0/rain.png");
    SDL_Surface* surfaceBlack = IMG_Load("/storage/emulated/0/black.bmp");
    SDL_Renderer * rend = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    SDL_Texture* cat = SDL_CreateTextureFromSurface(rend,surfacecat);
    SDL_Texture* black = SDL_CreateTextureFromSurface(rend,surfaceBlack);

    SDL_SetTextureBlendMode(cat,SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(black,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(black,255);//数字越小，越长尾巴
    SDL_SetTextureColorMod(cat,39,211,48);//设置cat的颜色
    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=5;
    rect.h=5;
    SDL_Rect rectwindow;
    rectwindow.x=0;
    rectwindow.y=0;
    rectwindow.w=600;
    rectwindow.h=800;
    Spot spot[50];
    int i;
    for(i=0;i<50;i++){
        spot[i].x=rand()%600;
        spot[i].y=rand()%800;
        spot[i].r=rand()%4+1;
        spot[i].speed=rand()%2+1;
        spot[i].alpha=rand()%255;
    }
    bool quit=false;
    SDL_Event event;
    while(quit == false){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){//点击关闭
                quit = true;
            } else if (event.type == SDL_MOUSEMOTION){
                rect.x=event.motion.x-rect.w/2;
                rect.y=event.motion.y-rect.h/2;
            }
        }
        SDL_RenderCopy(rend,black,NULL,&rectwindow);
        for (i = 0; i < 50; ++i){
            drawSpot(&spot[i],rend,cat);
            moveSpot(&spot[i]);
        }
        SDL_RenderPresent(rend);
        SDL_Delay(10);
    }
    SDL_FreeSurface(surfacecat);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}