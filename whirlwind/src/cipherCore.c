/*
 * cipherCore.c
 *
 *  Created on: 19 дек. 2013 г.
 *      Author: tihon
 */

#include "cipherCore.h"

//TODO провести тайминг каждой функции.
/**
 * Кодирует 1 символ. Возвращает массив - состоящий из пары шифросимволов.
 * @param conf рабочая конфигурацияя
 * @param symbol кодируемый символ
 * @param result указатель на результат - long[2]
 * @return массив long в куче. ВАЖНО! Освободить при ненадобности!
 */
ReturnCode cryptOneSymbol(CipherInst *conf, char symbol, long *result)
{
	long charPos = findSymbolPosInDict(conf, symbol);	//найти кодируемый символ
	if(charPos==-1)
	{
		printf("Can't find symbol '%c'[%d] in dict!\n", symbol, symbol);
		return SymbolNotFoundInDict;
	}
	long nextRandom = randVal(conf, conf->dictLen);	//случайно выбрать инициализатор для последовательности

	//результат - позиция найденного символа + инициализатор
	result[0] = charPos;
	result[1] = nextRandom;
	if(processWithdraw(conf, result))	//отката не было
		processChange(conf, result);	//изменить словарь
	return OK;
}

ReturnCode decryptOneSymbol(CipherInst *conf, long *pair, char *result)
{
	if(conf->support->dictSelected==0)
	{//работа со словарём в оперативной памяти
		result[0] = conf->dict.dictInMemory[pair[0]];
		srand48_r(pair[1], conf->support->randomBuffer);
		long randNum = randVal(conf, conf->dictLen);

		if(processWithdraw(conf, pair))	//отката не было
		{
			changeDict(conf, &pair[0], &randNum);
			if (conf->variability)	//если задана дополнительная изменчивость - выполнить её
				extraChangeDict(conf);
		}
	}
	else
	{//работа со словарём в файле
		//TODO реализовать
	}

	return OK;
}



/**
 * Возвращает позицию символа в словаре или -1, если символ отсутствует.
 * @param conf	рабочая конфигурация
 * @param symbol кодируемый символ
 */
long findSymbolPosInDict(CipherInst *conf, char symbol)
{
	long charPos = randVal(conf, conf->dictLen);
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
			//TODO сделать поиск по файлу
		}
	}
	return -1;
}


