void lights_on()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    unsigned long date = read_date(i);
    if (date > 0)
    {
      byte color;
      
      unsigned long time_til_expiry = date - now();
      if (time_til_expiry < YELLOW_THRESHOLD)
      {
        if (time_til_expiry < ORANGE_THRESHOLD)
        {
          if (time_til_expiry < RED_THRESHOLD)
            color = RED;
          else
            color = ORANGE;
        }
        else
          color = YELLOW;
      }
      else
        color = GREEN;
        
      leds[2 * i] = CHSV(color, 255, 50);
      leds[2 * i + 1] = CHSV(color, 255, 50);
    }
    else
    {
      leds[2 * i] = CHSV(0, 255, 0);
      leds[2 * i + 1] = CHSV(0, 255, 0);
    }
  }

  FastLED.show();
}

void lights_off()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(0, 255, 0);
  }

  FastLED.show();
}

void select(int index)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if ( (i == (2 * index)) || (i == (2 * index) + 1))
      leds[i] = CHSV(BLUE, 255, 50);
    else
      leds[i] = CHSV(0, 255, 0);
  }

  FastLED.show();
}
