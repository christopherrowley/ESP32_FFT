/* Code to count down from 60, and display count on LED matrix
*/

#include <FastLED_NeoMatrix.h>
#include <arduinoFFT.h>
// https://github.com/marcmerlin/FastLED_NeoMatrix/commit/d28d0400dbb50ffcb2d26983600281410235e272
#include "fonts.h"

// Definitions
#define LED_PIN         5             // LED strip data
#define COLOR_ORDER     GRB           // If colours look wrong, play with this
#define CHIPSET         WS2812B       // LED strip type
#define LED_VOLTS       5             // Usually 5 or 12
#define SERPENTINE     true                               // Set to false if you're LEDS are connected end to end, true if serpentine
#define MAX_MILLIAMPS   2000          // Careful with the amount of power here if running off USB port


int digit1 = 5;                // Index of digits to start from in letters
int digit2 = 9;   
const uint8_t kMatrixWidth = 16;                          // Matrix width
const uint8_t kMatrixHeight = 16;                         // Matrix height
#define NUM_LEDS       (kMatrixWidth * kMatrixHeight)     // Total number of LEDs
const int BRIGHTNESS_SETTINGS = 250;  // Configureable brightness. If power draw is an issue, lower value

// Timing:
long previousMillis = 0; 
long interval = 1000;  

char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'G', 'O' };
static uint16_t size = 1; // size 1 == 8 pixels high, 2 = 16

CRGB leds[NUM_LEDS];


// FastLED_NeoMaxtrix - see https://github.com/marcmerlin/FastLED_NeoMatrix for Tiled Matrixes, Zig-Zag and so forth
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, kMatrixWidth, kMatrixHeight,
  NEO_MATRIX_TOP        + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS       + NEO_MATRIX_ZIGZAG +
  NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS);


void setup() {
  Serial.begin(115200);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTS, MAX_MILLIAMPS);
  matrix->begin();
  matrix->setBrightness(BRIGHTNESS_SETTINGS);
  matrix->setTextWrap(false);
  matrix->setFont( &Century_Schoolbook_L_Bold[size] );
}

void loop() {
  // put your main code here, to run repeatedly:
  matrix_clear();

  // To make sure it runs only every second
  unsigned long currentMillis = millis();
  while(currentMillis - previousMillis < interval)   {
    currentMillis = millis();
  }

  // Program display
  // Set to GO if both are 0 -> updates are at the end of a loop

  if (digit1 == 0) && (digit2 == 0){

    // Set ones digit (2) right
  matrix->setTextColor(color(0,200,0)); 	
  matrix->setCursor(1, 8);
  matrix->print(letters[11]);

  // Set tens digit(1) left

  matrix->setTextColor(color(0,200,0)); 	
  matrix->setCursor(1, 0);
  matrix->print(letters[10]);

  // Digit handling for next loop -> reset
  digit1 = 5;
  digit2 = 9; 


  } else {

    // Set ones digit (2) right
    matrix->setTextColor(color(200,200,200)); 	
    matrix->setCursor(1, 8);
    matrix->print(letters[digit2]);
    // Set tens digit(1) left

    matrix->setTextColor(color(200,200,200)); 	
    matrix->setCursor(1, 0);
    matrix->print(letters[digit1]);

    // Digit handling for next loop
    if (digit2 > 0) {}
      digit2--;
    }  else {
      digit1--;
      digit2 = 9;
    }
  }

  previousMillis = currentMillis; 
  matrix_show();

}


















