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


int digit1 = 5;                                       // store height of secondary note - account for intermediate notes
int digit2 = 9;   
const uint8_t kMatrixWidth = 16;                          // Matrix width
const uint8_t kMatrixHeight = 16;                         // Matrix height
const int BRIGHTNESS_SETTINGS = 250;  // Configureable brightness. If power draw is an issue, lower value

// Timing:
long previousMillis = 0; 
long interval = 1000;  

char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'G', 'O' };
static uint8_t l;
static uint16_t size = 8;

CRGB leds[NUM_LEDS];

DEFINE_GRADIENT_PALETTE( colour_gp ) {
  0, 200,   200, 200,   //white
127, 220, 180,   180,   //red
255,  255,  150, 150 };  //reder

CRGBPalette16 colourPal = colour_gp;

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
  // Set to GO if both are 0

  if (digit1 == 0) && (digit2 == 0){

    // Set ones digit (2) right
  matrix->setTextColor(txtcolor); 	
  matrix->setCursor(10-size*0.55+offset, 17+size*0.75);
  matrix->print(letters[12]);

  // Set tens digit(1) left

  matrix->setTextColor(txtcolor); 	
  matrix->setCursor(10-size*0.55+offset, 17+size*0.75);
  matrix->print(letters[11]);


  } else {

    // Set ones digit (2) right
  matrix->setTextColor(txtcolor); 	
  matrix->setCursor(10-size*0.55+offset, 17+size*0.75);
  matrix->print(letters[l]);
  // Set tens digit(1) left

  matrix->setTextColor(txtcolor); 	
  matrix->setCursor(10-size*0.55+offset, 17+size*0.75);
  matrix->print(letters[l]);


  }

  




  // Digit handling for next loop
  if (digit2 > 0) {}
    digit--;
  }  else {
    digit1 = 9;
  }



  previousMillis = currentMillis; 
  matrix_show();

}








// Print Numbers //
void print1(int band, int barHeight) {
  int xStart = NUM_BANDS - BAR_WIDTH * band+ centerShift-1; // Move from left to right
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = TOP; y >= TOP - barHeight; y--) {
      matrix->drawPixel(x, y, ColorFromPalette( colourPal, y* 255/TOP , BRIGHTNESS_SETTINGS, LINEARBLEND)); // Same Colour across
      // matrix->drawPixel(x, y, ColorFromPalette( colourPal, y * (255 / (barHeight + 1)), BRIGHTNESS_SETTINGS)); // or slide colour bar up
    }
  }
}

















