#define WINDOW_WIDTH TILE_SIZE * MAP_WIDTH
#define WINDOW_HEIGHT TILE_SIZE * MAP_HEIGHT
#define TEXTURE_WIDTH 600
#define TEXTURE_HEIGHT 400
#define SURFACE_WIDTH 32
#define SURFACE_HEIGHT 32


#define MAP_HEIGHT       15
#define MAP_WIDTH        15
#define TILE_SIZE 30

#define eprintf(...) fprintf(stderr, __VA_ARGS__)



typedef enum {SWORD, AXE, SPEAR, BOW, WEAPON_ESIZE} Weapon;
typedef enum {INFL, INFH, CAVL, CAVH, RNGL, RNGH, UNIT_ESIZE} UnitClass;
typedef enum {PLAIN, FOREST, WATER, HOUSE, MOUNTAIN, WALL, TERRAIN_ESIZE} Terrain;

typedef enum {SHOULD_QUIT, FLAGS_ESIZE} Flags;




SDL_Rect sprites_rects[TERRAIN_ESIZE] = {
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


typedef struct unit_s {
	int camp;
	int leader;
	//UnitClass class;
	//Weapon weapon;
	int hp;
	int atk;
	int prc;
	int agi;
	struct unit_s *next;
} Unit;

typedef struct gamestate_s {
	int  running;
	int  nb_round;
	int *nb_unit;
	int *nb_leader;
} GameState;

typedef struct cell_s {
	int type;
	Unit *unit;
} Cell;