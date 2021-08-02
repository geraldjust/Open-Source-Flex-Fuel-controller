/***********************************************
 * Debug
 ***********************************************/
#ifdef DEBUG
#define SERIAL_BAUD    115200

static void debugPrint(void)
{
  Serial.print("Hrtz: " + freq );
  Serial.print("  ethanol%: " + ethanol) ;
  Serial.print("  ethenol ms: " + elapsed_HZ_MS);
  Serial.print("  Duty Cycle ms:");
  Serial.print(D_cycle);
  Serial.print("  Temperature in c:"  );
  Serial.println(Temperature_C);
}

static void initAndRegisterSerialTerminalDebug()
{
  Serial.begin(SERIAL_BAUD);
  flexFuelServiceFnTbl[FF_HDLR_SERIAL_PRINT_DIAG] = debugPrint;

}
#endif


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
  flexFuelServiceFnTbl[FF_HDLR_FAILSAFE]   = checkFailsafeValues;

  /* Debug features: Only used for debugging. */
  #ifdef DEBUG
  initAndRegisterSerialTerminalDebug();
  #endif
}
