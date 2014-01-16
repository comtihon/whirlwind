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
extern ReturnCode cryptString(CipherInst *conf, const char *string, unsigned long stringLen, unsigned long *result);

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
 * Шифрует файл. Шифротекст записивает в другой файл. По окончании закрывает файловый поток.
 * @param conf - рабочая конфигурация
 * @param cleanFile - шифруемый файловый поток
 * @param encryptedFile - зашифрованный файл
 * @return код возврата (ошибка либо успех)
 */
extern ReturnCode cryptFileToFile(CipherInst *conf, FILE *cleanFile, FILE *encryptedFile);

/**
 * Шифрует файл. Шифротекст записывает в память - в результат result.
 * @param conf - рабочая конфигурация
 * @param cleanFile - шифруемый файл
 * @param result - массив unsigned long для записи результата. Размер - размер файла (в байтах) * 2.
 * @return код возврата (ошибка либо успех)s
 */
extern ReturnCode cryptFileToMemory(CipherInst *conf, FILE *cleanFile, unsigned long *result);

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
 * Производит кодирование файла. Результат отдаёт в функцию, которая производит сохранение.
 * Делит файл на части по N мегабайт и кодирует каждую часть, сохраняя результат в функции.
 * @param conf - рабочая конфигурация
 * @param cleanFile - кодируемый файл
 * @param forResult - указатель на место сохранение результата
 * @param saveResult - указатель на функцию, которая сохраняет результат
 * @return
 */
ReturnCode cryptFile(CipherInst *conf, FILE *cleanFile, void *forResult,
        ReturnCode (*saveResult)(void *, unsigned long *, unsigned long));

/**
 * Записывает в файл пару шифросимволов.
 * @param encryptedFile - файл для записи шифросимволов
 * @param partResult - пара шифросимволов
 * @param partResultLen - позиция для записи
 * @return код возврата (ошибка либо успех)
 */
ReturnCode writeToFile(FILE *encryptedFile, unsigned long *partResult, unsigned long partResultLen);

/**
 * Записывает пару шифросимволов в память.
 * @param result - память для записи.
 * @param partResult - пара шифросимволов
 * @param partResultLen - позиция для записи
 * @return код возврата (ошибка либо успех)
 */
ReturnCode writeToMemory(unsigned long *result, unsigned long *partResult, unsigned long partResultLen);

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
