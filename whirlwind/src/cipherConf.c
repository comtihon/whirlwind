/*
 * cipherConf.c
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#include "cipherConf.h"

/**
 * Инициализирует сущность с настройками шифра
 * @param randInit - начальный инициализатор псевдослучайной последовательности
 * @param variability - численное значение изменчивости словаря в % (от 1 до 70)
 * @param withdraw - численное значение отката
 * @param withdrawDepth - глубина отката (не может быть больше его численного значения)
 * @return указатель на сущность или NULL
 */
CipherInst *init(long randInit, int variability, int withdraw, int withdrawDepth)
{
	if (withdrawDepth > withdraw)
	{
		printf("withdrawDepth can't be more, than withdrawCount!\n");
		return NULL;
	}

	CipherInst *instance = malloc(sizeof(CipherInst));	//выделить память под основную структуру
	if (!instance)
	{
		printf("Error allocating space for conf!\n");
		return NULL;
	}

	instance->support = malloc(sizeof(Support));	//выделить память под дополнительную структуру
	if (!instance->support)
	{
		printf("Error allocating space for conf`s support!\n");
		return NULL;
	}
	instance->support->lastWithdraw = 0;

	if (withdraw)
	{	//если включены откаты - выделить память под историю откатов и установить значения переменныхs
		instance->support->withdrawHistory = malloc(withdraw * sizeof(unsigned long*));
		if (!instance->support->withdrawHistory)
		{
			printf("Error allocating space for support's withdraw history!\n");
			return NULL;
		}
		for (int i = 0; i < withdrawDepth; i++)
		{
			instance->support->withdrawHistory[i] = malloc(2 * sizeof(unsigned long));
			if (!instance->support->withdrawHistory[i])
			{
				printf("Error allocating space for support's withdraw history!\n");
				return NULL;
			}
		}
		instance->withdraw = withdraw;	//откат - сброс 2х значений (символ-инициализатор).
		instance->withdrawDepth = withdrawDepth;
	}
	instance->variability = variability > 70 ? 70 : variability;
	instance->support->randomBuffer = malloc(sizeof(RandBuf));//выделить память под буфер псевдослучайной последовательности
	if (!instance->support->randomBuffer)
	{
		printf("Error allocating space for support's random buffer struct!\n");
		return NULL;
	}
	srand48_r(randInit, instance->support->randomBuffer);
	return instance;
}

/**
 * Добавляет в настройки словарь.
 * @param dict указатель на словарь
 * @param conf указатель на настройки
 * @param dictLen длина словаря
 * @return enum код возврата
 */
ReturnCode setDictWithMemory(char *dict, CipherInst *conf, unsigned long dictLen)
{
	if (conf->dict.dictInMemory)
	{
		printf("Error setting dict! Dict was already set.");
		return DictAlreadySet;
	}
	else
	{
		conf->dict.dictInMemory = dict;
		conf->support->dictSelected = 0;
		conf->dictLen = dictLen;
		return OK;
	}
}

ReturnCode setDictWithFile(FILE *dict, CipherInst *conf, unsigned long dictLen)
{
	if (conf->dict.dictInFile)
	{
		printf("Error setting dict! Dict was already set.");
		return DictAlreadySet;
	}
	else
	{
		conf->dict.dictInFile = dict;
		conf->support->dictSelected = 1;
		conf->dictLen = dictLen;
		printf("dictLen = %ld\n", dictLen);
		return OK;
	}
}

/**
 * Добавляет в настройки кодируемые данные
 * @param data указатель на данные
 * @param указатель на настройки
 * @param cryptLen длина кодируемой информации
 */
ReturnCode setDataWithFile(FILE *data, CipherInst *conf, unsigned long cryptLen)
{
	if (conf->data.cryptFile)
	{
		printf("Error setting data! Data was already set.");
		return DataAlreadySet;
	}
	else
	{
		conf->data.cryptFile = data;
		conf->support->dataSelected = 1;
		conf->cryptLen = cryptLen;
		return OK;
	}
}

/**
 * Освобождает память, занятую под настройки шифра
 * @param указатель на сущность с настройками
 */
void freeInst(CipherInst *conf)
{
	for (int i = 0; i < conf->withdrawDepth; i++)
		free(conf->support->withdrawHistory[i]);
	free(conf->support->withdrawHistory);
	free(conf->support->randomBuffer);
	free(conf->support);
	free(conf);
}
