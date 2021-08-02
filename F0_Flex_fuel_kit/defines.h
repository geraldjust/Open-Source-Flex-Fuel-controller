#ifndef DEFINES_H
#define DEFINES_H

/***********************************************
 * Defines
 ***********************************************/

/* NULL type. */
#define NULL           0

/* TODO: makefile define DEBUG flag. */
#define DEBUG

/* Ethanol content sensor input pin. */
#define PIN_SENSOR_IN            PA3

/* Output pin for converted ethanol content sensor value (0-5V analog). */
#define PIN_V_OUT                PA4

/* Flex fuel service handler IDs. */
enum
{
  /* Essential functions. */
  FF_HDLR_CALC_FREQ_DCYCLE = 0,
  FF_HDLR_CALC_ETH_CONTENT,

  /* Optional features. */
  FF_HDLR_CALC_FUEL_TEMP,
  FF_HDLR_FAILSAFE,
  FF_HDLR_BT,

  /* Debug features. */
  #ifdef DEBUG
  FF_HDLR_SERIAL_PRINT_DIAG,
  #endif

  /* Total handlers. */
  FF_HDLR_TOTAL
};

/***********************************************
 * Type definitions
 ***********************************************/

/* Flex fuel service function pointer type. */
typedef void (*flexFuelService_t) (void);

/***********************************************
 * Global Variables
 ***********************************************/
extern unsigned int freq;
extern volatile unsigned long start_Hz_MS;
extern volatile unsigned long elapsed_HZ_MS;
extern float D_cycle;
extern volatile unsigned long start_Duty_MS;
extern volatile unsigned long elapsed_Duty_MS;
extern unsigned int ethanol;
extern bool isError = false;
extern float Temperature_C;
extern unsigned int Temperature_F;
extern flexFuelService_t flexFuelServiceFnTbl[];

/***********************************************
 * Utility Macros
 ***********************************************/

/* Get frequency from elapsed time. */
#define CALC_FREQ_HERTZ(x)   ((1000 / x))

/* Get duty cycle from frequency. */
#define CALC_DCYCLE(period, activeTime)    (100 * (activeTime / period))

/* Get ethanol content from frequency. */
#define CALC_ETH_CONTENT(freqHz)             ((freqHz > 50) ? (freqHz - 50) : 0)

/* Get fuel temperature from duty cycle. */
#define CALC_FUEL_TEMP(dutyCycle)           ((40.25 * dutyCycle) - 81.25)

/* Convert celcius fuel temperature to eagle units. */
#define FUEL_TEMP_C_TO_F(cTemp)            ((cTemp * 1.8) + 32)

/***********************************************
 * Functions
 ***********************************************/

/*** Initilization. ***/

void registerServiceFns(void);
void setup(void);

/*** Main loop. ***/

void loop(void);

/*** Modules. ***/

void calcFreqAndDutyCycle(void);
void calcEthContent(void);
void calcFuelTemp(void);
void checkFailsafeValues(void);
void serviceBluetooth(void);

/*** Interrupt services. ***/

void SETUP_ISR();
void READ_Frequency();
void READ_DutyCycle();
void WRITE_AnalogDAC();

/*** Debug. ***/

#ifdef DEBUG
void debugPrint(void);
void initAndRegisterSerialTerminalDebug(void);
#endif

#endif /* DEFINES_H */