/*
 * cipherCore.h
 *
 *  Created on: 19 дек. 2013 г.
 *      Author: tihon
 */

#ifndef CIPHERCORE_H_
#define CIPHERCORE_H_

#define MAX_FILE_BUF_SIZE 1000000

#include "cipherSupport.h"
#include "cipherConf.h"
#include "cipherAddon.h"

/**
 * Кодирует строку символов. Возвращает код ошибки (или успеха).
 * Результат - закодированная строка, записанная в переменную.
 * @param conf - рабочая конфигурацияя
 * @param string - кодируемая строка
 * @param stringLen - длина строки
 * @param result - указатель на результат - long[stringLen * 2]
 * @return код возврата (ошибка либо успех)
 */
extern ReturnCode cryptString(CipherInst *conf, char *string, unsigned long stringLen, unsigned long *result);

/**
 * Декодирует строку пар шифрокодов в строку символов.
 * @param conf - рабочая конфигурация
 * @param cryptedPairs - закодированная строка
 * @param pairsLen - длина закодированной строки
 * @param result - указатель на буфер, в который будет писаться результат
 * @return код возврата (ошибка либо успех)
 */
extern ReturnCode decryptString(CipherInst *conf, unsigned long *cryptedPairs, unsigned long pairsLen, char *result);

/**
 * Кодирует 1 символ. Возвращает код ошибки (или успеха).
 * Результат - пара шифрокодов, записывается в переменную.
 * @param conf - рабочая конфигурацияя
 * @param symbol - кодируемый символ
 * @param result - указатель на результат - long[2]
 * @return код возврата (ошибка либо успех)
 */
extern ReturnCode cryptOneSymbol(CipherInst *conf, char symbol, unsigned long *result);

/**
 * Декодирует пару шифрокодов в 1 символ.
 * @param conf - рабочая конфигурация
 * @param pair - пара шифросимволов - long[2]
 * @param result - указатель на символ, в который нужно записать результат
 * @return код возврата (ошибка либо успех)
 */
extern ReturnCode decryptOneSymbol(CipherInst *conf, unsigned long *pair, char *result);

/**
 * Возвращает позицию символа в словаре или -1, если символ отсутствует.
 * @param conf	рабочая конфигурация
 * @param symbol кодируемый символ
 * @param result результат - позиция искомого символа
 * @return - код возврата (ошибка или ок)
 */
ReturnCode findSymbolPosInDict(CipherInst *conf, char symbol, unsigned long *result);

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
		unsigned long *result);

/**
 * Медленный поиск, который не использует буфер в оперативной памяти.
 * @param conf - рабочая конфигурация
 * @param symbol - символ, который требуется найти
 * @param result результат - позиция искомого символа
 * @return - код возврата (ошибка или ок)
 */
ReturnCode findSymbolInFile(CipherInst *conf, char symbol, unsigned long *result);

#endif /* CIPHERCORE_H_ */
