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
 * @return 0 - откат выполнен, 1 - откат не нужно выполнять.
 */
short processWithdraw(CipherInst *conf, long *result)
{
	for (int i = 0; i < 2; i++)	//сохранение данных для отката
		conf->support->withdrawHistory[conf->support->withdrawCount++] = result[i];

	if (conf->withdraw && conf->support->withdrawCount == conf->withdraw)	//время отката (если он включён)
	{
		long **extraPairs = malloc(conf->variability * sizeof(long *));	//выделить память под случайные числа
		for (int i = 0; i < conf->variability; i++)
			extraPairs[i] = malloc(2 * sizeof(long));

		withdraw(conf, extraPairs);	//выполнить откаты

		for (int i = 0; i < conf->variability; i++)	//освободить память
			free(extraPairs[i]);
		free(extraPairs);

		return 0;
	}
	return 1;
}

/**
 * Производит откаты
 * @param conf - рабочая конфигурация
 * @param extraPairs - память для организации рекурсии случайных чисел
 */
void withdraw(CipherInst *conf, long **extraPairs)
{
	long rand = 0;
	//откатить все изменения, кроме последних двух
	for (int i = conf->withdraw - 3; i >= 0; i -= 2)
	{
		srand48_r(conf->support->withdrawHistory[i], conf->support->randomBuffer);
		if (conf->variability > 0)		//если были случайные изменения - отменить их
			revertExtraChangeDict(conf, extraPairs);
		rand = randVal(conf, conf->dictLen);
		changeDict(conf, &rand, &conf->support->withdrawHistory[i - 1]);
	}
	srand48_r(time(NULL), conf->support->randomBuffer);//TODO как это здесь (и везде) влияет на надёжность и зачем вообще нужно?
	conf->support->withdrawCount = 0;	//обнулить счётчик откатов
}

/**
 * Обрабатывает изменение словаря
 * @param conf - рабочая конфигурация
 * @param result - результат кодирования [позиция кодируемого символа, случайный инициализатор]
 */
void processChange(CipherInst *conf, long *result)
{
	srand48_r(result[1], conf->support->randomBuffer);	//задать новую псевдослучайную последовательность
	long randomPos = randVal(conf, conf->dictLen);	//взять случайный символ в словаре
	changeDict(conf, result, &randomPos);	//поменять местами закодированный и случайный символ
	if (conf->variability)	//если задана дополнительная изменчивость - выполнить её
		extraChangeDict(conf);
}

/**
 * Меняет в словаре местами 2 символа.
 * @param firstPos - указатель на позицию первого символ
 * @param secondPos - указатель на позицию второго символа
 */
void changeDict(CipherInst *conf, long *firstPos, long *secondPos)
{
	if(*firstPos == *secondPos) //если позиции равны
		return;
	if (conf->support->dictSelected == 0)
	{	//работа с оперативной памятью
		char buf = conf->dict.dictInMemory[*firstPos];
		conf->dict.dictInMemory[*firstPos] = conf->dict.dictInMemory[*secondPos];
		conf->dict.dictInMemory[*secondPos] = buf;
	}
	else
	{	//работа с файлом
	    //TODO сделать изменение по файлу
	}
}

/**
 * Производит дополнительные изменения в словаре
 * @param conf - конфигурация
 */
void extraChangeDict(CipherInst *conf)
{
	long randomPos = 0;
	long secondRandPos = 0;	//TODO проценты вместо цикла от 1 до значения!
	for (int i = 0; i < conf->variability; i++)	//изменение словаря в зависимости от изменчивости
	{	//extraChangeDict
		randomPos = randVal(conf, conf->dictLen);
		secondRandPos = randVal(conf, conf->dictLen);
		changeDict(conf, &randomPos, &secondRandPos);
	}
}

/**
 * Отменяет дополнительные изменения словаря, сделанные с помощью extraChangeDict
 * @param conf - рабочая конфигурация
 * @param extraPairs - N случайных чисел. Генерируются и используются рекурсино.
 */
void revertExtraChangeDict(CipherInst *conf, long **extraPairs)
{
	for (int i = 0; i < conf->variability; i++)	//достать случайные значения
	{
		extraPairs[i][0] = randVal(conf, conf->dictLen);
		extraPairs[i][1] = randVal(conf, conf->dictLen);
	}
	for (int i = conf->variability - 1; i >= 0; i--)//поменять местами обратно с конца - случайные места друг на друга
		changeDict(conf, &extraPairs[i][0], &extraPairs[i][1]);
}
