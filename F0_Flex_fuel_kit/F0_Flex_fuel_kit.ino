


const int SEN_IN = PA3;//input Pin
const int V_out = PA4;//12bit DAC

//stuff for Ethenol Fuel content
volatile unsigned long start_Hz_MS;
volatile unsigned long elapsed_HZ_MS;
unsigned int Hrtz;
unsigned int Ethanol = 0;//ethanol %
bool isError = false;

//stuff for temp
volatile unsigned long start_Duty_MS;
volatile unsigned long elapsed_Duty_MS;
float D_cycle;//percent
float Temperature = 0;//fuel temp in C
unsigned int Temperature_F = 0;//fuel temp in F


void setup() {
  
  SETUP_ISR();
  Serial.begin(115200);
}

void loop() {

  Hrtz = (1000 / elapsed_HZ_MS); //ms to Hz
  D_cycle = 100*(elapsed_Duty_MS / elapsed_HZ_MS ); //high time/tcycle =  on time in ms

 
  //calculate ethanol percentage & temp
  if (Hrtz > 50) Ethanol = (Hrtz-50); else Ethanol = 0;
  Temperature = ((40.25 * D_cycle)-81.25);        //Calculate temperature for display (1ms = -40, 5ms = 80)
  Temperature_F = ((Temperature*1.8)+32);//in F

  Serial.print("Hrtz: " + Hrtz );
  Serial.print("  Ethanol%: " + Ethanol) ;
  Serial.print("  ethenol ms: " + elapsed_HZ_MS);
  Serial.print("  Duty Cycle ms:");
  Serial.print(D_cycle);
  Serial.print("  Temperature in c:"  );
  Serial.println(Temperature);
  
}















// ISR stuff

void SETUP_ISR() {
  pinMode(SEN_IN, INPUT);
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
