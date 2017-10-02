#include <jni.h>
#include <android/log.h>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_mixer.h"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)
//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct SDL_Window *window = NULL;
struct SDL_Renderer *render = NULL;

struct SDL_Texture *background = NULL;
struct SDL_Texture *image = NULL;

struct SDL_Surface *bmp = NULL;

/*
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or NULL if something went wrong.
 */
SDL_Texture* loadTexture(const char *file, SDL_Renderer *render) {
    struct SDL_Texture *texture = NULL;
    //Load the image
    bmp = SDL_LoadBMP(file);
    if (bmp == NULL) {
        LOG_E("SDL_LoadBMP failed %s", SDL_GetError());
    }
    //If the loading went ok, convert to texture and return the texture
    texture = SDL_CreateTextureFromSurface(render, bmp);
    SDL_FreeSurface(bmp);
    if (texture == NULL) {
        LOG_E("SDL_CreateTextureFromSurface failed %s", SDL_GetError());
    }
    return texture;
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    //Query the texture to get its width and height to use
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);//指定图像硬件的纹理tex绘制位置
    SDL_RenderCopy(ren, tex, NULL, &dst);//把硬件加速的纹理tex复制给渲染器ren
}

int main(int argc, char *argv[]) {
    char *filefolder = "/storage/emulated/0/";
    char *background_temp = "background.bmp";
    char *image_temp = "image.bmp";
    LOG_E("natvie_SDL %s", filefolder);
    //char *background_file = "/storage/sdcard0/background.bmp";
    char *background_file = (char*) malloc(
            strlen(filefolder) + strlen(background_temp) + 1);
    strcpy(background_file, filefolder);
    strcat(background_file, background_temp);

    //char *image_file = "/storage/sdcard0/image.bmp";
    char *image_file = (char*) malloc(
            strlen(filefolder) + strlen(image_temp) + 1);
    strcpy(image_file, filefolder);
    strcat(image_file, image_temp);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        LOG_E("SDL_Init failed %s", SDL_GetError());
    }

    window = SDL_CreateWindow("lesson2", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        LOG_E("SDL_CreateWindow failed %s", SDL_GetError());
    }

    render = SDL_CreateRenderer(window, -1,
                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == NULL) {
        LOG_E("SDL_CreateRenderer failed %s", SDL_GetError());
    }

    background = loadTexture(background_file, render);
    image = loadTexture(image_file, render);

    //Clear the window
    SDL_RenderClear(render);

    //Get the width and height from the texture so we know how much to move x,y by
    //to tile it correctly
    int bW, bH;
    SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
    //We want to tile our background so draw it 4 times
    renderTexture(background, render, 0, 0);
    renderTexture(background, render, bW, 0);
    renderTexture(background, render, 0, bH);
    renderTexture(background, render, bW, bH);
    //Draw our image in the center of the window
    //We need the foreground image's width to properly compute the position
    //of it's top left corner so that the image will be centered
    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(image, render, x, y);
    //Update the screen
    SDL_RenderPresent(render);
    SDL_Delay(2000);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(image);
//    SDL_DestroyTexture(render);
//    SDL_DestroyTexture(window);
    SDL_Quit();
    return 0;
}
