/*
 * cipherConf.h
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#ifndef CIPHER_H_
#define CIPHER_H_

#include "stdio.h"
#include "stdlib.h"

typedef enum {
	OK,
	DataAlreadySet,
	DictAlreadySet
}ReturnCode;

//структура настроек словаря
typedef struct
{
		//текущий словарь.
		union Dict{
				char *dictInMemory;	//указатель на память с заранее считанным словарём
				FILE *dictInFile;	//указатель на файл со словарём
		}dict;

		//дата, которую нужно зашифровать
		union Data{
				char *cryptString;	//указатель на строку в памяти
				FILE *cryptFile;	//укзаатель на файл
		}data;
		int randInit;				//инициализатор случайной последовательности.
		long cryptLen;				//длина кодируемой информации

		int variability;			//изменчивость словаря
		int withdraw;				//значение отката
		int withdrawCount;			//текущее значение отката
		//TODO callback function pointer
}CipherInst;

/**
 * Инициализирует сущность с настройками шифра
 * @return указатель на сущность или NULL
 */
extern CipherInst *init(int variability, int withdraw, long cryptLen);

/**
 * Добавляет в настройки словарь.
 * @param dict указатель на словарь
 * @param указатель на настройки
 */
extern ReturnCode setDictWithMemory(char *dict, CipherInst *);
extern ReturnCode setDictWithFile(FILE *dict, CipherInst *);

/**
 * Добавляет в настройки кодируемые данные
 * @param data указатель на данные
 * @param указатель на настройки
 */
extern ReturnCode setDataWithMemory(char *data, CipherInst *);
extern ReturnCode setDataWithFile(FILE *data, CipherInst *);


/**
 * Освобождает память, занятую под настройки шифра
 * @param указатель на сущность с настройками
 */
extern void freeInst(CipherInst *);

#endif /* CIPHER_H_ */
