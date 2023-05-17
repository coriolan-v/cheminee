#include <FastLED.h>

#define LED_PIN A10
#define LED_PIN_2 A9
#define NUM_LEDS 20
#define BRIGHTNESS 64
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
//CRGB leds_2[NUM_LEDS];

#define UPDATES_PER_SECOND 200
CRGBPalette16 gPal;
#define COOLING 55
#define FRAMES_PER_SECOND 60

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120
// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 PinkPalette PROGMEM;
extern const TProgmemPalette16 YellowPalette PROGMEM;
//int sensorPin = A8;  // select the input pin for the potentiometer

bool gReverseDirection = false;

void setup() {
  Serial.begin(9600);
  //pinMode(13, OUTPUT);
  //digitalWrite(13, HIGH);
  //pinMode(27, OUTPUT);
  //digitalWrite(27, LOW);

  //Serial.begin(9600);
  //delay(1000);  // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE, LED_PIN_2, COLOR_ORDER>(leds_2, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);

  //currentPalette = RainbowColors_p;
  //currentBlending = LINEARBLEND;

  //gPal = CRGBPalette16(CRGB::Black, CRGB::Pink, CRGB::Pink, CRGB::White);

  //FillLEDsFromPaletteColors();

  //chooseRandomPalette();
  currentPalette = PinkPalette;

  FastLED.setBrightness(30);

  //readPot();
}


unsigned long previousMillis_changeMode = 0;  // will store last time LED was updated

int animationMode = 0;

void loop() {
  //readPot();
  //unsigned long currentMillis_changeMode = millis();

  //60000
  // if (currentMillis_changeMode - previousMillis_changeMode >= 180000) {
  //   // save the last time you blinked the LED
  //   previousMillis_changeMode = currentMillis_changeMode;

  //   if (animationMode == 0)
  //   {
  //     animationMode = 1;
  //   } else {
  //     animationMode = 0;
  //   }

  //   Serial.println(animationMode);


  // }

  //if (animationMode == 0)
  //{

  currentBlending = LINEARBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors(startIndex);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  // } else if (animationMode == 1) {
  //     random16_add_entropy( random());
  //     Fire2012WithPalette();
  //     FastLED.show(); // display this frame
  //     FastLED.delay(1000 / FRAMES_PER_SECOND);
  //   }
  // ChangePalettePeriodically();

  //currentPalette = SetupBlackAndWhiteStripedPalette;
  //SetupBlackAndWhiteStripedPalette();
}

void chooseRandomPalette() {
  randomSeed(analogRead(1));
  int randomNum = random(0, 2);
  if (randomNum == 0) {
    currentPalette = YellowPalette;
  } else {
    currentPalette = PinkPalette;
  }
}

unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 100;  // interval at which to blink (milliseconds)


// void readPot() {
//   unsigned long currentMillis = millis();

//   if (currentMillis - previousMillis >= interval) {
//     // save the last time you blinked the LED
//     previousMillis = currentMillis;

//     int sensorValue = analogRead(sensorPin);
//     int brightness = map(sensorValue, 0, 4095, 0, 100);
//     FastLED.setBrightness(brightness);
//   }
// }

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t brightness = 255;

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    //leds_2[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically() {
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if (lastSecond != secondHand) {
    lastSecond = secondHand;

    Serial.println(secondHand);
    if (secondHand == 0) {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if (secondHand == 10) {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if (secondHand == 15) {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if (secondHand == 20) {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
    }
    if (secondHand == 25) {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }
    if (secondHand == 30) {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
    }
    if (secondHand == 35) {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
    }
    if (secondHand == 40) {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if (secondHand == 45) {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
    if (secondHand == 50) {
      currentPalette = PinkPalette;
      currentBlending = NOBLEND;
    }
    if (secondHand == 55) {
      currentPalette = PinkPalette;
      currentBlending = LINEARBLEND;
    }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette() {
  for (int i = 0; i < 16; ++i) {
    currentPalette[i] = CHSV(random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette() {
  // 'black out' all 16 palette entries...
  fill_solid(currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette() {
  CRGB purple = CHSV(HUE_PURPLE, 255, 255);
  CRGB green = CHSV(HUE_GREEN, 255, 255);
  CRGB black = CRGB::Black;

  currentPalette = CRGBPalette16(
    green, green, black, black,
    purple, purple, black, black,
    green, green, black, black,
    purple, purple, black, black);
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 PinkPalette PROGMEM = {
  CRGB::DeepPink,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
  CRGB::DeepPink,
  CRGB::Black,

  CRGB::DeepPink,
  CRGB::Gray,
  CRGB::DeepPink,
  CRGB::Black,

  CRGB::DeepPink,
  CRGB::DeepPink,
  CRGB::DeepPink,
  CRGB::DeepPink,
  CRGB::DeepPink,
  CRGB::DeepPink,
  CRGB::Gray,
  CRGB::Gray
};

const TProgmemPalette16 YellowPalette PROGMEM = {
  CRGB::Yellow,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
  CRGB::Yellow,
  CRGB::Black,

  CRGB::Yellow,
  CRGB::Gray,
  CRGB::Yellow,
  CRGB::Black,

  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Gray,
  CRGB::Gray
};


void Fire2012WithPalette() {
  // Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < SPARKING) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    uint8_t colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    int pixelnumber;
    if (gReverseDirection) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}



// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
