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
 */
long randVal(CipherInst *conf, long max);

#endif /* CIPHERSUPPORT_H_ */
