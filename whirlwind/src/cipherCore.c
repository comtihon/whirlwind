/*
 * cipherCore.c
 *
 *  Created on: 19 дек. 2013 г.
 *      Author: tihon
 */

#include "cipherCore.h"

ReturnCode cryptFileToFile(CipherInst *conf, FILE *encryptedFile, FILE *cleanFile)
{
	if (!cleanFile || !encryptedFile)
	{
		printf("File to work with is closed!\n");
		return FileStreamIsClosed;
	}
	return cryptFile(conf, encryptedFile, cleanFile, writeToFile);
}

ReturnCode cryptFileToMemory(CipherInst *conf, FILE *encryptedFile, unsigned long *result)
{
	if (!encryptedFile)
	{
		printf("File to work with is closed!\n");
		return FileStreamIsClosed;
	}
	return cryptFile(conf, encryptedFile, result, writeToMemory);
}

/**
 *
 * @param cleanFile
 * @param partResult
 * @param partResultLen
 * @return
 */
ReturnCode writeToFile(FILE *cleanFile, unsigned long *partResult, unsigned long partResultLen)
{
	if (!cleanFile)
	{
		//TODO обработка ошибок
	}
	unsigned long result = fwrite(partResult, sizeof(unsigned long), partResultLen, cleanFile);
	//TODO обработка ошибок записи
	return OK;
}

/**
 *
 * @param result
 * @param partResult
 * @param partResultLen
 * @return
 */
ReturnCode writeToMemory(unsigned long *result, unsigned long *partResult, unsigned long partResultLen)
{
	for(unsigned long i = 0; i < partResultLen; i++)
	{
		*result = partResult[i];
		result++;	//TODO протестировать при нескольких вызовах!
	}
	return OK;
}

ReturnCode decryptFileToFile(CipherInst *conf, FILE *encryptedFile, FILE *cleanFile)
{
	ReturnCode ret = OK;
	return OK;
}

ReturnCode decryptFileToMemory(CipherInst *conf, FILE *encryptedFile, unsigned long *result)
{
	ReturnCode ret = OK;
	return OK;
}

ReturnCode cryptFile(CipherInst *conf, FILE *encryptedFile, void *forResult,
        ReturnCode (*saveResult)(void *, unsigned long *, unsigned long))
{
	ReturnCode ret = OK;

	//определение размера файла
	fseek(encryptedFile, 0, SEEK_END);
	unsigned long fileSize = ftell(encryptedFile);
	rewind(encryptedFile);

	unsigned long partLen;

	char *buffer = NULL;
	unsigned long partResultLen = 0;
	unsigned long *partResult;

	long cycles = fileSize / MAX_FILE_BUF_SIZE;
	printf("Number of cycles = %d\n", cycles);

	for (long i = 0; i <= cycles; i++)	//цикл разбивает файл на куски по 10мб и кодирует каждый кусок
	{
		if (!encryptedFile)	//файловый сокет закрыт
		    return FileStreamIsClosed;	//дальнейший поиск невозможен
		partLen = fileSize > MAX_FILE_BUF_SIZE ? MAX_FILE_BUF_SIZE : fileSize;	//рассчитать размер куска
		fileSize -= partLen;

		//выделить в памяти место под считываемые и кодируемые данные
		if (partResultLen * 2 != partLen)
		{
			partResult = realloc(partResult, 2 * partLen * sizeof(unsigned long));
			partResultLen = partLen;
		}
		if (!buffer) buffer = malloc(partLen);

		//проверить выделенную память
		if (!buffer || !partResult)
		{	//память не выделилась (недостаточно памяти)
			if (buffer) free(buffer);
			if (partResult) free(partResult);
			return;	//TODO медленное кодирование файла
		}

		partResultLen = fread(buffer, 1, partLen, encryptedFile);	//считать кусок в буфер
		//  if (partResultLen != partLen)	//TODO обработка ошибки чтения

		if ((ret = cryptString(conf, buffer, partResultLen, partResult)) != OK)		//произошла ошибка кодирования
		{
			free(buffer);
			free(partResult);
			return ret;
		}
		else
			(*saveResult)(forResult, partResult, partResultLen);	//запись результата в файл или память
	}
	free(buffer);
	free(partResult);
	return ret;
}

/**
 * Кодирует строку символов. Возвращает код ошибки (или успеха).
 * Результат - закодированная строка, записанная в переменную.
 * @param conf - рабочая конфигурацияя
 * @param string - кодируемая строка
 * @param stringLen - длина строки
 * @param result - указатель на результат - long[stringLen * 2]
 * @return код возврата (ошибка либо успех)
 */
ReturnCode cryptString(CipherInst *conf, char *string, unsigned long stringLen, unsigned long *result)
{
	ReturnCode ret = OK;
	unsigned long *tempRes = malloc(2 * sizeof(unsigned long));
	unsigned long m = 0;
	for (unsigned long i = 0; i < stringLen; i++)
	{
		if ((ret = cryptOneSymbol(conf, string[i], tempRes)) != OK)
		{
			printf("Error crypting symbol %c[%d]\n", string[i], string[i]);
			free(tempRes);
			return ret;
		}
		result[m++] = tempRes[0];
		result[m++] = tempRes[1];
	}
	free(tempRes);
	return ret;
}

/**
 * Декодирует строку пар шифрокодов в строку символов.
 * @param conf - рабочая конфигурация
 * @param cryptedPairs - закодированная строка
 * @param pairsLen - длина закодированной строки
 * @param result - указатель на буфер, в который будет писаться результат
 * @return код возврата (ошибка либо успех)
 */
ReturnCode decryptString(CipherInst *conf, unsigned long *cryptedPairs, unsigned long pairsLen, char *result)
{
	ReturnCode ret = OK;
	char tempRes;
	unsigned long temp[2];
	unsigned long iter = 0;
	for (unsigned long i = 0; i < pairsLen; i++)
	{
		temp[0] = cryptedPairs[iter++];
		temp[1] = cryptedPairs[iter++];
		if ((ret = decryptOneSymbol(conf, temp, &tempRes)) != OK)
		{
			printf("Error decrypting pair %ld - %ld\n", cryptedPairs[i], cryptedPairs[i]);
			return ret;
		}
		result[i] = tempRes;
	}
	return ret;
}

//TODO провести тайминг каждой функции.
/**
 * Кодирует 1 символ. Возвращает код ошибки (или успеха).
 * Результат - пара шифрокодов, записывается в переменную.
 * @param conf - рабочая конфигурацияя
 * @param symbol - кодируемый символ
 * @param result - указатель на результат - long[2]
 * @return код возврата (ошибка либо успех)
 */
ReturnCode cryptOneSymbol(CipherInst *conf, char symbol, unsigned long *result)
{
	unsigned long charPos;
	//найти кодируемый символ
	switch (findSymbolPosInDict(conf, symbol, &charPos))
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
	if (pair[0] > conf->dictLen) return ArrayOutOfBounds;
	if (conf->support->dictSelected == 0)	//работа со словарём в оперативной памяти
		result[0] = conf->dict.dictInMemory[pair[0]];
	else
	{	//работа со словарём в файле
		fseek(conf->dict.dictInFile, pair[0], SEEK_SET);
		fread(&result[0], 1, 1, conf->dict.dictInFile);
	}
	srand48_r(pair[1], conf->support->randomBuffer);
	long randNum = randVal(conf, conf->dictLen);
	if (processWithdraw(conf, pair) == OK)	//отката не было
	{
		if ((ret = changeDict(conf, &pair[0], &randNum)) != OK) return ret;
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
	if (conf->dict.dictInFile)
		rewind(conf->dict.dictInFile);
	else
		return FileStreamIsClosed;

	unsigned long totalLen = conf->dictLen;

	char *buffer = NULL;
	for (long i = 0; i <= totalLen / MAX_FILE_BUF_SIZE; i++)
	{
		partLen = totalLen > MAX_FILE_BUF_SIZE ? MAX_FILE_BUF_SIZE : totalLen;
		totalLen -= partLen;

		if (!conf->dict.dictInFile)	//файловый сокет закрыт
		    return FileStreamIsClosed;	//дальнейший поиск невозможен

		if (!buffer)
		{
			buffer = malloc(partLen);	//можно выделять место под буфер
			if (!buffer)	//недостаточно места в памяти
			    return findSymbolInFile(conf, symbol, result);	//произвести медленный поиск
		}

		long readRes = fread(buffer, 1, partLen, conf->dict.dictInFile);	//считать весь файл в буфер
		//  if (readRes != partLen)	//TODO обработка ошибки чтения

		charPos = randVal(conf, conf->dictLen);
		if (findSymbolInMemory(buffer, partLen, charPos, symbol, result) == OK)	//ищем символ по буферу
		{
			free(buffer);
			*result = *result + MAX_FILE_BUF_SIZE * i;
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

