#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "board.h"
#include "interface.h"


int init_graphics (SDL_Window **window, SDL_Renderer **renderer, int img_flags)
{
	/* SDL_ INITS */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		eprintf("Cannot initialize SDL2 : %s\n", SDL_GetError());
		return -1;
	}

	if (TTF_Init())
	{
		eprintf("Cannot initialize TTF_Init : %s\n", TTF_GetError());
		return -1;
	}

	if ((IMG_Init(img_flags) & img_flags) != img_flags)
	{
		eprintf("Cannot initialize SDL_image : %s\n", IMG_GetError());
		return -1;
	}

	/* Window + Renderer */
	*window = SDL_CreateWindow("HistoRPG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (!*window)
	{
		eprintf("Failed to create window : %s\n", SDL_GetError());
		return -1;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (!*renderer)
	{
		eprintf("Failed to create renderer : %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

SDL_Texture *load_image (SDL_Renderer *renderer, const char *image_path)
{
	SDL_Surface *image_surface = IMG_Load(image_path);
	if (!image_surface) {
		eprintf("Failed to load image %s : %s\n", image_path, SDL_GetError());
		return NULL;
	}

	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_FreeSurface(image_surface);

	return image_texture;
}

SDL_Texture *load_text (SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *text)
{
	SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_FreeSurface(text_surface);
	return text_texture;
}

void stats_text (SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *text, SDL_Rect text_rect)
{
	SDL_Texture *text_texture;
	text_texture = load_text(renderer, font, color, text);

	SDL_QueryTexture(text_texture, NULL, NULL, &(text_rect.w), &(text_rect.h));
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_DestroyTexture(text_texture);
}

void render_hud (SDL_Renderer *renderer, TTF_Font *font)
{
	/* Objective */
	SDL_Rect text_rect;

	text_rect.x = 50;
	text_rect.y = 785;
	stats_text(renderer, font, gray, "Objective : ", text_rect);

	text_rect.x = STATS_OFFSET_X;
	text_rect.y = STATS_OFFSET_Y;
	/* Unit : */

	/* Weapon : */
	stats_text(renderer, font, black, "WEAPON", text_rect);

	/* Stats : */
	text_rect.y += 150;
	stats_text(renderer, font, red, "HP : ", text_rect);
	text_rect.y += 50;
	stats_text(renderer, font, green, "Attack : ", text_rect);
	text_rect.y += 50;
	stats_text(renderer, font, blue, "Precision : ", text_rect);
	text_rect.y += 50;
	stats_text(renderer, font, gray, "Agility : ", text_rect);

	/* Actions : */
	text_rect.y += 125;
	stats_text(renderer, font, white, "Pass turn", text_rect);
}

void render_board (SDL_Renderer *renderer, SDL_Texture *tiles_sprites, SDL_Texture *units_sprites, Battlefield *bt_field)
{
	for (int h = 0; h < bt_field->height; h++)
		for (int w = 0; w < bt_field->width; w++)
		{
			tile_rect.x = w * TILE_SIZE + TILE_OFFSET_X;
			tile_rect.y = h * TILE_SIZE + TILE_OFFSET_Y;

			SDL_RenderCopy(renderer, tiles_sprites, &terrain_sprites_rect[bt_field->cells[h][w].type], &tile_rect);
			if (bt_field->cells[h][w].unit)
			{
				SDL_SetTextureColorMod(units_sprites, (Uint8) ((bt_field->cells[h][w].unit->army * 150) % 255), 255, (Uint8) ((bt_field->cells[h][w].unit->army * 200) % 255)); //it's magic
				SDL_RenderCopy(renderer, units_sprites, &unit_sprites_rect[bt_field->cells[h][w].unit->u_class], &tile_rect);
			}
		}
}
/*********************************DONE*****************************************/

void render_cog (SDL_Renderer *renderer, SDL_Texture *cog_sprite)
{
	cog_rect.x = 0;
	cog_dest_rect.y = 0;
	SDL_RenderCopy(renderer, cog_sprite, &cog_rect, &cog_dest_rect);

	cog_rect.x = 100;
	cog_dest_rect.y = 50;
	SDL_RenderCopy(renderer, cog_sprite, &cog_rect, &cog_dest_rect);
	/*Got to change when mouse hovers the icon*/
}

void render_hover (SDL_Renderer *renderer, int x, int y)
{
	SDL_Rect hover = {
		.h = TILE_SIZE,
		.w = TILE_SIZE
	};

	hover.x = x * TILE_SIZE + TILE_OFFSET_X;
	hover.y = y * TILE_SIZE + TILE_OFFSET_Y;

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 120);
	SDL_RenderFillRect(renderer, &hover);

}

void cleanup_SDL (SDL_Window *window, SDL_Renderer *renderer)
{
	if (renderer)
		SDL_DestroyRenderer(renderer);
	if (window)
		SDL_DestroyWindow(window);
}

void cleanup_ressources (SDL_Texture *units_sprites, SDL_Texture *tiles_sprites, SDL_Surface *icon_sprite, SDL_Texture *background_sprite, SDL_Texture *portraits_sprite, SDL_Texture *cog_sprite, TTF_Font *font)
{

	if (units_sprites)
		SDL_DestroyTexture(units_sprites);
	if (tiles_sprites)
		SDL_DestroyTexture(tiles_sprites);
	if (icon_sprite)
		SDL_FreeSurface(icon_sprite);
	if (background_sprite)
		SDL_DestroyTexture(background_sprite);
	if (portraits_sprite)
		SDL_DestroyTexture(portraits_sprite);
	if (cog_sprite)
		SDL_DestroyTexture(cog_sprite);
	if (font)
		TTF_CloseFont(font);
}


///////////////////////////////////////////////////////////////////////////////
void draw_allowed_moves (Moves *movements, SDL_Renderer *renderer)
{
	for(int j = 0; j < movements->nb; j++)
	{
		// We draw a blue rectangle on the cell (in memory)
		color_rect.x = movements->allowed[j].x * TILE_SIZE + TILE_OFFSET_X;
		color_rect.y = movements->allowed[j].y * TILE_SIZE + TILE_OFFSET_Y;
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 120);
		SDL_RenderFillRect(renderer, &color_rect);
		// We display the "updated" board
	}
}

void convert_mouse_to_board (int *x, int *y)
{
	*x = (*x - TILE_OFFSET_X) / TILE_SIZE;
	*y = (*y - TILE_OFFSET_Y) / TILE_SIZE;
}



int main (int argc, char **argv)
{
	srand((size_t) time(NULL));
	int retval = EXIT_SUCCESS;

	/* GAME INIT*/
	GameState Game = {
		.running = 1,
		.nb_round = 0,
	};

	init_battle(&Game);
	printf("\n\n");

	init_armies( &Game );
	display_battlefield(&Game.battlefield); // CONSOLE

	printf("\nEnd of data initialization.\n\n");

	/***********************************SDL************************************/
	/* BASIC INIT */
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int img_flags = IMG_INIT_PNG;
	SDL_Event event;

	if (init_graphics(&window, &renderer, img_flags) < 0)
	{
		retval = EXIT_FAILURE;
		goto cleanup;
	}

	/* SPRITES INIT */
	SDL_Surface *icon_sprite = IMG_Load("sprites/icon.png");
	if (!icon_sprite) {
		eprintf("Failed to load icon : %s\n", SDL_GetError());
		retval = EXIT_FAILURE;
		goto cleanup;
	}
	SDL_SetWindowIcon(window, icon_sprite);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Texture *tiles_sprites = load_image(renderer, "sprites/simple_tiles.png");
	SDL_Texture *units_sprites = load_image(renderer, "sprites/simple_units.png");
	SDL_Texture *background_sprite = load_image(renderer, "sprites/background.png");
	SDL_Texture *portraits_sprite = load_image(renderer, "sprites/portraits.png");
	SDL_Texture *cog_sprite = load_image(renderer, "sprites/cog.png");

	if (!tiles_sprites || !units_sprites || !background_sprite || !portraits_sprite)
	{
		eprintf("Cannot initialize sprites : %s\n", SDL_GetError());
		retval = EXIT_FAILURE;
		goto cleanup;
	}

	TTF_Font *font = TTF_OpenFont("fonts/MAIAN.TTF", FONT_SIZE);
	if (!font) {
		eprintf("could not open font : %s\n", TTF_GetError());
		goto cleanup;
	}


	/* GAME RUNNING */
	int flags[FLAGS_ESIZE] = {0};
	Game.current_player = 1;
	Unit *selected_unit = NULL;
	Unit *attacked_unit = NULL;
	Coordinates slct_unit_cell;
	Moves movements;
	int x, y;
	int j;

	unsigned int lastTime = 0, currentTime;

	while (Game.running)
	{
		if (no_playable_unit(Game.all_armies.armies[Game.current_player].unit))
		{
			Game.current_player = (Game.current_player + 1) % Game.all_armies.nb;
			update_playable(Game.all_armies.armies[Game.current_player].unit);
		}


		/* EVENTS */
		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
				flags[SHOULD_QUIT] = 1;

			x = event.motion.x;
			y = event.motion.y;
			convert_mouse_to_board(&x, &y);

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				/*********************************** BOARD CLICK INTERACTION ******************************************/
				if (x > -1 && y > -1 && x < Game.battlefield.width && y < Game.battlefield.height &&
					event.motion.x > TILE_OFFSET_X && event.motion.y > TILE_OFFSET_Y)
				{
					if (event.button.button == SDL_BUTTON_RIGHT) // CANCEL the current selection
					{
						selected_unit = NULL;
						printf("CANCEL Selection : No soldier selected\n");
					}

					else if (event.button.button == SDL_BUTTON_LEFT)
					{
						printf("click : (%d, %d)\n", x, y);

						/* SELECTED TILE : UNIT ?*/
						if (Game.battlefield.cells[y][x].unit)
						{
							// SELECT an unit of the current player
							if (Game.current_player == Game.battlefield.cells[y][x].unit->army &&
								Game.battlefield.cells[y][x].unit->playable)
							{
								// Selection
								selected_unit = Game.battlefield.cells[y][x].unit;
								slct_unit_cell.y = y;
								slct_unit_cell.x = x;
								printf("A soldier has been selected\n");

								// Display allowed displacement
								allowed_displacement(&Game.battlefield, selected_unit, &slct_unit_cell, &movements);
							}


							/* FIGHT */
							else if (selected_unit && Game.current_player != Game.battlefield.cells[y][x].unit->army)
							{
								printf("FIGHT\n");

								attacked_unit = Game.battlefield.cells[y][x].unit;

								printf("Before the fight\n ");
								printf("Attacker 1 %d\n defenser 2 %d\n", selected_unit->hp, attacked_unit->hp);

								// Damage computing depending on weapons
								attacked_unit->hp -= damage(selected_unit, attacked_unit);
								if ((selected_unit->weapon == BOW && attacked_unit->weapon == BOW) ||
									(selected_unit->weapon != BOW && attacked_unit->weapon != BOW))
									selected_unit->hp -= damage(attacked_unit, selected_unit);

								printf("Before the fight\n ");
								printf("Attacker 1 %d\n defenser 2 %d\n", selected_unit->hp, attacked_unit->hp);

								selected_unit->playable = FALSE;
								//TODO if dead remove unit
								selected_unit = NULL;
							}
						}

						/* MOVE CURRENT UNIT */
						else if (!Game.battlefield.cells[y][x].unit && selected_unit)
						{
							int allowed = FALSE;
							j = 0;

							while (j < movements.nb && !allowed)// We check if the movement is allowed
							{
								if (x == movements.allowed[j].x &&	y == movements.allowed[j].y)
									allowed = TRUE;
								j++;
							}

							if (allowed)  // If it's allowed, we move the unit
							{
								Game.battlefield.cells[y][x].unit = selected_unit;
								Game.battlefield.cells[slct_unit_cell.y][slct_unit_cell.x].unit = NULL;
								selected_unit->playable = FALSE; // TODO
								selected_unit = NULL;
							}
						} // TODO selected_unit == NULL when current player change
					}
				}
			}

			if (flags[SHOULD_QUIT])
				Game.running = 0;
		}


		/* DISPLAY */
		/* -- render clear */
		SDL_SetRenderDrawColor(renderer, 0, 5, 50, 255);
		SDL_RenderClear(renderer);

		/* -- draw each layer in the correct order */
		SDL_RenderCopy(renderer, background_sprite, NULL, NULL);
		render_cog(renderer, cog_sprite);
		render_board(renderer, tiles_sprites, units_sprites, &Game.battlefield);
		render_hud(renderer, font);

		/* only draw the hover effect if the pointer is within the board */
		if (x > -1 && y > -1 && x < Game.battlefield.width  && y < Game.battlefield.height)
			render_hover(renderer, x, y);
		if (selected_unit)
			draw_allowed_moves(&movements, renderer);

		
		/* -- refresh only once */
		SDL_RenderPresent(renderer);
	}

	/* END */
cleanup:
	cleanup_SDL(window, renderer);
	cleanup_ressources(units_sprites, tiles_sprites, icon_sprite, background_sprite, portraits_sprite, cog_sprite, font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	free_everything(&Game.battlefield, Game.battlefield.height); //TODO : Free units allocated in init_army()

	return retval;
}
