#include "CipherBundle.h"
#include "cipherConf.h"
#include "cipherCore.h"

/*
 * Class:     com_cipher_whirlwind_CipherBundle
 * Method:    init
 * Signature: (JIII)Ljava/lang/Object;
 */
JNIEXPORT jlong JNICALL Java_CipherBundle_init(JNIEnv *env, jclass cls, jlong randInit,
        jint variability, jint withdraw, jint withdrawDepth)
{
	CipherInst *conf = init(randInit, variability, withdraw, withdrawDepth);
	return (long) conf;
}

/*
 * Class:     com_cipher_whirlwind_CipherBundle
 * Method:    cryptString
 * Signature: (Ljava/lang/Object;Ljava/lang/String;J[J)Ljava/lang/String;
 */
JNIEXPORT jint JNICALL Java_CipherBundle_cryptString(JNIEnv *env, jclass cls, jlong confPtr,
        jstring encryptingString, jlong stringLen, jlongArray result)
{
	CipherInst *conf = (CipherInst *) confPtr;

	const char *nativeString = (*env)->GetStringUTFChars(env, encryptingString, 0);
	if(!nativeString)
	{
		printf("Can't create C-array for encrypting!\n");
		return 0;
	}

	jlong *cArray = (*env)->GetLongArrayElements(env, result, NULL);
	if (!cArray)
	{
		(*env)->ReleaseStringUTFChars(env, encryptingString, nativeString);
		printf("Can't create C-array for result!\n");
		return 0;
	}

	if (cryptString(conf, nativeString, stringLen, cArray) != OK)
	{
		(*env)->ReleaseStringUTFChars(env, encryptingString, nativeString);
		(*env)->ReleaseLongArrayElements(env, result, cArray, JNI_ABORT);
		return 0;
	}
	else
	{
		(*env)->ReleaseStringUTFChars(env, encryptingString, nativeString);
		(*env)->ReleaseLongArrayElements(env, result, cArray, JNI_ABORT);
		return 1;
	}
}

/*
 * Class:     com_cipher_whirlwind_CipherBundle
 * Method:    freeMemory
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CipherBundle_freeMemory
(JNIEnv *env, jclass cls, jlong confPtr)
{
	CipherInst *conf = (CipherInst *)confPtr;
	freeInst(conf);
}
