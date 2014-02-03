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
#ifndef __LSM303_H__
#define __LSM303_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#if defined (__AVR_ATtiny85__)
 #include "TinyWireM.h"
 #define WIRE TinyWireM
#else
 #include "Wire.h"
 #define WIRE Wire
#endif

#define LSM303_ADDRESS_ACCEL          (0x32 >> 1)         // 0011001x
#define LSM303_ID                     (0b11010100)

class Adafruit_LSM303_supertiny
{
  public:
    typedef enum
    {                                                     // DEFAULT    TYPE
      LSM303_REGISTER_ACCEL_CTRL_REG1_A         = 0x20,   // 00000111   rw
      LSM303_REGISTER_ACCEL_OUT_X_L_A           = 0x28,
    } lsm303AccelRegisters_t;

    typedef struct lsm303AccelData_s
    {
      int x;
      int y;
      int z;
    } lsm303AccelData;
	
    bool begin(void);
    void read(void);

    lsm303AccelData accelData;    // Last read accelerometer data will be available here

    void write8(byte address, byte reg, byte value);
    byte read8(byte address, byte reg);

  private:
};

#endif
