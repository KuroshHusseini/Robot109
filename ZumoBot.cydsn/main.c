/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/
// test comment
#include <project.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>



   #if 1

// Week 4 Assignment 1

    

void sensors_up(void);

    

//global variables

struct sensors_ dig;

int sensorStatus;

    

void zmain(void)

{

    //initialize IR and reflectance sensors

    printf("\n\nInitializing sensors.");

    

    IR_Start();

    IR_flush();

    

    printf("\n\nIR sensor online\n");

    printf("IR buffer purged\n");

    

    reflectance_start();

    reflectance_set_threshold(9000, 9000, 9000, 9000, 9000, 9000);

    reflectance_digital(&dig);

    

    //start looking for lines

    sensors_up();

    

    printf("Reflectance sensors online\n\n");

    

    //wait for user to press SW1

    bool sw1Check = false;

    

    printf("Waiting. Press SW1 to continue.\n");

    while(SW1_Read() == 1)

    {

        if(SW1_Read() == 0){

            printf("SW1 pressed\n");

            sw1Check = true;

        }

    }

    

    while(sw1Check)

    {

        printf("moving to first line");

    }

    

    

    

    

    

    

    

    

}

    

void sensors_up(void)

{

    //Use reflectance sensors to check for lines

    /*  sensorstatus    0 = no line

                        1 = line covers all sensors

                        2 = line in the center

                        3 = line on the left

                        4 = line on the right
    
                        5 = line on the far left
    
                        6 = line on the far right
    
                        7 = line on the kinda far left
                           
                        8 = line on the kinda far right

    */

    while(true)

    {

        vTaskDelay(500);

        reflectance_digital(&dig);

        //printf("13: %5d 12: %5d 11: %5d 1: %5d 2: %5d 3: %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);

        //printf("%d\n\n", sensorStatus);

        

        

        //if sensors see nothing

        if (dig.l3 == 0 && dig.l2 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r2 == 0 && dig.r3 == 0){

            sensorStatus = 0;
            //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            }
        //if sensors see an intersection
        else if (dig.l3 == 1 && dig.l2 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 1 && dig.r3 == 1)
        {
            sensorStatus = 1;
            //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
        }
        //if sensors see a line in the center
        else if (dig.l3 == 0 && dig.l2 == 0 && dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 0 && dig.r3 == 0)
        {
            sensorStatus = 2;
            //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
        }
        //if sensors see a line on the left
        else if (dig.l3 == 1 && dig.l2 == 1 && dig.l1 == 0 && dig.r1 == 0 && dig.r2 == 0 && dig.r3 == 0)
        {
            sensorStatus = 3;
            //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
        }
         //if sensors see a line on the right
        else if (dig.l3 == 0 && dig.l2 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r2 == 1 && dig.r3 == 1)
        {
             sensorStatus = 4;
            //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);

        }       
        //if sensors on the far left
        else if (dig.l3 == 1 && dig.l2 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r2 == 0 && dig.r3 == 0)
        {
          sensorStatus = 5;
            //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
        }
        //if sensors on the far right
        else if (dig.l3 == 0 && dig.l2 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r2 == 0 && dig.r3 == 1)
        {
            sensorStatus = 6;
        }
        //if sensors on the kinda left 
        else if (dig.l3 == 0 && dig.l2 == 1 && dig.l1 == 1 && dig.r1 == 0 && dig.r2 == 0 && dig.r3 == 0)
        {
            sensorStatus = 7;
        }
        //if sensors on the kinda right 
        else if (dig.l3 == 0 && dig.l2 == 0 && dig.l1 == 0 && dig.r1 == 1 && dig.r2 == 1 && dig.r3 == 0)
        {
            sensorStatus = 8;

        }
        
       
    

}

    

#endif


