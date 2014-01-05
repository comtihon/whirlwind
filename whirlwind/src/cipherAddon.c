/*
 * cipherAddon.c
 *
 *  Created on: 20 дек. 2013 г.
 *      Author: tihon
 */

#include "cipherAddon.h"

/**
 * Запоминает закодированный результат. Если подошло время отката - выполняет откат.
 * @param conf - конфигурация
 * @param result - указатель на результат шифрования (пара ключ-инициализатор)
 * @return Withdraw - откат выполнен, OK - откат не нужно выполнять, или ошибка
 */
ReturnCode processWithdraw(CipherInst *conf, unsigned long *result)
{
	for (int i = 0; i < 2; i++)	//сохранение данных для отката
		conf->support->withdrawHistory[conf->support->withdrawCount++] = result[i];

	if (conf->withdraw && conf->support->withdrawCount == conf->withdraw)	//время отката (если он включён)
	{
		printf("Withdraw\n");
		unsigned long **extraPairs = malloc(conf->variability * sizeof(unsigned long *));//выделить память под случайные числа
		if (extraPairs)
		{
			for (int i = 0; i < conf->variability; i++)
			{
				extraPairs[i] = malloc(2 * sizeof(unsigned long));
				if (!extraPairs[i])
				{
					printf("Error allocating space for widthraw support [i]!\n", i);
					return ErrorAllocatingMemory;
				}
			}
		}
		else
		{
			printf("Error allocating space for widthraw support!\n");
			return ErrorAllocatingMemory;
		}

		ReturnCode ret = withdraw(conf, extraPairs);	//выполнить откаты

		for (int i = 0; i < conf->variability; i++)	//освободить память
			free(extraPairs[i]);
		free(extraPairs);

		return ret;
	}
	return OK;
}

/**
 * Производит откаты
 * @param conf - рабочая конфигурация
 * @param extraPairs - память для организации рекурсии случайных чисел
 * @return Withdraw или ошибку
 */
ReturnCode withdraw(CipherInst *conf, unsigned long **extraPairs)
{
	long rand = 0;
	ReturnCode ret;
	//откатить все изменения, кроме последних двух
	for (int i = conf->withdraw - 3; i >= 0; i -= 2)
	{
		srand48_r(conf->support->withdrawHistory[i], conf->support->randomBuffer);
		if (conf->variability > 0)		//если были случайные изменения - отменить их
			ret = revertExtraChangeDict(conf, extraPairs);
		if (ret != OK)
			return ret;
		rand = randVal(conf, conf->dictLen);
		ret = changeDict(conf, &rand, &conf->support->withdrawHistory[i - 1]);
		if (ret != OK)
			return ret;
	}
	srand48_r(time(NULL), conf->support->randomBuffer);	//TODO как это здесь (и везде) влияет на надёжность и зачем вообще нужно?
	conf->support->withdrawCount = 0;	//обнулить счётчик откатов
	return Withdraw;
}

/**
 * Обрабатывает изменение словаря
 * @param conf - рабочая конфигурация
 * @param result - результат кодирования [позиция кодируемого символа, случайный инициализатор]
 * @return ok или ошибку
 */
ReturnCode processChange(CipherInst *conf, unsigned long *result)
{
	ReturnCode ret;
	srand48_r(result[1], conf->support->randomBuffer);	//задать новую псевдослучайную последовательность
	long randomPos = randVal(conf, conf->dictLen);	//взять случайный символ в словаре
	if ((ret = changeDict(conf, &result[0], &randomPos)) != OK)	//поменять местами закодированный и случайный символ
		return ret;
	if (conf->variability)	//если задана дополнительная изменчивость - выполнить её
		ret = extraChangeDict(conf);
	return ret;
}

/**
 * Меняет в словаре местами 2 символа.
 * @param firstPos - указатель на позицию первого символ
 * @param secondPos - указатель на позицию второго символа
 * @return код ошибки или ок
 */
ReturnCode changeDict(CipherInst *conf, unsigned long *firstPos, unsigned long *secondPos)
{//TODO изменения в словаре не происходят! (в памяти)
	if (*firstPos == *secondPos) //если позиции равны
		return OK;
	if (conf->support->dictSelected == 0)
	{	//работа с оперативной памятью
		char buf = conf->dict.dictInMemory[*firstPos];
		conf->dict.dictInMemory[*firstPos] = conf->dict.dictInMemory[*secondPos];
		conf->dict.dictInMemory[*secondPos] = buf;
	}
	else
	{	//работа с файлом
		if (!conf->dict.dictInFile)
		{
			printf("Can't change dictinary! File stream is closed.\n");
			return FileStreamIsClosed;
		}
		char buf1;
		fseek(conf->dict.dictInFile, *firstPos, SEEK_SET);
		fread(&buf1, 1, 1, conf->dict.dictInFile);

		char buf2;
		fseek(conf->dict.dictInFile, *secondPos, SEEK_SET);
		fread(&buf2, 1, 1, conf->dict.dictInFile);

		fseek(conf->dict.dictInFile, *firstPos, SEEK_SET);
		fwrite(&buf2, 1, 1, conf->dict.dictInFile);

		fseek(conf->dict.dictInFile, *secondPos, SEEK_SET);
		fwrite(&buf1, 1, 1, conf->dict.dictInFile);

		printf("change %c on %ld to %c on %ld\n", buf2, *secondPos, buf1, *firstPos);
	}
	return OK;
}

/**
 * Производит дополнительные изменения в словаре
 * @param conf - конфигурация
 * @return код ошибки или ок
 */
ReturnCode extraChangeDict(CipherInst *conf)
{
	ReturnCode ret = OK;
	long randomPos = 0;
	long secondRandPos = 0;	//TODO проценты вместо цикла от 1 до значения!
	for (int i = 0; i < conf->variability; i++)	//изменение словаря в зависимости от изменчивости
	{	//extraChangeDict
		randomPos = randVal(conf, conf->dictLen);
		secondRandPos = randVal(conf, conf->dictLen);
		if ((ret = changeDict(conf, &randomPos, &secondRandPos)) != OK)
			return ret;
	}
	return ret;
}

/**
 * Отменяет дополнительные изменения словаря, сделанные с помощью extraChangeDict
 * @param conf - рабочая конфигурация
 * @param extraPairs - N случайных чисел. Генерируются и используются рекурсино.
 * @return код ошибки или ок
 */
ReturnCode revertExtraChangeDict(CipherInst *conf, unsigned long **extraPairs)
{
	ReturnCode ret = OK;
	for (int i = 0; i < conf->variability; i++)	//достать случайные значения
	{
		extraPairs[i][0] = randVal(conf, conf->dictLen);
		extraPairs[i][1] = randVal(conf, conf->dictLen);
	}
	for (int i = conf->variability - 1; i >= 0; i--)//поменять местами обратно с конца - случайные места друг на друга
		if ((ret = changeDict(conf, &extraPairs[i][0], &extraPairs[i][1])) != OK)
			return ret;
	return ret;
}
