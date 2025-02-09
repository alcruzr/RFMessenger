void ledFunction()
{
  while(inLedMenu)
  {
    button.tick();
    display.showMenu( &ledMenu );
  }
  if (ledFunctionSelected)
  {
    display.clear();
    switch (display.menuSelection(&ledMenu))
    {
      case 0://Rainbow
//          fill_rainbow(&(leds[0]), 10, 1);
//          FastLED.show();
          ledRainbowOn = true;
          ledFunctionSelected = false;
          Serial.println("Rainbow set");
          display.printFixed(0,  0, "LEDS Set to Rainbow! ", STYLE_BOLD);
          delay(2000);
          display.clear();
          break;

      case 1://White
          ledRainbowOn = false;
          fill_solid(&(leds[0]), NUM_LEDS, CRGB(100, 100, 100));
          FastLED.show();
          ledFunctionSelected = false;
          Serial.println("Color White Set");
          display.printFixed(0,  0, "LEDS Set to Color White! ", STYLE_BOLD);
          delay(2000);
          display.clear();
          break;

      case 2://Red
          ledRainbowOn = false;
          fill_solid(&(leds[0]), NUM_LEDS, CRGB(100, 0, 0));
          FastLED.show();
          ledFunctionSelected = false;
          Serial.println("Color Red Set");
          display.printFixed(0,  0, "LEDS Set to Color Red! ", STYLE_BOLD);
          delay(2000);
          display.clear();
          break;

      case 3://Green
          ledRainbowOn = false;
          fill_solid(&(leds[0]), NUM_LEDS, CRGB(0, 100, 0));
          FastLED.show();
          ledFunctionSelected = false;
          Serial.println("Color Green Set");
          display.printFixed(0,  0, "LEDS Set to Color Green! ", STYLE_BOLD);
          delay(2000);
          display.clear();
          break;

      case 4://Blue
          ledRainbowOn = false;
          fill_solid(&(leds[0]), NUM_LEDS, CRGB(0, 0, 100));
          FastLED.show();
          ledFunctionSelected = false;
          Serial.println("Color Blue Set");
          display.printFixed(0,  0, "LEDS Set to Color Blue! ", STYLE_BOLD);
          delay(2000);
          display.clear();
          break;
      case 5://Teal
          ledRainbowOn = false;
          fill_solid(&(leds[0]), NUM_LEDS, CRGB(0, 100, 100));
          FastLED.show();
          ledFunctionSelected = false;
          Serial.println("Color Teal Set");
          display.printFixed(0,  0, "LEDS Set to Color Teal! ", STYLE_BOLD);
          delay(2000);
          display.clear();
          break;
      case 6://OFF
          ledRainbowOn = false;
          fill_solid(&(leds[0]), NUM_LEDS, CRGB(0, 0, 0));
          FastLED.show();
          ledFunctionSelected = false;
          Serial.println("LEDS Turned Off");
          display.printFixed(0,  0, "LEDS Turned Off! ", STYLE_BOLD);
          delay(2000);
          display.clear();
          break;
    }
  }
  display.clear();
  ledFunctionSelected = false;
  inLedMenu = false;
}
