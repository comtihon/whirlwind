/*
 * cipherCore.c
 *
 *  Created on: 19 дек. 2013 г.
 *      Author: tihon
 */

#include "cipherCore.h"

//TODO провести тайминг каждой функции.
/**
 * Кодирует 1 символ. Возвращает массив - состоящий из пары шифрокодов.
 * @param conf - рабочая конфигурацияя
 * @param symbol - кодируемый символ
 * @param result - указатель на результат - long[2]
 * @return код возврата (ошибка либо успех)
 */
ReturnCode cryptOneSymbol(CipherInst *conf, char symbol, unsigned long *result)
{
	long charPos = findSymbolPosInDict(conf, symbol);	//найти кодируемый символ
	switch(charPos)
	{
		case FileStreamIsClosed:
			printf("Can't read dict's file!\n");
			return FileStreamIsClosed;
		case SymbolNotFoundInDict:
			printf("Can't find symbol '%c'[%d] in dict!\n", symbol, symbol);
			return SymbolNotFoundInDict;
	}
	unsigned long nextRandom = randVal(conf, conf->dictLen);	//случайно выбрать инициализатор для последовательности

	//результат - позиция найденного символа + инициализатор
	result[0] = charPos;
	result[1] = nextRandom;
	switch (processWithdraw(conf, result))
	{
		case 1:								//отката не было
			processChange(conf, result);	//изменить словарь
			return OK;
		case 0:								//был откат, перестановка не нужна
			return OK;
		case ErrorAllocatingMemory:			//произошла ошибка выделения памяти
			return ErrorAllocatingMemory;
	}
}

/**
 * Декодирует пару шифрокодов в 1 символ.
 * @param conf - рабочая конфигурация
 * @param pair - пара шифросимволов - long[2]
 * @param result - указатель на символ, в который нужно записать результат
 * @return код возврата (ошибка либо успех)
 */
ReturnCode decryptOneSymbol(CipherInst *conf, unsigned long *pair, char *result)
{
	if (conf->support->dictSelected == 0)
	{			//работа со словарём в оперативной памяти
		result[0] = conf->dict.dictInMemory[pair[0]];
		srand48_r(pair[1], conf->support->randomBuffer);
		long randNum = randVal(conf, conf->dictLen);

		if (processWithdraw(conf, pair))	//отката не было
		{
			changeDict(conf, &pair[0], &randNum);
			if (conf->variability)	//если задана дополнительная изменчивость - выполнить её
				extraChangeDict(conf);
		}
	}
	else
	{	//работа со словарём в файле
		//TODO реализовать
	}

	return OK;
}

/**
 * Возвращает позицию символа в словаре или -1, если символ отсутствует.
 * @param conf	рабочая конфигурация
 * @param symbol кодируемый символ
 * @return - позиция символа, -1, если символ отсутствует, различные ошибки
 */
unsigned long findSymbolPosInDict(CipherInst *conf, char symbol)
{
	long charPos = randVal(conf, conf->dictLen);	//случайная позиция, с которой будет производиться поиск символа
	if (conf->support->dictSelected == 0)	//если символ в памяти - произвести поиск и вернуть результат
		return findSymbolInMemory(conf->dict.dictInMemory, conf->dictLen, charPos, symbol);

	if(conf->dictLen < 1000000)
	{
		if(!conf->dict.dictInFile)	//файловый сокет закрыт
			return FileStreamIsClosed;	//дальнейший поиск невозможен
		//TODO каким-то образом поменять return, т.к. он unsigned, а ошибки < 0

		char *buffer = malloc(conf->dictLen);
		if(!buffer)	//TODO вместо возвращения ошибки реализовать поиск перебором с буфером в 1 байт
			return ErrorAllocatingMemory;

		long readRes = fread (buffer, 1, conf->dictLen, conf->dict.dictInFile);	//считать весь файл в буфер
		//  if (readRes != conf->dictLen)	//TODO обработка ошибки чтения

		long result = findSymbolInMemory(buffer, conf->dictLen, charPos, symbol);	//ищем символ по буферу
		free(buffer);
		return result;
	}
	return -1;
}

/**
 * Производит поиск по памяти
 * @param memory - указатель на память
 * @param memLength - длина памяти
 * @param start - откуда искать
 * @param symbol - что искать
 * @return позицию символа в памяти или -1, если символ в памяти отсутствует
 */
unsigned long findSymbolInMemory(char *memory, unsigned long memLength, unsigned long start, char symbol)
{
	for (unsigned long iterator = 0; iterator < memLength; iterator++)
	{
		if (start + iterator == memLength)		//обнуление позиции поиска
			start -= memLength;				//поиск по памяти сначала
		if (memory[start + iterator] == symbol)
			return start + iterator;
	}
	return -1;
}

