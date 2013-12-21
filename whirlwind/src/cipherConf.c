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
 * @return указатель на сущность или NULL
 */
CipherInst *init(long randInit, int variability, int withdraw)
{
	CipherInst *instance = malloc (sizeof(CipherInst));
	if(!instance)
	{
		printf("Error allocating space for conf!\n");
		return NULL;
	}

	instance->support = malloc (sizeof(Support));
	if(!instance->support)
	{
		printf("Error allocating space for conf`s support!\n");
		return NULL;
	}
	instance->support->lastWithdraw=0;

	instance->support->withdrawHistory = malloc(2 * withdraw * sizeof(long));
	if(!instance->support->withdrawHistory)
	{
		printf("Error allocating space for support's withdraw history!\n");
		return NULL;
	}

	instance->variability = variability > 70? 70 : variability;
	instance->withdraw = withdraw * 2;	//откат - сброс 2х значений (символ-инициализатор). Требуется удвоить.
	instance->support->randomBuffer = malloc(sizeof(RandBuf));
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
ReturnCode setDictWithMemory(char *dict, CipherInst *conf, long dictLen)
{
	if(conf->dict.dictInMemory)
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

ReturnCode setDictWithFile(FILE *dict, CipherInst *conf, long dictLen)
{
	if(conf->dict.dictInFile)
	{
		printf("Error setting dict! Dict was already set.");
		return DictAlreadySet;
	}
	else
	{
		conf->dict.dictInFile = dict;
		conf->support->dictSelected = 1;
		conf->dictLen = dictLen;
		return OK;
	}
}

/**
 * Добавляет в настройки кодируемые данные
 * @param data указатель на данные
 * @param указатель на настройки
 * @param cryptLen длина кодируемой информации
 */
ReturnCode setDataWithMemory(char *data, CipherInst *conf, long cryptLen)
{
	if(conf->data.cryptString)
	{
		printf("Error setting data! Data was already set.");
		return DataAlreadySet;
	}
	else
	{
		conf->data.cryptString = data;
		conf->support->dataSelected = 0;
		conf->cryptLen = cryptLen;
		return OK;
	}
}

ReturnCode setDataWithFile(FILE *data, CipherInst *conf, long cryptLen)
{
	if(conf->data.cryptFile)
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
	free(conf->support->withdrawHistory);
	free(conf->support->randomBuffer);
	free(conf->support);
	free(conf);
}
