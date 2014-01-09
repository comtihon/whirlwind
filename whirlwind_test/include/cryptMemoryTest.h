/*
 * cryptMemoryTest.h
 *
 *  Created on: 17 дек. 2013 г.
 *      Author: tihon
 */

#ifndef CRYPTMEMORYTEST_H_
#define CRYPTMEMORYTEST_H_

#include <string.h>
#include <time.h>

#include "cipherMainTest.h"
#include "whirlwind/include/cipherConf.h"
#include "whirlwind/include/cipherCore.h"

unsigned long rrandVal(RandBuf *randomBuffer, unsigned long max);

void cryptTestMemory(unsigned long *result, char *toCrypt, int dataLen);

void decryptTestMemory(char *result, unsigned long *pairs, int dataLen);

void cryptTestFile(unsigned long *result, char *toCrypt, int dataLen);

void decryptTestFile(char *result, unsigned long *pairs, int dataLen);

#endif /* CRYPTMEMORYTEST_H_ */
