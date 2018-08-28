/* tribble_incantation

This is the current program running in the LED tribble. 
It pulls data from an accelerometer, maps that data onto a 0-255 scale 
and then plugs those variables into fastleds HSV gradient function.


*/


#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 3.1.
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#include <Wire.h>
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed global definitions.

#define NUM_LEDS_PER_STRIP 8
#define NUM_LEDS NUM_LEDS_PER_STRIP
CRGB leds[NUM_LEDS_PER_STRIP]; 
//#define LED_CK 11
#define COLOR_ORDER GRB                                     
#define LED_TYPE WS2812                                       

float angles[3]; // yaw pitch roll

// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();
// Initialize changeable global variables.
uint8_t max_bright = 250;                                      // Overall brightness definition. It can be changed on the fly.




void setup() {

  //Serial.begin(9600);  
  Wire.begin();
  
  sixDOF.init(); //begin the IMU
  delay(5);
   
  LEDS.addLeds<LED_TYPE, 4 , COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP);
  LEDS.addLeds<LED_TYPE, 5 , COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP);
  LEDS.addLeds<LED_TYPE, 7 , COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP);
  LEDS.addLeds<LED_TYPE, 8 , COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP);
  LEDS.addLeds<LED_TYPE, 9 , COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP);
  LEDS.addLeds<LED_TYPE, 12 , COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP);
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
   set_max_power_in_volts_and_milliamps(5, 400);          // This is used by the power management functionality and is currently set at 5V, 400mA.
  FastLED.setBrightness(200);
   FastLED.setDither(25);
} // setup()


void loop () {
  blendme();
  FastLED.show();                        // Power managed display.
} // loop()


void blendme() {
   sixDOF.getEuler(angles);
  int axisA = map(angles[0], -180, 180, 100, 250); //
  int axisC = map(angles[1], -80, 80, 0, 60);      //maps the data from the accelerometer into useful numbers
  int axisB = map(angles[2], -180, 180, 10, 200);  //none get the full range to keep the colors from going crazy
  if ( axisA > 255) {
    axisA = axisA ;
  } else {
    axisA = (255 - (axisA - 255));  //turns the spectrum into a loop rather than a line, 
  }                                 //an attempt at keeping the colors from changing too 
                                    //dramatically
   if ( axisB > 255) {
    axisB = axisB ;
  } else {
    axisB = (255 - (axisB - 255));
  }
  
  uint8_t starthue = (axisA) + (axisC)  ;
 // uint8_t midhue = axisC ;
  uint8_t endhue = (axisB) + (axisC)  ;
  
 
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,(95 + (axisC)),250), CHSV(endhue,(255 - (axisC)),255), SHORTEST_HUES);    
 
} // blendme()
