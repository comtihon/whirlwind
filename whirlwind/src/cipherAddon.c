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
	for(int i = 0; i < 2; i++)	//сохранение данных для отката
		conf->support->withdrawHistory[conf->support->withdrawCount++] = result[i];

	if(conf->support->withdrawCount == conf->withdraw)	//время отката
	{
		for(int i = 0; i < conf->withdraw; i++)
		{
			withdraw(conf);
		}
		return 0;
	}
	return 1;
}

/**
 * Обрабатывает изменение словаря
 * @param conf - рабочая конфигурация
 * @param result - результат кодирования [позиция кодируемого символа, случайный инициализатор]
 */
void processChange(CipherInst *conf, long *result)
{
	srand48_r(result[1], &conf->support->randomBuffer);	//задать новую псевдослучайную последовательность
	long randomPos = randVal(conf, conf->dictLen);		//взять случайный символ в словаре
	changeDict(conf, result, &randomPos);	//поменять местами закодированный и случайный символ

	long secondRandPos = 0;	//TODO проценты вместо цикла от 1 до значения!
	for(int i = 0; i < conf->variability; i++)	//изменение словаря в зависимости от изменчивости
	{
		randomPos = randVal(conf, conf->dictLen);
		secondRandPos = randVal(conf, conf->dictLen);
		changeDict(conf, &randomPos, &secondRandPos);
	}
}

/**
 * Меняет в словаре местами 2 символа.
 * @param firstPos - указатель на позицию первого символ
 * @param secondPos - указатель на позицию второго символа
 */
void changeDict(CipherInst *conf, long *firstPos, long *secondPos)
{
	if(conf->support->dictSelected==0)
	{//работа с оперативной памятью
		char buf = conf->dict.dictInMemory[*firstPos];
		conf->dict.dictInMemory[*firstPos] = conf->dict.dictInMemory[*secondPos];
		conf->dict.dictInMemory[*secondPos] = buf;
	}
	else
	{//работа с файлом
		//TODO сделать изменение по файлу
	}
}

void withdraw(CipherInst *conf)
{
	long randInt = 0;
	//откатить все изменения, кроме последних двух
	for(int i = conf->support->withdrawCount - 3; i >= 0; i-=2)
	{
		srand48_r(conf->support->withdrawHistory[i], &conf->support->randomBuffer);
		randInt = randVal(conf, conf->dictLen);
		//TODO revertExtra changes
		changeDict(randVal(conf, conf->dictLen), conf->support->withdrawHistory[i-1]);
	}

	srand48_r(time(NULL), &conf->support->randomBuffer);	//TODO как это здесь (и везде) влияет на надёжность и зачем вообще нужно?

	//TODO откатить все изменения словаря кроме последних 2х (т.к. они не вызывались)
}
