Gball
=====

  Nerf soccerball studded with RGB Neopixels, driven by an Adafruit Trinket based on input from an LSM303 
  accelerometer.
  
  Ball glows yellow on top and green on the bottom.  It fades to blue when it's in free-fall, and 
  becomes red at high g's.
  
  Hardware:
    Adafruit "Trinket" - http://www.adafruit.com/products/1500 
      (Gemma and full-sized Arduinos should also work. )
    Adafruit Triple-axis accelerometer (LSM303) - http://www.adafruit.com/products/1120
    12x Adafruit Flora Neopixels http://www.adafruit.com/products/1260
    Lithium Ion Polymer Battery - 3.7v 100mAh - http://www.adafruit.com/products/1570
 
  Trinket Pin Mapping:
    #0: I2C data
    #1: Neopixel output
    #2: I2C clock
