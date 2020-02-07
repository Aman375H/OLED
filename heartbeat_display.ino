#include <FastLED.h>

#define LED_PIN 7
#define pulsePin 0
#define STRIP_TYPE WS2812
#define NUM_LEDS 256
#define STRIP_X 32
#define STRIP_Y 8
#define BRIGHTNESS 0.01 // float between 0 and 1

int BEEP[8] = {200, 500, 400, 800, 300, 900, 400, 300};
//int BEEP[8] = {300, 300, 300, 300, 300, 300, 300, 300};
int count = 0;
int Signal;
int pulseVals[STRIP_X];

CRGB leds[NUM_LEDS];


uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if( x & 0x01) {
    // Odd rows run backwards
    uint8_t reverseY = (STRIP_Y - 1) - y;
    i = (x * STRIP_Y) + reverseY;
  } else {
    // Even rows run forwards
    i = (x * STRIP_Y) + y;
  }
  
  return i;
}

void setup() {
  FastLED.addLeds<STRIP_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);

  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
  
//  for (int i = 0; i < STRIP_X-1; i++) {
//    pulseVals[i] = floor(STRIP_X/2);
//  }
}

void loop() { 
  Signal = analogRead(pulsePin);
  Serial.println(Signal);
  //int beat = round(abs((analogRead(A0) - 200)*0.0114));  // assuming from 200 to 900
  int raw_beat = Signal;
  int beat = round(abs((raw_beat)*0.0114));
  
  leds[beat] = CRGB(255*BRIGHTNESS, 0, 0);

  CRGB ledsCopy[NUM_LEDS]; 
  memcpy(ledsCopy, leds, sizeof(leds[0])*NUM_LEDS);

  //pulseVals[0] = beat;
  
  int bkg_beat = floor(raw_beat*0.2);
  for (int x = 0; x < STRIP_X; x++) {
    for (int y = 0; y < STRIP_Y; y++) {
      leds[XY (x, y)] = CRGB(bkg_beat*BRIGHTNESS,0,(255-bkg_beat)*BRIGHTNESS);
    }
  }

  for (int x = 0; x < STRIP_X-1; x++) {
    for (int y = 0; y < STRIP_Y; y++) {
//    if (pulseVals[x] > 3) {
//        for (int i = pulseVals[x]-1; i > 3; i--) {
//          leds[XY (x, pulseVals[x]+i)] = CRGB((255-bkg_beat)*BRIGHTNESS,0,bkg_beat*BRIGHTNESS);
//        }
//      }
//      else if (pulseVals[x] <= 3) {
//        for (int j = pulseVals[x]; j < 4; j++) {
//          leds[XY (x, pulseVals[x]-j)] = CRGB((255-bkg_beat)*BRIGHTNESS,0,bkg_beat*BRIGHTNESS);
//        }
//      }
 //   leds[XY (x, pulseVals[x])] = ;
    
      if (ledsCopy[XY (x,y)] == CRGB(255*BRIGHTNESS,0,0)) {
        leds[XY (x+1, y)] = ledsCopy[XY (x, y)];   // test this without making copy
      }

      if (beat > 3) {
        for (int i = beat-1; i > 3; i--) {
          leds[XY (0, i)] = CRGB(255*BRIGHTNESS,0,0);
        }
      }
      else if (beat <= 3) {
        for (int j = beat; j < 4; j++) {
          leds[XY (0, j)] = CRGB(255*BRIGHTNESS,0,0);
        }
      }
    }
  }

    FastLED.show();
    delay(100);

//    for (int i = STRIP_X-2; i > 0; i--)
//      pulseVals[i+1] = pulseVals[i];
}
