/***************************************************************************
  This is a library for the LSM303 Accelerometer and magnentometer/compass

  This version uses the TinyWire library for I2C communication, if running on an
  ATtiny85 (Adafruit Trinket or Gemma).  To save memory, everything but basic
  accelerometer functions has been stripped out.

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  

  Modified by Jason Goodman

  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include "Adafruit_LSM303_supertiny.h"

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/
bool Adafruit_LSM303_supertiny::begin()
{
  WIRE.begin();
//Serial.println("Wire");

  // Enable the accelerometer
  write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x27);
  
  return true;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
void Adafruit_LSM303_supertiny::read()
{
  // Read the accelerometer
  WIRE.beginTransmission((byte)LSM303_ADDRESS_ACCEL);
  WIRE.write(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80);
  WIRE.endTransmission();
  WIRE.requestFrom((byte)LSM303_ADDRESS_ACCEL, (byte)6);

  // Wait around until enough data is available
  while (WIRE.available() < 6);

  uint8_t xlo = WIRE.read();
  uint8_t xhi = WIRE.read();
  uint8_t ylo = WIRE.read();
  uint8_t yhi = WIRE.read();
  uint8_t zlo = WIRE.read();
  uint8_t zhi = WIRE.read();

  // Shift values to create properly formed integer (low byte first)
  accelData.x = (xlo | (xhi << 8)) >> 4;
  accelData.y = (ylo | (yhi << 8)) >> 4;
  accelData.z = (zlo | (zhi << 8)) >> 4;
  
}


/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/
void Adafruit_LSM303_supertiny::write8(byte address, byte reg, byte value)
{
  WIRE.beginTransmission(address);
  WIRE.write(reg);
  WIRE.write(value);
  WIRE.endTransmission();
}

byte Adafruit_LSM303_supertiny::read8(byte address, byte reg)
{
  byte value;

  WIRE.beginTransmission(address);
  WIRE.write(reg);
  WIRE.endTransmission();
  WIRE.requestFrom(address, (byte)1);
  value = WIRE.read();
  WIRE.endTransmission();

  return value;
}
