#include "LED.h"

const uint16_t PixelCount = 1;
const uint8_t PixelPin = 5;
uint8_t colorSaturation = 255;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

void LED_init(void)
{
  strip.Begin();
}

void LED_setSaturation(uint8_t saturation)
{
  colorSaturation = saturation;
}

void LED_setColor(LED_COLOR color)
{
  switch (color)
  {
    case RED:
      strip.SetPixelColor(0, RgbColor(colorSaturation, 0, 0));
      break;
    case GREEN:
      strip.SetPixelColor(0, RgbColor(0, colorSaturation, 0));
      break;
    case BLUE:
      strip.SetPixelColor(0, RgbColor(0, 0, colorSaturation));
      break;
    case YELLOW:
      strip.SetPixelColor(0, RgbColor(colorSaturation, colorSaturation, 0));
      break;
    case PURPLE:
      strip.SetPixelColor(0, RgbColor(colorSaturation, 0, colorSaturation));
      break;
    case CYAN:
      strip.SetPixelColor(0, RgbColor(0, colorSaturation, colorSaturation));
      break;
    case WHITE:
      strip.SetPixelColor(0, RgbColor(colorSaturation, colorSaturation, colorSaturation));
      break;
    case BLACK:
      strip.SetPixelColor(0, RgbColor(0, 0, 0));
      break;
  }
  strip.Show();
}
