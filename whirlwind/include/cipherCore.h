/*
 * cipherCore.h
 *
 *  Created on: 19 дек. 2013 г.
 *      Author: tihon
 */

#ifndef CIPHERCORE_H_
#define CIPHERCORE_H_

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
extern ReturnCode cryptOneSymbol(CipherInst *conf, char symbol, long *result);

/**
 * Декодирует пару шифрокодов в 1 символ.
 * @param conf - рабочая конфигурация
 * @param pair - пара шифросимволов - long[2]
 * @param result - указатель на символ, в который нужно записать результат
 * @return код возврата (ошибка либо успех)
 */
extern ReturnCode decryptOneSymbol(CipherInst *conf, long *pair, char *result);

/**
 * Возвращает позицию символа в словаре или -1, если символ отсутствует.
 * @param conf	рабочая конфигурация
 * @param symbol кодируемый символ
 */
long findSymbolPosInDict(CipherInst *conf, char symbol);


#endif /* CIPHERCORE_H_ */
