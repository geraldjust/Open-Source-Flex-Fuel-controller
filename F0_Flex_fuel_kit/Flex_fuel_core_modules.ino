#include "defines.h"

/* Calculate frequency and duty cycle of sensor input. */
void calcFreqAndDutyCycle(void)
{
  freq    = CALC_FREQ_HERTZ(elapsed_HZ_MS);
  D_cycle = CALC_DCYCLE(elapsed_Duty_MS,elapsed_HZ_MS);
}

/* Calculate the ethanol content of the sensor input. */
void calcEthContent(void)
{
  ethanol = CALC_ETH_CONTENT(freq);
}

/* Calculate fuel temperature of eth content. */
void calcFuelTemp(void)
{
  Temperature_C = CALC_FUEL_TEMP(D_cycle);
  Temperature_F = FUEL_TEMP_C_TO_F(Temperature_C);
}
