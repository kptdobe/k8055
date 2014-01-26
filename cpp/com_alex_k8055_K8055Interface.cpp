//
//  com_alex_k8055_K8055Interface.cpp
//  k8055
//
//  Created by Alexandre Capt on 11.01.14.
//  Copyright (c) 2014 Alexandre Capt. All rights reserved.
//

#include "com_alex_k8055_K8055Interface.h"
#include <k8055.h>

k8055* k = new k8055();

JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_OpenDevice
(JNIEnv *env, jclass jc, jint ji) {
    printf("Opening device!\n");
    
    return k->OpenDevice(ji);
}

JNIEXPORT void JNICALL Java_com_alex_k8055_K8055Interface_CloseDevice
(JNIEnv *env, jclass jc) {
    printf("Closing device!\n");
    
    k->CloseDevice();
}

JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ReadAnalogChannel
(JNIEnv *env, jclass jc, jint ji) {
    return k->ReadAnalogChannel(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ReadAllAnalog
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ReadAllAnalog
(JNIEnv *env, jclass jc, jint ji1, jint ji2) {
    UInt8 u1 = ji1;
    UInt8 u2 = ji2;
    return k->ReadAllAnalog(&u1,&u2);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    OutputAnalogChannel
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_OutputAnalogChannel
(JNIEnv *env, jclass jc, jint ji1, jint ji2) {
    return k->OutputAnalogChannel(ji1,ji2);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    OutputAllAnalog
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_OutputAllAnalog
(JNIEnv *env, jclass jc, jint ji1, jint ji2) {
    return k->OutputAllAnalog(ji1,ji2);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ClearAnalogChannel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ClearAnalogChannel
(JNIEnv *env, jclass jc, jint ji) {
    return k->ClearAnalogChannel(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ClearAllAnalog
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ClearAllAnalog
(JNIEnv *env, jclass jc) {
    return k->ClearAllAnalog();
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    SetAnalogChannel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_SetAnalogChannel
(JNIEnv *env, jclass jc, jint ji) {
    return k->SetAnalogChannel(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    SetAllAnalog
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_SetAllAnalog
(JNIEnv *env, jclass jc) {
    return k->SetAllAnalog();
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    WriteAllDigital
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_WriteAllDigital
(JNIEnv *env, jclass jc, jint ji) {
    return k->WriteAllDigital(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ClearDigitalChannel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ClearDigitalChannel
(JNIEnv *env, jclass jc, jint ji) {
    return k->ClearDigitalChannel(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ClearAllDigital
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ClearAllDigital
(JNIEnv *env, jclass jc) {
    return k->ClearAllDigital();
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    SetDigitalChannel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_SetDigitalChannel
(JNIEnv *env, jclass jc, jint ji) {
    return k->SetDigitalChannel(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    SetAllDigital
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_SetAllDigital
(JNIEnv *env, jclass jc) {
    return k-> SetAllDigital();
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ReadDigitalChannel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ReadDigitalChannel
(JNIEnv *env, jclass jc, jint ji) {
    return k->ReadDigitalChannel(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ReadAllDigital
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ReadAllDigital
(JNIEnv *env, jclass jc) {
    return k->ReadAllDigital();
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ResetCounter
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ResetCounter
(JNIEnv *env, jclass jc, jint ji) {
    return k->ResetCounter(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    ReadCounter
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_ReadCounter
(JNIEnv *env, jclass jc, jint ji) {
    return k->ReadCounter(ji);
}

/*
 * Class:     com_alex_k8055_K8055Interface
 * Method:    SetCounterDebounceTime
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_alex_k8055_K8055Interface_SetCounterDebounceTime
(JNIEnv *env, jclass jc, jint ji1, jint ji2) {
    return k->SetCounterDebounceTime(ji1,ji2);
}

