#include "defines.h"

/***********************************************
 * Debug
 ***********************************************/
#ifdef DEBUG
#define SERIAL_BAUD    115200

void debugPrint(void)
{
  Serial.print("Hrtz: " + freq );
  Serial.print("  ethanol%: " + ethanol) ;
  Serial.print("  ethenol ms: " + elapsed_HZ_MS);
  Serial.print("  Duty Cycle ms:");
  Serial.print(D_cycle);
  Serial.print("  Temperature in c:"  );
  Serial.println(Temperature_C);
}

void initAndRegisterSerialTerminalDebug(void)
{
  Serial.begin(SERIAL_BAUD);
  flexFuelServiceFnTbl[FF_HDLR_SERIAL_PRINT_DIAG] = debugPrint;

}
#endif

