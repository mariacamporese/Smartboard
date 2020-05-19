int read_light()
{
  int sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++)
  {
    sum += analogRead(PD_PIN);
  }
  return sum / NUM_SAMPLES;
}

bool movement()
{
  int ledOFF_light = read_light();
  //Serial.println(ledOFF_light);

  digitalWrite(LED_PIN, HIGH);
  delay(20);
  int ledON_light = read_light();
  //Serial.println(ledON_light);
  digitalWrite(LED_PIN, LOW);

  float percent_diff = (float)abs(ledOFF_light - ledON_light) / ledOFF_light;
  return percent_diff > THRESHOLD;
}
