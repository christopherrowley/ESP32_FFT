// Edited to add Neomatrix support for easier compatibility with different layouts.
// For time being, remove button.

#include <FastLED_NeoMatrix.h>
#include <arduinoFFT.h>


#define SAMPLES         1024          // Must be a power of 2
#define SAMPLING_FREQ   40000         // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE       1000          // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define AUDIO_IN_PIN    35            // Signal in on this pin
#define LED_PIN         5             // LED strip data
#define COLOR_ORDER     GRB           // If colours look wrong, play with this
#define CHIPSET         WS2812B       // LED strip type
#define MAX_MILLIAMPS   2000          // Careful with the amount of power here if running off USB port
const int BRIGHTNESS_SETTINGS =150;  // Configureable brightness. If power draw is an issue, lower value
#define LED_VOLTS       5             // Usually 5 or 12
#define NUM_BANDS       16            // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define NOISE           600           // Used as a crude noise filter, values below this are ignored was 500 for MAX4466
const uint8_t kMatrixWidth = 16;                          // Matrix width
const uint8_t kMatrixHeight = 16;                         // Matrix height
#define NUM_LEDS       (kMatrixWidth * kMatrixHeight)     // Total number of LEDs
#define BAR_WIDTH      (kMatrixWidth  / (NUM_BANDS - 1))  // If width >= 8 light 1 LED width per bar, >= 16 light 2 LEDs width bar etc
#define TOP            (kMatrixHeight - 0)                // Don't allow the bars to go offscreen
#define SERPENTINE     true                               // Set to false if you're LEDS are connected end to end, true if serpentine

// Sampling and FFT stuff
unsigned int sampling_period_us;
byte peak[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};              // The length of these arrays must be >= NUM_BANDS
int oldBarHeights[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int bandValues[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);


// FastLED stuff
CRGB leds[NUM_LEDS];
DEFINE_GRADIENT_PALETTE( purple_gp ) {
  0,   0, 255, 255,   //blue
255, 179,   0, 255 }; //purple
CRGBPalette16 purplePal = purple_gp;


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

  // Reset bandValues[]
  for (int i = 0; i<NUM_BANDS; i++){
    bandValues[i] = 0;
  }

  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }

  // Compute FFT
  FFT.DCRemoval(); // Remove DC offset
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); // Window to remove truncation effects in FFT
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
 
   //Serial.println(vReal[5]); // debug mic noise

  // Analyse FFT results
  for (int i = 2; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter
   
    
    //16 bands, 12kHz top band
      if (i>8   && i<=14  ) bandValues[0]  += (int)vReal[i];
      if (i>14   && i<=18  ) bandValues[1]  += (int)vReal[i];
      if (i>18   && i<=23  ) bandValues[2]  += (int)vReal[i];
      if (i>23   && i<=28  ) bandValues[3]  += (int)vReal[i];
      if (i>28   && i<=36  ) bandValues[4]  += (int)vReal[i];
      if (i>36   && i<=45  ) bandValues[5]  += (int)vReal[i];
      if (i>45   && i<=56  ) bandValues[6]  += (int)vReal[i];
      if (i>56   && i<=71  ) bandValues[7]  += (int)vReal[i];
      if (i>71   && i<=89  ) bandValues[8]  += (int)vReal[i];
      if (i>89   && i<=111  ) bandValues[9]  += (int)vReal[i];
      if (i>111   && i<=139  ) bandValues[10]  += (int)vReal[i];
      if (i>139   && i<=175  ) bandValues[11]  += (int)vReal[i];
      if (i>175   && i<=219  ) bandValues[12]  += (int)vReal[i];
      if (i>219   && i<=275  ) bandValues[13]  += (int)vReal[i];
      if (i>275   && i<=345  ) bandValues[14]  += (int)vReal[i];
      if (i>345             ) bandValues[15]  += (int)vReal[i];

    }
  }

  // Process the FFT data into bar heights
  for (byte band = 0; band < NUM_BANDS; band++) {

    // Scale the bars for the display
    int barHeight = bandValues[band] / AMPLITUDE;

    // if (band == 3) {
    //   Serial.println(barHeight); // debug mic
    // }

    if (barHeight > TOP) barHeight = TOP;

    // Small amount of averaging between frames
    barHeight = ((oldBarHeights[band] * 1) + barHeight) / 2;

    // Move peak up
    if (barHeight > peak[band]) {
      peak[band] = min(TOP, barHeight);
    }

    // Draw bars
    purpleBars(band, barHeight);

    // Draw peaks
    whitePeak(band);

    // Save oldBarHeights for averaging later
    oldBarHeights[band] = barHeight;
  }

  // Decay peak
  EVERY_N_MILLISECONDS(40) {
    for (byte band = 0; band < NUM_BANDS; band++)
      if (peak[band] > 0) peak[band] -= 1;
  }

  FastLED.show();
}

// PATTERNS BELOW //
void purpleBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = TOP; y >= TOP - barHeight; y--) {
      matrix->drawPixel(x, y, ColorFromPalette(purplePal, y * (255 / (barHeight + 1)), BRIGHTNESS_SETTINGS));
    }
  }
}

void whitePeak(int band) {
  int xStart = BAR_WIDTH * band;
  int peakHeight = TOP - peak[band] - 1;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    matrix->drawPixel(x, peakHeight, CHSV(0,0,200));
  }
}


