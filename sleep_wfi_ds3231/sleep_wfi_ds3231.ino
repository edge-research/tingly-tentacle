//Interrupts for Battery management/saving using MCU power down mode. /INT from DS3231 is connected to 'interrupt_pin' of MCU, and also needs a 10K pullup to VDD.

#include <Wire.h>
#include "DS3231.h"

#define interruptPin 6

DS3231 rtc; //Create the DS3231 object

const int interval_sec = 5; //interval in seconds
const int sleep_depth = 0; // Enable Idle0 mode - sleep CPU clock only
//const int sleep_depth = 1; // Idle1 - sleep CPU and AHB clocks
//const int sleep_depth=2; // Idle2 - sleep CPU, AHB, and APB clocks

void setup () 
{

  
	 Serial.begin(9600);
   Serial.println('beginning ...');
	 Wire.begin();
	
	 rtc.begin(); 
   
}


void loop () 
{

	for (int i=0;i<3;i++) { // indicate visually
    Blink(LED_BUILTIN,1000);
   }
   
   fall_asleep(interval_sec,sleep_depth);

   for (int i=0;i<3;i++) { // indicate visually
    Blink(LED_BUILTIN,100);
   }
   delay(1000);
   Serial.println(" External Interrupt detected "); //indicate on serial port

} 

  
//Interrupt service routine for external interrupt
void EIC_ISR()
{
  //Keep this as short as possible. Possibly avoid using function calls
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  
}

void Blink(byte PIN, int DELAY_MS) {
  //Blink an LED
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN, LOW);
  delay(DELAY_MS);
}


void fall_asleep (int interval_sec, int sleep_depth) {

  rtc.clearAlarm(); //resets the alarm interrupt status on the RTC
  attachInterrupt(digitalPinToInterrupt(interruptPin), EIC_ISR, FALLING);  // Attach interrupt to pin interrupt_pin with an ISR
 
  SYSCTRL->XOSC32K.reg |=  (SYSCTRL_XOSC32K_RUNSTDBY | SYSCTRL_XOSC32K_ONDEMAND); // set external 32k oscillator to run when idle or sleep mode is chosen
  REG_GCLK_CLKCTRL  |= GCLK_CLKCTRL_ID(GCM_EIC) |  // generic clock multiplexer id for the external interrupt controller
                       GCLK_CLKCTRL_GEN_GCLK1 |  // generic clock 1 which is xosc32k
                       GCLK_CLKCTRL_CLKEN;       // enable it
  while (GCLK->STATUS.bit.SYNCBUSY);              // write protected, wait for sync

  EIC->WAKEUP.reg |= EIC_WAKEUP_WAKEUPEN4;        // Set External Interrupt Controller to use channel 4 (interrupt_pin)


  if (sleep_depth==0) PM->SLEEP.reg |= PM_SLEEP_IDLE_CPU;  // Enable Idle0 mode - sleep CPU clock only
  if (sleep_depth==1) PM->SLEEP.reg |= PM_SLEEP_IDLE_AHB; // Idle1 - sleep CPU and AHB clocks
  if (sleep_depth==2) PM->SLEEP.reg |= PM_SLEEP_IDLE_APB; // Idle2 - sleep CPU, AHB, and APB clocks

  // It is either Idle mode or Standby mode, not both. 
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;   // Enable Standby or "deep sleep" mode
  
  
  DateTime now = rtc.now(); //get the current time
  DateTime nextAlarm = DateTime(now.unixtime() + interval_sec);
  rtc.enableAlarm(nextAlarm); //Sets the alarm on the RTC to the specified time (using the DateTime Object passed in)
  delay(10); //wait for a moment for everything to complete
  __WFI();
}

