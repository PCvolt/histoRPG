/* UTILITY */
#define WINDOW_WIDTH 1100
#define WINDOW_HEIGHT 830
#define eprintf(...) fprintf(stderr, __VA_ARGS__)

typedef enum {SHOULD_QUIT, FLAGS_ESIZE} Flags;


/* FONT */
#define FONT_SIZE 35

static SDL_Color black = {0, 0, 0, 0};
static SDL_Color red = {216, 43, 43, 0};
static SDL_Color blue = {0, 128, 255, 0};
static SDL_Color green = {51, 77, 0, 0};
static SDL_Color gray = {64, 64, 64, 0};
static SDL_Color white = {255, 255, 255, 0};


/* HUD Stats */
#define STATS_OFFSET_X 835
#define STATS_OFFSET_Y 300
#define PORTRAIT_OFFSET_X 820
#define PORTRAIT_OFFSET_Y 35
#define PORTRAIT_SIZE 250



static SDL_Rect portrait_rect = {
	.x = PORTRAIT_OFFSET_X,
	.y = PORTRAIT_OFFSET_Y,
	.w = PORTRAIT_SIZE,
	.h = PORTRAIT_SIZE
};

/* COG */
#define COG_OFFSET_X 835
#define COG_OFFSET_Y 300

static SDL_Rect cog_rect = {
	.x = 0,
	.y = 0,
	.w = 50,
	.h = 50
};
static SDL_Rect cog_dest_rect = {
	.x = 0,
	.y = 0,
	.w = 50,
	.h = 50
};

/* TILES */
#define TILE_SIZE 50
#define TILE_OFFSET_X 50
#define TILE_OFFSET_Y 35

static SDL_Rect tile_rect = {
	.x = 0,
	.y = 0,
	.w = TILE_SIZE,
	.h = TILE_SIZE
};

static SDL_Rect terrain_sprites_rect[TERRAIN_ESIZE] = {
	[PLAIN] = {
		.x = 0,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[FOREST] = {
		.x = TILE_SIZE,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[WATER] = {
		.x = TILE_SIZE * 2,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[MOUNTAIN] = {
		.x = TILE_SIZE * 3,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[HOUSE] = {
		.x = TILE_SIZE * 4,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[WALL] = {
		.x = TILE_SIZE * 5,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	}
};

static SDL_Rect unit_sprites_rect[UNIT_ESIZE] = {
	[RNGL] = {
		.x = 0,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[CAVL] = {
		.x = TILE_SIZE,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[INFL] = {
		.x = TILE_SIZE * 2,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[RNGH] = {
		.x = TILE_SIZE * 2,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[CAVH] = {
		.x = TILE_SIZE * 4,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	},
	[INFH] = {
		.x = TILE_SIZE * 5,
		.y = 0,
		.h = TILE_SIZE,
		.w = TILE_SIZE
	}
};

static SDL_Rect color_rect = {
	.w = TILE_SIZE,
	.h = TILE_SIZE
};

/* Prototypes */
int init_graphics (SDL_Window **window, SDL_Renderer **renderer, int img_flags);
SDL_Texture *load_image (SDL_Renderer *renderer, const char *image_path);

void sprites_on_renderer (SDL_Renderer *renderer, SDL_Texture *tiles, SDL_Texture *units, Battlefield *bt_field);
void cleanup_SDL (SDL_Window *window, SDL_Renderer *renderer);
void cleanup_ressources (SDL_Texture *units_sprites, SDL_Texture *tiles_sprites, SDL_Surface *icon_sprite, SDL_Texture *background_sprite, SDL_Texture *portraits_sprite, SDL_Texture *cog_sprite, TTF_Font *font);
