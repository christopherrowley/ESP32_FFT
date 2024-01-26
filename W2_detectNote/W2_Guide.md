# Workshop Guide!

Welcome to the music note analyzer workshop! In this workshop, we build aim to build a device that provides the information necessary to tune an instrument.

## Goals
In this workshop, we hope to develop a conceptual understanding of how the frequencies of sound waves map to musical notes. Each instrument obtains its unique sound by producing sound waves at many frequencies. We will apply our knowledge of the Fourier Transform to uncover the dominant frequency in a sound wave, which ends up being the musical tone we perceive the sound to be at. 

From this, we will compare this frequency to a known mathematical model, to determine how this frequency aligns with the primary music notes (i.e. A, A#, B, etc). Primarily, we are looking to convince ourselves that it is possible to take in any incoming sound signal, and break it down into its frequency components. 

## Theory

### Frequency and Music Notes
We will use the "C" note as an example. If we create a sinusodal wave that oscillates as a specific frequency, we get two spikes as our result from the Fourier transform (FT).  

![Linked across a row](../images/middleC.png)
In this case, we created a 261.6 Hz wave, and get two peaks following the FT, at -261.6 Hz and +261.6 Hz. The negative values are a consequence of the mathematics, and aren't interpretable for sound. For this exercise, we will ignore them and turn our focus to values above 0. 

This is great! We can determine the frequency content of an incoming sound wave, but how does this map to musical notes? 

We know that the frequency of musical notes changes exponentially, with base 2. We also know that the notes repeat every octave, with 12 different notes within an octave. If we start at some musical note f<sub>0</sub>, we can determine the frequency of a musical note 'n' notes away using the following formula:

<p align="center">f<sub>n</sub> = f<sub>0</sub>* 2<sup>(n/12)</sup> </p>

The means that the frequency of each note varies by a factor of  2<sup>(1/12)</sup> or ~1.05946. If we know that note 'C' in Octave 0 has frequency 16.35 Hz, we can use this as f<sub>0</sub> to map to any other music note. For example, we calculate that the plotted 'Middle C' note is 49 musical note steps away from this base note. 

If we want to see what this looks like as a plot: 
![Linked across a row](../images/NoteStep_linear.png)
From this, we can clearly see the exponential change of steps in frequency. If we take log2 of both sides of the equation, then we should expect a linear result. If we plot that, we see:

![Linked across a row](../images/NoteStep_log.png)

And this is more so how we percieve a change in musical tones, which means our ears are senstive to sound waves in a non-linear manner, even if we percieve them linearly.

### Music Instruments
To obtain a better appreciation of what separates a 'C' chord in piano and guitar, we will look at the Fourier transform of each. For a guitar C major chord we have:

![Linked across a row](../images/GuitarC.png)

The input audio looks quite random, but from the FT of that signal, we can see discrete peaks. If we look into the values of these peaks we see that they align with different musical notes! We perceive this to be a 'C' note, because it is the largest peak in the FT. We won't go into the music theory of what other notes best pair to make a nice sounding 'chord'. 

Let us compare the frequency spectrum from a guitar to one from a piano. The piano note is one octave lower to help us visualize differences. 

![Linked across a row](../images/GuitarVsPiano.png)
We have a primary difference of 1 octave in the C notes. But you can see that despite the piano being a lower note, it has more high frequency content that provides its characteristic sound. 

Hopefully this helps illustrate the usefulness of the fourier transform. Now we have a quantitative metric to be able to compare two signals, that provides meaningful interpretation. 

### Bringing It Together
Hopefully from the above, we can see:
- musical instruments differ in sound based on the contributions of additional frequencies
- the musical note we 'hear' is the dominant frequency in the wave
- we can use a mathematical formula to determine the frequency of any musical note

We can make our 'note detector' by pre-computing the frequency values of all the notes in a frequency range of interest (~10-5000 Hz), and then compare our detected frequency, to find how close it is to the theoretical frequency.

## Hardware setup
To begin, you should set your hardware up as illustrated in the README.md file in the main directory. 

Before Plugging into the breadboard, view the following diagrams to know how the wiring works on a breadboard.
![Linked across a row](horizontal-rows.png)

Due to the size of the ESP32, you will need to have it straddle two breadboards. 

![Linked across a row](horizontal-withIC.png)

The LED matrix has 3 sets of wires as seen in the picture below:

![LED wiring](LED_wires.png)

Since we are working with a single matrix, we don't need to worry about the 'increase voltage wire' or the '3PIN Male Connector'. You want to connect to the Data Input - Female Connector. This is easy to distinguish as it is the only one that you should be able to insert a jumper cable into. 

With this working knowledge, we can connect parts with jumper cables by plugging into the same row without crossing the gutter. The following connections need to be made:
- Connect the data line from the LED matrix to D5 on the ESP32
- Connect the VIN line from the LED matrix to VIN on the ESP32
- Connect the GND line from the LED matrix to GND next to VIN on the ESP32 (should be right next to it)
- Connect the data line from the microphone to D35 on the ESP32
- Connect the VIN line from the microphone to 3V3 on the ESP32
- Connect the GND line from the microphone to GND next to the 3V3 on the ESP32 (should be right next to it)
- Connect the ESP32 to the computer using the microUSB cord. This provides power to the entire system.

## Coding Steps:
1. Start by uploading W1_spectrumAnalyzer.ino to the ESP32 with all the default values. You may need to check that you have the right COM port selected. Hold down the boot button on the ESP32 while uploading. 
2. If the upload was successful, and all the parts are properly connected, then you should see the lights working now if you are in a noisy environment. If not, make some noises to test it! 
3. At the top of the arduino file, we have set up a user configuration zone. The two main variables of interest are the `AMPLITUDE` and `NOISE`. The amplitude sets the value for what signal from the microphone reaches the top of the LED. The noise variable defines the bottom. If you are in a noisy location, increasing the `NOISE` variable can help. You can guess and check... Or we can have the ESP32 print us the data values from the microphone to the Serial Monitor. 
4. Go to line ~86 and look for `//Serial.println(vReal[5]);`. Remove the `//` to uncomment the line, and the re-upload the sketch to the ESP32. Go to the serial monitor and watch the values popping up on the screen. Is there an average value at the low end that seems consistent? Take that value, add 20-50, and set that as the `NOISE` variable. Upload that update to the ESP32 and see how it now responds to noise at the bottom (quiet) end. Repeat as necessary so that the lights mostly start to turn on when you deliberately make noise.
5. Now we want to set the peak. First, lets comment out the line from before, by adding `//` back in front of it. Now jump to line ~125 and look for: `// if (band == 3) Serial.println(barHeight);` Uncomment this line, and reupload to ESP32. Just like in the last step, we are watching the serial monitor, but now, looking for the maximum values we see. This time, we want to make some noise (but not too much!), so that we know what we want the value to be for full illumination. Once you find a value, set that as the `AMPLITUDE` variable. Upload that update to the ESP32 and see how it now responds to noise at the top (loud) end. Repeat as necessary so that the lights respond the way you would like them to. You may want to go back to step 4 if you feel like the `NOISE` could be further adjusted. It doesn't need to be perfect, as the level of noise in the room will be changing. You just want it set well enough that you can see how it responds to new incoming noise. 

## Testing Steps:

The hard part is done and now the fun begins! We have the hardware working, and the code configured for our setup, now we want to put it to the test. We have a few audio files to test:

1. We can start by playing the `middle C` file. A single note (created synthetically of course) will consist of a sine wave at a specific frequency. As we play this note, we should see a single bar light up. 
2. Ok, so a single bar could have been a fluke. To test this some more, lets play a `frequency sweep` file. This should have the sound going from a low frequency to a high frequency. If our theory works, then we should see a bar light up on one side, and move across the LED matrix as it moves to a higher pitch.
3. Enough of the synthetic waves! What about real audio? What gives a guitar the sound of a guitar, despite playing the same musical note as a piano? To better visualize this, we may want to adjust the frequencies that the bars of our LED correspond to. While we can hear from ~20- 20,000 Hz, most of the instrument and vocal frequencies are from the 20-10,000 Hz range. The first 5 octaves are all below 1,000 Hz. To better see how we can pick up small differences in notes, copy the following into your code from lines ~98 to ~114, replacing the `if` statements that are there:
```   if (i<=3 )           bandValues[0]  += (int)vReal[i];
      if (i>3   && i<=4  ) bandValues[1]  += (int)vReal[i];
      if (i>4   && i<=5  ) bandValues[2]  += (int)vReal[i];
      if (i>5   && i<=6  ) bandValues[3]  += (int)vReal[i];
      if (i>6   && i<=7  ) bandValues[4]  += (int)vReal[i];
      if (i>7   && i<=9  ) bandValues[5]  += (int)vReal[i];
      if (i>9   && i<=11  ) bandValues[6]  += (int)vReal[i];
      if (i>11   && i<=14  ) bandValues[7]  += (int)vReal[i];
      if (i>14   && i<=18  ) bandValues[8]  += (int)vReal[i];
      if (i>18   && i<=22  ) bandValues[9]  += (int)vReal[i];
      if (i>22   && i<=28  ) bandValues[10]  += (int)vReal[i];
      if (i>28   && i<=35  ) bandValues[11]  += (int)vReal[i];
      if (i>35   && i<=44  ) bandValues[12]  += (int)vReal[i];
      if (i>44   && i<=55  ) bandValues[13]  += (int)vReal[i];
      if (i>55   && i<=69  ) bandValues[14]  += (int)vReal[i];
      if (i>69             ) bandValues[15]  += (int)vReal[i];

```
3. Continued... This changes the bins to be sensitive to frequencies between 100 and 3,000 Hz (2nd to into the 7th Octave). Before you upload this change, try talking into your microphone and see how the bars are clustered together. How does that change with the new frequency range of the bars? 
4. Now we want to do what we set out to do in 3. Lets play and compare a C note from a guitar and a piano. How do the bars look?
