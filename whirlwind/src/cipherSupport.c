/*
 * cipherSupport.c
 *
 *  Created on: 20 дек. 2013 г.
 *      Author: tihon
 */


#include "cipherSupport.h"

/**
 * Возвращает случайное число, больше 0 и меньше max
 * @param conf - рабочая конфигурация
 * @param maxx - максимально допустимое значениеs
 */
unsigned long randVal(CipherInst *conf, unsigned long max)	//TODO test (max <0), псевдослучайность, max == 1
{//TODO случайное значение убогое
	unsigned long out = 0;
	lrand48_r(conf->support->randomBuffer, &out);
	printf("random value %ld (%ld)\n", max? out % max : out, out);
	return max? out % max : out;
}
