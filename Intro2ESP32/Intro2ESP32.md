# Workshop Introduction Guide!

Before we get into the fun coding challenges, we want to make sure everyone has enough background knowledge.
For this, we need to get an understanding of the arduino programming environment, some basic coding skills, and then get into what an LED matrix is, and how do we control it. 

## Goals
Here we will get an understanding of Arduino programming, the FastLED programming package, as well as the RGBMatrix Library


## Setup Arduino IDE for ESP32 
If this is your first time connecting an ESP32 board, you need to add it to your ArduinoIDE.

    1. Go to `File->Preferences->Additional Board Manager URLs`
    2. Enter in: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    3. click "Ok" to close out of preferences. It should begin to add something.
    4. Go to `Tools->Board->Board Manager`
    5. A panel should pop up, and search "ESP32"
    6. Download the version by Espressif Systems (version 2.0.14 works)
    7. Now to set the board, go to `Tools->Board->esp32->"ESP32 Dev Module"`

If your computer is not recognizing the ESP32 as being connected, you may also need the ESP32 driver: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers (install instructions in the Release Notes text file of the download). Another possible source of this error is that the wrong "COM" port is selected from the Tools dropdown menu. 

## Basics
Arduino code needs to be broken up into 3 main blocks. You will see this structure when you start any new script:
1. Define all the necessary variables that the code needs to run (e.g. pin locations for connected hardware)
2. The 'setup' function - which tells the chip what to run at the very beginning to get things set up for the main function. This only runs once at boot or reset.
3. The 'loop' function - which contains the code that you plan to run continuously 


## Datatypes

Arduino has several primary data types to work with:

1. Integer (int): Used for whole numbers. Range: -32,768 to 32,767.

2. Unsigned Integer (unsigned int): Similar to int but only for non-negative numbers. Range: 0 to 65,535.

3. Long (long): For larger whole numbers. Range: -2,147,483,648 to 2,147,483,647.

4. Unsigned Long (unsigned long): Same as long but only positive. Range: 0 to 4,294,967,295.

5. Float (float): For floating-point numbers (decimals). Precision: 6-7 decimal digits.

6. Double (double): It's essentially the same as float on most Arduino boards, not offering double-precision.

7. Byte (byte): Stores an 8-bit unsigned number, 0 to 255.

8. Char (char): Stores a single character. A 'string' is made up of multiple characters.

9. Boolean (bool): Represents true or false.

## Hello World

We need to start setting the arduino internal clock in the setup() function. The following line will set the rate to 9600:

```Serial.begin(9600);```

The we need to put something in the loop() function. The following line will print the contents in the brackets into the serial monitor. 

```Serial.println("Hello World!");```

We will also add a delay. The delay(time) function pauses the code for the amount of 'time' defined in milliseconds. Here, lets add a 3 second delay with the following code in the loop():

```delay(1000);```

Next press the Upload button. Once uploaded, open the 'Serial Monitor' which can be found in the Tools drop down menu. You should see "Hello World!" appear in the serial monitor, every 3 seconds.

** If the serial monitor isn't printing properly, check the Baud rate in the Tools drop down and make sure it matches the value chosen for the serial monitor.

## Setup and Loop functions

To convince ourselves that setup runs only once, put the first line in setup, and the second line in loop, and check the serial monitor. Leave the Serial.begin and delay lines from before. 
```Serial.println("Running the setup function");```

```Serial.println("Running the loop function");```

## Defining variables

We can set variables to take on values before the setup and loop functions. We might want to do this if there are things that we don't want to change, or to prefill variables with values. We can try this out by defining two variables and printing their summation. At the top of the script insert the two lines of code:
```
int val1 = 2;
int val2 = 3; 
int resultNumber = 0;
```
We can leave the setup() function. In the loop() function, lets start it by printing the result:
```Serial.println(resultNumber);```

Then lets increment the resultNumber by val1+val2. This can be done with:

```resultNumber += val1 + val2;```

Insert the delay code from before so that the serial monitor doesn't print values too quickly. Upload the code to the chip and check the result in the serial monitor. If done correctly, it should count by 5.

## ESP32 LED Blink
The ESP32 dev board has a LED built in. To use it, we need to 'define' it at the beginning of the code:
```#define LED 2```
Then in the setup() function, include this:
```pinMode(LED,OUTPUT);```
This tells the board that it is connected to interface pin 2, and that this pin is used to output (not take data in, like a microphone might). We can then make the LED blink by putting the following in the loop() function:

```
delay(500);
digitalWrite(LED,HIGH);
delay(500);
digitalWrite(LED,LOW);
```

digitalWrite sets the pin defined by the first variable to either a high (5 or 3.3 Volt) state, or a low (0 Volt) state. In this case, it will turn the LED on (high) or off (low).

## For loops
Conditional statements are very common in programming. This tells the computer to do something only under certain circumstances. We can modify the above LED example to use a 'for' loop instead. The following code will do 5 blinks, before taking a longer (1.5 second) 'off' period. Clear your loop() function and insert the following code:

```
for (int i = 0; i < 5; i++) {
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED on
  delay(500);                       // Wait for half a second
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED off
  delay(500);
}
delay (1000);
```                      

## If and Else loops
These are other conditional statements that cause code to be executed when certain conditions are met. Let's modify the above example so that the 5th blink is 1 second long and the others are still half a second:
```
for (int i = 0; i < 5; i++) {
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED on
 if ( i == 5) { // Only on the 5th blink
   delay(1000); // Wait for a full second
 } else { // for all other blinks
   delay(500); // Wait for half a second
 }                    
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED off
  delay(500);
}
delay (1000);
```

#################################################################################
# LED Matrix
Now that we have the basics covered, lets take a jump and add the LED matrix. First we need to wire it up. 

## Hardware setup

Before Plugging into the breadboard, view the following diagrams to know how the wiring works on a breadboard.

![Linked across a row](../images/horizontal-rows.png)

Due to the size of the ESP32, you will need to have it straddle two breadboards. 

![Linked across a row](../images/horizontal-withIC.png)

The LED matrix has 3 sets of wires as seen in the picture below:

![LED wiring](../images/LED_wires.png)

Since we are working with a single matrix, we don't need to worry about the 'increase voltage wire' or the '3PIN Male Connector'. You want to connect to the Data Input - Female Connector. This is easy to distinguish as it is the only one that you should be able to insert a jumper cable into. 

With this working knowledge, we can connect parts with jumper cables by plugging into the same row without crossing the gutter. The following connections need to be made:
- Connect the data line from the LED matrix to D5 on the ESP32
- Connect the VIN line from the LED matrix to VIN on the ESP32
- Connect the GND line from the LED matrix to GND next to VIN on the ESP32 (should be right next to it)
- Connect the data line (OUT) from the microphone to D35 on the ESP32
- Connect the VCC line from the microphone to 3V3 on the ESP32
- Connect the GND line from the microphone to GND next to the 3V3 on the ESP32 (should be right next to it)
- Connect the ESP32 to the computer using the microUSB cord. This provides power to the entire system.

![Circuit for using a microphone](../images/esp32Circuit.png)

## Download packages
These are needed to help us control the LED with less effort, and for later on in the workship.
From the Arduino library manager, you will need:
    - FastLED Neomatrix (by Marc Merlin v1.1.0)
    - arduinoFFT library (by Enrique Condes v1.6.2)
    - Adafruit_GFX // graphics support for matrix
    
    - To load the Arduino FFT (or other) library:

        - In the IDE menu, select `Sketch->Include Library-> Manage Libraries->search for 'FFT'` 
                The correct version is https://github.com/kosme/arduinoFFT

### To add FastLED matrix option:
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, kMatrixWidth, kMatrixHeight,
  NEO_MATRIX_TOP        + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS       + NEO_MATRIX_ZIGZAG +
  NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS);


## LED Matrix Basics:
Draw a pixel in solid white

`matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));`

Fill the screen with green

`matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 7, 0));`

Draw a box in yellow

`matrix.drawRect(0, 0, 32, 16, matrix.Color333(7, 7, 0));`

Draw an 'X' in red
```
matrix.drawLine(0, 0, 31, 15, matrix.Color333(7, 0, 0));
matrix.drawLine(31, 0, 0, 15, matrix.Color333(7, 0, 0));
```
Draw a blue circle

`matrix.drawCircle(7, 7, 7, matrix.Color333(0, 0, 7));`

Fill a violet circle

`matrix.fillCircle(23, 7, 7, matrix.Color333(7, 0, 7));`

Fill the screen with 'black'

`matrix.fillScreen(matrix.Color333(0, 0, 0));`


## Writing on the screen:
There is a matrix print option which is similar to serial print.  But we first need to set where our cursor will be. 
```
// draw some text!
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
 
  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('1');
  matrix.setTextColor(matrix.Color333(7,4,0)); 
  matrix.print('6');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0)); 
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(0,7,0));  
  matrix.print('2');
 
  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color333(0,7,7)); 
  matrix.print('*');
  matrix.setTextColor(matrix.Color333(0,4,7)); 
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7)); 
  matrix.print("B");
  matrix.setTextColor(matrix.Color333(7,0,4)); 
  matrix.print("*");
```

## Scrolling Text
We can achieve scrolling text by using the above combined with a 'for' loop to navigate across. We will need to set a scroll speed which effectively acts as a delay:

`int scrollSpeed 150 // milliseconds`

Let's also set the message we want as a string"
`char LEDmessage[] = { 'H','e','l','l','o' };  // we have to enter the word as individual characters!`  

We need some other definitions related to the hardware:
```
#define MATRIXPIN           5   // LED strip data
#define MATRIX_TILE_WIDTH   16 // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_TILE_HEIGHT  16 // height of each matrix
#define NUMMATRIX (MATRIX_TILE_WIDTH*MATRIX_TILE_HEIGHT)
#define COLOR_ORDER     GRB           // If colours look wrong, play with this
#define CHIPSET         WS2812B       // LED strip type
CRGB matrixleds[NUMMATRIX];
uint8_t matrix_brightness = 255;
int x    = matrix.width(); // Value to keep track of cursor position
int y    = 5; // Helps to center text 
int stringPixLength = LEDmessage.length() * 8 // Assumes each letter may be ~ 8 pixels wide with a space

// Color definitions - you can pick the one you like most! I will continue with WHITE in setup()
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define GREEN 0x07E0
#define WHITE 0xFFFF
```


We need to set up the LED matrix. So in the setup() function include the following:
```
Serial.begin(115200);
FastLED.addLeds<CHIPSET, MATRIXPIN, COLOR_ORDER>(matrixleds, NUMMATRIX).setCorrection(TypicalLEDStrip);
matrix->begin();
matrix->setBrightness(matrix_brightness);
matrix->setTextWrap(false);
matrix->setFont(); // size 1 == 8 pixels high, 2 = 16 &FreeSans9pt7b 
matrix->setTextSize(1);
matrix->setTextColor( WHITE ); 
```

The magic happens in the loop() function. It is best to start by clearing the matrix. We will make the text scroll by continually modifying x. 
```
FastLED.clear();
matrix.setCursor(x, y);
matrix.print( LEDmessage );
x-- // Decrease x by 1 for next loop to move text to left
if(x < -stringPixLength) {
  x = 0; // reset word.
} 
matrix.show();
delay(scrollSpeed);
```



## Snake of increasing length
We can start a snake with a single pixel that then moves through the matrix. After completing, it could start over with a length of 1 pixel longer. 






