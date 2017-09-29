
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "config.h"

/* note
 *
 * faire une struct Sprite, avec un élement .frames qui indique combien de frames il y a pour l'animation,
 * via `current_frame % sprite.frames` on obtient le numéro de la frame à afficher, comme ça ça marche simplement
 * sans ajout aussi sur des sprites statiques, puisqu'un modulo 1 donnera toujours 0, donc la première frame !
 *
 * Du coup c'est possible de charger un sprite en deux fichiers : l'image qui contient les frames consécutives, et
 * un fichier de description qui contient le nom de l'image, la taille d'une frame dans l'image en pixels, ainsi
 * qu'une liste dans quel ordre afficher les frames. Du coup au moment de lancer le jeu tout ce qu'il faudra faire
 * c'est lire tous les fichiers de descripteurs de sprite, charger l'image indiquée, lire les données de la taille
 * d'une frame, lire l'ordre des frames et compter combien il y en a, du coup c'est possible d'avoir une struct du type :
 *
 * struct {
 *	SDL_Texture *texture;    -> la texture of course
 *	SDL_Rect *frames_coords; -> les coordonnées et la taille dans la texture source de chaque frame, à voir comment le génerer ceci-dit, via le descripteur ?
 *	int *frames_index;       -> l'index de chaque frame, pour une animation bête en 2 frames ça sera {0, 1}
 *	int frames;              -> la taille du tableau précédent
 *	int frame_offset;        -> éventuellement indiquer de combien de frames décaler l'animation, pas que tout bouge en même temps
 *
 * } Sprite;
 * */


/* note 2
 * * pourquoi avoir plusieurs images par sprite, et ne pas simplement avoir une seule grosse image avec tous les sprites dedans ? §§
 * */


enum {
	TILE_NONE,
	TILE_GRASS,
	TILE_WATER,
	TILE_WALL,
	TILE_DIRT,
	TILE_SAND,
	TILE_TREE,
	TILE_GRAVEL,
	TILE_ESIZE
} TileType;

/*                        r,   g,   b */
static int tile_colors[TILE_ESIZE][3] = {
	[TILE_NONE]   = {  0,   0,   0},
	[TILE_GRASS]  = {  0, 255,  70},
	[TILE_WATER]  = {  0, 100, 255},
	[TILE_WALL]   = {128, 128, 128},
	[TILE_DIRT]   = {128, 128,  20},
	[TILE_SAND]   = {200, 200,  10},
	[TILE_TREE]   = { 10, 150,  50},
	[TILE_GRAVEL] = {150,   0, 120}
};

enum {
	FLAG_SHOULD_QUIT,
	FLAG_ESIZE
} Flags;

static int flags[FLAG_ESIZE];


static void  draw_map(SDL_Renderer *renderer, int *map);
static void  keyboard_poll(SDL_Event *event, int *keys);


#define len(x) (sizeof(x) / sizeof(x[0]))
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define key_pressed(keys, keysold, key) (keys[key] && !keysold[key])
#define key_released(keys, keysold, key) (!keys[key] && keysold[key])
#define rrand(min, max) (rand() % (max - min + 1) + min) /* [min;max] */


void
draw_map (SDL_Renderer *renderer, int *map)
{
	SDL_Rect dest = {
		.w = CONF_MAP_TILESIZE,
		.h = CONF_MAP_TILESIZE
	};

	for (int x = 0; x < CONF_MAP_X; x++) {
		for (int y = 0; y < CONF_MAP_Y; y++) {
			dest.x = x * CONF_MAP_TILESIZE;
			dest.y = y * CONF_MAP_TILESIZE;

			int tile = map[x + (y * CONF_MAP_X)];

			if (tile > TILE_ESIZE - 1)
				eprintf("Invalid tile (%d,%d) : %d\n", x, y, tile);
			else
				SDL_SetRenderDrawColor(renderer, (Uint8) tile_colors[tile][0], (Uint8) tile_colors[tile][1], (Uint8) tile_colors[tile][2], 255);

			SDL_RenderFillRect(renderer, &dest);
		}
	}
}

void
keyboard_poll (SDL_Event *event, int *keys)
{
	SDL_Scancode keycode = event->key.keysym.scancode; /* not necessary, but makes the following code easier to read and write */

	if (keycode < SDL_NUM_SCANCODES) { /* out of bounds safety check */
		if (event->type == SDL_KEYDOWN) keys[keycode] = 1;
		if (event->type == SDL_KEYUP)   keys[keycode] = 0;
	}
}


int
main (int argc, char **argv)
{
	int retval = EXIT_SUCCESS;

	int running = 1;

	int keys[SDL_NUM_SCANCODES] = {0}; /* group those two in a struct ? */
	int keysold[SDL_NUM_SCANCODES] = {0};

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Event event;

	int map[CONF_MAP_X * CONF_MAP_Y] = {0};

	srand((size_t)time(NULL));

	for (int x = 0; x < CONF_MAP_X; x++) {
		for (int y = 0; y < CONF_MAP_Y; y++) {
			map[x + (y * CONF_MAP_X)] = rrand(1, TILE_ESIZE - 1);
		}
	}

	/* init */

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		eprintf("Cannot initialize SDL : %s\n", SDL_GetError());
		retval = EXIT_FAILURE;
		goto cleanup_exit;
	}


	window = SDL_CreateWindow(CONF_APP_NAME, CONF_WINPOS, CONF_WINPOS, CONF_WINW, CONF_WINH, 0);
	if (!window) {
		eprintf("Cannot create window : %s\n", SDL_GetError());
		retval = EXIT_FAILURE;
		goto cleanup_img;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		eprintf("Cannot create renderer : %s\n", SDL_GetError());
		retval = EXIT_FAILURE;
		goto cleanup_window;
	}

	/* actual game loop */

	while (running) {

		/* read all events, update keyboard state */

		memcpy(keysold, keys, sizeof(keys));

		while (SDL_PollEvent(&event)) {
			keyboard_poll(&event, keys);

			if (event.type == SDL_QUIT)
				flags[FLAG_SHOULD_QUIT] = 1;
		}

		/* react to events */

		if (key_released(keys, keysold, CONF_KEY_EXIT) || flags[FLAG_SHOULD_QUIT])
			running = 0;

		/* draw */

		SDL_SetRenderDrawColor(renderer, CONF_CLEAR_COLOR, 255);
		SDL_RenderClear(renderer);

		draw_map(renderer, map);

		SDL_RenderPresent(renderer);

		/* dirty time management 'cause I'm lazy */
		SDL_Delay(100);
	}

	/* cleanup */

//cleanup_renderer:
	SDL_DestroyRenderer(renderer);
cleanup_window:
	SDL_DestroyWindow(window);
cleanup_img:
	SDL_Quit();
cleanup_exit:
	return retval;
}

