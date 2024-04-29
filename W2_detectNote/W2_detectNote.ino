  /* C.R. new code for this:
  The frequency of each bin will have width SAMPLING_FREQ/Samples:
  SAMPLING_FREQ   5000 // max detectable freq = 2500Hz
  SAMPLES         2048 // gives us bin width 5000/2048 = 2.4
  Guitar vs piano, most notes we were picking up were frequencies below 2000 Hz. So we can increase samples, but also lower sampling freq.
  If you lower sampling freq too much, this will probably cause some aliasing issues. But since this analysis looks at the peak fundamental frequency
  Then we can probably get away with lower! Have them change the sampling frequency to 15000 and the samples to 4096 (not enough memory for 8192). 
  This gives us a bin width of ~3.6 (or 1.8), which gives us enough to fit to at octave 2 which starts at 65 Hz. 
  */

#include <FastLED_NeoMatrix.h>
#include <arduinoFFT.h>
#include <math.h> 

// User Configuration zone! //
#define NOISE_PEAK      100           // Used as a crude noise filter, values below this are ignored was 15000 for MAX4466

// End User Configuration zone //

#define LED_PIN         5             // LED strip data
#define SAMPLES         4096          // Must be a power of 2
#define SAMPLING_FREQ   8192         // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AUDIO_IN_PIN    35            // Signal in on this pin
#define COLOR_ORDER     GRB           // If colours look wrong, play with this
#define CHIPSET         WS2812B       // LED strip type
#define MAX_MILLIAMPS   2000          // Careful with the amount of power here if running off USB port
const int BRIGHTNESS_SETTINGS =150;  // Configureable brightness. If power draw is an issue, lower value
#define LED_VOLTS       5             // Usually 5 or 12
#define NUM_BANDS       12            // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
const uint8_t kMatrixWidth = 16;                          // Matrix width
const uint8_t kMatrixHeight = 16;                         // Matrix height
#define NUM_LEDS       (kMatrixWidth * kMatrixHeight)     // Total number of LEDs
#define BAR_WIDTH      1                                  // If width >= 8 light 1 LED width per bar, >= 16 light 2 LEDs width bar etc
#define TOP            (kMatrixHeight - 0)                // Don't allow the bars to go offscreen
#define SERPENTINE     true                               // Set to false if you're LEDS are connected end to end, true if serpentine

// Sampling and FFT stuff
unsigned int sampling_period_us;
int oldBarHeights[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int bar2Height = 0;                                       // store height of secondary note - account for intermediate notes
int bar1Height = 0;                                       // store height of first note
byte bar1Loc = 0;                                         // bin location of primary note
byte bar2Loc = 0;                                         // bin location of secondary note
double vReal[SAMPLES];
double vImag[SAMPLES];
double peakF = 0.0;   // FFT peak frequency.
double peakM = 0.0;  // FFT peak magnitude
unsigned long newTime;
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);
int centerShift = 2;  // Number of columns to shift bars over

// FastLED stuff
CRGB leds[NUM_LEDS];

// ***************************************************
// Define different colors, pick one! Comment out the others
// takes 4 values: index, then RGB

// // Purple-blue
// DEFINE_GRADIENT_PALETTE( colour_gp ) {
//   0,   0,  0, 255,   //blue
// 255, 179,   0, 255 }; //purple

// // Sunset
// DEFINE_GRADIENT_PALETTE( colour_gp ) {
//   0, 141,   0, 180,   //purple
// 127, 255, 192,   0,   //yellow
// 255,   0,   5, 255 };  //blue

// Rainbow
DEFINE_GRADIENT_PALETTE( colour_gp ) {
  0,   255, 0,  0,   //red
 50,   255, 128, 0,   //orange
100,   255,   255, 0,   //yellow
150,   0, 255, 0,   //green
200,   0, 0, 255,   //blue
255,   141,   0, 100 }; //purple

// Fire
// DEFINE_GRADIENT_PALETTE( colour_gp ) {
// 0,   200, 200,  200,   //white 
// 70,   255, 218,    0,   //yellow
// 175,   255,   0,    0,  //red
// 255,   150,   0,    0 };  //red

// ***************************************************
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
  FastLED.setBrightness(BRIGHTNESS_SETTINGS);
  FastLED.clear();
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
}


void loop() {

  FastLED.clear();

  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }

  // Compute FFT
  FFT.DCRemoval(); // Remove DC offset
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); // Window to remove truncation effects in FFT - should help with aliasing too.
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  // C.R. we can extract the major peak using a built in function:
  FFT.MajorPeak( &peakF, &peakM);

  // Serial.print(peakF, 2); // Frequency of Peak -> 2 is for 2 decimal points. 
  // Serial.print(", ");
  // Serial.println(peakM, 2); // Magnitude of Peak

  // Only light up bars if peakF is greater than NOISE_PEAK

  if (peakM > NOISE_PEAK ) {

    FastLED.clear(); // Fresh canvas

    // Peak freq, and solve for number of steps (55.0040869Hz is A note Octave 1)
    // take log2 using log math rule-> log2(x) = log10(X)/log10(2); [since log2 isnt a function]
    double noteSteps = log10( peakF/55.0040869)/log10(2)  *12; 

    // Take the remainder after division with 12 to remove octave scaling
    noteSteps = fmod(noteSteps,  12); // % takes remainder following division, result is from 0 to 11

    // Now we know we need to divide it up into 2 for 16 bar split
    bar2Height = round( fmod(noteSteps, 1) * kMatrixHeight)-1; // % takes remainder following division. might need to cast.
    bar1Height = kMatrixHeight - bar2Height;

    // deal with wrap around for A and G# 
    bar1Loc = byte( floor(noteSteps));  // floor should come with math.h

    if (bar1Loc == 11) {
        bar2Loc = 0;
    } else {
        bar2Loc = bar1Loc+1;
    }


    // Set Bar1
    // Small amount of averaging between frames
    float t1 = float(oldBarHeights[bar1Loc]);
    float t2 = float(bar1Height);
    float roundBar = round( ( t1*1.0 + 3.0*t2 ) / 4.0); 

    int barHeight = int(roundBar); // Push back to integer
    if (barHeight > TOP) barHeight = TOP;

    // Draw bars
    colourBars(bar1Loc, barHeight); 

    // Save oldBarHeights for averaging later
    oldBarHeights[bar1Loc] = barHeight;

    // Set Bar2
    t1 = float(oldBarHeights[bar2Loc]);
    t2 = float(bar2Height);
    roundBar = round( ( t1*1.0 + 3.0*t2 ) / 4.0); 

    barHeight = int(roundBar); // Push back to integer
    if (barHeight > TOP) barHeight = TOP;

    // Draw bars
    colourBars(bar2Loc, barHeight); 

    // Save oldBarHeights for averaging later
    oldBarHeights[bar2Loc] = barHeight;

  } else {
    // If just background noise, display blank
    FastLED.clear();
  }

  FastLED.show();
}

// PATTERNS BELOW //
void colourBars(int band, int barHeight) {
  int xStart = NUM_BANDS - BAR_WIDTH * band+ centerShift-1; // Move from left to right
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = TOP; y >= TOP - barHeight; y--) {
      matrix->drawPixel(x, y, ColorFromPalette( colourPal, y* 255/TOP , BRIGHTNESS_SETTINGS, LINEARBLEND)); // Same Colour across
      // matrix->drawPixel(x, y, ColorFromPalette( colourPal, y * (255 / (barHeight + 1)), BRIGHTNESS_SETTINGS)); // or slide colour bar up
    }
  }
}



