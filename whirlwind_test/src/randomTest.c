/*
 * randomTest.c
 *
 *  Created on: 09 янв. 2014 г.
 *      Author: tihon
 */

#include "randomTest.h"

int main()
{
	RandBuf *buf = malloc(sizeof(RandBuf));
	srand48_r(25, buf);

	unsigned long firstRandom[5];
	for(int i = 0; i < 5; i++)
		firstRandom[i] = rrandVal(buf, 25);

	srand48_r(25, buf);
	unsigned long secondRandom[5];
	for(int i = 0; i < 5; i++)
		secondRandom[i] = rrandVal(buf, 25);
	free(buf);

	for(int i = 0; i < 5; i++)
		if(firstRandom[i] != secondRandom[i])
		{
			printf("Error! Number mismatch: %d != %d\n", firstRandom[i], secondRandom[i]);
			return -1;
		}
	return 0;
}

unsigned long rrandVal(RandBuf *randomBuffer, unsigned long max)
{
	unsigned long out = 0;
	lrand48_r(randomBuffer, &out);
	return max? out % max : out;
}
