#include <FastLED.h>

#define LED_PIN 7
#define pulsePin 0
#define STRIP_TYPE WS2812
#define NUM_LEDS 256
#define STRIP_X 32
#define STRIP_Y 8
#define BRIGHTNESS 0.1 // float between 0 and 1

CRGB leds[NUM_LEDS];

uint16_t XY(uint8_t x, uint8_t y)
{
  uint16_t i;
  if (x & 0x01)
  {
    // Odd rows run backwards
    uint8_t reverseY = (STRIP_Y - 1) - y;
    i = (x * STRIP_Y) + reverseY;
  }
  else
  {
    // Even rows run forwards
    i = (x * STRIP_Y) + y;
  }

  return i;
}

void setup()
{
  FastLED.addLeds<STRIP_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);

  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
}

void loop()
{
  // code to get text from translate API
  char *text = "A";
  printString(text);
}

void printChar(char c)
{
  char *character_data = character_array[c];
  int row_count = 1;

  while (row_count - 1 != character_data[0])
  {
    for (int j = 0; j < 8; j++)
    {
      if (character_data[j * row_count + 1 + 8 * (row_count - 1)] == 1)
        leds[j] = CRGB(255 * BRIGHTNESS, 0, 0);
    }
    row_count += 1;

    CRGB ledsCopy[NUM_LEDS];
    memcpy(ledsCopy, leds, sizeof(leds[0]) * NUM_LEDS);

    for (int x = 0; x < STRIP_X - 1; x++)
    {
      for (int y = 0; y < STRIP_Y; y++)
      {
        if (ledsCopy[XY(x, y)] == CRGB(255 * BRIGHTNESS, 0, 0))
        {
          leds[XY(x + 1, y)] = ledsCopy[XY(x, y)];
        }
      }
    }
    FastLED.show();
    delay(100);
  }
}

void printString(char *s)
{
  while (*s != 0)
  {
    printChar(*s);
    s++;
  }
}
