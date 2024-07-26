#ifndef BARR_H
#define BARR_H

/**********************
Function Declerations
**********************/

int BARR_CountBitsOff(unsigned long word);
int BARR_CountBitsOn(unsigned long word);
int BARR_GetBitState(unsigned long word, unsigned int index);
void BARR_SetBit(unsigned long *word, unsigned int index, unsigned int state);
void BARR_TurnBitOn(unsigned long *word, unsigned int index);
void BARR_TurnBitOff(unsigned long *word, unsigned int index);
void BARR_TurnAllOn(unsigned long *word);
void BARR_TurnAllOff(unsigned long *word);
void BARR_FlipAll(unsigned long *word);
void BARR_FlipBit(unsigned long *word, unsigned int index);
void BARR_RotateRight(unsigned long *word, int times);
void BARR_RotateLeft(unsigned long *word, int times);
void BARR_Mirror(unsigned long *word);
int BARR_IsBitOn(unsigned long word, unsigned int index);
int BARR_IsBitOff(unsigned long word, unsigned int index);
void BARR_ToString(char *buffer, unsigned long word);



#endif
