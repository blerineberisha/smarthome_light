#include <Arduino.h>
#include <M5Core2.h>
#include <FastLED.h>
#include <M5_DLight.h>
#include <M5GFX.h>

// data pin of port c
#define LED_PIN     14
// data pin of port a
#define LIGHT_PIN   26
#define NUM_LEDS    30
#define CHIPSET     SK6812
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];
M5_DLight sensor;
uint16_t lux;
M5GFX display;
M5Canvas canvas(&display);
char info[40];
uint16_t brightness;

// TO DO: dynamic brightness of led strip based on lux value

void oneAfterAnother(){
// Move a single white led 
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::White;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(100);

      // Turn our current led back to black for the next loop around
      leds[whiteLed] = CRGB::Black;
   }
}

void pride() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}


void setup(){
  M5.begin();
  FastLED.addLeds<SK6812, LED_PIN, RGB>(leds, NUM_LEDS)
  .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
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
}


void lightStuff(){
  lux = sensor.getLUX();
  sprintf(info, "lux: %d", lux);
  canvas.fillSprite(BLACK);
  canvas.drawString(info, 160, 120);
  canvas.pushSprite(0, 0);
}


void loop(){
  pride();
  FastLED.show();
  lightStuff();
}
// ----------------------------------------------------------------------------
// MQTT callback
// ----------------------------------------------------------------------------

/*void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // Parse Payload into String
  char * buf = (char *)malloc((sizeof(char)*(length+1)));
  memcpy(buf, payload, length);
  buf[length] = '\0';
  String payloadS = String(buf);
  payloadS.trim();

  if(String(topic) == "example") {
    if(payloadS == "on") {
      lv_led_on(led);
    }
    if(payloadS == "off") {
      lv_led_off(led);
    }
  }
}*/