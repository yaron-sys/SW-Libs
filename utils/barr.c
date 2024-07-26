#include "barr.h"

#define LONG_BIT 64
#define CHAR_BIT 8

/* Declare side functions */
void SetMirrorLUT(void);
void SetCountLUT(void);
unsigned char MirrorByte(unsigned char num);
int CountBitsOnLUT(unsigned char word);


/* Declare static LUT for mirror and count */
static unsigned char mirror_LUT[257];
static unsigned char count_LUT[257];


/* unsigned long of number 1 */
static const unsigned long MASK_ONE = 1;


/********************
Function Definitions
*********************/

int BARR_CountBitsOff(unsigned long word)
{
	int count = 0;
	
	/* make '0' bits to '1' */
	word = ~word;
	
	/* count only '1' bits */
	while (word)
	{
		word &= (word - 1);
		++count;
	}
	
	return (count);
}



int BARR_GetBitState(unsigned long word, unsigned int index)
{
	/* check if bit is on/off in its current position */
	return ((word & (MASK_ONE << index)) ? 1 : 0);
}



void BARR_SetBit(unsigned long *word, unsigned int index, unsigned int state)
{
	if (state)
	{
		/* if state is greater than 0 - set bit in current position */
		*word |= (MASK_ONE << index);
	}
	
	else
	{
		/* if state is 0 - unset bit in current position */
		*word &= ~(MASK_ONE << index);
	}
}



void BARR_TurnBitOn(unsigned long *word, unsigned int index)
{
	/* turn bit on in current position */
	*word |= (MASK_ONE << index);
}



void BARR_TurnBitOff(unsigned long *word, unsigned int index)
{
	/* turn bit off in current position */
	*word &= ~(MASK_ONE << index);
}



void BARR_TurnAllOn(unsigned long *word)
{
	/* turn all bits on */
	*word |= ~0;
}



void BARR_TurnAllOff(unsigned long *word)
{
	/* turn all bits off */
	*word = 0;
}



void BARR_FlipAll(unsigned long *word)
{
	/* flip all bits ('0' to '1' and vice versa) */
	*word = ~(*word);

}


void BARR_FlipBit(unsigned long *word, unsigned int index)
{
	/* flip a bit in current position (xor) */
	*word ^= (MASK_ONE << index);
}



void BARR_RotateRight(unsigned long *word, int times)
{
	if (0 > times)
	{
		/* if negative, rotate left with positive sign of times */
		BARR_RotateLeft(word, -times);
	}
	
	else
	{
		/* rotate right */
		*word = (*word >> times) | (*word << (LONG_BIT - times));
	}
   
}



void BARR_RotateLeft(unsigned long *word, int times)
{
	if (0 > times)
	{
		/* if negative, rotate right with positive sign of times */
		BARR_RotateRight(word, -times);
	}
	
	else
	{
		/* rotate left */
		*word = (*word << times) | (*word >> (LONG_BIT - times));
	}
	
}



int BARR_IsBitOn(unsigned long word, unsigned int index)
{
	/* if bit on return 1. otherwise 0 */
	return ((word & (MASK_ONE << index)) ? 1 : 0);
}



int BARR_IsBitOff(unsigned long word, unsigned int index)
{
	/* if bit off return 1. otherwise 0 */
	return ((word & (MASK_ONE << index)) ? 0 : 1);
}



void BARR_ToString(char *buffer, unsigned long word)
{

	int shift = LONG_BIT - 1;
	int i = 0;
	
	if (!buffer)
	{
		return;
	}
	
	while (shift >= 0)
	{
		
		/* bit is ON in a particular position - add '1' */
		if (word & (MASK_ONE << shift))
		{
			buffer[i++] = '1';
		}
		
		/* bit is OFF in a particular position - add '0' */
		else
		{
			buffer[i++] = '0';
		}
		
		--shift;
	}
	
	buffer[i] = '\0';
}


int BARR_CountBitsOn(unsigned long word)
{
	int shift = 0;
	int count = 0;

	/* if not initialized before - set up LUT of count bits in a byte */
	if (count_LUT[256] == 0)
	{
		SetCountLUT();
	}
	
	
	while (shift != LONG_BIT)
	{
		/* count set bits in a particular byte */
		count += count_LUT[ (word >> shift) & 0xFF ];
		
		/* promote shift to next byte */
		shift += CHAR_BIT;
	}

	return (count);	
}


void BARR_Mirror(unsigned long *word)
{
	int shift = 0;
	unsigned long mirror_byte = 0;
	unsigned long mirror_word = 0;
	
	/* if not initialized before - set up LUT of mirror bytes */
	if (mirror_LUT[256] == 0)
	{
		SetMirrorLUT();
	}
	
	while (shift != LONG_BIT)
	{
		/* holds the current byte mirror */
		mirror_byte = mirror_LUT[ (*word >> shift) & 0xFF ];
		
		/* move byte mirror to the relevant opposite position */
		mirror_word |= (mirror_byte << (LONG_BIT - CHAR_BIT - shift));
		
		/* promote shift to next byte */
		shift += CHAR_BIT;
	}
	
	*word = mirror_word;
}



/* SIDE FUNCTION SET-MIRROR-LUT */
void SetMirrorLUT(void)
{
	int i;
	
	for (i = 0; i < 256; ++i)
	{
		mirror_LUT[i] = MirrorByte(i);
	}
	
	/* last index is set to 1 - LUT is filled up */
	mirror_LUT[i] = 1;
}


/* SIDE FUNCTION SET-COUNT-LUT */
void SetCountLUT(void)
{
	int i;
	
	for (i = 0; i < 256; ++i)
	{
		count_LUT[i] = CountBitsOnLUT(i);
	}
	
	/* last index is set to 1 - LUT is filled up */
	count_LUT[i] = 1;
}


/* SIDE FUNCTION BYTE-MIRROR-LUT */
unsigned char MirrorByte(unsigned char num)
{
	num = ((num & 0xf0) >> 4) | ((num & 0x0f) << 4);
	num = ((num & 0xcc) >> 2) | ((num & 0x33) << 2);
	num = ((num & 0xaa) >> 1) | ((num & 0x55) << 1);
	
	return (num);
}


/* SIDE FUNCTION COUNT-BITS-ON-LUT */
int CountBitsOnLUT(unsigned char word)
{
	int count = 0;
	
	/* count only '1' bits */
	while (word)
	{
		word &= (word - 1);
		++count;
	}

	return (count);
}




/* NOT IN USE */
/*
void BARR_Mirror(unsigned long *word)
{
	
	int i;
	unsigned long reverse = 0;
	
	for(i = 0; i < LONG_BIT; i++)
	{
		if((*word & (MASK_ONE << i)))
		{
			reverse |= (MASK_ONE << ((LONG_BIT - 1) - i));
		}
	}
	
	*word = reverse;
}
*/

