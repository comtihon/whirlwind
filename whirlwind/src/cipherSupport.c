/*
 * cipherSupport.c
 *
 *  Created on: 20 дек. 2013 г.
 *      Author: tihon
 */


#include "cipherSupport.h"

/**
 * Возвращает случайное число, больше 0 и меньше max
 */
long randVal(CipherInst *conf, long max)	//TODO test (max <0), псевдослучайность, max == 1
{
	long out = 0;
	lrand48_r(&conf->support->randomBuffer, &out);
	return max? out % max : out;
}
