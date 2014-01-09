/*
 * cipherMainTest.h
 *
 *  Created on: 09 янв. 2014 г.
 *      Author: tihon
 */

#ifndef CIPHERMAINTEST_H_
#define CIPHERMAINTEST_H_

#include <stdio.h>

#define INIT init(time(NULL), 2, 7, 3)

#define TO_CRYPT "m this is a test phrase"
#define DICT "qwertyuio pasdfghjk lzxcvbnm"

/**
 * Creates dict file and fills it with standart dictionary
 * @param name - filename
 * @return 0 if no error occurred
 */
short createDictFile(char *name)
{
	FILE *createFile = fopen(name, "w");
	if(!createFile)
	{
		printf("Can't create file %s!\n", name);
		return 1;
	}
	fwrite (DICT , sizeof(char), strlen(DICT), createFile);
	fclose(createFile);
	return 0;
}

/**
 * Creates two equal dict files.
 * @return 0 if no error occurred
 */
short prepareDictFiles()
{
	return (createDictFile("1.dct") || createDictFile("2.dct"));
}

#endif /* CIPHERMAINTEST_H_ */
