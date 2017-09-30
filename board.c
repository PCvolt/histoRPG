#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"


void init_battlefield (Battlefield *bt_field, int input_width, int input_height)
{
	bt_field->width = input_width;
	bt_field->height = input_height;


	bt_field->cells = malloc(sizeof(Cell) * (size_t) input_height);

	for (int i = 0; i < input_height; i++)
		for (int j = 0; j < input_width; j++)
		{
			bt_field->cells[i] = malloc(sizeof(Cell) * (size_t) input_width);
			bt_field->cells[i][j].unit = NULL;
		}
	
	random_fill_battlefield(bt_field);
}

void random_fill_battlefield (Battlefield *bt_field)
{
	for (int i = 0; i < bt_field->height; i++)
		for (int j = 0; j < bt_field->width; j++)
			bt_field->cells[i][j].type = rand() % TERRAIN_ESIZE;
}

void display_battlefield (Battlefield *bt_field)
{
	printf("\n\n");
	for (int i = 0; i < bt_field->height; i++)
	{
		for (int j = 0; j < bt_field->width; j++)
			printf("%d ", bt_field->cells[i][j].type);
		printf("\n");
	}
}


int main()
{
	srand((size_t)time(NULL));

	Battlefield *bt_field = malloc(sizeof(Battlefield));


	/*Safe method of input for number <= 0*/
	int size_x;
	int size_y;
	char buffer_x[10];
	char buffer_y[10];
	do{
		printf("Please enter the width(x) and height(y) of the battlefield :\n");
		fgets(buffer_x, 9, stdin);
		fgets(buffer_y, 9, stdin);
		size_x = (int) strtol(buffer_x, NULL, 10);
		size_y = (int) strtol(buffer_y, NULL, 10);
	} while (size_x <= 0 || size_y <= 0);


	init_battlefield(bt_field, size_x, size_y);
	display_battlefield(bt_field);

	
	/*END*/
	for (int i = 0; i < size_y; ++i)
		free(bt_field->cells[i]);
	free(bt_field->cells);
	free(bt_field);

	return EXIT_SUCCESS;
}