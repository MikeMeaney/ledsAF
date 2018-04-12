#include "AffectedLEDs.h"

// Create a Button named PatsyStone. It will be your friend, and you two will spend lots of time together.
// You may be wondering about those two slashes and this gray text- they're called comments, and
// don't affect the code. Think of this as the voice of the narrator.
AffectedLEDs PatsyStone = AffectedLEDs();
int LEDS = 137;

byte ballColors[7][3] = { {0xff, 0x40,0}, 
                        {0xff   ,0x00    , 0x33}, 
                        {0xff   , 0x00   , 0xff},
                        {0x00   , 0x55   , 0x11},
                        {0x40   , 0x00   , 0xff},
                        {0x00   , 0x00   , 0x22},
                        {0x80   , 0x80   , 0x80}};


// The code in setup() runs once when the device is powered on or reset. Used for setting up states, modes, etc
void setup() {
    // Tell PatsyStone to get everything ready to go
    // Use PatsyStone.begin(1); if you have the original SparkButton, which does not have a buzzer or a plastic enclosure
    // to use, just add a '1' between the parentheses in the code below.
    //PatsyStone.setNumLeds(LEDS);
    PatsyStone.begin();
}

/* loop(), in contrast to setup(), runs all the time. Over and over again.
Remember this particularly if there are things you DON'T want to run a lot. Like Particle.publish() */
void loop() {

    //PatsyStone.ALfadeInOut(0xff,0x00,0x00);
    //PatsyStone.ALfadeInOut(0xff,0xff,0xff);
    //PatsyStone.ALfadeInOut(0x00,0x00,0xff);

    //PatsyStone.rainbow(5);
    //PatsyStone.ALmeteorRain(0x22,0x00,0xff, 10, 12, true, 5);
    //PatsyStone.ALmeteorRain(0x00,0x22,0xff, 10, 12, true, 5);
    //PatsyStone.ALTwinkle(0x00, 0x88, 0xff, 10, LEDS, false);
    //PatsyStone.ALTwinkle(0xff, 0x44, 0, 10, LEDS, true);

    PatsyStone.ALfire(55,120,15);
    PatsyStone.ALbouncingColoredBalls(7, ballColors); //Inf. Loop


    // Now you're blinking! Play with which LED is blinking (1-11), the delays between, and the color.
}
