// includes
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "FastLED.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

// set number of pixels in strip
#define NUM_LEDS 150
// segment size for the ripple effect (How many leds per segment)
#define SEGMENT_SIZE 20
// number of segments needed to cover the full strip
#define NUM_SEGMENTS ((NUM_LEDS + SEGMENT_SIZE - 1) / SEGMENT_SIZE)
// set pin to control WS2812B
#define LED_PIN 4
// set initial brightness 0-255
int brightness = 225;
bool connectionFailed = false;
// create CRGB led object
CRGB leds[NUM_LEDS];

// assign pins to TX and RX for player
static const uint8_t PIN_MP3_TX = 2;
static const uint8_t PIN_MP3_RX = 3;
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// create the player object
DFRobotDFPlayerMini myPlayer;

void setup()
{
  // Short delay for power rails to steady just in case
  delay(100);
  // set up the LED strip
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  // 200ms blue for "powered on"
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(200);
  // set up the MP3 player
  Serial.begin(9600);
  softwareSerial.begin(9600);
  
  // mp3 player connection test and results
  if (myPlayer.begin(softwareSerial))
  {
    Serial.println("Connection successful.");
    myPlayer.volume(20);
    // blink green for good
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    for (int blink = 0; blink <= 4; blink += 1)
    {
      FastLED.show();
      delay(250);
      FastLED.clear();
      FastLED.show();
      delay(50);
    }
  }
  else
  {
    Serial.println("Connection failed fart.");
    // be red forever for bad
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    connectionFailed = true;
    return; // skip the clear below — stay red
  }
  delay(100);
  FastLED.clear();
  FastLED.show();
}

// Randomizes segment fire order for the ripple look
void shuffleSegmentOrder(int order[], int n)
{
  for (int i = n - 1; i > 0; i--)
  {
    int j = random(0, i + 1);
    int temp = order[i];
    order[i] = order[j];
    order[j] = temp;
  }
}

void loop()
{
  // If we failed connecting to mp3 player just stay red foreverr
  if (connectionFailed)
  {
    return; // skip everything else forever
  }
  // volume defines both the led brightness and delay after flash
  int volMin = 15;
  int volMax = 28;
  int randomVol = random(volMin, volMax);

  // upper value should be one more than total tracks
  int randomTrack = random(1, 52);

  // lightning variables

  // number of flashes
  int flashCount = random(5, 15);
  // flash white brightness range - 0-255
  int flashBrightnessMin = 10;
  int flashBrightnessMax = 220;

  // flash duration range - ms
  int flashDurationMin = 5;
  int flashDurationMax = 100;

  // flash off range - ms
  int flashOffsetMax = 75;
  // time to next flash range - ms
  int nextFlashDelayMin = 3;
  int nextFlashDelayMax = 75;
  // map white value to volume - louder is brighter
  int flashBrightness = map(randomVol, volMin, volMax, flashBrightnessMin, flashBrightnessMax);

  // map flash to thunder delay - invert mapping
  int thunderDelay = map(randomVol, volMin, volMax, 1000, 250);

  // randomize pause between strikes
  // longests track length - ms
  long longestTrack = 30000;
  // intensity - closer to longestTrack is more intense
  long stormIntensity = 120000;
  long strikeDelay = random(longestTrack, stormIntensity);

  // debug serial print
  Serial.println("FLASH");
  Serial.print("Track: ");
  Serial.println(randomTrack);
  Serial.print("Volume: ");
  Serial.println(randomVol);
  Serial.print("Brightness: ");
  Serial.println(flashBrightness);
  Serial.print("Thunder delay: ");
  Serial.println(thunderDelay);
  Serial.print("Strike delay: ");
  Serial.println(strikeDelay);
  Serial.print("-");

  for (int flash = 0; flash <= flashCount; flash += 1)
  {
    // single shared color for this entire flash pass
    int colorV = random(0, 50);
    int r = random(40, 80);
    int g = random(10, 25);
    int b = random(0, 10);\
    CRGB color = CRGB(r + colorV, g + colorV, b + colorV);

    // build and shuffle segment order so every flash ripples differently
    int segmentOrder[NUM_SEGMENTS];
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
      segmentOrder[i] = i;
    }
    shuffleSegmentOrder(segmentOrder, NUM_SEGMENTS);

    // scale segment delay down so total ripple buildup stays fast
    // regardless of how many segments the strip is divided into
    int segDelayMin = 0;
    int segDelayMax = max(1, flashOffsetMax / NUM_SEGMENTS);

    // fire every segment (guarantees full NUM_LEDS coverage) in shuffled order
    for (int s = 0; s < NUM_SEGMENTS; s++)
    {
      int segIndex = segmentOrder[s];
      int segStart = segIndex * SEGMENT_SIZE;
      int segEnd = min(segStart + SEGMENT_SIZE, NUM_LEDS);

      for (int i = segStart; i < segEnd; i++)
      {
        leds[i] = color;
      }

      FastLED.setBrightness(flashBrightness);
      FastLED.show();
      delay(random(segDelayMin, segDelayMax + 1));
    }

    // hold the full-strip flash briefly
    delay(random(flashDurationMin, flashDurationMax));
    FastLED.clear();
    FastLED.show();

    delay(random(nextFlashDelayMin, nextFlashDelayMax));
  }

  // pause between flash and thunder
  delay(thunderDelay);

  // trigger audio - randomize volume and track
  myPlayer.volume(randomVol);
  myPlayer.play(randomTrack);

  delay(strikeDelay);
}