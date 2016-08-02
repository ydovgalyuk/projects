/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
//#define TIME_SLICE_MS  1000  // ms regulation time
struct schedulle_t
{
  bool Active;
  int OnP[2];
  int OffP[2];
};


int RED[5] = {2, 3, 4, 5, 6};
int GREEN = 8;
int LEVEL = A0;
int PULSE = 9;
int HIGH_OUT = 11;
int TEMP_OUT = A1;
int TEMP_IN = A2;
int ZERO_IN = 10;

long gLevel = 0;
int gCycle = 2;        // frequency = 500 Hz -> 2ms

schedulle_t schedule[2] = { {0}, {0}};
int zeroDelay = 25; // *50 uS

unsigned int cycleCounter = 0;
unsigned long avrgBuff = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  for(int i = 0; i < 5; ++i)
    pinMode(RED[i], OUTPUT);
  
  pinMode(GREEN, OUTPUT);
  pinMode(PULSE, OUTPUT);  
  
  pinMode(HIGH_OUT, OUTPUT);
  pinMode(ZERO_IN, INPUT);  
  
  pinMode(TEMP_OUT, OUTPUT);
  //pinMode(TEMP_IN, INPUT);  
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  cli();		// switch interrupts off while messing with their settings  

  // here we setup the zero crossing interrupt on pin 10  
//  PCICR = 0x01;         // Enable PCINT0 interrupt - 01
//  PCMSK0 = 0b00000100;  // Enable interrupts on the pins: PCINT2,  
//  PCMSK1 = 0b00000000;  // Enable interrupts on the pins: 

  //set timer2 interrupt at 20kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 20khz increments
  OCR2A = 199;// = (16*10^6) / (20000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei();		// turn interrupts back on

}

//         ____------____                                    ____--- 
//      __/           |  \__                              __/
//     /<-----75%---->|    |\                            /
//    /    zero       |    | \                          /   zero
// --/------|---------|----|--\---|--------------------/------|
//   <zDelay>   <On>  <Off><On>\ zero                 /<zDelay> 
//                              \                    /
//                               --                --
//                                 \____      ____/
//                                      ------
//  OnTimer  = 10ms(next zero event) - <zDelay> - 200uS(to be sure before the event)
//  OffTimer = OnTimer + 200uS + PulseWidth
// 
/*
ISR(PCINT0_vect)  // this is called when Zero crossing is detected
{
  if (digitalRead(ZERO_IN)) // only detect zero at positive sub-period
  {
    long PulseWidth = 10*20 * gLevel / 1023; // *50 uS >>> 10 ms * (analogRead / 1023)
    int onP1   = 10*20 - zeroDelay;  // * 50 uS >> 10ms - zeroDelay
    int offP1  = onP1 + PulseWidth;  // * 100 uS
    if ( PulseWidth > 9 * 20 )  
    {  // if we run for more than 9.0ms out of 10ms then we do not stop
      schedule[0].Active = 0;
      schedule[1].Active = 0;
      digitalWrite(PULSE, HIGH);
    }
    else
    {    
      //cli();		// switch interrupts off while messing with their settings  
      
      int idx = schedule[0].Active ? 1 : 0;
      schedule[idx].OnP[0]  = onP1;
      schedule[idx].OffP[0] = offP1;
      schedule[idx].OnP[1]  = onP1 + 200;
      schedule[idx].OffP[1] = offP1 + 200;
      schedule[idx].Active = 1;
     
      //sei();		// turn interrupts back on
    }
  }
}
*/

long gControlPeriod = 20000; // * 100uS => 100mS
long gControlCounter = 0;
long onTime = 0;
ISR(TIMER2_COMPA_vect) // timer compare interrupt service routine
{
  if (gControlCounter == 0) // at the beginning of control cycle
  {
    onTime = gLevel * gControlPeriod / 1023;
  }
  if (gControlCounter++ < onTime)
  {
    digitalWrite(PULSE, HIGH);
    digitalWrite(GREEN, HIGH);
  }
  else
  {
    digitalWrite(PULSE, LOW);
    digitalWrite(GREEN, LOW);
  }
  if ( gControlCounter == gControlPeriod )
  {
    gControlCounter = 0;
  }
}

/*
ISR(TIMER2_COMPA_vect) // timer compare interrupt service routine
{  
  // we are here every 50uS
  for(int idx = 0; idx < 2; ++idx)
  {
    for(int per = 0; per < 2 && schedule[idx].Active; ++per)
    {
      if ( schedule[idx].OnP[per] > 0 ) // check if the timer is running
      {
        schedule[idx].OnP[per]--;	// if running decrease counter
    	if ( schedule[idx].OnP[per] == 0 )  // timer event is triggered
    	{
    	  // here we switch on the trigger 
    	  digitalWrite(PULSE, HIGH);
    	}
      }  
      if ( schedule[idx].OffP[per] > 0 ) // check if the timer is running
      {
        schedule[idx].OffP[per]--;  // if running decrease counter
    	if ( schedule[idx].OffP[per] == 0 )  // timer event is triggered
    	{
    	  // here we switch off the trigger 
    	  digitalWrite(PULSE, LOW);
          if ( per == 1 ) // if we finished the schedulle (last part of Period 2)
            schedule[idx].Active = 0;
    	}   
      }
    }
  }
}
*/

#define LED_CNT  5
#define CKL_CNT  10
void display_value(int cycle)  // cycle no out of CKL_CNT
{
  int AI = gLevel;
  int full = LED_CNT * AI / 1023;
  int rest = AI - (full * 1023 / LED_CNT);
  int cyclesOn = CKL_CNT * rest / (1023 / LED_CNT);

  for(int i = 0; i < full; ++i)
    digitalWrite(RED[i], HIGH);

  if ( rest > 0 )
  {
    if ( (cycle % CKL_CNT) + 1 < cyclesOn )
      digitalWrite(RED[full], HIGH);
    else
      digitalWrite(RED[full], LOW);

    for(int i = full + 1; i < LED_CNT; ++i)
      digitalWrite(RED[i], LOW);
  }
  else
  {
    for(int i = full; i < LED_CNT; ++i)
      digitalWrite(RED[i], LOW);
  }
}

double readTemperature(int RawADC) 
{
	double Temp;
	Temp = log(((10240000/RawADC) - 10000));
	Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
	Temp = Temp - 273.15;            // Convert Kelvin to Celcius
	//Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
	return Temp;
}

int tempValue = 0;
// the loop function runs over and over again forever
void loop() 
{  
  digitalWrite(HIGH_OUT, HIGH);  
  digitalWrite(TEMP_OUT, HIGH);  
  
  tempValue = analogRead(TEMP_IN);
  Serial.print("T=");
  Serial.println(tempValue);

  /*
  // each cycle(2ms) we refresh display, passing in the cycle number 
  // and increasing the cycle counter by 1
  display_value(cycleCounter++); 

  if (cycleCounter % 5 == 0)  // each 5th (10ms) cycle we add value to avrg buffer
    avrgBuff += analogRead(LEVEL);  
  if ( cycleCounter % 50 == 0 ) // each 50th cycle (100ms) we calc average
  {
    gLevel = avrgBuff / 11;
    avrgBuff = 0;    
  }
  */

  delay(1000);  // delay for 2 ms
}
