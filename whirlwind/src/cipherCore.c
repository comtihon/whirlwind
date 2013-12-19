/*
 * cipherCore.c
 *
 *  Created on: 19 дек. 2013 г.
 *      Author: tihon
 */

#include "cipherCore.h"


int *cryptOneSymbol(CipherInst *conf, char symbol)
{

}

/**
 * Возвращает позицию символа в словаре или -1, если символ отсутствует.
 */
long findSymbolPosInDict(CipherInst *conf, char symbol)
{

	long charPos = 0;	//TODO find from random place
	for(long iterator = 0; iterator < conf->dictLen; iterator++)
	{
		if(conf->dictSelected==0)
		{//поиск словаря в памяти
			//TODO charPos+iterator > conf->dictLen
			if(conf->dict.dictInMemory[charPos+iterator]==symbol)
				return charPos+iterator;
		}
		else
		{//поиск словаря в файле

		}
	}
	return -1;
}


