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
	//TODO добавить в документацию, что словарь должен составляться именно так.
	char *dict = malloc(25);
	strcpy(dict, "qwertyuio pasdfghjk lzxcvbnm");

	char *original = malloc(25);
	strcpy(original, dict);

	char *toCrypt = "very secret hello world";
	CipherInst *instance = init(time(NULL), 2, 7);
	if (instance)
	{

		int dataLen = strlen(toCrypt);

		setDataWithMemory(toCrypt, instance, dataLen);
		setDictWithMemory(dict, instance, strlen(dict));

		long *result = malloc(2 * dataLen * sizeof(long));
		long *tempRes = malloc(2*sizeof(long));
		int m = 0;

		for (int i = 0; i < dataLen; i++)
		{
			printf("%d of %d\n", i, dataLen-1);
			if(cryptOneSymbol(instance, toCrypt[i], tempRes)!=OK)
			{
				printf("Error crypting symbol %c[%d]\n", toCrypt[i], toCrypt[i]);
				break;
			}
			result[m++] = tempRes[0];
			result[m++] = tempRes[1];
			printf("%c = %d - %d\n", dict[i], tempRes[0], tempRes[1]);

		}
		printf("1\n");
		free(tempRes);
		printf("2\n");
		free(result);
		printf("3\n");
		freeInst(instance);
		printf("4\n");
	}
	free(dict);
	printf("5\n");
	free(original);
	printf("6\n");

	return 0;
}
