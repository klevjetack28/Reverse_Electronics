#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define IR_PIN 3

struct SignalEntry
{
  const char* name;
  const uint16_t* data;
  size_t size;
};

const uint16_t Disney_Plus[] PROGMEM = {9020, 4490, 580, 540, 590, 1670, 580, 540, 590, 1670, 
      580, 540, 590, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1660, 590, 1670, 580, 550, 580, 540, 590, 540, 
      590, 1670, 580, 1670, 580, 550, 580, 540, 590, 1670, 
      580, 1670, 590, 540, 580, 550, 580, 550, 580, 540, 
      590, 1670, 580, 1670, 580, 550, 580, 550, 580, 1670, 
      580, 1670, 590, 1660, 590, 1660, 590, 38290, 9040, 4490, 
      590, 540, 580, 1670, 590, 540, 590, 1660, 590, 540, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 580, 1670, 
      590, 1660, 590, 540, 590, 540, 590, 540, 580, 1670, 
      590, 1660, 590, 540, 590, 540, 590, 1660, 590, 1660, 
      590, 540, 590, 540, 590, 540, 590, 1660, 590, 1660, 
      590, 1670, 580, 540, 590, 540, 590, 1670, 580, 1670, 
      580, 1670, 590, 540, 580, 38290, 9040, 4490, 580, 1670, 
      590, 540, 580, 550, 580, 550, 580, 540, 590, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      580, 550, 580, 550, 580, 550, 580, 1670, 580, 1670, 
      590, 1660, 590, 540, 590, 1660, 590, 1670, 580, 1670, 
      580, 1670, 590, 540, 590, 540, 590, 540, 580, 1670, 
      590, 540, 590, 540, 580, 550, 580, 550, 580, 1670, 
      580, 1670, 580 };
const uint16_t Volume_Up[] PROGMEM = {9020, 4490, 580, 540, 590, 1670, 580, 550, 580, 1670, 
      580, 550, 580, 1670, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      580, 1670, 590, 1660, 590, 1660, 590, 1670, 580, 1670, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 540, 
      580, 550, 580, 550, 580, 540, 590, 540, 590, 1670, 
      580, 1670, 580, 1670, 590, 1660, 590, 38300, 9040, 4490, 
      590, 540, 590, 1660, 590, 540, 590, 1670, 580, 540, 
      590, 1670, 580, 1670, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 590, 540, 580, 550, 580, 550, 580, 1670, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 580, 1670, 
      590, 540, 590, 540, 580, 550, 580, 1670, 590, 540, 
      580, 550, 580, 550, 580, 550, 580, 1670, 580, 1670, 
      590, 1660, 590, 540, 580 };      
const uint16_t Settings[] PROGMEM = {9030, 4480, 590, 540, 590, 1660, 590, 540, 590, 1670, 
      580, 540, 590, 1670, 580, 1670, 590, 1660, 590, 1670, 
      580, 1670, 580, 1670, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 1670, 580, 550, 580, 550, 
      580, 540, 590, 540, 590, 1670, 580, 1670, 580, 550, 
      580, 550, 580, 1670, 580, 1670, 590, 1660, 590, 1670, 
      580, 540, 590, 540, 590, 1660, 590, 38300, 9040, 4490, 
      580, 550, 580, 1670, 590, 540, 580, 1670, 590, 540, 
      590, 1660, 590, 1660, 590, 1660, 590, 1670, 580, 1670, 
      590, 1660, 590, 540, 590, 540, 580, 550, 580, 1670, 
      590, 1660, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 540, 590, 1660, 590, 1660, 590, 1660, 590, 540, 
      590, 1660, 590, 1670, 580, 1670, 580, 1670, 590, 540, 
      590, 540, 580, 540, 590 };      
const uint16_t Apple_TV[] PROGMEM = {9020, 4490, 580, 540, 590, 1670, 580, 540, 590, 1670, 
      580, 550, 580, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1660, 590, 1670, 580, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 580, 1670, 
      590, 540, 580, 1670, 590, 1660, 590, 1660, 590, 540, 
      590, 540, 590, 540, 590, 540, 590, 1660, 590, 540, 
      590, 540, 580, 550, 580, 1670, 580, 38300, 9040, 4490, 
      580, 550, 580, 1670, 580, 550, 580, 1670, 580, 550, 
      580, 1670, 580, 1670, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 580, 550, 580, 540, 590, 540, 590, 1660, 
      590, 1670, 580, 1670, 580, 1670, 590, 1660, 590, 540, 
      590, 1660, 590, 1660, 590, 1660, 590, 1670, 580, 550, 
      580, 540, 590, 540, 590, 1660, 590, 540, 590, 540, 
      590, 540, 590, 540, 580, 38290, 9040, 4480, 590, 1670, 
      580, 550, 580, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1670, 580, 1670, 580, 550, 580, 540, 590, 540, 
      590, 540, 590, 540, 590, 540, 590, 1660, 590, 1660, 
      590, 1660, 590, 540, 590, 1660, 590, 1670, 580, 1670, 
      580, 1670, 590, 540, 590, 540, 580, 550, 580, 1670, 
      590, 540, 580, 550, 580, 540, 590, 540, 590, 1670, 
      580, 1660, 590 };
const uint16_t Mute[] PROGMEM = {9020, 4480, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1670, 580, 1670, 580, 550, 580, 550, 580, 540, 
      590, 1670, 580, 1670, 590, 540, 590, 540, 580, 550, 
      580, 550, 580, 550, 580, 1670, 580, 550, 580, 550, 
      580, 1670, 580, 1670, 590, 1660, 590, 1670, 580, 1670, 
      590, 540, 580, 1670, 590, 1660, 590, 38300, 9040, 4490, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1670, 580, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1670, 580, 550, 580, 540, 590, 540, 590, 540, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 1670, 
      580, 1670, 580, 1670, 590, 1660, 590, 1670, 580, 540, 
      590, 1670, 580, 540, 590 };
const uint16_t Power[] PROGMEM = {9020, 4480, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 1660, 590, 1670, 580, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      580, 1670, 590, 1660, 590, 1660, 590, 1670, 580, 1670, 
      580, 550, 580, 1670, 580, 550, 580, 550, 580, 550, 
      580, 540, 590, 540, 590, 540, 590, 1660, 590, 540, 
      590, 1660, 590, 1670, 580, 1660, 590, 38300, 9040, 4490, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 580, 1670, 
      590, 1660, 590, 540, 590, 540, 590, 540, 580, 1670, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 590, 540, 
      580, 1670, 590, 540, 580, 550, 580, 1670, 580, 550, 
      580, 550, 580, 540, 590, 1670, 580, 550, 580, 1670, 
      580, 1670, 590, 530, 590 };
const uint16_t Pause[] PROGMEM = {9020, 4480, 590, 540, 590, 1660, 590, 540, 590, 1670, 
      580, 540, 590, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1660, 590, 1670, 580, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1670, 580, 540, 590, 540, 590, 1670, 
      580, 1670, 580, 550, 580, 550, 580, 1670, 580, 550, 
      580, 1670, 580, 1670, 590, 540, 590, 540, 580, 1670, 
      590, 1660, 590, 540, 590, 1660, 590, 38300, 9040, 4480, 
      590, 540, 590, 1660, 590, 540, 590, 1670, 580, 540, 
      590, 1670, 580, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1670, 580, 550, 580, 540, 590, 1670, 580, 1670, 
      580, 550, 580, 550, 580, 1670, 580, 1670, 590, 1660, 
      590, 1670, 580, 540, 590, 540, 590, 1670, 580, 1670, 
      580, 550, 580, 540, 590   };
const uint16_t Volume_Down[] PROGMEM = {9020, 4490, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 1670, 580, 1670, 580, 1670, 
      590, 1660, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 540, 590, 1660, 590, 540, 590, 540, 590, 540, 
      580, 1670, 590, 1660, 590, 1660, 590, 1670, 580, 550, 
      580, 1670, 590, 1660, 590, 1660, 590, 38310, 9040, 4490, 
      580, 550, 580, 1670, 590, 540, 580, 1670, 590, 540, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 580, 1670, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 540, 590, 540, 590, 1660, 590, 1670, 
      580, 1670, 590, 1660, 590, 1660, 590, 540, 590, 1660, 
      590, 1670, 580, 540, 590 };
const uint16_t Right[] PROGMEM = {9020, 4490, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 1660, 590, 1670, 580, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      580, 1670, 590, 1660, 590, 1660, 590, 540, 590, 1670, 
      580, 1670, 580, 550, 580, 1670, 580, 550, 580, 540, 
      590, 540, 590, 1670, 580, 540, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 1660, 590, 38300, 9040, 4490, 
      590, 540, 580, 1670, 590, 540, 590, 1660, 590, 540, 
      590, 1660, 590, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1670, 580, 1670, 590, 540, 580, 1670, 590, 1660, 
      590, 540, 590, 1660, 590, 540, 590, 1670, 580, 540, 
      590, 1670, 580, 550, 580, 550, 580, 1670, 580, 550, 
      580, 1670, 590, 530, 590 };
const uint16_t Forward[] PROGMEM = {9010, 4490, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 1660, 590, 1670, 580, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 580, 550, 
      580, 1670, 590, 1660, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1670, 580, 540, 590, 1660, 590, 38300, 9040, 4480, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1670, 580, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1660, 590, 1670, 580, 550, 580, 1670, 580, 550, 
      580, 1670, 580, 550, 580, 1670, 580, 1670, 590, 540, 
      590, 1660, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 540, 580 };
const uint16_t Netflix[] PROGMEM = {9020, 4490, 590, 540, 580, 1670, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 1660, 590, 1670, 580, 1670, 
      590, 1660, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1670, 580, 540, 590, 1670, 580, 550, 
      580, 540, 590, 1670, 580, 550, 580, 1670, 590, 540, 
      580, 1670, 590, 540, 590, 1660, 590, 1660, 590, 540, 
      590, 1660, 590, 540, 590, 1660, 590, 38300, 9040, 4490, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1660, 590, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1670, 580, 540, 590, 1670, 580, 550, 580, 540, 
      590, 1670, 580, 550, 580, 1670, 580, 1670, 590, 1660, 
      590, 540, 590, 1660, 590, 1670, 580, 550, 580, 1670, 
      580, 550, 580, 540, 590, 38290, 9040, 4490, 590, 1660, 
      590, 540, 590, 540, 590, 540, 580, 550, 580, 1670, 
      590, 1660, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 540, 580, 550, 580, 550, 580, 1670, 580, 1670, 
      590, 1660, 590, 540, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 580, 550, 580, 550, 580, 540, 590, 1670, 
      580, 550, 580, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1660, 590 };
const uint16_t Prime_Video[] PROGMEM = {9020, 4480, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 1670, 580, 1670, 580, 1670, 
      590, 1660, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 580, 550, 
      580, 1670, 580, 550, 580, 550, 580, 1670, 580, 550, 
      580, 540, 590, 540, 590, 1670, 580, 540, 590, 1670, 
      580, 1670, 580, 550, 580, 1660, 590, 38300, 9040, 4490, 
      580, 540, 590, 1670, 580, 540, 590, 1670, 580, 540, 
      590, 1670, 580, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1660, 590, 1670, 580, 1670, 580, 550, 580, 1670, 
      590, 540, 580, 550, 580, 1670, 590, 1660, 590, 540, 
      590, 540, 580, 1670, 590, 540, 590, 1660, 590, 1660, 
      590, 540, 590, 540, 580, 38290, 9040, 4480, 590, 1670, 
      580, 540, 590, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1660, 590, 1670, 580, 550, 580, 540, 590, 540, 
      590, 540, 590, 540, 590, 540, 590, 1660, 590, 1660, 
      590, 1670, 580, 540, 590, 1670, 580, 1670, 590, 1660, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 1660, 
      590, 540, 590, 540, 580, 550, 580, 550, 580, 1670, 
      580, 1670, 580 };
const uint16_t Home[] PROGMEM = {9020, 4490, 590, 540, 590, 1660, 590, 540, 590, 1670, 
      580, 540, 590, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 1670, 590, 540, 580, 550, 580, 550, 
      580, 1670, 580, 1670, 590, 1660, 590, 1660, 590, 540, 
      590, 540, 590, 540, 590, 540, 590, 540, 590, 540, 
      580, 550, 580, 550, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 1670, 590, 1660, 580, 38310, 9040, 4480, 
      590, 540, 590, 1670, 580, 540, 590, 1670, 580, 550, 
      580, 1670, 580, 1670, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 580, 550, 580, 550, 580, 540, 590, 1670, 
      580, 1670, 590, 1660, 590, 1660, 590, 540, 590, 540, 
      590, 540, 590, 540, 590, 540, 580, 1670, 590, 540, 
      590, 540, 580, 1670, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 590, 530, 590 };
const uint16_t Back[] PROGMEM = {9020, 4490, 590, 540, 580, 1670, 590, 540, 580, 1670, 
      590, 540, 580, 1670, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 580, 1670, 590, 540, 580, 550, 580, 550, 
      580, 1670, 580, 1670, 590, 540, 580, 1670, 590, 1660, 
      590, 540, 590, 540, 590, 1660, 590, 1660, 590, 540, 
      590, 1660, 590, 540, 590, 540, 590, 1660, 590, 1670, 
      580, 540, 590, 540, 590, 1660, 590, 38300, 9040, 4490, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1670, 580, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 550, 580, 540, 590, 540, 590, 1660, 
      590, 1670, 580, 550, 580, 1670, 580, 1670, 590, 540, 
      580, 550, 580, 1670, 590, 1660, 590, 1660, 590, 1670, 
      580, 550, 580, 540, 590, 1670, 580, 1670, 590, 540, 
      580, 550, 580, 540, 590 };
const uint16_t Reverse[] PROGMEM = {9020, 4490, 580, 540, 590, 1670, 580, 550, 580, 1670, 
      580, 550, 580, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 1670, 580, 550, 580, 550, 580, 540, 
      590, 1670, 580, 1670, 580, 550, 580, 540, 590, 1670, 
      580, 540, 590, 1670, 580, 1670, 580, 550, 580, 540, 
      590, 1670, 580, 1670, 580, 550, 580, 1670, 580, 550, 
      580, 540, 590, 1670, 580, 1660, 590, 38300, 9040, 4490, 
      580, 550, 580, 1670, 580, 550, 580, 1670, 580, 550, 
      580, 1670, 590, 1660, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 580, 550, 580, 550, 580, 540, 590, 1670, 
      580, 1670, 580, 550, 580, 550, 580, 1670, 580, 550, 
      580, 1670, 590, 1660, 590, 540, 590, 1660, 590, 1660, 
      590, 1670, 580, 540, 590, 1670, 580, 550, 580, 540, 
      590, 1670, 580, 540, 590 };
const uint16_t Down[] PROGMEM = {9020, 4490, 580, 550, 580, 1670, 580, 550, 580, 1670, 
      590, 540, 580, 1670, 590, 1660, 590, 1670, 580, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 590, 540, 
      580, 550, 580, 1670, 590, 1660, 590, 540, 590, 540, 
      590, 540, 590, 540, 590, 1660, 590, 1660, 590, 540, 
      590, 540, 590, 1660, 590, 1660, 590, 38300, 9040, 4490, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1660, 590, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 550, 580, 540, 590, 540, 590, 1670, 
      580, 1670, 580, 1670, 590, 1660, 590, 540, 590, 540, 
      590, 1660, 590, 1670, 580, 540, 590, 1670, 580, 550, 
      580, 550, 580, 1670, 580, 1670, 590, 540, 590, 540, 
      580, 1670, 590, 540, 580 };
const uint16_t Undo[] PROGMEM = {9020, 4490, 580, 550, 580, 1670, 590, 540, 580, 1670, 
      590, 540, 590, 1660, 590, 1660, 590, 1670, 580, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 1670, 580, 1670, 580, 550, 
      580, 1670, 590, 1660, 590, 1660, 590, 540, 590, 540, 
      590, 540, 590, 540, 590, 1660, 590, 38300, 9040, 4480, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1660, 590, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1660, 590, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1660, 590, 1670, 580, 540, 590, 540, 590, 540, 
      590, 540, 590, 540, 580 };
const uint16_t Up[] PROGMEM = {9020, 4480, 590, 540, 590, 1660, 590, 540, 590, 1670, 
      580, 540, 590, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1670, 580, 1670, 580, 550, 580, 540, 590, 540, 
      590, 1670, 580, 1670, 580, 1670, 590, 540, 580, 550, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      580, 550, 580, 1670, 590, 1660, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 1660, 590, 38310, 9040, 4480, 
      590, 540, 590, 1660, 590, 540, 590, 1670, 580, 540, 
      590, 1670, 580, 1670, 580, 1670, 590, 1660, 590, 1670, 
      580, 1670, 580, 550, 580, 550, 580, 540, 590, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 1660, 
      590, 1660, 590, 540, 590, 540, 590, 1660, 590, 540, 
      590, 1670, 580, 1670, 580, 550, 580, 550, 580, 1670, 
      590, 1660, 590, 540, 580 };
const uint16_t Left[] PROGMEM = {9020, 4490, 580, 550, 580, 1670, 580, 550, 580, 1670, 
      590, 540, 580, 1670, 590, 1660, 590, 1670, 580, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1670, 580, 540, 590, 1670, 580, 1670, 
      590, 1660, 590, 1670, 580, 540, 590, 540, 590, 540, 
      590, 1660, 590, 540, 590, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1660, 590, 1660, 590, 38300, 9050, 4480, 
      590, 540, 590, 1660, 590, 540, 590, 1670, 580, 540, 
      590, 1670, 580, 1670, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 580, 550, 580, 550, 580, 540, 590, 1670, 
      580, 1670, 580, 550, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 540, 590, 540, 590, 1660, 590, 1670, 
      580, 550, 580, 540, 590, 540, 590, 540, 590, 1660, 
      590, 1670, 580, 540, 590 };
const uint16_t OK[] PROGMEM = {9020, 4490, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1670, 580, 1670, 580, 1670, 590, 1660, 
      590, 1660, 590, 1670, 580, 540, 590, 540, 590, 540, 
      590, 1660, 590, 1670, 580, 550, 580, 1670, 580, 550, 
      580, 1670, 590, 540, 580, 1670, 590, 540, 590, 540, 
      590, 1660, 590, 540, 590, 1660, 590, 540, 590, 1660, 
      590, 540, 590, 1660, 590, 1660, 590, 38300, 9040, 4480, 
      590, 540, 590, 1660, 590, 540, 590, 1670, 580, 540, 
      590, 1670, 580, 1670, 580, 1670, 590, 1660, 590, 1660, 
      590, 1670, 580, 550, 580, 540, 590, 540, 590, 1670, 
      580, 1670, 580, 550, 580, 1670, 580, 550, 580, 1670, 
      580, 550, 580, 1670, 590, 540, 580, 1670, 590, 1660, 
      590, 540, 590, 1660, 590, 540, 590, 1660, 590, 540, 
      590, 1660, 590, 540, 580, 38280, 9040, 4490, 590, 1660, 
      590, 540, 590, 540, 580, 550, 580, 550, 580, 1670, 
      580, 1670, 590, 1660, 590, 540, 590, 540, 590, 540, 
      590, 540, 580, 550, 580, 540, 590, 1670, 580, 1670, 
      590, 1660, 590, 540, 590, 1660, 590, 1660, 590, 1670, 
      580, 1670, 590, 540, 590, 540, 580, 550, 580, 1670, 
      580, 550, 580, 550, 580, 540, 590, 540, 590, 1670, 
      580, 1660, 590};

SignalEntry signalMap[] = {
  {"Disney_Plus", Disney_Plus, sizeof(Disney_Plus) / sizeof(uint16_t) }, 
  {"Volume_Up", Volume_Up, sizeof(Volume_Up) / sizeof(uint16_t) }, 
  {"Settings", Settings, sizeof(Settings) / sizeof(uint16_t) }, 
  {"Apple_TV", Apple_TV, sizeof(Apple_TV) / sizeof(uint16_t) }, 
  {"Mute", Mute, sizeof(Mute) / sizeof(uint16_t) }, 
  {"Power", Power, sizeof(Power) / sizeof(uint16_t) }, 
  {"Pause", Pause, sizeof(Pause) / sizeof(uint16_t) }, 
  {"Volume_Down", Volume_Down, sizeof(Volume_Down) / sizeof(uint16_t) }, 
  {"Right", Right, sizeof(Right) / sizeof(uint16_t) }, 
  {"Forward", Forward, sizeof(Forward) / sizeof(uint16_t) }, 
  {"Netflix", Netflix, sizeof(Netflix) / sizeof(uint16_t) }, 
  {"Prime_Video", Prime_Video, sizeof(Prime_Video) / sizeof(uint16_t) }, 
  {"Home", Home, sizeof(Home) / sizeof(uint16_t) }, 
  {"Back", Back, sizeof(Back) / sizeof(uint16_t) }, 
  {"Reverse", Reverse, sizeof(Reverse) / sizeof(uint16_t) }, 
  {"Down", Down, sizeof(Down) / sizeof(uint16_t) }, 
  {"Undo", Undo, sizeof(Undo) / sizeof(uint16_t) }, 
  {"Up", Up, sizeof(Up) / sizeof(uint16_t) }, 
  {"Left", Left, sizeof(Left) / sizeof(uint16_t) }, 
  {"OK", OK, sizeof(OK) / sizeof(uint16_t) }, 
  {"NULL", NULL, 0 },
};

volatile int signalArrIndex = 0;
volatile int signalIndex = 0;
volatile uint16_t lastDelay = 0;
volatile bool newDelay = false;
volatile bool lastWasMark = false;

uint16_t buffer[256];

SignalEntry currentSignal = signalMap[signalArrIndex];
size_t signalLength = currentSignal.size;

#define ROWS 4
#define COLS 4

int rowPins[ROWS] = { 10, 11, 12, 13 };
int colPins[COLS] = { 6, 7, 8, 9 };

void init_buttons()
{
    for (int i = 0; i < ROWS; i++)
    {
        pinMode(rowPins[i], OUTPUT);
        digitalWrite(rowPins[i], HIGH);
    }
    for (int j = 0; j < COLS; j++)
    {
         pinMode(colPins[j], INPUT_PULLUP);
    }
}

int matrix_pressed() {
  int result = -1;
  for (int row = 0; row < ROWS; row++) {
    digitalWrite(rowPins[row], LOW);

    for (int col = 0; col < COLS; col++) {
      if (digitalRead(colPins[col]) == LOW) {
        Serial.print("Detected LOW at row ");
        Serial.print(row);
        Serial.print(", col ");
        Serial.println(col);
        result = row * 10 + col;
      }
    }
    digitalWrite(rowPins[row], HIGH);
  }
  delay(250);  // let signal settle
  return result;  // no press
}

#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define LCD_ADDRESS 0x27
#define MSG_BUF LCD_HEIGHT

struct Message
{
    const char* name;
    int index;
} messages[MSG_BUF];

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);

ISR(TIMER1_COMPA_vect) {
  static uint16_t remainingDelay = 0;
  static bool handlingSplit = false;

  TCNT1 = 0;

  if (remainingDelay > 0) {
    uint16_t chunk = min(remainingDelay, (uint16_t)32767);
    remainingDelay -= chunk;

    if (chunk < 5) chunk = 5;
    OCR1A = (chunk * 2) - 1;

    lastDelay = chunk;
    newDelay = true;

    if (remainingDelay == 0 && handlingSplit) {
      signalIndex++;
      handlingSplit = false;
    }
    return;
  }

  if (signalIndex < signalLength) {
    bool isMark = (signalIndex % 2 == 0);

    if (isMark) {
      TCCR2A |= _BV(COM2B1);  // enable PWM
    } else {
      TCCR2A &= ~_BV(COM2B1); // disable PWM
      PORTD &= ~_BV(PD3);
    }

    uint16_t delay = buffer[signalIndex];

    if (delay > 32767) {
      remainingDelay = delay - 32767;
      delay = 32767;
      handlingSplit = true;
    } else {
      signalIndex++;
      handlingSplit = false;
    }

    OCR1A = (delay * 2) - 1;

    lastDelay = delay;
    lastWasMark = isMark;
    newDelay = true;
  } else {
    // Done transmitting
    TCCR1B &= ~_BV(CS11);
    TIMSK1 &= ~_BV(OCIE1A);
    TCCR2A &= ~_BV(COM2B1);
    PORTD &= _BV(PD3);
    signalIndex = 0;
  }
}

void loadbuffer(size_t size)
{
  for (int i = 0; i < size; i++)
  {
    buffer[i] = pgm_read_word(&currentSignal.data[i]);
  }
}

#define NUM_MACROS 10
#define MAX_MACRO_LEN 16
bool macros[NUM_MACROS];

#define DELAYS 11

struct Delay {
  const char *name;
  int delay_val;
};

const Delay delayMap[] = {
  { "DELAY_250", 250 },
  { "DELAY_500", 500 },
  { "DELAY_750", 750 },
  { "DELAY_1000", 1000 },
  { "DELAY_1500", 1500 },
  { "DELAY_2000", 2000 },
  { "DELAY_2500", 2500 },
  { "DELAY_3000", 3000 },
  { "DELAY_5000", 5000 },
  { "DELAY_7500", 7500 },
  { "DELAY_10000", 10000 }
};

const int macroMap[NUM_MACROS][MAX_MACRO_LEN][2] PROGMEM = {
  { {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}, {1, delayMap[0].delay_val}},
  { {12, delayMap[7].delay_val}, {8, delayMap[1].delay_val}, {19, delayMap[1].delay_val}, {19, delayMap[7].delay_val}, {15, delayMap[1].delay_val}, {15, delayMap[1].delay_val}, {15, delayMap[3].delay_val}, {19, delayMap[7].delay_val}, {19, delayMap[1].delay_val}}
};

const int macroLength[NUM_MACROS] = { 10, 9 };

#define UP    3
#define DOWN  33
#define LEFT  23
#define RIGHT 13
#define OK    32
#define BACK 30
#define ONE   0
#define TWO   1
#define THREE 2
#define FOUR  10
#define FIVE  11
#define SIX   12
#define SEVEN 20
#define EIGHT 21
#define NINE  22
#define ZERO  31

enum State {
  IDLE = 1,
  EDIT_MACRO = 2,
  EDIT_DELAY = 3,
  MACRO = 4,
  SELECTION = 5,
};

void print_option()
{
    char message[LCD_WIDTH];
    sprintf(message, "%d>%s", messages[0].index, messages[0].name);
    lcd.setCursor(0, 0);
    lcd.print(message);
    sprintf(message, "%d>%s", messages[1].index, messages[1].name);
    lcd.setCursor(0, 1);
    lcd.print(message);
}

void cycle_down_delay() {
    Message temp;
    temp.name = messages[1].name;
    temp.index = messages[1].index;
    // check index bounds loop around
    messages[1].name = delayMap[temp.index + 1].name;
    messages[1].index = temp.index + 1;
    messages[0].name = temp.name;
    messages[0].index = temp.index;
}

void cycle_up_delay() {
    Message temp;
    temp.name = messages[0].name;
    temp.index = messages[0].index;
    // check index bounds loop around
    messages[0].name = delayMap[temp.index - 1].name;
    messages[0].index = temp.index - 1;
    messages[1].name = temp.name;
    messages[1].index = temp.index;  
}

void cycle_down_macro() 
{
    Message temp;
    temp.name = messages[1].name;
    temp.index = messages[1].index;
    // check index bounds loop around
    messages[1].name = signalMap[temp.index + 1].name;
    messages[1].index = temp.index + 1;
    messages[0].name = temp.name;
    messages[0].index = temp.index;
}

void cycle_up_macro()
{
    Message temp;
    temp.name = messages[0].name;
    temp.index = messages[0].index;
    // check index bounds loop around
    messages[0].name = signalMap[temp.index - 1].name;
    messages[0].index = temp.index - 1;
    messages[1].name = temp.name;
    messages[1].index = temp.index;
}

#define MACRO_ROWS 2
#define MACRO_COLUMNS 5
#define OFFSET 3
#define SIG 0
#define DELAY 1

void display_macros()
{
  char str[LCD_WIDTH];
  for (int i = 0; i < MACRO_ROWS; i++) {
    for (int j = 0; j < MACRO_COLUMNS; j++) {
      sprintf(str, "%d", (i * MACRO_COLUMNS + j));
      lcd.setCursor(j * OFFSET + 2, i);
      lcd.print(str);
    }
  }
}

#define MAX_MACRO_DIS 5

void saveInitialMacroMapToEEPROM() {
  for (int i = 0; i < NUM_MACROS; i++) {
    int temp[MAX_MACRO_LEN][2];

    for (int j = 0; j < MAX_MACRO_LEN; j++) {
      temp[j][SIG] = 1;        // Or whatever default signal you want
      temp[j][DELAY] = 100;    // Default delay in ms
    }

    EEPROM.put(i * sizeof(temp), temp);
  }
  EEPROM.update(1020, 1); // Mark as initialized
}

int macroBuffer[MAX_MACRO_LEN][2];

void loadMacro(int index) {
  EEPROM.get(index * sizeof(macroBuffer), macroBuffer);
}

void saveMacro(int index) {
  EEPROM.put(index * sizeof(macroBuffer), macroBuffer);
}

void display_macro(int macro, int offset) {
  loadMacro(macro);
  int lcd_index = 0;
  char str[2];
  for (int i = 0; i < MAX_MACRO_DIS; i++) {
    lcd.setCursor(i * OFFSET + 1, 0);
    int index = (offset >= MAX_MACRO_DIS) ? i + offset - MAX_MACRO_DIS + 1 : i;
    sprintf(str, "%d", macroBuffer[index][SIG]);
    lcd.print(str);
  }
}

void selection(int row, int col, int start) {
  lcd.setCursor(col * OFFSET + start, row);
  lcd.print(">");
}

void remove_selection(int start) {
  for (int i = 0; i < MACRO_ROWS; i++) {
    for (int j = 0; j < MACRO_COLUMNS; j++) {
      lcd.setCursor(j * OFFSET + start, i);
      lcd.print(" ");
    }
  }
}

void remove_edit_selection(int row) {
  lcd.setCursor(0, row);
  lcd.print(" ");
}

void setup() {
  Serial.begin(9600);
  pinMode(IR_PIN, OUTPUT);
  
  init_buttons();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
  
  if (EEPROM.read(1020) != 1) {
    saveInitialMacroMapToEEPROM(); // write once
  }
  
  // --- Timer2: 38kHz PWM on pin 3 (OC2B) ---
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS21);
  OCR2A = 51;
  OCR2B = OCR2A / 2;

  // --- Timer1: CTC for modulating MARK/SPACE timings ---
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= _BV(WGM12);
  OCR1A = (currentSignal.data[0] * 2) - 1;
  sei();
}

#define MACRO_START 1
#define SELECTION_START 0
#define EDIT_START 0

volatile bool isTransmitting = false;

void sendSignal(int macro) {
  if (isTransmitting) {
    Serial.println("Blocked: still transmitting");
    return;
  }
  isTransmitting = true;
  
  loadMacro(macro);  // Load from EEPROM into RAM buffer
  for (int i = 0; i < MAX_MACRO_LEN; i++) {
    int macroIndex = macroBuffer[i][SIG];
    if (macroIndex != 20) {
      currentSignal = signalMap[macroIndex];
      signalLength = currentSignal.size;
      loadbuffer(signalLength);
      TCNT1 = 0;
      TCCR1B |= _BV(CS11);
      TIMSK1 |= _BV(OCIE1A);
      while (TCCR1B & _BV(CS11)) {
        // Wait for ISR to turn off Timer1
      }
      delay(delayMap[macroBuffer[i][DELAY]].delay_val);
      Serial.print("delay: ");
      Serial.println(delayMap[macroBuffer[i][DELAY]].delay_val);
    }
  }
  isTransmitting = false;
  init_buttons();
}

State state = State::IDLE;
unsigned long lastTime = 0, frameCount = 0;
bool blinkOn = false;
int row = 0, col = 0, macro_col = 0, edit_row = 0, selected_macro = 0;

void loop() {
  int index = 0;
  
  if ((index = matrix_pressed()) != -1)
  {
    switch(index)
    {
      case UP:
        if (state == State::EDIT_MACRO) {
          if (edit_row > 0) {
            cycle_up_macro();
            edit_row--;
          }
        } else if (state == State::EDIT_DELAY) { 
          if (edit_row > 0) {
            cycle_up_delay();
            edit_row--;
          }
        } else if (state == State::MACRO) {
          if (row > 0) {
            row--;
          }
        }
        break;
      case DOWN:
        if (state == State::EDIT_MACRO) {
          if (edit_row < (sizeof(signalMap) / sizeof(signalMap[0]))) {
            cycle_down_macro();
            edit_row++;
          }
        } else if (state == State::EDIT_DELAY) { 
           if (edit_row < (sizeof(delayMap) / sizeof(delayMap[0]))) {
            cycle_down_delay();
            edit_row++;
           }
        } else if (state == State::MACRO) {
          if (row < MACRO_ROWS) {
            row++;
          }
        }
        break;
      case LEFT:
          if (state == State::MACRO) {
            if (col > 0) {
              col--;
            }
          } else if (state == State::SELECTION) {
            if (macro_col > 0) {
              macro_col--; 
            }
          }
        break;
      case RIGHT:
          if (state == State::MACRO) {
            if (col < MACRO_COLUMNS) {
              col++;
            }
          } else if (state == State::SELECTION) {
            if (macro_col < (MAX_MACRO_LEN)) {
              macro_col++; 
            }
          }
          
        break;  
      case OK:
          if (state == State::IDLE) {
            state = State::MACRO;
          } else if (state == State::MACRO) {
            row = 0;
            state = State::SELECTION;
          } else if (state == State::SELECTION) {
            messages[0].name = signalMap[0].name;
            messages[0].index = 0;
            messages[1].name = signalMap[1].name;
            messages[1].index = 1;
            state = State::EDIT_MACRO;
          } else if (state == State::EDIT_MACRO) {
            loadMacro(selected_macro);
            macroBuffer[macro_col][SIG] = messages[0].index;
            edit_row = 0;
            messages[0].name = delayMap[0].name;
            messages[0].index = 0;
            messages[1].name = delayMap[1].name;
            messages[1].index = 1;
            state = State::EDIT_DELAY;
          } else if (state == State::EDIT_DELAY) {
            macroBuffer[macro_col][DELAY] = messages[0].index;
            saveMacro(selected_macro);
            edit_row = 0;
            state = State::SELECTION;
          }
        break;
      case BACK:
          if (state == State::SELECTION) {
            state = State::MACRO;
          } else if (state == State::EDIT_MACRO) {
            state = State::SELECTION;
          } else if (state == State::EDIT_DELAY) {
            state = State::EDIT_MACRO;
          }
        break;
      case ZERO:
          sendSignal(0);
        break;
      case ONE:
          sendSignal(1);
        break;
      case TWO:
          sendSignal(2);
        break;
      case THREE:
          sendSignal(3);
        break;
      case FOUR:
          sendSignal(4);
        break;
      case FIVE:
          sendSignal(5);
        break;
      case SIX:
          sendSignal(6);
        break;
      case SEVEN:
          sendSignal(7);
        break;
      case EIGHT:
          sendSignal(8);
        break;
      case NINE:
          sendSignal(9);
        break;
    }
  }
  if (frameCount % 2 == 0) {
    lcd.clear();
  }
  unsigned long now = millis();   
  if (now - lastTime >= 250) {
    lastTime = now;
    if (state == State::MACRO) {
      display_macros();
      blinkOn = (frameCount % 4) < 2;
      if (blinkOn) {
        selection(row, col, MACRO_START);
      } else {
        remove_selection(MACRO_START);
      }
    } else if (state == State::SELECTION) {
      selected_macro = row * MACRO_COLUMNS + col;
      display_macro(selected_macro, macro_col);
      blinkOn = (frameCount % 4) < 2;
      if (blinkOn) {
        int stateIndex = (macro_col < MAX_MACRO_DIS - 1) ? macro_col : MAX_MACRO_DIS - 1;
        selection(0, stateIndex, SELECTION_START);
      } else {
        remove_selection(SELECTION_START);
      }
    } else if (state == State::EDIT_MACRO) {
      print_option();
      int stateIndex = (edit_row > 2) ? edit_row : 2 - 1;
      remove_edit_selection(stateIndex);
    } else if (state == State::EDIT_DELAY) {
      print_option();
      blinkOn = (frameCount % 4) < 2;
      int stateIndex = (edit_row > 2) ? edit_row : 2 - 1;
      remove_edit_selection(stateIndex);
    }
    frameCount++;
  }
}
