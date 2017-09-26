#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


int main(int argc, char *argv[]) {


	SDL_Init(SDL_INIT_EVERYTHING);

	/*WINDOW + RENDERER + TEXTURE*/
	SDL_Window *window;
	SDL_Renderer *renderer;

	if (SDL_CreateWindowAndRenderer(960, 720, SDL_WINDOW_SHOWN, &window, &renderer) != 0 || !window || !renderer)
		goto Fail;
	SDL_SetWindowTitle(window, "HistoRPG");

	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 200, 200);

	/*DRAW*/
	SDL_SetRenderDrawColor(renderer, 30, 40, 100, 255);
	SDL_RenderClear(renderer); //background

	SDL_SetRenderTarget(renderer, texture); //target the texture

	SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255);
	SDL_Rect rectangle = {50, 50, 100, 100};
	SDL_RenderFillRect(renderer, &rectangle);
	////

	SDL_SetRenderTarget(renderer, NULL); //back to renderer

	/*DISPLAY*/
	SDL_RenderPresent(renderer);
	SDL_Delay(4000);
	/*END*/
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;

	Fail:
	if (renderer)
		SDL_DestroyRenderer(renderer);
	if (window)
		SDL_DestroyWindow(window);
	fprintf(stderr, "Error SDL : %s", SDL_GetError());
	SDL_Quit();

	return EXIT_FAILURE;
}

/*Most SDL2 functions return 0 as success condition*/
	

	/*DRAW THINGS*/
	/*
	SDL_Point point = {0, 0};
	SDL_Rect rectangle;

	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = 60;
	rectangle.h = 20;
	*/

/*	SDL_Rect rect[50];

	for(int i = 0; i < 50; i++)
{ 
    rect[i].w = 50;
    rect[i].h = 50;
    rect[i].x = 100 * (i % 5) + 50 * ((i / 5) % 2);
    rect[i].y = 50 * (i / 5);
}
	SDL_RenderFillRects(renderer, rect, 50); */