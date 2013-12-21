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
	strcpy(dict, "qwertyuiopasdfghjklzxcvbnm");

	char *toCrypt = "very secret hello world";
	CipherInst *instance = init(time(NULL), 2, 7);
	if (instance)
	{

		int dataLen = strlen(toCrypt);

		setDataWithMemory(toCrypt, instance, dataLen);
		setDictWithMemory(dict, instance, strlen(dict));

		long *result = malloc(2 * dataLen * sizeof(long));
		int m = 0;

		for (int i = 0; i < dataLen; i++)
		{
			printf("%d of %d\n", i, dataLen-1);
			long *tempRes = cryptOneSymbol(instance, dict[i]);
			result[m++] = tempRes[0];
			result[m++] = tempRes[1];
			printf("%c = %d - %d\n", dict[i], tempRes[0], tempRes[1]);
			free(tempRes);
		}

		printf("free result\n");
		free(result);
		printf("free instance\n");
		freeInst(instance);
		printf("free dict\n");
	}
	free(dict);

	return 0;
}
