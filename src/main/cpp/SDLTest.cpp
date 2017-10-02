#include <jni.h>
#include <android/log.h>

#define LOG_I(...) __android_log_print(ANDROID_LOG_ERROR , "main", __VA_ARGS__)

#include "SDL.h"
#include "SDL_log.h"
#include "SDL_main.h"
#include "SDL_image.h"
//int main(int argc, char *argv[]) {
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1) {
//        LOG_I("SDL_Init failed %s", SDL_GetError());
//        return 0;
//    }
//
//    //编写你的代码
//
//    LOG_I("SDL_Init Success!");
//
//    SDL_Quit();
//    return 0;
//}


int main(int argc, char *argv[]) {
    //第一步：初始化SDL多媒体框架->SDL_Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1) {
        LOG_I("SDL_Init failed %s", SDL_GetError());
        return 0;
    }
    LOG_I("SDL_Init Success!");

    //第二步：初始化SDL窗口
    //参数一：窗口名称->要求必需是UTF-8编码
    //参数二：窗口在屏幕上面X坐标
    //参数三：窗口在屏幕上面Y坐标
    //参数四：窗口在屏幕上面宽
    int width = 640;
    //参数五：窗口在屏幕上面高
    int height = 352;
    //参数六：窗口状态(打开的状态:SDL_WINDOW_OPENGL)
    SDL_Window* sdl_window = SDL_CreateWindow("Dream开车",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              width ,
                                              height,
                                              SDL_WINDOW_OPENGL);//SDL_WIFULLSCREEN全屏，SDL_WIFULLSHOW
    if (sdl_window == NULL){
        LOG_I("窗口创建失败");
        return 0;
    }

    //第三步：创建渲染器->渲染窗口(OpenGL ES)
    //最新一期VIP课程
    //参数一：渲染目标窗口
    //参数二：从哪里开始渲染(-1:默认从第一个为止开始)
    //参数三：渲染类型
    //SDL_RENDERER_SOFTWARE:软件渲染
    //...
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);
    /*======================================================================*/
//    SDL_Surface* sdl_surface = SDL_GetWindowSurface(sdl_window);
//    SDL_Surface* img=SDL_LoadBMP("/storage/emulated/0/test.png");
//    SDL_Rect rect;
//    rect.x=100;
//    rect.y=100;
//    //将图片贴到surface上
//    //参数二为以图片当中的位置开始切割，参数四是贴到窗口的位置
//    SDL_LowerBlitScaled(img,NULL,sdl_surface,&rect);
//    //刷新窗口
//    SDL_UpdateWindowSurface(sdl_window);
//    SDL_Surface* bigcoin=IMG_Load("/storage/emulated/0/test.png");//png和gif图片支持背景透明
    /*======================================================================*/
    //第四步：创建纹理
    //参数一：纹理目标渲染器
    //参数二：渲染格式
    //参数三：绘制方式(SDL_TEXTUREACCESS_STREAMING:频繁绘制)
    //参数四：纹理宽
    //参数五：纹理高
    SDL_Texture * sdl_texture = SDL_CreateTexture(sdl_renderer,
                      SDL_PIXELFORMAT_IYUV,
                      SDL_TEXTUREACCESS_STREAMING,
                      width,
                      height);

    //第五步：设置纹理数据->播放YUV视频
    //着色器语言(着色器)、渲染器、纹理等等...
    //第一点：打开YUV文件(手机：)
    FILE* yuv_file = fopen("/storage/emulated/0/DreamTestFile/Test.yuv","rb+");
    if (yuv_file == NULL){
        LOG_I("文件打开失败");
        return 0;
    }

    //第二点：循环读取YUV视频像素数据格式每一帧画面->渲染->设置纹理数据
    //定义缓冲区(内存空间开辟多大?)
    //Y:U:V = 4 : 1 : 1
    //假设：Y = 1.0  U = 0.25  V = 0.25
    //宽度：Y + U + V = 1.5
    //换算：Y + U + V = width * height * 1.5
    char buffer_pix[width * height * 3 / 2];

    //定义渲染器区域
    SDL_Rect sdl_rect;
    while (true){
        //一行一行的读取
        fread(buffer_pix, 1, width * height * 3 / 2, yuv_file);
        //判定是否读取完毕
        if (feof(yuv_file)){
            break;
        }

        //设置纹理数据
        //参数一：目标纹理对象
        //参数二：渲染区域(NULL:表示默认屏幕窗口宽高)
        //参数三：视频像素数据
        //参数四：帧画面宽
        SDL_UpdateTexture(sdl_texture, NULL, buffer_pix, width);

        //第六步：将纹理数据拷贝到渲染器
        sdl_rect.x = 0;
        sdl_rect.y = 0;
        sdl_rect.w = width;
        sdl_rect.h = height;

        //先清空
        SDL_RenderClear(sdl_renderer);
        //再渲染
        SDL_RenderCopy(sdl_renderer,sdl_texture,NULL,&sdl_rect);

        //第七步：显示帧画面
        SDL_RenderPresent(sdl_renderer);

        //第八步：延时渲染(没渲染一帧间隔时间)
        SDL_Delay(20);
    }


    //第九步：是否内存
    fclose(yuv_file);

    SDL_DestroyTexture(sdl_texture);

    SDL_DestroyRenderer(sdl_renderer);


    //第十步：推出SDL程序
    SDL_Quit();
    return 0;
}
