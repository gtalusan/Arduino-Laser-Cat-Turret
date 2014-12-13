#pragma once/*<=DON'T TOUCH!!!!!!!*/
/**************************************************************************/
/*!                   ****Customizable Settings****

Use this file to change the laser settings (e.g. the amount of time the
laser runs before shutting off in sleep mode).


                      ****EDIT THIS FILE ONLY****
/**************************************************************************/





/**************************************************************************/
/***********************Continuous Mode Settings***************************/
/************************* (ON => forever) ********************************/
/**************************************************************************/
/*                    (General purpose. Always on.)                       */
/* Settings:                                                              */
/*                                NONE                                    */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/***********************INTERMITTENT Mode Settings*************************/
/********************* (ON => OFF => ON => REPEAT) ************************/
/**************************************************************************/
/*               (Good for when the kitty is home alone)                  */
/*SETTINGS:                                                               */
/*                                                                        */
/* MINUTES the laser stays ON in intermittent mode for each cycle.        */
/*------------------------------------------------------------------------*/
  #define INTERMITTENT_ON_TIME   1 /*<=========== CHANGE ME (Default=15) */
/*------------------------------------------------------------------------*/
/*                                                                        */
/* MINUTES the laser stays OFF in intermittent mode for each cycle.       */
/*------------------------------------------------------------------------*/
  #define INTERMITTENT_OFF_TIME  2 /*<=========== CHANGE ME (Default=60) */
/*------------------------------------------------------------------------*/
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/***************************SLEEP Mode Settings****************************/
/************************ (ON => OFF => FOREVER) **************************/
/**************************************************************************/
/*       (Good for distracting a kitty when you want to go to sleep)      */
/* SETTINGS:                                                              */
/*                                                                        */
/* MINUTES the laser stays ON before turning OFF FOREVER.                 */
/* (You must change mode with dial to restart laser.)                     */
/*------------------------------------------------------------------------*/
  #define MINUTES_BEFORE_SLEEP    30 /*<=========== CHANGE ME (Default=30)*/
/*------------------------------------------------------------------------*/
/*                                                                        */
/**************************************************************************/
