/*
 * cipherSupport.h
 *
 *  Created on: 20 дек. 2013 г.
 *      Author: tihon
 */

#ifndef CIPHERSUPPORT_H_
#define CIPHERSUPPORT_H_

#include "cipherConf.h"

/**
 * Возвращает случайное число, больше 0 и меньше max
 * @param conf - рабочая конфигурация
 * @param maxx - максимально допустимое значениеs
 */
unsigned long randVal(CipherInst *conf, unsigned long max);

#endif /* CIPHERSUPPORT_H_ */
