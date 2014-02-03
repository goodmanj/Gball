/* Gball
  
  Nerf soccerball studded with RGB Neopixels, driven by an Adafruit Trinket based on input from an LSM303 
  accelerometer.
  
  Ball glows yellow on top and green on the bottom.  It fades to blue when it's in free-fall, and becomes red at high g's.
  
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
*/

#include <TinyWireM.h>
#include "Adafruit_LSM303_supertiny.h"// Use my super tiny version of LSM303 driver.
#include <Adafruit_NeoPixel.h>

#define I2CDATA_PIN 0
#define PIXEL_PIN 1
#define I2CCLOCK_PIN 2
#define NPIXELS 12
#define SMOOTHING 3

Adafruit_LSM303_supertiny accel; 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
long int xstore,ystore,zstore;  // Lowpass filter storage bin

int lowpass(int input, byte strength, long int *storage) {
  /* Integer low-pass autoregressive filter.  Smooth out signals over 2^strength samples.
     
     Long integer "storage" stores intermediate data.
     
     */
     *storage = *storage - (*storage >> strength) + input;
     return *storage >> strength;
}

// ------------------------ Main Program -----------------------------

void setup() {
  TinyWireM.begin(); // I2C init
  accel.begin();
  pixels.begin();
  pixels.setBrightness(100);  // Augh too bright!
  pixels.show(); // Initialize all pixels to 'off'
}

// Coordinates of LEDs in x,y,z.  For a soccer ball, the black spots are at the vertices of an icosahedron.
// http://en.wikipedia.org/wiki/Icosahedron

int ledx[] = {-255,   -114,   -114,   -114,   -114,   -114,    114,    114,    114,    114,    114,   255};
int ledy[] = {   0,   -228,    -70,   184,   184,    -70,    -184,   70,  228,   70,    -184,     0};
int ledz[] = {   0,     0,  216,   134,    -134,   -216,   134,  216,     0,   -216,    -134,     0};

// Define colors used by the ball.
const byte red[] = {255,0,0};
const byte yel[] = {255,200,0};
const byte grn[] = {0,255,50};
const byte blu[] = {0,0,255};

void loop() {
  byte top[3],bot[3];   // Color of top and bottom of ball
  byte color[3];        // Color of each pixel
  byte i,j;             // i = loop over colors, j = loop over pixels
  
  accel.read();
  
  // Smooth out sudden changes in the accelerometer signal
  long x = lowpass(accel.accelData.x,SMOOTHING,&xstore);
  long y = lowpass(accel.accelData.y,SMOOTHING,&ystore);
  long z = lowpass(accel.accelData.z,SMOOTHING,&zstore);

  // Total magnitude of gravity, in milligees
  long g = long(min(sqrt(long(x*x)+long(y*y)+long(z*z)),2000));
  
  // Ball will be blue at zero g, yellow & green at one g, and red at 2g or more.  Linearly interpolate between
  // these colors for both top and bottom.
  
  if (g < 1000) {
      top[0] = (blu[0]*(1000-g)+yel[0]*g)/1000;
      bot[0] = (blu[0]*(1000-g)+grn[0]*g)/1000;
      top[1] = (blu[1]*(1000-g)+yel[1]*g)/1000;
      bot[1] = (blu[1]*(1000-g)+grn[1]*g)/1000;
      top[2] = (blu[2]*(1000-g)+yel[2]*g)/1000;
      bot[2] = (blu[2]*(1000-g)+grn[2]*g)/1000;
      // Annoyingly, a for() loop takes too much memory.
  } else {
      top[0] = (yel[0]*(2000-g)+red[0]*(g-1000))/1000;
      bot[0] = (grn[0]*(2000-g)+red[0]*(g-1000))/1000;
      top[1] = (yel[1]*(2000-g)+red[1]*(g-1000))/1000;
      bot[1] = (grn[1]*(2000-g)+red[1]*(g-1000))/1000;
      top[2] = (yel[2]*(2000-g)+red[2]*(g-1000))/1000;
      bot[2] = (grn[2]*(2000-g)+red[2]*(g-1000))/1000;
  }
  
  for (j=0; j < pixels.numPixels(); j++) { // Each pixel
    // Project the gravity vector onto the coordinates of this pixel, to find out how close to the top of the ball the pixel is
     long topness = (x*ledx[j]+y*ledy[j]+z*ledz[j])/g;
     long botness = -topness;
    // Cap topness and bottomness at 0-255 just in case.  Adding 25 to both means the colors fade smoothly from "top" to "bottom" when near
    // the midpoint of the ball.
     topness = max(min(topness+25,255),0);
     botness = max(min(botness+25,255),0);
     for (i=0; i<3; i++) {
       // Set the color of this pixel based on how close to top/bottom it is.
            color[i] = (topness*top[i] + botness*bot[i])/255;
     }
     pixels.setPixelColor(j, color[0],color[1],color[2]); 
  }
  pixels.show(); // Push colors to pixels.
}

