// Code from https://github.com/NikodemBartnik/Small-Projects/blob/main/Guitar%20Tuner/Code/guitar_tuner.ino 

// Looking to make a tuner:
/*
Frequencies are on a logarithmic scale, because they double with each octave. 
Hence, you cannot calculate semitones with simple interpolation like this; the correct formula is:

f = f0 * 2 ^ (n / 12)

...where:

    f is the frequency you want

    f0 is the frequency of your reference pitch (e.g. a = 440 Hz)

    n is the number of semitones above or below the reference pitch (e.g. c' is three semitones above a, so that's n = 3)

So let's do it for A#: f = 440 Hz, n = 1, so f = 440 Hz * 2 ^ (1 / 12), which is approximately 466.1637615180899 Hz.

Every semitone is 1.05916... (21/12) times higher frequency that the semitone below is. 
So, you move up the scale be multiplying frequencies, not adding.*/



/* Matlab code:
LED_height = 16;


nOctaves = 8;
f_0 = 13.75;
musicalTones = f_0 * 2.^((0:(12*nOctaves-1))/12)';
toneNames = ["A";"A#";"B";"C";"C#";"D";"D#";"E";"F";"F#";"G";"G#"]; %
%  If you want to know the octave, include this: + string(1:(3+nOctaves-1));
toneNames = repmat(toneNames, 8,1);


inputFreq = 615; % hz % between G# and A
% To make proportional bins, do log2 transform
f_trans = log2(inputFreq/f_0) *12 +1; % need the plus 1 for 1 indexing

% We then want the remainder after division with 12 to remove octave
% scaling
f_transNorm = rem(f_trans, 12);


% Now we know we need to divide it up into 2 for 16 bar split
bar2 = round(rem(f_transNorm,1) *LED_height);
bar1 = LED_height - bar2;

% clear LED board, then assign new light:
bar1Loc = floor(f_transNorm); % add one for 1 indexing
if bar1Loc == 0
    bar1Loc = 12;
end
if bar1Loc == 12 % wrap around.
    bar2Loc = 1;
else
    bar2Loc = bar1Loc+1;
end

figure; scatter( [ bar1Loc,bar2Loc], [bar1, bar2]); xlim([1 12]); ylim([0 16])
xticks(1:12); xticklabels(["A","A#","B","C","C#","D","D#","E","F","F#","G","G#"])


*/



void DisplayNoteAndBar(double frequency){

int ArrayWithNoteAndBarWidth[2];
DetectClosestNote(frequency, ArrayWithNoteAndBarWidth);

char note = NoteNumberToString(ArrayWithNoteAndBarWidth[0]);
Serial.println(frequency);

    if(ArrayWithNoteAndBarWidth[1] < 64){
      analogWrite(TOO_LOW, map(ArrayWithNoteAndBarWidth[1], 0, 64, 255, 0));
      digitalWrite(PERFECT, LOW);
      digitalWrite(TOO_HIGH, LOW);
    }else if(ArrayWithNoteAndBarWidth[1] > 64){
      analogWrite(TOO_HIGH, map(ArrayWithNoteAndBarWidth[1], 64, 128, 0, 255));
      digitalWrite(PERFECT, LOW);
      digitalWrite(TOO_LOW, LOW);
    }else{
      digitalWrite(PERFECT, HIGH);
      digitalWrite(TOO_LOW, LOW);
      digitalWrite(TOO_HIGH, LOW);
    }

}


int* DetectClosestNote(double frequency, int *arr){
  if(ukuleleMode == false){
  if(InRange(frequency, 62, 102)){
    arr[0] = 6;
    arr[1] = map(frequency, 62, 102, 1, 128);
  }else if(InRange(frequency, 100, 120)){
    arr[0] = 5;
    arr[1] = map(frequency, 100, 120, 1, 128);
  }else if(InRange(frequency, 120, 165)){
    arr[0] = 4;
    arr[1] =  map(frequency, 127, 167, 1, 128);
  }else if(InRange(frequency, 165, 210)){
    arr[0] = 3;
    arr[1] = map(frequency, 176, 216, 1, 128);
  }else if(InRange(frequency, 210, 290)){
    arr[0] = 2;
    arr[1] = map(frequency, 217, 277, 1, 128);
  }else if(InRange(frequency, 290, 380)){
    arr[0] = 1;
    arr[1] = map(frequency, 290, 370, 1, 128);
  }
  }else{
    //notes for ukulele
   if(InRange(frequency, 420, 460)){
    arr[0] = 5;
    arr[1] = map(frequency, 420, 460, 1, 128);
  }else if(InRange(frequency, 310, 350)){
    arr[0] = 4;
    arr[1] = map(frequency, 310, 350, 1, 128);
  }else if(InRange(frequency, 232, 292)){
    arr[0] = 3;
    arr[1] =  map(frequency, 232, 292, 1, 128);
  }else if(InRange(frequency, 372, 412)){
    arr[0] = 2;
    arr[1] = map(frequency, 372, 412, 1, 128);
  }
  }
}

  bool InRange(double frequency, int low_limit, int high_limit){
    if(frequency < high_limit && frequency > low_limit){
      return true;
    }else{
      return false;
    }
  }


  char NoteNumberToString(int note_number){
  
    switch(note_number){
        case 1:
            digitalWrite(STRING_E1, HIGH);
            digitalWrite(STRING_B, LOW);
            digitalWrite(STRING_G, LOW);
            digitalWrite(STRING_D, LOW);
            digitalWrite(STRING_A, LOW);
            digitalWrite(STRING_E2, LOW);
            
            return 'E';
        break;
        case 2:
            digitalWrite(STRING_E1, LOW);
            digitalWrite(STRING_B, HIGH);
            digitalWrite(STRING_G, LOW);
            digitalWrite(STRING_D, LOW);
            digitalWrite(STRING_A, LOW);
            digitalWrite(STRING_E2, LOW);
            return 'B';
        break;
        case 3:
            digitalWrite(STRING_E1, LOW);
            digitalWrite(STRING_B, LOW);
            digitalWrite(STRING_G, HIGH);
            digitalWrite(STRING_D, LOW);
            digitalWrite(STRING_A, LOW);
            digitalWrite(STRING_E2, LOW);
            return 'G';
        break;
        case 4:
            digitalWrite(STRING_E1, LOW);
            digitalWrite(STRING_B, LOW);
            digitalWrite(STRING_G, LOW);
            digitalWrite(STRING_D, HIGH);
            digitalWrite(STRING_A, LOW);
            digitalWrite(STRING_E2, LOW);
            return 'D';
        break;
        case 5:
            digitalWrite(STRING_E1, LOW);
            digitalWrite(STRING_B, LOW);
            digitalWrite(STRING_G, LOW);
            digitalWrite(STRING_D, LOW);
            digitalWrite(STRING_A, HIGH);
            digitalWrite(STRING_E2, LOW);
            return 'A';
        break;
        case 6:
            digitalWrite(STRING_E1, LOW);
            digitalWrite(STRING_B, LOW);
            digitalWrite(STRING_G, LOW);
            digitalWrite(STRING_D, LOW);
            digitalWrite(STRING_A, LOW);
            digitalWrite(STRING_E2, HIGH);
            return 'e';
        break;
    }
}