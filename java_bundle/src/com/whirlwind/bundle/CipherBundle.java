package com.whirlwind.bundle;

/**
 * Created by tihon on 13.01.14.
 */
public class CipherBundle {

    static {
        System.load("/home/tihon/whirlwind-core/modules/whirlwind/build/libwhirlwind_java_bunlde.jnilib");
    }

    public static native long init(long randInit, int variability, int withdraw, int withdrawDepth);

    public static native int cryptString(long confPtr, String string, long stringLen, long[] result);

    public static native void freeMemory(long confPtr);
}
