#include "pins_arduino.h"
#include <Arduino.h>
#include <avr/iotn85.h>
#include <avr/interrupt.h>

// #include <cstdint>

// #define TESTPWM

const uint8_t DUTY_CYCLE_PEN_DOWN = 205;
const uint8_t DUTY_CYCLE_PEN_UP = 170;

int led=PIN_B1;
int readPWM = PIN_B1;

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



void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pwm_setup();
  



 
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


const uint8_t PEN_DOWN_THRES = 4;
const uint8_t MAX_LOOP_PER_PHASE = 20;

uint8_t lowCnt = 0;
uint8_t highCnt = 0;
uint8_t oldLvl = 0;
uint8_t cnt = 0;
uint32_t allCnt = 0;



void calcDutyCycle(const uint8_t high, const uint8_t low, const uint8_t lcnt)
{
  uint8_t tempPhCnt = high + low;
  if(tempPhCnt <= lcnt)
  {
    if(high > PEN_DOWN_THRES)
    {
      setDutyCycle(DUTY_CYCLE_PEN_DOWN);
    }else{
      setDutyCycle(DUTY_CYCLE_PEN_UP);
    }
  }
}

void resetVars()
{
  lowCnt = 0;
  highCnt = 0;

  cnt = 0;
}


void simulatePWM()
{
  static uint8_t callCnt = 0;

  if(callCnt <= 15)
  {
    digitalWrite(led, HIGH);
  }else {
    digitalWrite(led, LOW);
  }

  callCnt++;

  if(callCnt >= MAX_LOOP_PER_PHASE)
  {
    callCnt = 0;
  }
}

void loop() {
#ifdef TESTPWM
  testPWM();
#else
  
  if(allCnt > 10000 && allCnt < 20000)
    simulatePWM();
  // else
  //  digitalWrite(led, LOW);

  if(cnt >= MAX_LOOP_PER_PHASE)
  {
    resetVars();
  }

  // read pwm signal of/for laser
  uint8_t lvl = digitalRead(readPWM);

  // rising edge / new phase
  if(lvl == HIGH && oldLvl == LOW)
  {
    calcDutyCycle(highCnt, lowCnt, cnt);
    resetVars();
  }

  if(lvl == HIGH)
  {
    highCnt++;
  }else {
    lowCnt++;
  }
  oldLvl = lvl;

  cnt++;
  allCnt++;
  delayMicroseconds(50);

#endif
}

