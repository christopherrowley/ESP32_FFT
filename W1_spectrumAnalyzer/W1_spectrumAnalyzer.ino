// Edited to add Neomatrix support for easier compatibility with different layouts.
// For time being, remove button.
#include <FastLED_NeoMatrix.h>
#include <arduinoFFT.h>

// ***************************************************
// User Configuration zone! //
#define NOISE           150           // Used as a crude noise filter, values below this are ignored was 500 for MAX4466
#define AMPLITUDE       1000          // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.

// End User Configuration zone //
// ***************************************************

#define LED_PIN         5             // LED strip data
const uint16_t  SAMPLES = 1024; // arduinoFFT 2.0.3
const float SAMPLING_FREQ = 30000;
#define AUDIO_IN_PIN    35            // Signal in on this pin
#define COLOR_ORDER     GRB           // If colours look wrong, play with this
#define CHIPSET         WS2812B       // LED strip type
#define MAX_MILLIAMPS   1500          // Careful with the amount of power here if running off USB port
const int BRIGHTNESS_SETTINGS =80;  // Configureable brightness. If power draw is an issue, lower value
#define LED_VOLTS       5             // Usually 5 or 12
#define NUM_BANDS       16            // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
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
float vReal[SAMPLES]; // arduinoFFT 2.0.3
float vImag[SAMPLES]; // arduinoFFT 2.0.3
unsigned long newTime;
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, SAMPLES, SAMPLING_FREQ);  // arduinoFFT 2.0.3 https://github.com/kosme/arduinoFFT/wiki

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
  delay(500);
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

  // Compute FFT - arduinofft 2.0.2
  FFT.dcRemoval(); // Remove DC offset
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); // Window to remove truncation effects in FFT
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();
 
    // debug mic noise
 // Serial.println(vReal[5]); 

  // Analyse FFT results
  for (int i = 2; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter
   
    
    //16 bands, 12kHz top band
      if (i>15   && i<=19  ) bandValues[0]  += (int)vReal[i];
      if (i>19   && i<=23  ) bandValues[1]  += (int)vReal[i];
      if (i>23   && i<=28  ) bandValues[2]  += (int)vReal[i];
      if (i>28   && i<=35  ) bandValues[3]  += (int)vReal[i];
      if (i>35   && i<=42  ) bandValues[4]  += (int)vReal[i];
      if (i>42   && i<=51  ) bandValues[5]  += (int)vReal[i];
      if (i>51   && i<=63  ) bandValues[6]  += (int)vReal[i];
      if (i>63   && i<=77  ) bandValues[7]  += (int)vReal[i];
      if (i>77   && i<=94  ) bandValues[8]  += (int)vReal[i];
      if (i>94   && i<=114  ) bandValues[9]  += (int)vReal[i];
      if (i>114   && i<=140  ) bandValues[10]  += (int)vReal[i];
      if (i>140   && i<=171  ) bandValues[11]  += (int)vReal[i];
      if (i>171   && i<=208  ) bandValues[12]  += (int)vReal[i];
      if (i>208   && i<=254  ) bandValues[13]  += (int)vReal[i];
      if (i>254   && i<=310  ) bandValues[14]  += (int)vReal[i];
      if (i>310   && i<=379 ) bandValues[15]  += (int)vReal[i];

    }
  }

  // Process the FFT data into bar heights
  for (byte band = 0; band < NUM_BANDS; band++) {

    // Scale the bars for the display
    int barHeight = bandValues[band] / AMPLITUDE;

    // debug mic 
    // if (band == 3) Serial.println(barHeight); 

    if (barHeight > TOP) barHeight = TOP;

    // Small amount of averaging between frames
    barHeight = ((oldBarHeights[band] * 1) + barHeight) / 2;

    // Move peak up
    if (barHeight > peak[band]) {
      peak[band] = min(TOP, barHeight);
    }

    // Draw bars
    colourBars(band, barHeight);

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

// ******************************************************
// FUNCTIONS FOR COLOURING LEDS //
// ******************************************************
void colourBars(int band, int barHeight) {
  int xStart = NUM_BANDS - BAR_WIDTH * band-1; // left to right
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = TOP; y >= TOP - barHeight; y--) {
      matrix->drawPixel(x, y, ColorFromPalette( colourPal, y* 255/TOP , BRIGHTNESS_SETTINGS, LINEARBLEND)); // Same Colour across
      // matrix->drawPixel(x, y, ColorFromPalette( colourPal, y * (255 / (barHeight + 1)), BRIGHTNESS_SETTINGS)); // or slide colour bar up
    }
  }
}

void whitePeak(int band) {
  int xStart = NUM_BANDS - BAR_WIDTH * band-1; // left to right
  int peakHeight = TOP - peak[band] - 1;
  if (peakHeight < TOP-1) { // Don't draw if at bottom
    for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
      matrix->drawPixel(x, peakHeight, CHSV(0,0,200));
    }
  }
}


