
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

public class Tester {

    public static void main(String[] args) {
        //open connect to interface
        K8055Interface.open(0);

        //digital read sample
        long time = System.currentTimeMillis();
        long start = time;
        int nbRead = 0;
        while( time < ( start + (1000 * 30))) {

            for (int i= 1; i <= 5; i++) {
                try {
                    int value = K8055Interface.readDigitalChannel(i);
                    nbRead ++;
                    System.out.println("Read DI no " + i + ": " + value);

                } catch(Exception e) {
                    System.out.println("ERROR : ");
                    e.printStackTrace();
                }
            }
            time = System.currentTimeMillis();
        }

        System.out.println("Nb read: " + nbRead);

        //read analog channel 1
        int v1 = K8055Interface.readAnalogChannel(1);
        System.out.println("Analog Channel 1 Value = " + v1);

        //read analog channel 2
        int v2 = K8055Interface.readAnalogChannel(2);
        System.out.println("Analog Channel 2 Value = " + v2);

        //set digital channels - one after the other - "tinsel garland"
        for(int j=0;j<2;j++) {
            for (int i= 1; i <= 8; i++) {
                try {
                    K8055Interface.setDigitalChannel(i);
                    sleep(100);
                } catch(Exception e) {
                    System.out.println("ERROR : ");
                    e.printStackTrace();
                }
            }
            K8055Interface.clearAllDigital();
            sleep(100);
        }

        //set all digital - all digital lights on
        K8055Interface.setAllDigital();
        sleep();

        //clear all digital - all digital lights off
        K8055Interface.clearAllDigital();
        sleep();

        //set all analog - all analog lights on
        K8055Interface.setAllAnalog();
        sleep();

        //clear all analog - all analog lights off
        K8055Interface.clearAllAnalog();
        sleep();

        //close connection to interface
        K8055Interface.close();

    }

    public static void sleep() {
        sleep(1000);
    }

    public static void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
