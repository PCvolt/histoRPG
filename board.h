typedef enum {INFL, INFH, CAVL, CAVH, RNGL, RNGH, UNIT_ESIZE} UnitClass;
typedef enum {SWORD, AXE, SPEAR, BOW, WEAPON_ESIZE} Weapon;
typedef enum {PLAIN, FOREST, WATER, HOUSE, MOUNTAIN, WALL, TERRAIN_ESIZE} Terrain;

typedef struct unit_s {
	int camp;
	int leader;
	UnitClass class;
	Weapon weapon;
	int hp;
	int atk;
	int prc;
	int agi;
	struct unit_s *next;
} Unit;

typedef struct cell_s {
	int type;
	Unit *unit;
} Cell;

typedef struct{
	int width;
	int height;
	Cell **cells;
} Battlefield;

void init_battlefield (Battlefield *bt_field, int, int);
void random_fill_battlefield (Battlefield *bt_field);
void display_battlefield (Battlefield *bt_field);
