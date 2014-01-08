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

void testRandom();

unsigned long rrandVal(RandBuf *randomBuffer, unsigned long max);

void cryptTestMemory(unsigned long *result, char *toCrypt, int dataLen);

void decryptTestMemory(char *result, unsigned long *pairs, int dataLen);

void cryptTestFile(unsigned long *result, char *toCrypt, int dataLen);

void decryptTestFile(char *result, unsigned long *pairs, int dataLen);

#endif /* MAIN_H_ */
