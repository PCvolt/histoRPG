#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include "definitions.h"


int init_graphics (SDL_Window **, SDL_Renderer **);
SDL_Texture *load_image (SDL_Renderer *, const char *);



int main (int argc, char **argv)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	SDL_Event event;

	int retval = EXIT_SUCCESS;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		eprintf("Cannot initialize SDL2 : %s\n", SDL_GetError());
		return EXIT_FAILURE; /* toujours quitter directement en cas d'early-fail */
	}

	if (init_graphics(&window, &renderer) < 0)
	{
		retval = EXIT_FAILURE;
		goto cleanup;
	}

	SDL_Texture *tiles = load_image(renderer, "sprites/simple_tiles.bmp");
	if (!tiles)
	{
		retval = EXIT_FAILURE;
		goto cleanup;
	}


	/* BOARD EXAMPLE*/ /*to be replaced *************************************************************/
	srand((size_t)time(NULL));

	Cell cell[225];
	for (int i = 0; i < 225; ++i)
	{
		cell[i].unit = NULL;
		cell[i].type = rand() % 6;
	}


	/* GAME RUNNING */
	int flags[FLAGS_ESIZE] = {0};
	GameState gamestate= {
		.running = 1,
		.nb_round = 0,
		.nb_unit = NULL
	};
	while (gamestate.running) 
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				flags[SHOULD_QUIT] = 1;
		}

		if (flags[SHOULD_QUIT])
			gamestate.running = 0;

		/*DRAW*/
		SDL_SetRenderDrawColor(renderer, 0, 5, 50, 255);
		SDL_RenderClear(renderer); //background


		SDL_Rect dest_rect = {
		dest_rect.x = 0,
		dest_rect.y = 0,
		dest_rect.w = 30,
		dest_rect.h = 30
		};


		for (int i = 0; i < 225; ++i)
		{
			SDL_RenderCopy(renderer, tiles, &sprites_rects[cell[i].type], &dest_rect);
			
			if (i % 15 == 14)
			{
				dest_rect.x = 0;
				dest_rect.y += 30;
			}
			else
				dest_rect.x += 30;
		}


		SDL_RenderPresent(renderer);
	}


	/* END */
cleanup:
	if (tiles)
		SDL_DestroyTexture(tiles);
	if (renderer)
		SDL_DestroyRenderer(renderer);
	if (window)
		SDL_DestroyWindow(window);

	SDL_Quit();

	return retval;
}


int init_graphics (SDL_Window **window, SDL_Renderer **renderer)
{
	*window = SDL_CreateWindow("HistoRPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!*window)
	{
		eprintf("Failed to create render : %s\n", SDL_GetError());
		return -1;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (!*renderer)
	{
		eprintf("Failed to create render : %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}


SDL_Texture *load_image (SDL_Renderer *renderer, const char *image_path)
{
	SDL_Surface *image_surface = SDL_LoadBMP(image_path);
	if (!image_surface) {
		eprintf("Failed to load image %s : %s\n", image_path, SDL_GetError());
		return NULL;
	}

	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_FreeSurface(image_surface);

	return image_texture; //<< on retourne l'addresse
}
