#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

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
typedef enum {LDR, INFL, INFH, CAVL, CAVH, RNGL, RNGH, UNIT_ESIZE} UnitClass;
typedef enum {EMPTY, PLAIN, FOREST, HOUSE, WATER, MOUNTAIN, TERRAIN_ESIZE} Terrain;

typedef enum {SHOULD_QUIT, FLAGS_ESIZE} Flags;

/* Cette notation te permet de définir (avant compilation avec des valeurs fixes)
 * la valeur d'une case précise d'un tableau en mémoire, donc là pour obtenir
 * la faiblesse de l'épée te suffit de faire weapon_weakness[SWORD] et ça va
 * te poindre SPEAR. Remarque que j'ai rien mis pour BOW mais c'est pas grave
 * ça sera initialisé à 0, (les tableaux dont au moins une valeur est initialisée
 * seront mis à 0 pour les autres, donc là SWORD est en première position dans
 * l'enum et vaut donc 0, t'étonne pas si weapon_weakness[BOW] retourne SWORD sans
 * que tu l'aies défini */
int weapon_weaknesses[WEAPON_ESIZE] = {
	[SWORD] = SPEAR,
	[AXE]   = SWORD,
	[SPEAR] = AXE
};

/* une position c'est un point, et point c'est plus petit à écrire :p */
typedef struct point_s {
	int x, y;
} Point;

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


typedef struct gamestate_s {
	int  running;
	int  nb_round;
	int *nb_unit; // nb unit par equipe
	// int *nb_leader; // nb leader par equipe
} GameState;


typedef struct cell_s {
	int type; // plain, forest, house, water, mountain, wall...
	Unit *unit;
} Cell;
