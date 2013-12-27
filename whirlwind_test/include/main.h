/*
 * main.h
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <string.h>
#include <time.h>

#include "whirlwind/include/cipherConf.h"

void cryptTest(unsigned long *result, char *toCrypt, int dataLen);

void decryptTest(char *result, unsigned long *pairs, int dataLen);

#endif /* MAIN_H_ */
