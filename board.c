#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"


const int class_attributes[UNIT_ESIZE][STATS_ESIZE] = {
	{100, 35, 85, 3},//INFL {atk, prc, agi, hp}
	{140, 50, 70, 2},//INFH
	{ 80, 45, 75, 5},//CAVL
	{120, 60, 65, 4},//CAVH
	{100, 50, 90, 4},//RNGL
	{250, 95, 60, 1} //RNGH
};

/*******************************BATTLEFIELD************************************/


void display_battlefield (Battlefield *bt_field)
{
	printf("\n\n");
	for (int i = 0; i < bt_field->height; i++)
	{
		for (int j = 0; j < bt_field->width; j++)
		{
			if (bt_field->cells[i][j].unit == NULL)
				printf("%d ", bt_field->cells[i][j].type);
			else printf("  ");
		}
		printf("\n");
	}
}//for debug purposes only


void free_everything (Battlefield *bt_field, int size_y)
{
	for (int i = 0; i < size_y; ++i)
		free(bt_field->cells[i]);
	free(bt_field->cells);
	free(bt_field);
}

/**********************************ARMIES**************************************/

void init_armies( GameState *Game )
{
	//for (int i = 0; i < nb_camp; i++)
	for (int i = 0; i < Game->all_armies.nb ; i++){

		Unit *unit = Game->all_armies.armies[i].unit;
		while (unit)
		{
			unit->army = i;
			unit->leader = 0;

			unit->hp  = class_attributes[unit->u_class][HP];
			unit->atk = class_attributes[unit->u_class][ATK];
			unit->prc = class_attributes[unit->u_class][PRC];
			unit->agi = class_attributes[unit->u_class][AGI];

			unit = unit->next;
		}
	}
	//display_armies(armies, nb_camp);
}

void place_unit (GameState *Game)
{
	Unit *p;
	int x , y;
	for (int i = 0; i < Game->all_armies.nb; i++)
	{
		p = Game->all_armies.armies[i].unit;

		while (p)
		{
			x = rand() % Game->battlefield.height;
			y = rand() % Game->battlefield.width;

			if (!Game->battlefield.cells[x][y].unit)
			{
				Game->battlefield.cells[x][y].unit = p;
				p = p->next;
			}
		}
	}
}

void display_armies (Unit *armies, int nb_camp)
{
	Unit *p;
	for (int i = 0; i < nb_camp; i++)
	{
		p = armies+i;
		printf("ARMY %d :\n", i);
		while (p != NULL)
		{
			printf("\tUnit %d %d\n", p->u_class, p->weapon);
			p = p->next;
		}
	}
}//for debug purposes only

void init_battle (GameState *Game)
{
	FILE* file = NULL;
	file = fopen( "map2.txt", "r");

	if (!file){
		printf("Error, map2.txt cannot be opened\n");
		return;//TODO : return an error if failed
	}
	else
		printf("\n\nLOADING : Map2.txt successfully loaded !\n");

	fscanf( file, "%d %d", &Game->battlefield.height, &Game->battlefield.width );


	// ALLOCATION BATTLEFIELD
	int i, j;


	Game->battlefield.cells = malloc(sizeof(Cell *) * (size_t) Game->battlefield.height);

	for (i = 0; i < Game->battlefield.height; i++)
	{
		Game->battlefield.cells[i] = malloc( sizeof(Cell) * (size_t) Game->battlefield.width);
		for (j = 0; j < Game->battlefield.width; j++)
			Game->battlefield.cells[i][j].unit = NULL;
	}


	// READ BATTLEFIELD
	for (i = 0; i < Game->battlefield.height; i++)
		for (j = 0; j < Game->battlefield.width; j++)
			fscanf(file, "%d", &Game->battlefield.cells[i][j].type);


	// READ ARMIES
	fscanf( file, "%d", &Game->all_armies.nb );


	// ALLOCATION ARMIES
	Game->all_armies.armies = malloc(sizeof(Army) * (size_t) Game->all_armies.nb);

	// READ UNITS
	Unit *p;
	for(i = 0; i < Game->all_armies.nb; i++)
	{
		fscanf(file, "%d", &Game->all_armies.armies[i].nb_unit);

		// ALLOCATION
		Game->all_armies.armies[i].unit = malloc(sizeof(Unit));

		p = Game->all_armies.armies[i].unit;

		for(j = 0; j < Game->all_armies.armies[i].nb_unit; j++)
		{
			if (j < Game->all_armies.armies[i].nb_unit - 1)
				p->next = malloc(sizeof(Unit));
			else
				p->next = NULL;

			p = p->next;
		}

		// READ CLASS
		p = Game->all_armies.armies[i].unit;
		while (p)
		{
			fscanf( file, "%d", &p->u_class );
			p = p->next;
		}

		// READ WEAPON
		p = Game->all_armies.armies[i].unit;
		while (p)
		{
			fscanf( file, "%d", &p->weapon );
			p = p->next;
		}
	}

	// READ AND PLACEMENT
	int x, y;

	for(i = 0; i < Game->all_armies.nb; i++)
	{
		p = Game->all_armies.armies[i].unit;
		while (p)
		{
			fscanf( file, "%d %d", &y, &x);
			Game->battlefield.cells[y][x].unit = p;
			p = p->next;
		}
	}
	// DISPLAY IN CONSOLE
	display_all(Game);

	// CLOSE THE FILE
	fclose(file);
}


void display_all (GameState *Game)
{
	int i, j;

	printf("[width * height] : %d %d\n\n", Game->battlefield.width, Game->battlefield.height);

   // BATTLEFIELD
	for (i = 0; i < Game->battlefield.height; i++)
	{
		for (j = 0; j < Game->battlefield.width; j++)
			printf("%d ", Game->battlefield.cells[i][j].type);
		printf("\n");
	}

	// ARMIES
	printf("\nARMIES number: %d\n", Game->all_armies.nb);

	for (i = 0; i < Game->all_armies.nb; i++)
	{
		printf("---Army #%d---\n", i);
		printf("nb_units: %d\n", Game->all_armies.armies[i].nb_unit);


		// (CLASS, WEAPON)
		Unit *p = Game->all_armies.armies[i].unit;
		while (p)
		{
			printf("(%d : %d) | ", p->u_class, p->weapon);
			p = p->next;
		}
		printf("\n");
	}
}

/**********************************IN GAME**************************************/

void update_playable (Unit *unit)
{
	Unit *p = unit;
	while (p)
	{
		p->playable = 1;
		p = p->next;
	}
	printf("\nNEW TURN ! All units are now playable.\n");
}

int no_playable_unit (Unit *unit)
{
	Unit *p = unit;

	while (p)
	{
		if (p->playable)
			return FALSE;
		p = p->next;
	}

	printf("No playable unit.\n");
	return TRUE;
}

int damage (Unit *attacker, Unit *defender)
{
	double dmg = (double) attacker->atk;
	int roll = rand() % 100;

	if (roll >= attacker->prc)
		return 0; //MISSED /*On s'épargne les calculs de dégâts, si on rate.*/

	if( attacker->weapon != BOW && defender->weapon != BOW && attacker->weapon != defender->weapon){
		if (attacker->weapon % (WEAPON_ESIZE - 2) <= defender->weapon )
			dmg *= 1.4;  //DMG +40% IF STRONG
		else
			dmg *= 0.6; // DMG -40% IF WEAK
	}

	else if (attacker->weapon != BOW && defender->weapon == BOW) //TRIANGLE VS BOW
		dmg *= 1.4;

	else if (attacker->weapon == BOW && (defender->u_class == CAVL || defender->u_class == CAVH))
		dmg *= 2; // BOW VS CAVALRY

	if (roll < 2)
		dmg *= 3; //CRITICAL HIT 2% FOR 3x DMG

	return (int) dmg;
}

// --------------------------------------------------------
// TODO
// --------------------------------------------------------

void allowed_displacement (Battlefield *bt_field, Unit *unit, Coordinates *unit_cell, Moves *movements)
{
	int x = unit_cell->x;
	int y = unit_cell->y;

	// TODO : This function should be able to find the possible movements of "unit",
	//        In function of its class, the type of ground (forest, plain...)
	// DON'T FORGET to check that the coordinates are not outside of the battlefield

	movements->allowed[0].y = y-1;
	movements->allowed[0].x = x;

	movements->allowed[1].y = y+1;
	movements->allowed[1].x = x;

	movements->allowed[2].y = y;
	movements->allowed[2].x = x-1;

	movements->allowed[3].y = y;
	movements->allowed[3].x = x+1;

	// DON'T FORGET to update the number of cell where the unit can go :
	movements->nb = 4;
}
