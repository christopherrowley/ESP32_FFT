
#include <FastLED.h> // Code for LEDs
// Strobe light designed for Halloween with repetitive flashes of red added in. 


/* initialize Hardware Variables */
#define LED_PIN           5 // Where LED strip is connected to board
#define NUM_LEDS         256
#define COLOR_ORDER     GRB           // If colours look wrong, play with this
#define CHIPSET         WS2812B       // LED strip type
#define MAX_MILLIAMPS   2000          // Careful with the amount of power here if running off USB port
const int BRIGHTNESS_SETTINGS =150;  // Configureable brightness. If power draw is an issue, lower value
#define LED_VOLTS       5             // Usually 5 or 12


// Lighting timing specific
#define TIME_LIGHT_ON         175 // milliseconds
#define TIME_LIGHT_OFF        175 // milliseconds
#define REPEAT_C1    10 // Times to repeat colour 1
#define REPEAT_C2    2 // Times to repeat colour 1
int c1_counter = 0;
int c2_counter = 0;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;


/* setup memory block for the leds */
CRGB leds[NUM_LEDS];



void setup() {

  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTS, MAX_MILLIAMPS);
  FastLED.setBrightness(BRIGHTNESS_SETTINGS);

  Serial.println("\nStarting strobe light...");

  /* Setup Pin for LED data export */
  pinMode(LED_PIN, OUTPUT);
}


void loop() {

  // Loop over first colour
  for (int lc = 0; lc < REPEAT_C1; lc++){

    // Start by turning light on white: 
    for (int i = 0; i < NUM_LEDS; i++)  {
      leds[i] = CRGB(250, 250, 250);
    }
    delay(5);
    FastLED.show();

    currentMillis = millis();
    // wait to keep the light on for the interval
    while(currentMillis - previousMillis < TIME_LIGHT_ON)   {
      currentMillis = millis();
    }

    // then turn light off.
    FastLED.clear();
    FastLED.show();
    previousMillis = currentMillis; 

    // keep the light off for the interval
    currentMillis = millis();

    // Then wait until its been off long enough.
    while(currentMillis - previousMillis < TIME_LIGHT_OFF)   {
      currentMillis = millis();
    }
    previousMillis = currentMillis; 
  }

// Loop over second colour
  for (int lc = 0; lc < REPEAT_C2; lc++){

    // Start by turning light on white: 
    for (int i = 0; i < NUM_LEDS; i++)  {
      leds[i] = CRGB(250, 0, 0);
    }

    delay(5);
    FastLED.show();

    currentMillis = millis();
    // wait to keep the light on for the interval
    while(currentMillis - previousMillis < TIME_LIGHT_ON)   {
      currentMillis = millis();
    }

    // then turn light off.
    FastLED.clear();
    FastLED.show();
    previousMillis = currentMillis; 

    // keep the light off for the interval
    currentMillis = millis();

    // Then wait until its been off long enough.
    while(currentMillis - previousMillis < TIME_LIGHT_OFF)   {
      currentMillis = millis();
    }
    previousMillis = currentMillis; 
  } // End loop colour 2. 

}


