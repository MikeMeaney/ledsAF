#include "AffectedLEDs.h"
#include "neopixel.h"
#include <math.h>
//----------------- Button ----------------//

uint8_t pin = 6;
uint8_t b1 = 4;
uint8_t b2 = 5;
uint8_t b3 = 3;
uint8_t b4 = 7;

// start with pixel output not floating to avoid random pixels turning on during power-up
STARTUP(pinMode(pin, INPUT_PULLDOWN);)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, pin, PIXEL_TYPE);

AffectedLEDs::AffectedLEDs(){

}

void AffectedLEDs::begin(){
    strip.begin();
    strip.show();

   
    pinMode(b1, INPUT_PULLUP);
    pinMode(b2, INPUT_PULLUP);
    pinMode(b3, INPUT_PULLUP);
    pinMode(b4, INPUT_PULLUP);
}

void AffectedLEDs::begin(int i){
    //allow for alternate pin assignements 
    if(i == 1 || i == 0){
        pin = 17;
        b1 = 1;
        b2 = 2;
        b3 = 3;
        b4 = 4;
    }

    strip.begin();
    strip.setPin(pin);
    strip.show();

    pinMode(b1, INPUT_PULLUP);
    pinMode(b2, INPUT_PULLUP);
    pinMode(b3, INPUT_PULLUP);
    pinMode(b4, INPUT_PULLUP);
}

/* If you solder on a 12th LED (WS2812B), then you'll want to call
 * this function b.setNumLeds(12); after you call b.begin(); Any number of
 * LEDs can also be soldered onto the output of the button.
 */
void AffectedLEDs::setNumLeds(uint8_t i) {
    strip.updateLength(i);
}

void AffectedLEDs::ledOn(uint8_t i, uint8_t r, uint8_t g, uint8_t b){
    //i-1 shifts the location from human readable to the right index for the LEDs
    if(i == 12 && strip.getNumLeds() < 12){
        strip.setPixelColor(0, strip.Color(r,g,b));
        strip.setPixelColor(10, strip.Color(r,g,b));
    }
    else{
        strip.setPixelColor(i-1, strip.Color(r,g,b));
    }
    strip.show();
}

void AffectedLEDs::smoothLedOn(float i, uint8_t r, uint8_t g, uint8_t b){
    //uint8_t intI = lrintf(i);
    //Serial.print("intI: ");
    //Serial.println(intI);

    //float differ = i-(float)intI + 0.5;
    //Serial.print("differ: ");
    //Serial.println(differ);

    float tempI;
    float differ = modff(i, &tempI);
    uint8_t intI = (uint8_t)tempI;


    // checks to see if it's reeeeally close to being an integer
    //if(abs(differ) < 0.01){
      // intI-1 shifts the location from human readable to the right index for the LEDs
    //  strip.setPixelColor(intI-1, strip.Color(r,g,b));
    //  Serial.println("tripped int check");
    //}
    //else {
      // diff > 0 means that it's closer to the lower one
      float differ1 = 1.0-differ;
      //differ1 = logf(differ1);
      //differ = logf(differ);
      if(differ > 0.5){
        differ1 /= 2;
        //strip.setPixelColor(intI-2, strip.Color((int)(differ1*r),(int)(differ1*g),(int)(differ1*b)));
        strip.setPixelColor(intI-1, strip.Color((int)(differ1*r),(int)(differ1*g),(int)(differ1*b)));
        strip.setPixelColor(intI, strip.Color((int)(differ*r),(int)(differ*g),(int)(differ*b)));
      }
      else {
        differ /= 2;
        //strip.setPixelColor(intI-2, strip.Color((int)(differ*r),(int)(differ*g),(int)(differ*b)));
        strip.setPixelColor(intI-1, strip.Color((int)(differ1*r),(int)(differ1*g),(int)(differ1*b)));
        strip.setPixelColor(intI, strip.Color((int)(differ*r),(int)(differ*g),(int)(differ*b)));
      }
    //}
    strip.show();
}

void AffectedLEDs::ledOff(uint8_t i){
    ledOn(i,0,0,0);
}

void AffectedLEDs::allLedsOff(){
    for(int i = 0; i<PIXEL_COUNT; i++){
            strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
}

void AffectedLEDs::allLedsOn(uint8_t r, uint8_t g, uint8_t b){
    for(int i = 0; i<PIXEL_COUNT; i++){
            strip.setPixelColor(i, strip.Color(r, g, b));
    }
    strip.show();
}

uint8_t AffectedLEDs::buttonOn(uint8_t i){
    if(b1 == 4){
        return !digitalRead(i+3);
    }
    else {
        return !digitalRead(i);
    }
}

uint8_t AffectedLEDs::allButtonsOn(){
    if(!digitalRead(b1) && !digitalRead(b2) && !digitalRead(b3) && !digitalRead(b4)) {
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t AffectedLEDs::allButtonsOff(){
    if(digitalRead(b1) && digitalRead(b2) && digitalRead(b3) && digitalRead(b4)) {
        return 1;
    }
    else {
        return 0;
    }
}

uint32_t AffectedLEDs::wheel(uint8_t WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void AffectedLEDs::rainbow(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            uint8_t idx = ((i * 256 / strip.numPixels()) + j) & 255;
            strip.setPixelColor(i, wheel(idx));
        }
        strip.show();
        delay(wait);
    }
}

void AffectedLEDs::advanceRainbow(uint8_t amount, uint8_t wait) {
    uint16_t i;
    static uint16_t j = 0;

    if (amount == 0) {
        j = 0;
        return;
    }

    if (j < 256) {
        j += amount;
    } else {
        j = 0;
    }

    for(i=0; i< 12; i++) {
        uint8_t idx = ((i * 256 / strip.numPixels()) + j) & 255;
        strip.setPixelColor(i, wheel(idx));
    }
    strip.show();
    if (wait > 0) delay(wait);
}

//----------- Affected LED Effects
void AffectedLEDs::ALshowStrip(){
    strip.show();
}

void AffectedLEDs::ALsetAll(byte red, byte green, byte blue) {
  for(int i = 0; i < strip.numPixels(); i++ ) {
    ALsetPixel(i, red, green, blue); 
  }
  ALshowStrip();
}

void AffectedLEDs::ALsetPixel(int Pixel, byte red, byte green, byte blue) {
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void AffectedLEDs::ALmeteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay)
{
    allLedsOn(0,0,0);
  
    for(int i = strip.numPixels()+strip.numPixels(); i > 0; i--) {
    
    
    // fade brightness all LEDs one step
    for(int j=0; j<strip.numPixels(); j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        ALfadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <strip.numPixels()) && (i-j>=0) ) {
        ALsetPixel(i-j, red, green, blue);
      } 
    }
   
    ALshowStrip();
    delay(SpeedDelay);
  }
}

void AffectedLEDs::ALbouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;
  
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
  
  for (int i = 0 ; i < BallCount ; i++) {   
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0; 
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2); 
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
  
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
  
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (strip.numPixels() - 1) / StartHeight);
    }
  
    for (int i = 0 ; i < BallCount ; i++) {
      ALsetPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
    }
    
    ALshowStrip();
    allLedsOn(0,0,0);
    Particle.process();
  }
}

void AffectedLEDs::ALfadeInOut(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    allLedsOn(r,g,b);
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    allLedsOn(r,g,b);
  }
}

void AffectedLEDs::ALTwinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  ALsetAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     ALsetPixel(random(strip.numPixels()),red,green,blue);
     ALshowStrip();
     delay(SpeedDelay);

     if(OnlyOne) { 
           allLedsOff(); 
     }
   }
  
  delay(SpeedDelay);
}

void AffectedLEDs::ALfadeToBlack(int ledNo, byte fadeValue) {
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
}

void AffectedLEDs::ALfire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[137];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < strip.numPixels(); i++) {
    cooldown = random(0, ((Cooling * 10) / strip.numPixels()) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= strip.numPixels() - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < strip.numPixels(); j++) {
    ALsetPixelHeatColor(j, heat[j] );
  }

  ALshowStrip();
  delay(SpeedDelay);
}



void AffectedLEDs::ALsetPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    ALsetPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    ALsetPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    ALsetPixel(Pixel, heatramp, 0, 0);
  }
}

//----------- --------------------
void AffectedLEDs::setBrightness(uint8_t brightness) {
    strip.setBrightness(brightness);
}

uint8_t AffectedLEDs::getBrightness() {
    return strip.getBrightness();
}

int AffectedLEDs::getBPM() {
    return bpm;
}

void AffectedLEDs::setBPM(int beats_per_minute) {
    bpm = beats_per_minute;
}

void AffectedLEDs::playSong(String song){
    char inputStr[song.length()];
    song.toCharArray(inputStr,song.length());

    // Uncomment for debugging, will affect BPM.
    // Serial.println(inputStr);

    char *note = strtok(inputStr,",");
    char *duration = strtok(NULL,", \n");

    while (duration != NULL) {
        playNote(note,String(duration).toInt());
        // Uncomment for debugging, will affect BPM.
        // Serial.printlnf("note: %s dur: %s", note, duration);
        note = strtok(NULL,",");
        duration = strtok(NULL,", \n");
    }
}

/*
 * Supports all notes, sharp and flat.  Sharp can be 's','S' or '#'. Flat can be 'b' or 'B'.
 * Octaves supported are 0-9, however only part of 0 is audible.
 * Rest notes require a duration just like regular notes and can be 'r','R',"rest" or "REST".
 * All song strings should end with ',',' ' or '\n' or else the last digit in a multi-digit
 * duration will be truncated, i.e. 16 will become 1.
 *
 * // EXAMPLE USAGE
 * b.playSong("C0,8,CS0,8,D0,8,DS0,8,E0,8,F0,8,FS0,8,G0,8,GS0,8,A0,8,AS0,8,B0,8\n");
 * b.playSong("C1,8,CS1,8,D1,8,DS1,8,E1,8,F1,8,FS1,8,G1,8,GS1,8,A1,8,AS1,8,B1,8\n");
 * b.playSong("C2,8,Cs2,8,D2,8,Ds2,8,E2,8,F2,8,Fs2,8,G2,8,Gs2,8,A2,8,As2,8,B2,8\n");
 * b.playSong("C3,8,DB3,8,D3,8,EB3,8,E3,8,F3,8,GB3,8,G3,8,AB3,8,A3,8,BB3,8,B3,8\n");
 * b.playSong("C4,8,Db4,8,D4,8,Eb4,8,E4,8,F4,8,Gb4,8,G4,8,Ab4,8,A4,8,Bb4,8,B4,8\n");
 * b.playSong("c5,8,db5,8,d5,8,eb5,8,e5,8,f5,8,gb5,8,g5,8,ab5,8,a5,8,bb5,8,b5,8\n");
 * b.playSong("C6,8,C#6,8,D6,8,D#6,8,E6,8,F6,8,F#6,8,G6,8,G#6,8,A6,8,A#6,8,B6,8\n");
 * b.playSong("C7,8,R,8,D7,8,REST,8,E7,8,F7,8,R,8,G7,8,REST,8,A7,8,R,8,B7,8\n");
 * b.playSong("C8,16,CS8,16,D8,16,DS8,16,E8,16,F8,16,FS8,16,G8,16,GS8,16,A8,16,AS8,16,B8,16\n");
 * b.playSong("C9,32,CS9,32,D9,32,DS9,32,E9,32,F9,32,FS9,32,G9,32,GS9,32,A9,32,AS9,32,B9,32,C9,32,CS9,32,D9,32,DS9,32,E9,32,F9,32,FS9,32,G9,32,GS9,32,A9,32,AS9,32,B9,32\n");
 */
void AffectedLEDs::playNote(String note, int duration) {

    int noteNum = 0;
    int noteSharp = 0;
    int noteFlat = 0;
    int octave = 5;
    int freq = 256;

    int octIndex = note.length()-1; // octave is the last char
    String tempOctave = note.substring(octIndex, octIndex+1);
    octave = tempOctave.toInt();
    String tempNote = note.substring(0, octIndex);
    tempNote.toUpperCase();
    if (tempNote.length()==2) {
        if (tempNote.charAt(1)=='S' || tempNote.charAt(1)=='#') {
            noteSharp = 1;
        } else if (tempNote.charAt(1)=='B') {
            noteFlat = 1;
        }
    }

    // Using 240 instead of 250 after calibrating at 60 BPM and 240 BPM with a stopwatch.
    // Seems to work well with and without Serial logging, so that is ok to leave uncommented below.
    if (duration != 0) {
        duration = 1000*240/bpm/duration;
    }

    switch (tempNote.charAt(0)) {
        case 'C':
            noteNum = 0 + noteSharp;
            break;
        case 'D':
            noteNum = 2 + noteSharp - noteFlat;
            break;
        case 'E':
            noteNum = 4 - noteFlat;
            break;
        case 'F':
            noteNum = 5 + noteSharp;
            break;
        case 'G':
            noteNum = 7 + noteSharp - noteFlat;
            break;
        case 'A':
            noteNum = 9 + noteSharp - noteFlat;
            break;
        case 'B':
            noteNum = 11 - noteFlat;
            break;
        case 'R': // Rest note 'R' or 'REST'
            octave = -1;
            break;
        default:
            break;
    }

    // based on equation at http://www.phy.mtu.edu/~suits/NoteFreqCalcs.html and the Verdi tuning
    // fn = f0*(2^1/12)^n where n = number of half-steps from the reference frequency f0
    freq = float(256*pow(1.05946,(     12.0*(octave-4)        +noteNum)));
    //          C4^  (2^1/12)^    12 half-steps in an octave      ^how many extra half-steps within that octave, 0 for 'C'

    // Serial.printlnf("%-3s o:%-2d f:%-5d d:%d", tempNote.c_str(), octave, int(freq), duration);

    if (octave != -1) tone(D0, int(freq), duration);
    delay(duration);
    if (octave != -1) noTone(D0);
}
