
#include "defines.h"
/* #include "flex_fuel_def.h" */ /* TODO: makefile should include libraries based on platform/board. */

/***********************************************
 * Global Variables
 ***********************************************/

/*** Frequency. ***/
unsigned int freq;
volatile unsigned long start_Hz_MS;
volatile unsigned long elapsed_HZ_MS;

/*** Duty cycle. ***/
float D_cycle;
volatile unsigned long start_Duty_MS;
volatile unsigned long elapsed_Duty_MS;

/*** Ethanol content. ***/
unsigned int ethanol = 0;

/*** Failsafe. ***/
bool isError = false;

/*** Fuel Temperature. ***/
float Temperature_C = 0;
unsigned int Temperature_F = 0;

/************************************************
 * Main loop service procedures
 ************************************************/

/* Function table for flex fuel service functions. */
flexFuelService_t flexFuelServiceFnTbl[FF_HDLR_TOTAL] = {NULL};

/************************************************
 * Initialization functions
 ************************************************/
void registerServiceFns(void)
{
  /* ESSENTIAL FEATURES: DO NOT COMMENT ANY OF THESE OUT UNLESS YOU WANT YOUR CAR COOKED */
  flexFuelServiceFnTbl[FF_HDLR_CALC_FREQ_DCYCLE] = calcFreqAndDutyCycle;
  flexFuelServiceFnTbl[FF_HDLR_CALC_ETH_CONTENT] = calcEthContent;

  /* Optional features: Initialize if desired, comment out if not needed. */
  flexFuelServiceFnTbl[FF_HDLR_CALC_FUEL_TEMP]   = calcFuelTemp;
  flexFuelServiceFnTbl[FF_HDLR_FAILSAFE]         = checkFailsafeValues;
  flexFuelServiceFnTbl[FF_HDLR_BT]               = serviceBluetooth;

  /* Debug features: Only used for debugging. */
  #ifdef DEBUG
  initAndRegisterSerialTerminalDebug();
  #endif
}

/************************************************
 * Setup: software entry point.
 ************************************************/
void setup(void)
{
  SETUP_ISR();
  registerServiceFns();

  /* Run loop(). */
}

/************************************************
 * Main service loop
 ************************************************/
void loop(void)
{
  /* Service loop. */
  for (unsigned int i = 0; i < sizeof(flexFuelServiceFnTbl); i++)
  {
    if (flexFuelServiceFnTbl[i] != NULL)
    {
      flexFuelServiceFnTbl[i]();
    }
  }
}
