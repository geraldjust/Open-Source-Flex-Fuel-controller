
/* #include "flex_fuel_def.h" */ /* TODO: makefile should include libraries based on platform/board. */

/***********************************************
 * Defines
 ***********************************************/

/* Ethanol content sensor input pin. */
#define PIN_SENSOR_IN            PA3

/* Output pin for converted ethanol content sensor value (0-5V analog). */
#define PIN_V_OUT                PA4

/* NULL type. */
#define NULL           0

/* TODO: makefile define DEBUG flag. */
#define DEBUG

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
 * Global Variables
 ***********************************************/

/*** Duty cycle and frequency. ***/

/* Frequency Utility. */
unsigned int freq;
volatile unsigned long start_Hz_MS;
volatile unsigned long elapsed_HZ_MS;

/* Duty cycle. */
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

/* Flex fuel service function pointer type. */
typedef void (*flexFuelService_t) (void);

/* Flex fuel service handler IDs. */
enum
{
  /* Essential functions. */
  FF_HDLR_CALC_FREQ_DCYCLE = 0,
  FF_HDLR_CALC_ETH_CONTENT,

  /* Optional features. */
  FF_HDLR_CALC_FUEL_TEMP,
  FF_HDLR_FAILSAFE,

  /* Debug features. */
  #ifdef DEBUG
  FF_HDLR_SERIAL_PRINT_DIAG,
  #endif

  /* Total handlers. */
  FF_HDLR_TOTAL
};

/* Function table for flex fuel service functions. */
flexFuelService_t flexFuelServiceFnTbl[FF_HDLR_TOTAL] = {NULL};

/* Calculate frequency and duty cycle of sensor input. */
static void calcFreqAndDutyCycle(void)
{
  freq    = CALC_FREQ_HERTZ(elapsed_HZ_MS);
  D_cycle = CALC_DCYCLE(elapsed_Duty_MS,elapsed_HZ_MS);
}

/* Calculate the ethanol content of the sensor input. */
static void calcEthContent(void)
{
  ethanol = CALC_ETH_CONTENT(freq);
}

/* Calculate fuel temperature of eth content. */
static void calcFuelTemp(void)
{
  Temperature_C = CALC_FUEL_TEMP(D_cycle);
  Temperature_F = FUEL_TEMP_C_TO_F(Temperature_C);
}

/* Check failsafe values and trigger failsafe if needed. */
static void checkFailsafeValues(void)
{
  /* TODO: Implement failsafe. */
  isError = false;
}

/************************************************
 * Interrupt services
 ************************************************/
void SETUP_ISR() {
  pinMode(PIN_SENSOR_IN, INPUT);
  attachInterrupt(SEN_IN, READ_Frequency, RISING);
  attachInterrupt(SEN_IN, READ_DutyCycle, FALLING);
  start_Hz_MS = micros();
  start_Duty_MS = micros();
}

//rise to rise to check for Hz
void READ_Frequency() {
  elapsed_HZ_MS = micros() - start_Hz_MS;
  start_Hz_MS = micros();
  start_Duty_MS = micros();//for temp rising edge
}

//rise starts in thr freq cycle and ends in a falling
void READ_DutyCycle() {
  elapsed_Duty_MS = micros() - start_Duty_MS;//falling edge
}

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

void setup(void)
{
  SETUP_ISR();
  registerServiceFns();
}

/************************************************
 * Main service loop
 ************************************************/
void loop()
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

