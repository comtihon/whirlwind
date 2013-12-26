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
 * Кодирует 1 символ. Возвращает массив - состоящий из пары шифросимволов.
 * @param conf рабочая конфигурацияя
 * @param symbol кодируемый символ
 * @param result указатель на результат - long[2]
 * @return массив long в куче. ВАЖНО! Освободить при ненадобности!
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
 */
unsigned long findSymbolPosInDict(CipherInst *conf, char symbol);

/**
 * Производит поиск по памяти
 * @param memory - указатель на память
 * @param memLength - длина памяти
 * @param start - откуда искать
 * @param symbol - что искать
 * @return позицию символа в памяти или -1, если символ в памяти отсутствует
 */
unsigned long findSymbolInMemory(char *memory, unsigned long memLength, unsigned long start, char symbol);

/**
 * Медленный поиск, который не использует буфер в оперативной памяти.
 * @param conf - рабочая конфигурация
 * @param symbol - символ, который требуется найти
 * @return позиция символа или ошибка
 */
unsigned long findSymbolInFile(CipherInst *conf, char symbol);

#endif /* CIPHERCORE_H_ */
