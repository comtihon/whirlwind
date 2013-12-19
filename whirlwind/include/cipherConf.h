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

typedef enum
{
	OK,
	DataAlreadySet,
	DictAlreadySet
} ReturnCode;

typedef struct
{
		long *withdrawHistory;		//данные для отката
		int withdrawCount;			//текущее значение отката
		short dictSelected;			//0 - словарь в памяти, 1 - словарь в файле
		short dataSelected;			//0 - дата в памяти, 1 - дата в файле
		struct drand48_data *randomBuffer;//буфер случайной последовательности (thread-safe).
}Support;

//структура настроек словаря
typedef struct
{
		//текущий словарь.
		union Dict
		{
				char *dictInMemory;	//указатель на память с заранее считанным словарём
				FILE *dictInFile;	//указатель на файл со словарём
		} dict;
		long cryptLen;				//длина кодируемой информации

		//дата, которую нужно зашифровать
		union Data
		{
				char *cryptString;	//указатель на строку в памяти
				FILE *cryptFile;	//укзаатель на файл
		} data;
		long dictLen;				//длина словаря

		Support *support;

		int variability;			//изменчивость словаря
		int withdraw;				//значение отката
		//TODO callback function pointer
} CipherInst;

/**
 * Инициализирует сущность с настройками шифра
 * @return указатель на сущность или NULL
 */
extern CipherInst *init(long randInit, int variability, int withdraw,
        long cryptLen);

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
