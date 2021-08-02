#include "defines.h"

/* Scale 0-100% ethanol to 8 bit value (0-255) */
#define SCALE_ETH_TO_8_BIT(e)     (e * 2.55)

/************************************************
 * Interrupt services
 ************************************************/
void SETUP_ISR() {
  pinMode(PIN_SENSOR_IN, INPUT);
  attachInterrupt(PIN_SENSOR_IN, READ_Frequency, RISING);
  attachInterrupt(PIN_SENSOR_IN, READ_DutyCycle, FALLING);
  start_Hz_MS = micros();
  start_Duty_MS = micros();
}

//rise to rise to check for Hz
void READ_Frequency() {
  elapsed_HZ_MS = micros() - start_Hz_MS;
  start_Hz_MS = micros();
  start_Duty_MS = micros();//for temp rising edge
}

//rise starts in the freq cycle and ends in a falling
void READ_DutyCycle() {
  elapsed_Duty_MS = micros() - start_Duty_MS; //falling edge
}

/* Write ethanol content to voltage out pin. */
void WRITE_AnalogDAC(){
  digitalWrite(PIN_V_OUT, SCALE_ETH_TO_8_BIT(ethanol));
}
