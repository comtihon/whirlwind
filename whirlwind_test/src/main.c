/*
 * main.c
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#include "main.h"

#include "whirlwind/include/cipherCore.h"	//TODO временное.

#define INIT init(time(NULL), 2, 7, 3)

int main()
{
	char *toCrypt = "m this is a test phrase";
	int dataLen = strlen(toCrypt);

	char *toDecrypt = malloc(dataLen);
	unsigned long *result = malloc(2 * dataLen * sizeof(unsigned long));


//	testRandom();
	//TODO move to CTest
	cryptTestMemory(result, toCrypt, dataLen);
	printf("crypt finished\n");
//	decryptTestMemory(toDecrypt, result, dataLen);
//	printf("decrypt finished: %s\n", toDecrypt);

//	cryptTestFile(result, toCrypt, dataLen);
//	printf("crypt file finished\n");
//	decryptTestFile(toDecrypt, result, dataLen);
//	printf("decrypt file finished: %s\n", toDecrypt);

	free(result);
	free(toDecrypt);
	return 0;
}

unsigned long rrandVal(RandBuf *randomBuffer, unsigned long max)
{
	unsigned long out = 0;
	lrand48_r(randomBuffer, &out);
	return max? out % max : out;
}

void testRandom()
{
	RandBuf *buf = malloc(sizeof(RandBuf));
	srand48_r(25, buf);

	unsigned long rr;
	for(int i = 0; i < 5; i++)
	{
		rr = rrandVal(buf, 25);
		printf("%ld ", rr);
	}
	printf("\n");
	srand48_r(25, buf);
	for(int i = 0; i < 5; i++)
	{
		rr = rrandVal(buf, 25);
		printf("%ld ", rr);
	}
	printf("\n");

	free(buf);
}

void cryptTestMemory(unsigned long *result, char *toCrypt, int dataLen)
{
	printf("cryptTestMemory\n");
	char *dict = malloc(25); //TODO добавить в документацию, что словарь должен составляться именно так.
	strcpy(dict, "qwertyuio pasdfghjk lzxcvbnm");

	CipherInst *instance = INIT;
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

void cryptTestFile(unsigned long *result, char *toCrypt, int dataLen)
{
	printf("cryptTestFile\n");

	CipherInst *instance = INIT;
	if (instance)
	{
		FILE *fDictPtr = fopen("/home/tihon/dict1", "r+");	//TODO test empty	//TODO test non existent
		if (fDictPtr)
		{
			fseek(fDictPtr, 0, SEEK_END);
			unsigned long fileSize = ftell(fDictPtr);
			setDictWithFile(fDictPtr, instance, fileSize);
			cryptString(instance, toCrypt, dataLen, result);
			freeInst(instance);
			fclose(fDictPtr);
		}
		else
			printf("Can't open dictionary file!\n");
	}
	else
		printf("Can't create cipher instance!\n");
}

void decryptTestMemory(char *result, unsigned long *pairs, int dataLen)
{
	printf("decryptTestMemory\n");
	char *dict = malloc(25); //TODO добавить в документацию, что словарь должен составляться именно так.
	strcpy(dict, "qwertyuio pasdfghjk lzxcvbnm");

	CipherInst *instance = INIT;
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

void decryptTestFile(char *result, unsigned long *pairs, int dataLen)
{
	printf("decryptTestFile\n");

	CipherInst *instance = INIT;
	if (instance)
	{
		FILE *fDictPtr = fopen("/home/tihon/dict2", "r+");
		if (fDictPtr)
		{
			fseek(fDictPtr, 0, SEEK_END);
			unsigned long fileSize = ftell(fDictPtr);
			setDictWithFile(fDictPtr, instance, fileSize);
			decryptString(instance, pairs, dataLen, result);
			freeInst(instance);
			fclose(fDictPtr);
		}
		else
			printf("Error opening file!\n");
	}
	else
		printf("Can't create cipher instance!\n");
}
