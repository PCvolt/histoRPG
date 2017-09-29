#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "definitions.h"

// Function to initialize the battlefield structure
int init_battlefield( Battlefield *bt_field, int size_i, int size_j )
{
    int i, j;

    bt_field->size_i = size_i;
    bt_field->size_j = size_j;
    // Allocation of the array of cells
    bt_field->cells = malloc( size_i * sizeof( Cell ) );
    for ( i=0; i<size_i; i++ ){
        bt_field->cells[i] = malloc( size_j * sizeof( Cell ) );
        bt_field->cells[i][j].unit = NULL;
    }
    // Fill the array of cells randomly with different type (plain, water...)
    random_fill_battlefield( bt_field );

    return 0;
}

// Function to fill the array of cells randomly with different type (plain, water...)
int random_fill_battlefield( Battlefield *bt_field )
{
    int i, j;

    for ( i=0; i<bt_field->size_i; i++ )
        for ( j=0; j<bt_field->size_j; j++ )
            bt_field->cells[i][j].type = rand()%TERRAIN_ESIZE;
}

void DEBUG_display_battlefield( Battlefield *bt_field )
{
    int i, j;
    for ( i=0; i<bt_field->size_i; i++ ){
        for ( j=0; j<bt_field->size_j; j++ )
            printf( "%d ", bt_field->cells[i][j].type);
        printf("\n");
    }
}

void test( Battlefield *bt_field )
{
    printf("Test function :\n\n");
    init_battlefield( bt_field, 15, 15 );
    DEBUG_display_battlefield( bt_field );
}

int main()
{
    Battlefield *bt_field = malloc( sizeof ( Battlefield ) );
    //int size_i, size_j;

	srand(time(NULL)); // initialisation de rand
    test( bt_field );

    // initialization of the battlefield
    /*
    do{
        printf("please enter the high and length of the battlefield : ");
        scanf("%d%d", &size_i, &size_j);
    } while ( size_i<=0 ||size_j<=0 );
    */

    // TODO : free battlefield.cell
    free( bt_field );
    return 0;
}
