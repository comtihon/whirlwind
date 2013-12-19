/*
 * main.c
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */


#include "main.h"

int main()
{
	char *toCrypt = "very secret hello world";
	CipherInst *instance = init(2,7,strlen(toCrypt));
	setDataWithMemory(toCrypt, instance);

	freeInst(instance);
	return 0;
}
