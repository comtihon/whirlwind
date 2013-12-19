/*
 * cipherCore.c
 *
 *  Created on: 19 дек. 2013 г.
 *      Author: tihon
 */

#include "cipherCore.h"


long *cryptOneSymbol(CipherInst *conf, char symbol)
{
	long charPos = findSymbolPosInDict(conf, symbol);	//найти кодируемый символ
	if(charPos==-1)
	{
		printf("Can't find symbol %s[%d] in dict!\n", symbol, symbol);
		return NULL;
	}

	long nextRandom = 0;
	lrand48_r(&conf->support->randomBuffer, &nextRandom);	//случайно выбрать инициализатор для последовательности

	//результат - позиция найденного символа + инициализатор
	long *result = malloc(2*sizeof(long));
	result[0] = charPos;
	result[1] = nextRandom;

	//TODO add withdraw
	//TODO change dict

	return result;	//TODO free result (doc)
}

/**
 * Возвращает позицию символа в словаре или -1, если символ отсутствует.
 * @param conf	рабочая конфигурация
 * @param symbol кодируемый символ
 */
long findSymbolPosInDict(CipherInst *conf, char symbol)
{
	long charPos = 0;
	lrand48_r(&conf->support->randomBuffer, &charPos);
	charPos = charPos % conf->dictLen;
	for(long iterator = 0; iterator < conf->dictLen; iterator++)
	{
		if(conf->support->dictSelected==0)
		{//поиск словаря в памяти
			if(charPos + iterator == conf->dictLen)		//обнуление позиции поиска
				charPos -= conf->dictLen;
			if(conf->dict.dictInMemory[charPos+iterator]==symbol)
				return charPos+iterator;
		}
		else
		{//поиск словаря в файле

		}
	}
	return -1;
}


