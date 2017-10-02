#include <jni.h>
#include <android/log.h>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)

int main(int argc,char* argv[]){
    int width=800;
    int height=600;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                          width,height,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer * rend = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend,255,255,255,255);//RGBA顺序
    //SDL_RenderDrawPoint(rend,100,100);//参数二三：坐标值
    //SDL_RenderDrawLine(rend,0,0,100,100);//rend,x1,y1,x2,y2
    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=100;
    rect.h=100;
    //SDL_RenderDrawRect(rend,&rect);
    //SDL_RenderFillRect(rend,&rect);//实心
    SDL_Event event;
    int x1=0;//保存上一次位置
    int y1=0;
    int state=1;//0表示按下鼠标，开始作画；1表示松开
    bool quit=false;
    while(quit == false){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){//点击关闭
                quit = true;
            } else if (event.type  == SDL_MOUSEMOTION){
                if (state==0){
                    int x=event.motion.x;
                    int y=event.motion.y;
                    //避免初次点击位置从(0,0)开始
                    if (x1==0 && y1==0){
                        x1=x;
                        y1=y;
                    }
                    //SDL_RenderDrawLine(rend,0,0,x,y);
                    //SDL_RenderDrawLine(rend,width/2,height/2,x,y);
                    //SDL_RenderDrawPoint(rend,x,y);有断续,不是连续的线条
                    SDL_RenderDrawLine(rend,x1,y1,x,y);
                    x1=x;
                    y1=y;
                }
            }  if (event.type==SDL_MOUSEBUTTONDOWN){
                state=0;
                //避免和上次首尾相连
                int x=event.motion.x;
                int y=event.motion.y;
                x1=x;
                y1=y;
            }  if (event.type==SDL_MOUSEBUTTONUP){
                state=1;
            }
        }
        SDL_RenderPresent(rend);
        SDL_Delay(10);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}