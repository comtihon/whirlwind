/*
 * cipherAddon.h
 *
 *  Created on: 20 дек. 2013 г.
 *      Author: tihon
 */

#ifndef CIPHERADDON_H_
#define CIPHERADDON_H_

#include <time.h>
#include <stdlib.h>

#include "cipherConf.h"
#include "cipherSupport.h"


/**
 * Запоминает закодированный результат. Если подошло время отката - выполняет откат.
 * @param conf - конфигурация
 * @param result - указатель на результат шифрования (пара ключ-инициализатор)
 * @return Withdraw - откат выполнен, OK - откат не нужно выполнять, или ошибка
 */
ReturnCode processWithdraw(CipherInst *conf, unsigned long *result);

/**
 * Производит откаты
 * @param conf - рабочая конфигурация
 * @param extraPairs - память для организации рекурсии случайных чисел
 * @return ok или ошибку
 */
ReturnCode withdraw(CipherInst *conf, unsigned long **extraPairs);

/**
 * Обрабатывает изменение словаря
 * @param conf - рабочая конфигурация
 * @param result - результат кодирования [позиция кодируемого символа, случайный инициализатор]
 * @return ok или ошибку
 */
ReturnCode processChange(CipherInst *conf, unsigned long *result);

/**
 * Меняет в словаре местами 2 символа.
 * @param firstPos - указатель на позицию первого символ
 * @param secondPos - указатель на позицию второго символа
 * @return код ошибки или ок
 */
ReturnCode changeDict(CipherInst *conf, unsigned long *firstPos, unsigned long *secondPos);

/**
 * Производит дополнительные изменения в словаре
 * @param conf - конфигурация
 * @return код ошибки или ок
 */
ReturnCode extraChangeDict(CipherInst *conf);

/**
 * Отменяет дополнительные изменения словаря, сделанные с помощью extraChangeDict
 * @param conf - рабочая конфигурация
 * @param extraPairs - N случайных чисел. Генерируются и используются рекурсино.
 * @return код ошибки или ок
 */
ReturnCode revertExtraChangeDict(CipherInst *conf, unsigned long **extraPairs);

#endif /* CIPHERADDON_H_ */
