/*
 * main.c
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#include "main.h"

#include "whirlwind/include/cipherCore.h"	//TODO временное.

int main()
{
	char *toCrypt = "this is a test phrase";
	int dataLen = strlen(toCrypt);

	char *toDecrypt = malloc(dataLen);
	unsigned long *result = calloc(2 * dataLen, sizeof(unsigned long));

	cryptTest(result, toCrypt, dataLen);
	printf("crypt finished\n");
	decryptTest(toDecrypt, result, dataLen);
	printf("decrypt finished: %s\n", toDecrypt);
	free(result);
	free(toDecrypt);
	return 0;
}

void cryptTest(unsigned long *result, char *toCrypt, int dataLen)
{
	printf("cryptTest\n");
	char *dict = malloc(25); //TODO добавить в документацию, что словарь должен составляться именно так.
	strcpy(dict, "qwertyuio pasdfghjk lzxcvbnm");

	CipherInst *instance = init(time(NULL), 2, 7);
	if (instance)
	{
		setDictWithMemory(dict, instance, strlen(dict));
		cryptString(instance, toCrypt, dataLen, result);
		freeInst(instance);
	}
	else
		printf("Can't create cipher instance!\n");
	free(dict);
}

void decryptTest(char *result, unsigned long *pairs, int dataLen)
{
	printf("decryptTest\n");
	char *dict = malloc(25); //TODO добавить в документацию, что словарь должен составляться именно так.
	strcpy(dict, "qwertyuio pasdfghjk lzxcvbnm");

	CipherInst *instance = init(time(NULL), 2, 7);
	if (instance)
	{
		setDictWithMemory(dict, instance, strlen(dict));
		decryptString(instance, pairs, dataLen, result);
		freeInst(instance);
	}
	else
		printf("Can't create cipher instance!\n");

	free(dict);
}
