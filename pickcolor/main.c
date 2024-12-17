#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_W 800
#define WINDOW_H 800
void doMouseLeftDown(SDL_Renderer *render)
{
    int x, y;
    static int cnt;
    SDL_Surface* surface; 
    uint32_t pixel;
    uint8_t r, g, b, a;

    SDL_GetMouseState(&x, &y);

    surface = SDL_CreateRGBSurfaceWithFormat(0, 800, 800, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(render, NULL, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);

    pixel = ((Uint32*)surface->pixels)[y * (surface->pitch / 4) + x];
    SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

    printf("%#6x ", (r << 16 | g << 8 | b));
    fflush(stdout);
    if(++cnt == 4)
    {
	cnt = 0;
	printf("\n");
    }

    SDL_FreeSurface(surface);
}

void accept_event(SDL_Renderer* render, int *quit)
{
    SDL_Event e;
    static int cnt = 0;

    while (SDL_PollEvent(&e))
    {
	switch (e.type) {
	    case SDL_QUIT:
		*quit = 1;
		break;
	    case SDL_MOUSEBUTTONDOWN:
		if (e.button.button == SDL_BUTTON_LEFT) {
		    doMouseLeftDown(render);
		}
		break;
	}
    }
}

int main(int argc, char* argv[])
{
    int quit = 0;

    if (argc < 2) {
	printf("usage: %s <picture>\n", argv[0]);
	return -1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Couldn't initialize SDL: %s\n", SDL_GetError());
	exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Couldn't initialize SDL: %s\n", SDL_GetError());
	goto freesdl;
    }

    SDL_Window *window = SDL_CreateWindow("Pick Color", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    if (!window) {
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Couldn't open %d x %d window: %s\n", 800, 800, SDL_GetError());
	goto freeimage;
    }

    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!render) {
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Couldn't create render: %s\n", SDL_GetError());
	goto freewindow;
    }

    SDL_Surface* imageSurface = IMG_Load(argv[1]);
    if (!imageSurface) {
	printf("IMG_Load Error: %s\n", IMG_GetError());
	goto freerender;
    }

    // 创建纹理
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, imageSurface    );
    SDL_FreeSurface(imageSurface);

    if (!texture) {
	printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
	goto freerender;
    }

    // 获取图片的原始宽高
    int imgWidth, imgHeight;
    SDL_QueryTexture(texture, NULL, NULL, &imgWidth, &imgHeight);

    // 目标区域，即窗口大小 (800x800)
    SDL_Rect destRect = {0, 0, 800, 800};

    SDL_RenderClear(render);
    SDL_RenderCopy(render, texture, NULL, &destRect);
    SDL_RenderPresent(render);

    while (!quit)
    {
	accept_event(render, &quit);

	// 对帧率要求不高, 大约30fps, 足够了.
	SDL_Delay(33);
    }

    SDL_DestroyTexture(texture);

freerender:
    SDL_DestroyRenderer(render);
freewindow:
    SDL_DestroyWindow(window);
freeimage:
    IMG_Quit();
freesdl:
    SDL_Quit();
    return 0;
}
