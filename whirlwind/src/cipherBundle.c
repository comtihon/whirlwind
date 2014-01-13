#include "cipherBundle.h"
#include "cipherConf.h"
#include "cipherCore.h"


/*
 * Class:     com_cipher_whirlwind_CipherBundle
 * Method:    init
 * Signature: (JIII)Ljava/lang/Object;
 */
JNIEXPORT jlong JNICALL Java_com_cipher_whirlwind_CipherBundle_init
(JNIEnv *env, jclass cls, jlong randInit, jint variability, jint withdraw, jint withdrawDepth)
{
    CipherInst *conf = init(randInit, valarray, withdraw, withdrawDepth);
    return (long)conf;
}

/*
 * Class:     com_cipher_whirlwind_CipherBundle
 * Method:    cryptString
 * Signature: (Ljava/lang/Object;Ljava/lang/String;J[J)Ljava/lang/String;
 */
JNIEXPORT jint JNICALL Java_com_cipher_whirlwind_CipherBundle_cryptString
(JNIEnv *env, jclass cls, jlong confPtr, jstring encryptingString, jlong stringLen, jlongArray result)
{
    CipherInst *conf = (CipherInst *)confPtr;

    const char *nativeString = (*env)->GetStringUTFChars(env, javaString, 0);
    if(cryptString(conf, nativeString, stringLen, result)!=OK)
    {
        (*env)->ReleaseStringUTFChars(env, javaString, nativeString);
        return 0;
    }
    else
    {
        (*env)->ReleaseStringUTFChars(env, javaString, nativeString);
        return 1;
    }
}

/*
 * Class:     com_cipher_whirlwind_CipherBundle
 * Method:    freeMemory
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_cipher_whirlwind_CipherBundle_freeMemory
(JNIEnv *env, jclass cls, jlong confPtr)
{
    CipherInst *conf = (CipherInst *)confPtr;
    freeInst(conf);
}
