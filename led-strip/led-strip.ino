#include <FastLED.h>

#define NUM_LEDS 150
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define BUTTON_PIN 12
#define POTENTIOMETER_PIN A1
#define LED_PIN 13
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];
CRGB oneColorArray[] = {
  CRGB(0, 218, 255), CRGB(0, 3, 255), CRGB(134, 22, 255), CRGB(249, 176, 255), CRGB(255, 0, 0), CRGB(255, 246, 23), 
  CRGB(255, 126, 0), CRGB(172, 255, 195), CRGB(0, 255, 11), CRGB(69, 154, 255), CRGB(255, 51, 251), CRGB(255, 253, 215)
  };

CRGBPalette16 currentPalette;
TBlendType currentBlending;
int pressCounter;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p FL_PROGMEM;
extern const TProgmemPalette16 noPallet_p FL_PROGMEM;

void ChangePalettePeriodically();
void FillLEDsFromPaletteColors(uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();
void Blink();
void OneColor();
void MultiColors();
void ChangeBrightness();

void setup() {
  delay(3000);  // power-up safety delay
  pinMode(BUTTON_PIN, INPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  ChangeBrightness();

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  pressCounter = 0;
}

void loop() {
  if (digitalRead(BUTTON_PIN) == 1) {
    pressCounter++;
    delay(500);
  }

  int arrayLength = sizeof(oneColorArray) / sizeof(oneColorArray[0]);

  if (pressCounter < arrayLength) {
    OneColor();
  } else {
    MultiColors();
  } 

  if (pressCounter == arrayLength + 1) {
    pressCounter = 0;
  }

  ChangeBrightness();  
}

void MultiColors() {
  ChangePalettePeriodically();

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1;  // motion speed

  if (currentPalette == noPallet_p && currentBlending == NOBLEND) {
    Blink();
  } else {
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t brightness = 255;

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
void ChangePalettePeriodically() {
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if (lastSecond != secondHand) {
    lastSecond = secondHand;

    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
    if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
    if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
    if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
    if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
    if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
    if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
    if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
    if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
    if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
    if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    if( secondHand == 59)  { currentPalette = noPallet_p;              currentBlending = NOBLEND; }
  }
}

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

void Blink() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CHSV(random8(), random8(), random8());
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Black;
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
}

void OneColor() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = oneColorArray[pressCounter];
  }
  FastLED.show();
}

void ChangeBrightness() {
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  int brightness = map(potentiometerValue, 0, 1023, 0, 255);
  FastLED.setBrightness(brightness);
}

const TProgmemPalette16 myRedWhiteBluePalette_p FL_PROGMEM = {
  CRGB::Red,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

const TProgmemPalette16 noPallet_p FL_PROGMEM = {};