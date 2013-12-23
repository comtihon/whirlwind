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
	char *toCrypt = "very secret hello world";
	int dataLen = strlen(toCrypt);

	long *result = malloc(2 * dataLen * sizeof(long));

	cryptTest(result, toCrypt, dataLen);
	printf("crypt finished\n");
	decryptTest(NULL, result, dataLen);
	printf("decrypt finished\n");
	free(result);

	return 0;
}

void cryptTest(long *result, char *toCrypt, int dataLen)
{
	printf("cryptTest\n");
	char *dict = malloc(25); //TODO добавить в документацию, что словарь должен составляться именно так.
	strcpy(dict, "qwertyuio pasdfghjk lzxcvbnm");

	CipherInst *instance = init(time(NULL), 2, 7);
	if (instance)
	{
		setDataWithMemory(toCrypt, instance, dataLen);	//TODO насколько это вообще нужно?
		setDictWithMemory(dict, instance, strlen(dict));

		long *tempRes = malloc(2 * sizeof(long));
		int m = 0;

		for (int i = 0; i < dataLen; i++)
		{
			printf("%d of %d\n", i, dataLen - 1);
			if (cryptOneSymbol(instance, toCrypt[i], tempRes) != OK)
			{
				printf("Error crypting symbol %c[%d]\n", toCrypt[i], toCrypt[i]);
				break;
			}
			result[m++] = tempRes[0];
			result[m++] = tempRes[1];
			printf("%c = %d - %d\n", dict[i], tempRes[0], tempRes[1]);

		}
		free(tempRes);
		freeInst(instance);
	}
	free(dict);
}

void decryptTest(char *result, long *pairs, int dataLen)
{
	printf("decryptTest\n");
	char *dict = malloc(25); //TODO добавить в документацию, что словарь должен составляться именно так.
	strcpy(dict, "qwertyuio pasdfghjk lzxcvbnm");

	CipherInst *instance = init(time(NULL), 2, 7);
	if (instance)
	{
		setDictWithMemory(dict, instance, strlen(dict));

		char tempRes;
		int m = 0;

		long temp[2];

		for (int i = 0; i < dataLen; i+=2)
		{
			temp[0] = pairs[i];
			temp[1] = pairs[i+1];
			printf("%d of %d\n", i, dataLen - 1);
			if (decryptOneSymbol(instance, temp, &tempRes) != OK)
			{
				printf("Error decrypting pair %ld - %ld\n", pairs[i], pairs[i]);
				break;
			}
			printf("%c", tempRes);
		}
	}
	freeInst(instance);

	free(dict);
}
