/*
 * cipherAddon.h
 *
 *  Created on: 20 дек. 2013 г.
 *      Author: tihon
 */

#ifndef CIPHERADDON_H_
#define CIPHERADDON_H_

#include "cipherConf.h"

/**
 * Запоминает закодированный результат. Если подошло время отката - выполняет откат.
 * @param conf - конфигурация
 * @param result - указатель на результат шифрования (пара ключ-инициализатор)
 * @return 0 - откат выполнен, 1 - откат не нужно выполнять.
 */
short processWithdraw(CipherInst *conf, long *result);

/**
 * Обрабатывает изменение словаря
 * @param conf - рабочая конфигурация
 * @param result - результат кодирования [позиция кодируемого символа, случайный инициализатор]
 */
void processChange(CipherInst *conf, long *result);

/**
 * Меняет в словаре местами 2 символа.
 * @param firstPos - указатель на позицию первого символ
 * @param secondPos - указатель на позицию второго символа
 */
void changeDict(CipherInst *conf, long *firstPos, long *secondPos);

#endif /* CIPHERADDON_H_ */
