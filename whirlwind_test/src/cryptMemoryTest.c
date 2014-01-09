/*
 * cryptMemoryTest.c
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#include "cryptMemoryTest.h"

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("Wrong number of arguments!\nNeed:[Mem|File]\n");
		return -1;
	}

	int dataLen;

	char *toDecrypt;
	unsigned long *result;

	if(!strcmp("Mem", argv[1]))
	{
		dataLen = strlen(TO_CRYPT);
		result = malloc(2 * dataLen * sizeof(unsigned long));
		cryptTestMemory(result, TO_CRYPT, dataLen);

		toDecrypt = malloc(dataLen);
		decryptTestMemory(toDecrypt, result, dataLen);
		if(strcmp(toDecrypt, TO_CRYPT))
		{
			printf("Result mismatch!\n1:%s\n2:%s\n",TO_CRYPT, toDecrypt);
			free(result);
			free(toDecrypt);
			return -1;
		}
		free(result);
		free(toDecrypt);
	}
	else if(!strcmp("File", argv[1]))
	{
		if(prepareDictFiles())
		{
			printf("Can't prepare files for dicts!\n");
			return -1;
		}
		dataLen = strlen(TO_CRYPT);
		result = malloc(2 * dataLen * sizeof(unsigned long));
		cryptTestFile(result, TO_CRYPT, dataLen);

		toDecrypt = malloc(dataLen);
		decryptTestFile(toDecrypt, result, dataLen);
		if(strcmp(toDecrypt, TO_CRYPT))
		{
			printf("Result mismatch!\n1:%s\n2:%s\n",TO_CRYPT, toDecrypt);
			free(result);
			free(toDecrypt);
			return -1;
		}
		free(result);
		free(toDecrypt);
	}
	else
	{
		printf("Incorrect arguments!\n Need:[Mem|File]");
		return -1;
	}

	return 0;
}

void cryptTestMemory(unsigned long *result, char *toCrypt, int dataLen)
{
	printf("######################################cryptTestMemory\n");
	char *dict = malloc(29); //TODO добавить в документацию, что словарь должен составляться именно так.
	strcpy(dict, DICT);

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

void decryptTestMemory(char *result, unsigned long *pairs, int dataLen)
{
	printf("######################################decryptTestMemory\n");
	char *dict = malloc(29); //TODO добавить в документацию, что словарь должен составляться именно так.
	strcpy(dict, DICT);

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

void cryptTestFile(unsigned long *result, char *toCrypt, int dataLen)
{
	printf("######################################cryptTestFile\n");

	CipherInst *instance = INIT;
	if (instance)
	{
		FILE *fDictPtr = fopen("1.dct", "r+");	//TODO test empty	//TODO test non existent
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

void decryptTestFile(char *result, unsigned long *pairs, int dataLen)
{
	printf("######################################decryptTestFile\n");

	CipherInst *instance = INIT;
	if (instance)
	{
		FILE *fDictPtr = fopen("2.dct", "r+");
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
