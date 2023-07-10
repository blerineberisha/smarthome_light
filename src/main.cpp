#include <Arduino.h>
#include <M5Core2.h>
#include <FastLED.h>
#include <M5_DLight.h>
#include <M5GFX.h>
#include <networking.h>

// data pin of port c
#define LED_PIN 14
// data pin of port a
#define LIGHT_PIN 26
#define NUM_LEDS 30
#define CHIPSET SK6812
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];
M5_DLight sensor;
M5GFX display;
M5Canvas canvas(&display);
char info[40];
uint16_t brightness;

/*
TO DO: 
- GUI 
- Model house
- configure mqtt and telegram chatbot
- md doc
- instruction manual
- finish up all the LED functions (-> rainbow, single color, blink in color, white, party, off)
*/

// values for different lighting environments/lux values taken from:
// https://learn.microsoft.com/en-us/windows/win32/sensorsapi/understanding-and-interpreting-lux-values
uint8_t getLedBrightness()
{
  int lux = sensor.getLUX();
  if (lux <= 200)
  {
    brightness = 255;
  }
  else if (lux > 200 && lux <= 1000)
  {
    brightness = 150;
  }
  else if (lux > 1000 && lux <= 5000)
  {
    brightness = 70;
  }
  else if (lux > 5000 && lux < 30000)
  {
    brightness = 20;
  }
  else
  {
    brightness = 0;
  }

  return brightness;
}


void oneAfterAnother()
{
  FastLED.setBrightness(getLedBrightness());
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1)
  {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = RED;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(50);

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;
  }
}

void pride()
{
  FastLED.setBrightness(getLedBrightness());
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16; // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  // Parse Payload into String
  char *buf = (char *)malloc((sizeof(char) * (length + 1)));
  memcpy(buf, payload, length);
  buf[length] = '\0';
  String payloadS = String(buf);
  payloadS.trim();
  if (String(topic) == "color")
  {
    if (payloadS == String("color"))
    {
      FastLED.showColor(RED);
    }
  }
  if (String(topic) == "status")
  {
    if (payloadS == String("status"))
    {
      FastLED.setBrightness(0);
    }
  }
}

void setup()
{
  M5.begin();
  FastLED.addLeds<SK6812, LED_PIN, RGB>(leds, NUM_LEDS);
  display.begin();
  canvas.setTextDatum(MC_DATUM);
  canvas.setColorDepth(1);
  canvas.setFont(&fonts::Orbitron_Light_24);
  canvas.setTextSize(2);
  canvas.createSprite(display.width(), display.height());
  canvas.setPaletteColor(1, ORANGE);
  Serial.println("Sensor begin.....");
  sensor.begin();
  sensor.setMode(CONTINUOUSLY_H_RESOLUTION_MODE);
  //setup_wifi();
  //mqtt_init(mqtt_callback);
  //mqtt_sub();
}

void lightStuff()
{
  sprintf(info, "lux: %d", sensor.getLUX());
  canvas.fillSprite(BLACK);
  canvas.drawString(info, 160, 120);
  canvas.pushSprite(0, 0);
}

void colorLight(String color)
{
  FastLED.setBrightness(getLedBrightness());
  FastLED.showColor(RED);
}

void loop(){
  oneAfterAnother();
  FastLED.show();
  lightStuff();
  //mqtt_loop();
}