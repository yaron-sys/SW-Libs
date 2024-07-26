#include <stdio.h>   /* printf */
#include <assert.h>  /* assert */

#include "barr.h"
#include "sorts.h"

#define TABLE_SIZE 8
#define CALC_BIT_IDX(row,col) (((row) * 8) + (col))

void KnightsTour(void);
static int KnightsTourImpl(unsigned long board, int step, int row, int col,
						   int steps[TABLE_SIZE][TABLE_SIZE]);
static int IsValidStep(unsigned long board, int row, int col);
static void SetNeigbors(int neigbors[8][2], int row, int col);
static size_t CountValidSteps(unsigned long board, int neigbors[8][2]);
static size_t GetKey(const void *element,  void *params);
static void PrintBoard(int steps[TABLE_SIZE][TABLE_SIZE]);

int main()
{
	KnightsTour();
	
	return (0);
}

void KnightsTour(void)
{
    /* set a board and steps table */
    int steps[TABLE_SIZE][TABLE_SIZE] = {{0}};
	unsigned long board = 0;

    /* Start from 0,0 and start playing */
    if (0 == KnightsTourImpl(board, 1, 0, 0, steps))
    {
        PrintBoard(steps);
    }
}

/* complexity of O(8^n) */
static int KnightsTourImpl(unsigned long board, int step, int row, int col,
						   int steps[TABLE_SIZE][TABLE_SIZE])   
{
	int i = 0;
	int neigbors[8][2] = {{0}};
	int sorted_neigbors[8][2] = {{0}};
	size_t histogram[9] = {0};
	
	if (step > TABLE_SIZE * TABLE_SIZE)
	{
		return (0);
	}
	
	if (!IsValidStep(board, row, col))
	{
		return (1);
	}
	
	BARR_TurnBitOn(&board, CALC_BIT_IDX(row,col));
	
	/* set potential neigbors of current knight */
	SetNeigbors(neigbors, row, col);

	/* sort neigbors by best path */
	if (SortsCountingSortImpl(sorted_neigbors, neigbors, 8, 2 * sizeof(int), 
		histogram, 9, GetKey, &board))
	{
		return (1);
	}
	
	for (i = 0; i < 8; ++i)
	{
		if (0 == KnightsTourImpl(board, step + 1, sorted_neigbors[i][0], 
				 sorted_neigbors[i][1], steps))
		{
			steps[row][col] = step;
		
			return (0);
		}
	}
 
   return (1);
}

/******************************************************************************* 
check if row,col are valid indexes in chess board
********************************************************************************/
static int IsValidStep(unsigned long board, int row, int col)
{
    return ((row >= 0) && (row < TABLE_SIZE) && (col >= 0) && (col < TABLE_SIZE) &&
    		(BARR_IsBitOff(board, CALC_BIT_IDX(row,col))));
}

/******************************************************************************* 
set potential neigbors of a knight move and save in array.
********************************************************************************/
static void SetNeigbors(int neigbors[8][2], int row, int col)
{
	/* define next moves of Knight */
	static const int next_moves[8][2] = {{2,1}, {1,2}, {-1,2}, {-2,1},
										 {-2,-1}, {-1,-2}, {1,-2}, {2,-1}};
	int i = 0;
	
	/* potential neigbors */
	for (i = 0; i < 8; ++i)
	{
		neigbors[i][0] = row + next_moves[i][0];
		neigbors[i][1] = col + next_moves[i][1];
	}
}

/******************************************************************************* 
Returns number of valid steps of a neigbor.
********************************************************************************/
static size_t CountValidSteps(unsigned long board, int neigbors[8][2])
{
	size_t count = 0;
	int i = 0;
	
	for (i = 0; i < 8; ++i)
	{
		count += IsValidStep(board, neigbors[i][0], neigbors[i][1]);
	}
	
	return (count);
}

/******************************************************************************* 
Element is the current coordinate move. params is the board 
Returns number of legal steps.
********************************************************************************/
static size_t GetKey(const void *element,  void *params)
{
	int neigbors[8][2] = {{0}};
	
	assert(element != NULL);
	assert(params != NULL);
	
	/* set potential neigbors */
	SetNeigbors(neigbors, ((int *)element)[0], ((int *)element)[1]);

	return (CountValidSteps(*(unsigned long *)params, neigbors));	
}

/******************************************************************************* 
Print solution matrix steps
********************************************************************************/
static void PrintBoard(int steps[TABLE_SIZE][TABLE_SIZE])
{
    int i = 0;
    int j = 0;
    
    for (i = 0; i < TABLE_SIZE; ++i)
    {
        for (j = 0; j < TABLE_SIZE; ++j)
        {
        	printf("%.2d  ", steps[i][j]);
        }
        printf("\n");
    }
}
