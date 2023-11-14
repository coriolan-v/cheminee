/*
   FastLED Mapping Demo: https://github.com/jasoncoon/led-mapper
   Copyright (C) 2022 Jason Coon, Evil Genius Labs LLC

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <bluefruit.h>
#include <FastLED.h> // https://github.com/FastLED/FastLED

FASTLED_USING_NAMESPACE

// Based on FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// change these to match your data pin, LED type, and color order
#define DATA_PIN MOSI
#define DATA_PIN_2 6
#define LED_TYPE WS2811
#define COLOR_ORDER BRG // RBG

#define BRIGHTNESS 128

// start of data copied from LED Mapper:

#define NUM_LEDS 100

byte coordsX[NUM_LEDS] = { 0, 3, 5, 8, 10, 13, 15, 18, 21, 23, 26, 28, 31, 33, 36, 39, 41, 44, 46, 49, 52, 54, 57, 59, 62, 64, 67, 70, 72, 75, 77, 80, 82, 85, 88, 90, 93, 95, 98, 100, 103, 106, 108, 111, 113, 116, 118, 121, 124, 126, 129, 131, 134, 137, 139, 142, 144, 147, 149, 152, 155, 157, 160, 162, 165, 167, 170, 173, 175, 178, 180, 183, 185, 188, 191, 193, 196, 198, 201, 203, 206, 209, 211, 214, 216, 219, 222, 224, 227, 229, 232, 234, 237, 240, 242, 245, 247, 250, 252, 255 };
byte coordsY[NUM_LEDS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
byte angles[NUM_LEDS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 5, 7, 10, 19, 64, 109, 118, 121, 122, 123, 124, 125, 125, 125, 125, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };
byte radii[NUM_LEDS] = { 255, 250, 245, 240, 235, 230, 224, 219, 214, 209, 204, 199, 194, 189, 184, 179, 173, 168, 163, 158, 153, 148, 143, 138, 133, 128, 122, 117, 112, 107, 102, 97, 92, 87, 82, 77, 71, 66, 61, 56, 51, 46, 41, 36, 31, 26, 21, 16, 11, 6, 3, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 77, 82, 87, 92, 97, 102, 107, 112, 117, 122, 128, 133, 138, 143, 148, 153, 158, 163, 168, 173, 179, 184, 189, 194, 199, 204, 209, 214, 219, 224, 230, 235, 240, 245, 250 };
// end of data copied from LED Mapper

CRGB leds[NUM_LEDS];
//CRGB leds2[NUM_LEDS];

#define FRAMES_PER_SECOND 60
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t offset = 0; // rotating "base color" used by many of the patterns
uint8_t speed = 15;

boolean autoplay = true;
uint8_t autoplaySeconds = 2;

void setup()
{
  //  delay(3000); // 3 second delay for recovery

  Serial.begin(9600);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection(TypicalSMD5050);

  //FastLED.setMaxPowerInVoltsAndMilliamps(12, 1000); // 1A

  FastLED.setBrightness(BRIGHTNESS);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList patterns = {
    // 2D map examples:
    clockwisePalette,
    counterClockwisePalette,
    outwardPalette,
    inwardPalette,
    northPalette,
    northEastPalette,
    eastPalette,
    southEastPalette,
    southPalette,
    southWestPalette,
    westPalette,
    northWestPalette,
    inwardsAndOutwardsPalettes,

    // standard FastLED demo reel examples:
    //  rainbow,
    //  rainbowWithGlitter,
    //  confetti,
    //  sinelon,
    //  juggle,
    //  bpm
};

const uint8_t patternCount = ARRAY_SIZE(patterns);

uint8_t currentPatternIndex = 3; // Index number of which pattern is current

CRGBPalette16 IceColors_p = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);

const CRGBPalette16 palettes[] = {
    RainbowColors_p,
    RainbowStripeColors_p,
    CloudColors_p,
    LavaColors_p,
    OceanColors_p,
    ForestColors_p,
    PartyColors_p,
    HeatColors_p,
    IceColors_p,
};

const uint8_t paletteCount = ARRAY_SIZE(palettes);

uint8_t currentPaletteIndex = 3;
CRGBPalette16 currentPalette = palettes[currentPaletteIndex];

boolean autoplayPalettes = false;
uint8_t autoplayPaletteSeconds = autoplaySeconds * patternCount;

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  patterns[currentPatternIndex]();

  offset = beat8(speed);

  // do some periodic updates
  // EVERY_N_SECONDS(autoplaySeconds)
  // {
  //   if (autoplay)
  //   {
  //     nextPattern(); // change patterns periodically
  //   }
  // }

  // EVERY_N_SECONDS(autoplayPaletteSeconds)
  // {
  //   // change palettes periodically
  //   if (autoplayPalettes)
  //   {
  //     nextPalette();
  //   }
  // }

  // send the 'leds' array out to the actual LED strip
  // FastLED.show(); called automatically, internally by FastLED.delay below:

  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  currentPatternIndex = (currentPatternIndex + 1) % patternCount;
}

void nextPalette()
{
  // add one to the current palette number, and wrap around at the end
  currentPaletteIndex = (currentPaletteIndex + 1) % paletteCount;
  currentPalette = palettes[currentPaletteIndex];
}

// 2D map examples:

void inwardsAndOutwardsPalettes()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + radii[i]);
  }

  // for (uint16_t i = 0; i < NUM_LEDS; i++)
  // {
  //   leds2[i] = ColorFromPalette(currentPalette, offset - radii[i]);
  // }
}

void clockwisePalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + angles[i]);
  }
}

void counterClockwisePalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - angles[i]);
  }
}

void outwardPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - radii[i]);
  }
}

void inwardPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + radii[i]);
  }
}

void northPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - coordsY[i]);
  }
}

void northEastPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - (coordsX[i] + coordsY[i]));
  }
}

void eastPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - coordsX[i]);
  }
}

void southEastPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - coordsX[i] + coordsY[i]);
  }
}

void southPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + coordsY[i]);
  }
}

void southWestPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + coordsX[i] + coordsY[i]);
  }
}

void westPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + coordsX[i]);
  }
}

void northWestPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + coordsX[i] - coordsY[i]);
  }
}

// standard FastLED demo reel examples:

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, offset, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter)
  {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(offset + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(offset, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++)
  { // 9948
    leds[i] = ColorFromPalette(palette, offset + (i * 2), beat - offset + (i * 10));
  }
}

const byte dotCount = 3;
const byte hues = 240 / dotCount;

void juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  for (int i = 0; i < dotCount; i++)
  {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(i * hues, 200, 255);
  }
}