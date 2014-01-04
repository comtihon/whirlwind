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
	unsigned long charPos;
	switch (findSymbolPosInDict(conf, symbol, &charPos))	//найти кодируемый символ
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
		case OK:								//отката не было
			return processChange(conf, result);	//изменить словарь
		case Withdraw:								//был откат, перестановка не нужна
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
	ReturnCode ret = OK;
	if (conf->support->dictSelected == 0)	//работа со словарём в оперативной памяти
		result[0] = conf->dict.dictInMemory[pair[0]];
	else
	{	//работа со словарём в файле
		fseek(conf->data.cryptFile, pair[0], SEEK_SET);
		fread(result, 1, conf->dictLen, conf->dict.dictInFile);
	}

	srand48_r(pair[1], conf->support->randomBuffer);
	long randNum = randVal(conf, conf->dictLen);

	if (processWithdraw(conf, pair) == OK)	//отката не было
	{
		if ((ret = changeDict(conf, &pair[0], &randNum)) != OK)
			return ret;
		if (conf->variability)	//если задана дополнительная изменчивость - выполнить её
			extraChangeDict(conf);
	}

	return ret;
}

/**
 * Возвращает позицию символа в словаре или -1, если символ отсутствует.
 * @param conf	рабочая конфигурация
 * @param symbol кодируемый символ
 * @param result результат - позиция искомого символа
 * @return - код возврата (ошибка или ок)
 */
ReturnCode findSymbolPosInDict(CipherInst *conf, char symbol, unsigned long *result)
{
	long charPos;	//случайная позиция, с которой будет производиться поиск символа
	if (conf->support->dictSelected == 0)	//если символ в памяти - произвести поиск и вернуть результат
	{
		charPos = randVal(conf, conf->dictLen);
		return findSymbolInMemory(conf->dict.dictInMemory, conf->dictLen, charPos, symbol, result);
	}

	int cycles = 0;
	long partLen = 0;
	long readRes;
	char *buffer = NULL;

	for (long i = 0; i <= conf->dictLen / MAX_FILE_BUF_SIZE; i++)
	{
		if (!conf->dict.dictInFile)	//файловый сокет закрыт
			return FileStreamIsClosed;	//дальнейший поиск невозможен

		if (!buffer)
		{
			buffer = malloc(partLen);	//можно выделять место под буфер
			if (!buffer)	//недостаточно места в памяти
				return findSymbolInFile(conf, symbol, result);	//произвести медленный поиск
		}

		long readRes = fread(buffer, 1, conf->dictLen, conf->dict.dictInFile);	//считать весь файл в буфер
		//  if (readRes != conf->dictLen)	//TODO обработка ошибки чтения

		charPos = randVal(conf, conf->dictLen);
		if (findSymbolInMemory(buffer, conf->dictLen, charPos, symbol, result) == OK)	//ищем символ по буферу
		{
			free(buffer);
			*result = ftell(conf->dict.dictInFile) - *result;//TODO неверное вычисление. Придётся рассчитывать относительно пройденных циклов
			return OK;
		}
	}

	return SymbolNotFound;
}

/**
 * Медленный поиск, который не использует буфер в оперативной памяти.
 * @param conf - рабочая конфигурация
 * @param symbol - символ, который требуется найти
 * @param result результат - позиция искомого символа
 * @return - код возврата (ошибка или ок)
 */
ReturnCode findSymbolInFile(CipherInst *conf, char symbol, unsigned long *result)
{
	char verySmallBuffer;

	fseek(conf->dict.dictInFile, 0, SEEK_END);	//определение размера файла
	unsigned long fileSize = ftell(conf->dict.dictInFile);

	unsigned long charPos = randVal(conf, conf->dictLen);	//поиск со случайного места
	fseek(conf->dict.dictInFile, charPos, SEEK_SET);

	for (unsigned long iterator = 0; iterator < fileSize; iterator++)
	{
		if (charPos + iterator == fileSize)		//обнуление позиции поиска
		{
			charPos = 0;
			rewind(conf->dict.dictInFile);				//поиск по файлу сначала
		}
		if (fgetc(conf->dict.dictInFile) == symbol)
		{
			*result = ftell(conf->dict.dictInFile) - 1;
			return OK;
		}
	}
	return SymbolNotFound;
}

/**
 * Производит поиск по памяти
 * @param memory - указатель на память
 * @param memLength - длина памяти
 * @param start - откуда искать
 * @param symbol - что искать
 * @param result результат - позиция искомого символа
 * @return - код возврата (ошибка или ок)
 */
ReturnCode findSymbolInMemory(char *memory, unsigned long memLength, unsigned long start, char symbol,
		unsigned long *result)
{
	for (unsigned long iterator = 0; iterator < memLength; iterator++)
	{
		if (start + iterator == memLength)		//обнуление позиции поиска
			start -= memLength;				//поиск по памяти сначала
		if (memory[start + iterator] == symbol)
		{
			*result = start + iterator;
			return OK;
		}
	}
	return SymbolNotFound;
}

