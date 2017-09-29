#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED


#define MAP_HEIGHT       15
#define MAP_WIDTH        15
#define MAP_CELL_SIZEPX  32

/* ESIZE à la fin, ça permet de savoir le nombre d'éléments
 * dans l'enum d'une manière toute simple :
 * L'enum commence à 0 de toutes façons, si on a un seul élément
 * il vaudra 0 et la valeur ESIZE qui sera en deuxième vaudra 1,
 * donc on a un élément dans l'enum, pratique pour des loops ou
 * tester si on a pas une valeur trop grande avant d'indexer un array */

typedef enum {SWORD, AXE, SPEAR, BOW, WEAPON_ESIZE} Weapon;
typedef enum {INFL, INFH, CAVL, CAVH, RNGL, RNGH, UNIT_ESIZE} UnitClass;
typedef enum {PLAIN, FOREST, HOUSE, WATER, MOUNTAIN, TERRAIN_ESIZE} Terrain;

typedef enum {SHOULD_QUIT, FLAGS_ESIZE} Flags;

typedef struct gamestate_s {
	int  running;
	int  nb_round;
	int *nb_unit; // nb unit par equipe
	// int *nb_leader; // nb leader par equipe
} GameState;

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

typedef struct cell_s {
	int type; // plain, forest, house, water, mountain, wall...
	Unit *unit;
} Cell;

typedef struct{
   int size_i, size_j;
   Cell **cells;
   int taille_pixel;
} Battlefield;

int random_fill_battlefield( Battlefield *bt_field );
void DEBUG_display_battlefield( Battlefield *bt_field );
void test( Battlefield *bt_field );
int init_battlefield( Battlefield *bt_field, int size_i, int size_j );

#endif // DEFINITIONS_H_INCLUDED
