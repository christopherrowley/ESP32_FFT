# Workshop Introduction Guide!

Before we get into the fun coding challenges, we want to make sure everyone has enough background knowledge.
For this, we need to get an understanding of what an LED matrix is, and how do we control it. 

## Goals
Here we will get an understanding of the FastLED programming package, as well as the RGBMatrix Library



## Coding Steps:
1. Start by uploading W1_spectrumAnalyzer.ino to the ESP32 with all the default values. You may need to check that you have the right COM port selected. Hold down the boot button on the ESP32 while uploading. 




### To add FastLED matrix option:
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, kMatrixWidth, kMatrixHeight,
  NEO_MATRIX_TOP        + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS       + NEO_MATRIX_ZIGZAG +
  NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS);









