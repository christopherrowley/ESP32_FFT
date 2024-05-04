/* Code to count down from 60, and display count on LED matrix
*/

// https://github.com/marcmerlin/FastLED_NeoMatrix/commit/d28d0400dbb50ffcb2d26983600281410235e272
#include <Adafruit_GFX.h> // New for this one -> graphics support
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>
//#include "fonts.h"
//#include <Fonts/FreeMono9pt7b.h>

// Definitions
#define MATRIXPIN           5   // LED strip data
#define MATRIX_TILE_WIDTH   16 // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_TILE_HEIGHT  16 // height of each matrix
#define MATRIX_TILE_H       1  // number of matrices arranged horizontally
#define MATRIX_TILE_V       1  // number of matrices arranged vertically

// Used by NeoMatrix
#define mw (MATRIX_TILE_WIDTH *  MATRIX_TILE_H)
#define mh (MATRIX_TILE_HEIGHT * MATRIX_TILE_V)
#define NUMMATRIX (mw*mh)
#define COLOR_ORDER     GRB           // If colours look wrong, play with this
#define CHIPSET         WS2812B       // LED strip type

// Color definitions
// #define BLACK 0x0000
// #define BLUE 0x001F
// #define RED 0xF800
// #define CYAN 0x07FF
// #define MAGENTA 0xF81F
// #define YELLOW 0xFFE0
#define GREEN 0x07E0
#define WHITE 0xFFFF

uint8_t matrix_brightness = 32;
const int buttonPin = 2;
//const int buzzer = 9; //buzzer to arduino pin 9

CRGB matrixleds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, MATRIX_TILE_H, MATRIX_TILE_V, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_LEFT +  NEO_TILE_PROGRESSIVE);


// Specific for text
int digit1 = 5;                // Index of digits to start from in letters
int digit2 = 9;   
int size = 1; 
int cursX = 2;
int cursY = 5; 

// Timing:
unsigned long previousMillis = 0; 
long interval = 1000;  

char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'G', 'O' };
char printLetters[]= {'5', '9', ' '};


void setup() {
  delay(100);
  Serial.begin(115200);
  FastLED.addLeds<CHIPSET, MATRIXPIN, COLOR_ORDER>(matrixleds, NUMMATRIX).setCorrection(TypicalLEDStrip);
  matrix->begin();
  matrix->setBrightness(matrix_brightness);
  matrix->setTextWrap(false);
  //matrix->setFont( ); // size 1 == 8 pixels high, 2 = 16
  matrix->setFont(); // size 1 == 8 pixels high, 2 = 16 &FreeSans9pt7b 
  matrix->setTextSize(1);

  pinMode(buttonPin, INPUT);
  //pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
}

void loop() {
  // put your main code here, to run repeatedly:
  FastLED.clear();
  //memset(matrixleds, 0, NUMMATRIX*3);

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    //Reset the counter
    digit1 = 5;
    digit2 = 9;
  } 

  // To make sure it runs only every second
  unsigned long currentMillis = millis();
  while(currentMillis - previousMillis < interval)   {
    currentMillis = millis();
  }

  // Program display
  // Set to GO if both are 0 -> updates are at the end of a loop

  if ((digit1 == 0) && (digit2 == 0)) {

    matrix->setTextColor( GREEN ); 	
    matrix->setCursor(cursX, cursY);
    printLetters[0] = letters[10];
    printLetters[1] = letters[11];
    matrix->print( printLetters );

    // Digit handling for next loop -> reset
    digit1 = 5;
    digit2 = 9; 

  } else {

    matrix->setTextColor( WHITE ); 	
    matrix->setCursor(cursX, cursY);
    printLetters[0] = letters[digit1]; // this only is updated on the turnover
    printLetters[1] = letters[digit2]; // This one is updated every time
    matrix->print( printLetters );
  
    // Digit handling for next loop
    if (digit2 > 0) {
      digit2--;
    } else {
      digit1--;
      digit2 = 9;
    }
  }

  // // Tones on 3 2, 1, GO -> Note we have already decremented the counter above.
  // if (digit1 == 5) & (digit2 < 9){
  //   tone(buzzer, 1000); // Send 1KHz sound signal...
  //   delay(500);        // ...for 0.5 sec
  //   noTone(buzzer);     // Stop sound...
  // }
  // else if (digit1 == 0) & (digit2 < 3){
  //   tone(buzzer, 400); // Send 1KHz sound signal...
  //   delay(500);        // ...for 0.5 sec
  //   noTone(buzzer);     // Stop sound...
  // }


  previousMillis = currentMillis; 
  matrix->show();

}


















