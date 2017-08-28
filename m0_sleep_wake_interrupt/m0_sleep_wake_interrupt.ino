
#include <Wire.h>
#include <DS3231.h>


#define interruptPin 6
volatile bool SLEEP_FLAG;

DS3231 rtc; //initialize the Real Time Clock

int interval_sec = 5; //this is the interval which the RTC will wake the MCU (microcontroller)


void EIC_ISR(void) {
  SLEEP_FLAG ^= true;  // toggle SLEEP_FLAG by XORing it against true
}

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  rtc.begin();
  
  Serial.begin(9600);
  //delay(3000); // wait for console opening

  
  delay(100); //wait for a moment for everything to complete


  //attachInterrupt(digitalPinToInterrupt(interruptPin), EIC_ISR, CHANGE);  // Attach interrupt to pin 6 with an ISR and when the pin state CHANGEs

  attachInterrupt(digitalPinToInterrupt(interruptPin), EIC_ISR, CHANGE);  // Attach interrupt to pin 6 with an ISR and when the pin state CHANGEs


  SYSCTRL->XOSC32K.reg |=  (SYSCTRL_XOSC32K_RUNSTDBY | SYSCTRL_XOSC32K_ONDEMAND); // set external 32k oscillator to run when idle or sleep mode is chosen
  REG_GCLK_CLKCTRL  |= GCLK_CLKCTRL_ID(GCM_EIC) |  // generic clock multiplexer id for the external interrupt controller
                       GCLK_CLKCTRL_GEN_GCLK1 |  // generic clock 1 which is xosc32k
                       GCLK_CLKCTRL_CLKEN;       // enable it
  while (GCLK->STATUS.bit.SYNCBUSY);              // write protected, wait for sync

  EIC->WAKEUP.reg |= EIC_WAKEUP_WAKEUPEN4;        // Set External Interrupt Controller to use channel 4 (pin 6)

  
  PM->SLEEP.reg |= PM_SLEEP_IDLE_CPU;  // Enable Idle0 mode - sleep CPU clock only
  //PM->SLEEP.reg |= PM_SLEEP_IDLE_AHB; // Idle1 - sleep CPU and AHB clocks
  //PM->SLEEP.reg |= PM_SLEEP_IDLE_APB; // Idle2 - sleep CPU, AHB, and APB clocks

  // It is either Idle mode or Standby mode, not both. 
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;   // Enable Standby or "deep sleep" mode

  SLEEP_FLAG = false; // begin awake
  
}

void loop() {
  // put your main code here, to run repeatedly:

 for (int i=0;i<3;i++) {
 Blink(LED_BUILTIN,100);
 }
 
 DateTime now = rtc.now(); //get the current time
  
  DateTime nextAlarm = DateTime(now.unixtime() + interval_sec);

  rtc.clearAlarm();
  rtc.enableAlarm(nextAlarm);

  // now go to sleep
      __WFI();  // wake from interrupt

 rtc.clearAlarm();
 for (int i=0;i<3;i++) {
 Blink(LED_BUILTIN,1000);
 }

}

void Blink(byte PIN, int DELAY_MS) {
  //Blink an LED
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN, LOW);
  delay(DELAY_MS);
}
