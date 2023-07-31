#include <SDL.h>
#include <SDL_ttf.h>
#include <iconv.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024
#define main    main

int utf8_to_ucs4(char* inbuf, size_t inlen, char* outbuf, size_t outlen) {
    iconv_t cd;
    char* pin = inbuf;
    char* pout = outbuf;
    size_t outbytesleft = outlen;

    cd = iconv_open("UCS-2LE", "UTF-8");
    if (cd == (iconv_t)-1) {
        perror("iconv_open");
        return -1;
    }

    memset(outbuf, 0, outlen);
    if (iconv(cd, &pin, &inlen, &pout, &outbytesleft) == (size_t)-1) {
        perror("iconv");
        return -1;
    }

    iconv_close(cd);
    return 0;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL_ttf example", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("Zpix.ttf", 36);
    if (font == NULL) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return 1;
    }

    char source[] = "你好，世界！";
    char dest[BUFFER_SIZE];
    if (utf8_to_ucs4(source, strlen(source), dest, BUFFER_SIZE) < 0) {
        printf("Conversion failed.\n");
        return -1;
    }

    SDL_Color color = {255, 255, 255, 0};
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, "测试", color);
    if (surface == NULL) {
        printf("TTF_RenderUNICODE_Solid Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

//    // 设置目标矩形的位置和大小
//    SDL_Rect dst;
//    dst.x = 200; // 位置：窗口的宽度的一半减去纹理的宽度的一半
//    dst.y = 150; // 位置：窗口的高度的一半减去纹理的高度的一半
//    dst.w = 400; // 宽度：400像素
//    dst.h = 300; // 高度：300像素

//    SDL_RenderClear(renderer);
//// 传递目标矩形给 SDL_RenderCopy
//    SDL_RenderCopy(renderer, texture, NULL, &dst);
//    SDL_RenderPresent(renderer);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000); // Wait for 3 seconds

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
