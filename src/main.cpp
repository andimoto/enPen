#include "pins_arduino.h"
#include <Arduino.h>
#include <avr/iotn85.h>
#include <avr/interrupt.h>

// #define TESTPWM

const uint8_t DUTY_CYCLE_PEN_DOWN = 205;
const uint8_t DUTY_CYCLE_PEN_UP = 170;

int led=PIN_B1;
int readPWM = PIN_B2;

void setDutyCycle(const uint8_t dutyCycle)
{
  // allow only up and down. no other values
  if(dutyCycle == DUTY_CYCLE_PEN_DOWN || dutyCycle == DUTY_CYCLE_PEN_UP)
  {
    OCR0A = dutyCycle;
  }
}

void pwm_setup()
{
    cli(); // 1. Disable global interrupts during setup

    DDRB |= (1 << PB0);                    // Set PB1 as output
    TCCR0A = 0x00;                         // Normal mode, no PWM
    TCCR0B = 0x00;                         // Timer stopped for now
    
    // 2. Configure TCCR0A for Fast PWM mode & Non-inverting output
    // COM0A1 = 1: Clear OC0A on Compare Match, set OC0A at BOTTOM (non-inverting)
    // WGM01 = 1, WGM00 = 1: Fast PWM mode (Mode 3)
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

    TCCR0B |= (1 << CS00) | (1 << CS01);   // Prescaler = 64 @ 8MHz

    // TIMSK |= (1 << TOIE0);                 // Enable Timer0 overflow interrupt

    TCNT0 = 0;                             // Start counting from zero

    setDutyCycle(DUTY_CYCLE_PEN_UP);

    sei();                                 // Enable global interrupts
}

void startUpLed()
{
  digitalWrite(led, HIGH);
  delay(250);
  digitalWrite(led, LOW);
  delay(250);
  digitalWrite(led, HIGH);
  delay(250);
  digitalWrite(led, LOW);
  delay(250);
  digitalWrite(led, HIGH);
  delay(250);
  digitalWrite(led, LOW);
}


void setupTimer1() {
  cli(); // 1. Disable global interrupts during setup
  TCCR1 = 0x00;                  // normal mode counter

  TCCR1 |= (0<<CS13)|(1<<CS12)|(1<<CS11)|(0<<CS10);  // prescaler set to 64 -> 1/8MHz * 64 = 40us
  TCNT1 = 0;                      // reset counter

  TIMSK |= (0 << TOIE1);
  sei(); // 7. Re-enable global interrupts
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(readPWM, INPUT);
  pwm_setup();
  setupTimer1();

  startUpLed();
}


void testPWM()
{
  static uint8_t cnt = 0;

  cnt++;
  if(cnt <= 2)
  {
    setDutyCycle(DUTY_CYCLE_PEN_DOWN);
    digitalWrite(led, LOW);
  }else {
    setDutyCycle(DUTY_CYCLE_PEN_UP);
    digitalWrite(led, HIGH);
  }

  if(cnt >= 4)
  {
    cnt = 0;
  }
   
  delay(500);
}

uint16_t startHighPhaseA = 0;
uint16_t startLowPhaseA = 0;
uint16_t startHighPhaseB = 0;
uint16_t startLowPhaseB = 0;
uint8_t slotA = 1;
uint8_t oldLvl = 0;

void setHighStartTime(const uint16_t strtT)
{
  if(slotA)
  {
    startHighPhaseA = strtT;
  }else{
    startHighPhaseB = strtT;
  }
}

void setLowStartTime(const uint16_t strtT)
{
  if(slotA)
  {
    startLowPhaseA = strtT;
  }else{
    startLowPhaseB = strtT;
  }
}

void changeSlot()
{
  slotA ^= 1;
}


uint16_t getDiffTime(const uint16_t t1, const uint16_t t2)
{
  // The timestamp counter is 8-bit, so subtraction wraps modulo 256.
  return (uint16_t)((t2 - t1) & 0xFFu);
}


// timer1 counts to 256 in 2ms
// thus: 1ms = 128 => 25% or 128 = 32
const uint8_t PEN_DOWN_THRES = 64;
void calcDutyCycle()
{
  uint16_t diff = 0;
  if(slotA)
  {
    diff = getDiffTime(startHighPhaseB, startLowPhaseB);
  }else{
    diff = getDiffTime(startHighPhaseA, startLowPhaseA);
  }


  if(diff > PEN_DOWN_THRES)
  {
    setDutyCycle(DUTY_CYCLE_PEN_DOWN);
  }else{
    setDutyCycle(DUTY_CYCLE_PEN_UP);
  }


}




uint32_t loopCnt = 0;
uint32_t tmoStart = 100;

void loop() {
#ifdef TESTPWM
  testPWM();
#else
  

  // read pwm signal of/for laser
  uint8_t lvl = digitalRead(readPWM);
  uint16_t timCurr = (uint16_t)TCNT1;

  // rising edge / new phase
  if(lvl == HIGH && oldLvl == LOW)
  {
    changeSlot();
    setHighStartTime(timCurr);
    calcDutyCycle(); 
    tmoStart = 1000;
  }

  if(lvl == LOW && oldLvl == HIGH)
  {
    setLowStartTime(timCurr);
  }


  oldLvl = lvl;
  loopCnt++;
  if(tmoStart >= 1)
  {
    tmoStart--;
  }else{
    setDutyCycle(DUTY_CYCLE_PEN_UP);
  }


#if 1
  if(lvl == HIGH)
  {
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }
#endif
  // delayMicroseconds(35);

#endif
}

