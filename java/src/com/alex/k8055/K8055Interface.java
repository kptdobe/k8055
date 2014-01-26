/*
 * Copyright (c) 2014 Alexandre Capt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

package com.alex.k8055;

public class K8055Interface {
    private static native int OpenDevice(int i);
    private static native void CloseDevice();
    
    // Analog channel functions
    private static native int ReadAnalogChannel (int channel);
    private static native int ReadAllAnalog(int channel1 , int channel2);
    private static native int OutputAnalogChannel(int channel, int data);
    private static native int OutputAllAnalog(int channel1, int channel2);
    private static native int ClearAnalogChannel(int channel);
    private static native int ClearAllAnalog();
    private static native int SetAnalogChannel(int channel);
    private static native int SetAllAnalog();

    // Digital channel output functions
    private static native int WriteAllDigital(int data);
    private static native int ClearDigitalChannel(int channel);
    private static native int ClearAllDigital();
    private static native int SetDigitalChannel(int channel);
    private static native int SetAllDigital();

    // Digital channel output functions
    private static native int ReadDigitalChannel(int channel);
    private static native int ReadAllDigital();

    // Counter functions
    private static native int ResetCounter(int counter);
    private static native int ReadCounter(int counter);
    private static native int SetCounterDebounceTime(int counter, int debounceTime);

    static {
        System.out.println("java.library.path : " + System.getProperty("java.library.path") );
        System.loadLibrary( "k8055" );
    }

    static int open(int address) {
        return OpenDevice(address);
    }

    static void close() {
        CloseDevice();
    }

    static int readAnalogChannel(int channel) {
        return ReadAnalogChannel(channel);
    }

    static int readAllAnalog(int channel1 , int channel2) {
        return ReadAllAnalog(channel1, channel2);
    }

    static int outputAnalogChannel(int channel, int data) {
        return OutputAnalogChannel(channel, data);
    }

    static int outputAllAnalog(int channel1, int channel2) {
        return OutputAllAnalog(channel1, channel2);
    }

    static int clearAnalogChannel(int channel) {
        return ClearAnalogChannel(channel);
    }

    static int clearAllAnalog() {
        return ClearAllAnalog();
    }

    static int setAnalogChannel(int channel) {
        return SetAnalogChannel(channel);
    }

    static int setAllAnalog() {
        return SetAllAnalog();
    }

    // Digital channel output functions
    static void writeAllDigital(int i) {
        WriteAllDigital(i);
    }

    static int clearDigitalChannel(int channel) {
        return ClearDigitalChannel(channel);
    }

    static int clearAllDigital() {
        return ClearAllDigital();
    }

    static int setDigitalChannel(int channel) {
        return SetDigitalChannel(channel);
    }

    static int setAllDigital() {
        return SetAllDigital();
    }

    // Digital channel output functions
    static int readDigitalChannel(int channel) {
        return ReadDigitalChannel(channel);
    }

    static int readAllDigital() {
        return ReadAllDigital();
    }

    // Counter functions
    static int resetCounter(int counter) {
        return ResetCounter(counter);
    }

    static int readCounter(int counter) {
        return ReadCounter(counter);
    }

    static int setCounterDebounceTime(int counter, int debounceTime) {
        return SetCounterDebounceTime(counter, debounceTime);
    }
}
