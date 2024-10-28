# Workshop Introduction Guide!

Before we get into the fun coding challenges, we want to make sure everyone has enough background knowledge.
For this, we need to get an understanding of what an LED matrix is, and how do we control it. 

## Goals
Here we will get an understanding of Arduino programming, the FastLED programming package, as well as the RGBMatrix Library



## Basics
We break up arduino code into 3 main blocks:
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

## Setup and Loop functions

To convince ourselves that setup runs only once, put the first line in setup, and the second line in loop, and check the serial monitor. Leave the Serial.begin and delay lines from before. 
```Serial.println("Sunning the setup function");```

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

## LED Matrix
Now that we have the basics covered, lets take a jump and add the LED matrix. 

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








