#define FALSE 0
#define TRUE !FALSE

typedef enum {INFL, INFH, CAVL, CAVH, RNGL, RNGH, UNIT_ESIZE} UnitClass;
typedef enum {HP, ATK, PRC, AGI, STATS_ESIZE} Stats;
typedef enum {SWORD, AXE, SPEAR, BOW, WEAPON_ESIZE} Weapon;

typedef enum {PLAIN, FOREST, WATER, HOUSE, MOUNTAIN, WALL, TERRAIN_ESIZE} Terrain;



/*BATTLEFIELD AND UNITS*/
typedef struct unit_s {
	int army;
	int leader;
	int u_class;
	int weapon;
	int hp;
	int atk;
	int prc;
	int agi;
	struct unit_s *next;

	int playable;
} Unit;

typedef struct coordinates{
    int x, y;
} Coordinates;

typedef struct moves{
    int nb;
    Coordinates allowed[100];
} Moves;

typedef struct cell_s {
	int type;
	Unit *unit;
} Cell;

typedef struct{
	int width;
	int height;
	Cell **cells;
} Battlefield;



/*ARMIES*/
typedef struct {
    //int camp;
	int nb_unit;
	Unit *unit;
} Army;

typedef struct {
	Army *armies;
	int nb;
} All_Armies;

typedef struct gamestate_s {
	int  running;
	int  nb_round;

    int  current_player;

    Battlefield battlefield;
    All_Armies all_armies;
} GameState;

extern const int class_attributes[UNIT_ESIZE][STATS_ESIZE];



// INITIALIZATION OF A GAME BATTLE
void init_battle (GameState *game);
void init_armies (GameState *game);
void place_unit (GameState *game);

void allowed_displacement( Battlefield *bt_field, Unit *unit, Coordinates *unit_cell, Moves *movements );

void update_playable (Unit *unit);
int no_playable_unit (Unit *unit);
int damage (Unit *attacker, Unit *defender);

void free_everything (Battlefield *bt_field, int size_y);

// CONSOLE
void display_battlefield (Battlefield *bt_field);
void display_armies (Unit *armies, int nb_camp);
void display_all (GameState *game);
