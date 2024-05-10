void ledRainbowTask(void * parameter) //use DO...WHILE loops instead of WHILE loops
{
  for(;;)//do this task forever
  {

    if(ledRainbowOn)
    {
      FastLED.setBrightness(150);
      fill_rainbow( leds, NUM_LEDS, gHue, 7);
      FastLED.show();
      FastLED.delay(1000/FRAMES_PER_SECOND);//1000
    
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    }
    else
    {
      vTaskDelay(100);
    }
    
  
  }  
}
