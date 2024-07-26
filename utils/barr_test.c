#include <stdio.h>
#include <stdlib.h>
#include "barr.h"


/* Print bit array */
void PrintBin(unsigned long word)
{
	int i = 63;
	
	while (i >= 0)
	{
		word & ((unsigned long) 1 << i) ? printf("1") : printf("0");
		
		if((i % 8) == 0)
		{
			printf(" ");
		}
		
		--i;
	}
}



int main(int argc, char *argv[])
{

		/********************
		Initialize veriables
		*********************/
	unsigned long word = 0xFAF2; /* 1111 1010 1111 0010 */
	char *buffer = NULL;  /* create a pointer to str buffer */
	
	
		/****************
		Testing Functions
		*****************/
	
	/* BARR_CountBitsOff TEST */
	printf("Count Bits Off TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	printf("\nBits off: %d", BARR_CountBitsOff(word));
	/*printf("\nBits on: %d", BARR_CountBitsOn(word));*/
	
	printf("\n\n\n");
	
	
	
	
	/* BARR_GetBitState TEST */
	printf("Get Bit State TEST\n");
	printf("Bit state in index 0: %d\n", BARR_GetBitState(word, 0));
	printf("Bit state in index 6: %d\n", BARR_GetBitState(word, 6));
	printf("Bit state in index 20: %d\n", BARR_GetBitState(word, 20));
	printf("Bit state in index 16: %d", BARR_GetBitState(word, 15));
	
	printf("\n\n\n");
	
	
	
	
	/* BARR_SetBit TEST */
	printf("Set Bit (off / on) TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	BARR_SetBit(&word, 16, 1);
	printf("\nSet bit ON in index 16:\n");
	PrintBin(word);

	BARR_SetBit(&word, 35, 1);
	printf("\nSet bit ON in index 35:\n");
	PrintBin(word);
	
	BARR_SetBit(&word, 5, 0);
	printf("\nSet bit OFF in index 5:\n");
	PrintBin(word);
	
	printf("\n\n\n");
			
						
	
			
	/* BARR_TurnBitOn TEST */
	printf("Turn Bit On TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	BARR_TurnBitOn(&word, 17);
	printf("\nTurn bit ON in index 17:\n");
	PrintBin(word);

	BARR_TurnBitOn(&word, 63);
	printf("\nTurn bit ON in index 63:\n");
	PrintBin(word);
	
	BARR_TurnBitOn(&word, 44);
	printf("\nTurn bit ON in index 44:\n");
	PrintBin(word);
	
	printf("\n\n\n");

	
	
	
	/* BARR_TurnBitOff TEST */
	printf("Turn Bit Off TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	BARR_TurnBitOff(&word, 15);
	printf("\nTurn bit OFF in index 15:\n");
	PrintBin(word);

	BARR_TurnBitOff(&word, 63);
	printf("\nTurn bit OFF in index 63:\n");
	PrintBin(word);
	
	BARR_TurnBitOff(&word, 13);
	printf("\nTurn bit OFF in index 13:\n");
	PrintBin(word);
	
	printf("\n\n\n");


	
	/* BARR_TurnAllOn TEST */
	printf("Turn All ON TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	BARR_TurnAllOn(&word);
	printf("\nTurn all bits ON:\n");
	PrintBin(word);
	
	printf("\n\n\n");
	
	
	/* BARR_TurnAllOff TEST */
	printf("Turn All OFF TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	BARR_TurnAllOff(&word);
	printf("\nTurn all bits OFF:\n");
	PrintBin(word);
	
	printf("\n\n\n");
	
	
	
	
	/* BARR_FlipAll TEST */
	printf("Flip All bits TEST\n");
	printf("Original number:\n");
	word = 0xFAF2;
	PrintBin(word);
	
	BARR_FlipAll(&word);
	printf("\nFlip all:\n");
	PrintBin(word);
	
	printf("\n\n\n");
	
	
	
	
	/* BARR_FlipBit TEST */
	printf("Flip bit TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	BARR_FlipBit(&word, 31);
	printf("\nFlip bit in index 31:\n");
	PrintBin(word);
	
	BARR_FlipBit(&word, 14);
	printf("\nFlip bit in index 14:\n");
	PrintBin(word);
	
	BARR_FlipBit(&word, 7);
	printf("\nFlip bit in index 7:\n");
	PrintBin(word);
	
	
	printf("\n\n\n");
	
	
	
	/* BARR_RotateRight TEST */
	printf("Rotate Right TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	BARR_RotateRight(&word, 16);
	printf("\nRotate right 16 times:\n");
	PrintBin(word);
	
	BARR_RotateRight(&word, 65);
	printf("\nRotate right 65 times:\n");
	PrintBin(word);
	
	BARR_RotateRight(&word, -30);
	printf("\nRotate right -30 times:\n");
	PrintBin(word);
	
	BARR_RotateRight(&word, 0);
	printf("\nRotate right 0 times:\n");
	PrintBin(word);
	
	printf("\n\n\n");
	
	
	/* BARR_RotateLeft TEST */
	printf("Rotate Left TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	BARR_RotateLeft(&word, 10);
	printf("\nRotate left 10 times:\n");
	PrintBin(word);
	
	BARR_RotateLeft(&word, 65);
	printf("\nRotate left 65 times:\n");
	PrintBin(word);
	
	BARR_RotateLeft(&word, -30);
	printf("\nRotate left -30 times:\n");
	PrintBin(word);
	
	BARR_RotateLeft(&word, 0);
	printf("\nRotate left 0 times:\n");
	PrintBin(word);
	
	printf("\n\n\n");
	
	
	/* BARR_Mirror TEST */
	/*
	printf("Mirror TEST\n");
	printf("Original number:\n");
	word = 0xFF00FF;
	PrintBin(word);
	
	BARR_Mirror(&word);
	printf("\nMirror:\n");
	PrintBin(word);
	
	BARR_Mirror(&word);
	printf("\nMirror again:\n");
	PrintBin(word);
	
	printf("\n\n\n");
	*/
	
	
	/* BARR_IsBitOn TEST */
	word = 0xFF00FF;
	printf("Is Bit On TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	printf("\nIs bit on in index 18 ? %d", BARR_IsBitOn(word, 18));
	printf("\nIs bit on in index 10 ? %d", BARR_IsBitOn(word, 10));
	printf("\nIs bit on in index 35 ? %d", BARR_IsBitOn(word, 35));
	printf("\nIs bit on in index 4 ? %d", BARR_IsBitOn(word, 4));
	
	printf("\n\n\n");
	
	
	
	/* BARR_IsBitOff TEST */
	printf("Is Bit Off TEST\n");
	printf("Original number:\n");
	PrintBin(word);
	
	printf("\nIs bit off in index 18 ? %d", BARR_IsBitOff(word, 18));
	printf("\nIs bit off in index 10 ? %d", BARR_IsBitOff(word, 10));
	printf("\nIs bit off in index 35 ? %d", BARR_IsBitOff(word, 35));
	printf("\nIs bit off in index 4 ? %d", BARR_IsBitOff(word, 4));
	
	printf("\n\n");
	
	
	printf("*****************************************************************");
	
	printf("\nLUT TESTS:\n");
	
	/* BARR_ToString TEST */
	buffer = (char*) malloc(sizeof(char) * 65);  /* Allocating 65 bytes of chars */
	
	/* ToString TEST 1 */
	word = 0xFFFFFFFFFFFFFFFF;
	printf("\nWord that should be copied to string: %lu\n", word);
	PrintBin(word);
	
	BARR_ToString(buffer, word);
	printf("\nString buffer after word was copied:\n");
	printf("%s", buffer);
	
	/* ToString TEST 2 */
	word = 0xABCFF0FFAF1F2F7F;
	printf("\n\nWord that should be copied to string: %lu\n", word);
	PrintBin(word);
	
	BARR_ToString(buffer, word);
	printf("\nString buffer after word was copied:\n");
	printf("%s", buffer);
	
	/* ToString TEST 3 */
	word = 0x0000FFAAAFF2FFBE;
	printf("\n\nWord that should be copied to string: %lu\n", word);
	PrintBin(word);
	
	BARR_ToString(buffer, word);
	printf("\nString buffer after word was copied:\n");
	printf("%s", buffer);
	
	free(buffer);
	printf("\n\n");
	
	
	
	/* BARR_CountBitsOnLUT TESTS */
	printf("\nCount Bits On LUT TEST:\n");
	word = 0xFFFFFFFFFFFFFFFF;
	printf("\nsum of count bit on LUT: %d\n", BARR_CountBitsOn(word));
	PrintBin(word);
	
	word = 0x0000FFFF0FFBFFFF;
	printf("\nsum of count bit on LUT: %d\n", BARR_CountBitsOn(word));
	PrintBin(word);
	
	word = 0x00000000000BA000;
	printf("\nsum of count bit on LUT: %d\n", BARR_CountBitsOn(word));
	PrintBin(word);
	
	word = 0x0000000000000000;
	printf("\nsum of count bit on LUT: %d\n", BARR_CountBitsOn(word));
	PrintBin(word);
	
	word = 0x0020C0003000010E;
	printf("\nsum of count bit on LUT: %d\n", BARR_CountBitsOn(word));
	PrintBin(word);
	
	printf("\n\n");
	
	
	
	/* BARR_MirrorLUT TEST 1 */
	printf("\nword mirror on LUT TEST:\n");
	word = 0x0000FFFF0FFBFFFF;
	printf("\nnum before mirror:\n");
	PrintBin(word);
	
	BARR_Mirror(&word);
	printf("\nnum after mirror:\n");
	PrintBin(word);
	
	BARR_Mirror(&word);
	printf("\nsecond mirror to get original number:\n");
	PrintBin(word);
	
	
	/* BARR_MirrorLUT TEST 2 */
	word = 0xC002FEA0FFBBFFF;
	printf("\n\nnum before mirror:\n");
	PrintBin(word);
	
	BARR_Mirror(&word);
	printf("\nnum after mirror:\n");
	PrintBin(word);
	
	BARR_Mirror(&word);
	printf("\nsecond mirror to get original number:\n");
	PrintBin(word);
	
	printf("\n\n");
	
	
	return (0);
}



