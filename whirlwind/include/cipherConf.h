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
	DictAlreadySet,
	SymbolNotFoundInDict
} ReturnCode;

typedef struct drand48_data
{
	unsigned short int __x[3];	/* Current state.  */
	unsigned short int __old_x[3]; /* Old state.  */
	unsigned short int __c;	/* Additive const. in congruential formula.  */
	unsigned short int __init;	/* Flag for initializing.  */
	__extension__ unsigned long long int __a;	/* Factor in congruential
					   formula.  */
}RandBuf;

typedef struct
{
		long *withdrawHistory;		//данные для отката
		int lastWithdraw;			//последний записанный откат
		int withdrawCount;			//текущее значение отката
		short dictSelected;			//0 - словарь в памяти, 1 - словарь в файле
		short dataSelected;			//0 - дата в памяти, 1 - дата в файле
		RandBuf *randomBuffer;//буфер случайной последовательности (thread-safe).
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
 * @param randInit - начальный инициализатор псевдослучайной последовательности
 * @param variability - численное значение изменчивости словаря в % (от 1 до 100)
 * @param withdraw - численное значение отката
 * @return указатель на сущность или NULL
 */
extern CipherInst *init(long randInit, int variability, int withdraw);

/**
 * Добавляет в настройки словарь.
 * @param dict указатель на словарь
 * @param conf указатель на настройки
 * @param dictLen длина словаря
 * @return enum код возврата
 */
extern ReturnCode setDictWithMemory(char *dict, CipherInst *conf, long dictLen);
extern ReturnCode setDictWithFile(FILE *dict, CipherInst *conf, long dictLen);

/**
 * Добавляет в настройки кодируемые данные
 * @param data указатель на данные
 * @param указатель на настройки
 */
extern ReturnCode setDataWithMemory(char *data, CipherInst *conf, long cryptLen);
extern ReturnCode setDataWithFile(FILE *data, CipherInst *conf, long cryptLen);

/**
 * Освобождает память, занятую под настройки шифра
 * @param указатель на сущность с настройками
 */
extern void freeInst(CipherInst *);

#endif /* CIPHER_H_ */
