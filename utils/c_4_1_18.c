#include <stdio.h>
#include <string.h>


void FlipStr(char *str);
void CleanDelims(char *str, const char *delim, char ch);
void SumNum(const char *num1, const char *num2, char *buff);


/**********************************************************
 clean delimiters from string and replace with char - O(n)
***********************************************************/
void CleanDelims(char *str, const char *delim, char ch)
{
	char *read = str;  /* scan the string */
	char *write = str; /* re-write string */
	
	/* search for first non-delim charecter - start position */
	while((*read != '\0') && (strchr(delim, *read) != NULL))
	{
		++read;
	}

	while(*read != '\0')
	{
		/* first is not a delimiter */
		if(NULL == strchr(delim, read[0]))
		{
			*write++ = *read;
		}
		
		/* second is not delimiter (first is delim) */
		else if(NULL == strchr(delim, read[1]))
		{
			*write++ = ch;  /* replace delim with char */
		}
		
		++read;
	}
	
	/* private case - string of delimiters only */
	if(write == str)
	{
		*write++ = ch;
	}
	
	*write = '\0';
}



/***************
 Flip a String
****************/
void FlipStr(char *str)
{
	int start = 0;
	int end = strlen(str) - 1;
	int tmp = 0;
	
	while(start < end)
	{
		tmp = str[start];
		str[start++] = str[end];
		str[end--] = tmp;
	}
}


/**********************************************************
 Sums two positive whole numbers and writes to buff - O(n)
 **********************************************************/
void SumNum(const char *num1, const char *num2, char *buff)
{
	int idx1 = strlen(num1) - 1;  /* num1 index of end of string */
	int idx2 = strlen(num2) - 1;  /* num2 index of end of string */
	
	int carry = 0;
	int sum = 0;
	int i = 0;
	
	/* sum all digits of both numbers */
	while(idx1 >= 0 && idx2 >= 0)
	{
		sum = (num1[idx1--] - '0') + (num2[idx2--] - '0') + carry;	
		buff[i++] = (sum % 10) + '0';
		carry = sum / 10;
	}
	
	/* if num1 length is bigger than num2 - complete sum */
	while (idx1 >= 0)
	{
		sum = (num1[idx1--] - '0') + carry;
		buff[i++] = (sum % 10) + '0';
		carry = sum / 10;
	}
	
	/* if num2 length is bigger than num1 - complete sum */
	while (idx2 >= 0)
	{
		sum = (num2[idx2--] - '0') + carry;
		buff[i++] = (sum % 10) + '0';
		carry = sum / 10;
	}
	
	/* add last carry if is not equall to 0 */
	if (carry != 0)
	{
		buff[i++] = carry + '0';
	}
	
	/* add string closure */
	buff[i] = '\0';
	
	/* flip string */
	FlipStr(buff);
	
}


int main(int argc, char *argv[])
{
	char str[] = "	   h   e	*   l        l  o    ";
	char delim[] = "	* ";
	const char num1[] = "123";
	const char num2[] = "99";
	char sum[10];
	
	/* Sum up 2 number strings */
	SumNum(num1, num2, sum);
	printf("sum of \"%s\" + \"%s\" = \"%s\"", num1, num2, sum);
	printf("\nlength of result: %lu\n" ,strlen(sum));

	
	/* Clean Delimiters */	
	CleanDelims(str, delim, '-');
	printf("\nstr after CleanDelims:\n%s", str);
	printf("\nlength of str: %lu\n", strlen(str));
	


	return (0);
}
