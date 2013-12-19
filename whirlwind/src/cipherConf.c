/*
 * cipherConf.c
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#include "cipherConf.h"

/**
 * Инициализирует сущность с настройками шифра
 * @return указатель на сущность или NULL
 */
CipherInst *init(int variability, int withdraw, long cryptLen)
{
	CipherInst *instance = malloc (sizeof(CipherInst));
	if(!instance)
	{
		printf("Error allocating space for conf!\n");
		return NULL;
	}
	return instance;
}

/**
 * Добавляет в настройки словарь.
 * @param dict указатель на словарь
 * @param указатель на настройки
 */
ReturnCode setDictWithMemory(char *dict, CipherInst *conf)
{
	if(conf->dict.dictInMemory)
	{
		printf("Error setting dict! Dict was already set.");
		return DictAlreadySet;
	}
	else
	{
		conf->dict.dictInMemory = dict;
		return OK;
	}
}

ReturnCode setDictWithFile(FILE *dict, CipherInst *conf)
{
	if(conf->dict.dictInFile)
	{
		printf("Error setting dict! Dict was already set.");
		return DictAlreadySet;
	}
	else
	{
		conf->dict.dictInFile = dict;
		return OK;
	}
}

/**
 * Добавляет в настройки кодируемые данные
 * @param data указатель на данные
 * @param указатель на настройки
 */
ReturnCode setDataWithMemory(char *data, CipherInst *conf)
{
	if(conf->data.cryptString)
	{
		printf("Error setting data! Data was already set.");
		return DataAlreadySet;
	}
	else
	{
		conf->data.cryptString = data;
		return OK;
	}
}

ReturnCode setDataWithFile(FILE *data, CipherInst *conf)
{
	if(conf->data.cryptFile)
	{
		printf("Error setting data! Data was already set.");
		return DataAlreadySet;
	}
	else
	{
		conf->data.cryptFile = data;
		return OK;
	}
}


/**
 * Освобождает память, занятую под настройки шифра
 * @param указатель на сущность с настройками
 */
void freeInst(CipherInst *conf)
{
	free(conf);
}
