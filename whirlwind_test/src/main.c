/*
 * main.c
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#include "main.h"

int main()
{
	char *dict = "qwertyuiopasdfghjklzxcvbnm";
	char *toCrypt = "very secret hello world";
	CipherInst *instance = init(time(NULL), 2, 7);
	setDataWithMemory(toCrypt, instance, strlen(toCrypt));
	setDictWithMemory(dict, instance, strlen(dict));

	freeInst(instance);
	return 0;
}
