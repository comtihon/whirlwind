package com.cipher.whirlwind;

/**
 * Created by tihon on 13.01.14.
 */
public class CipherBundle {

    static {
        System.loadLibrary("lib/libwhirlwind_java_bundle.so");
    }

    public static native long init(long randInit, int variability, int withdraw, int withdrawDepth);

    public static native int cryptString(long confPtr, String string, long stringLen, long[] result);

    public static native void freeMemory(long confPtr);
}
