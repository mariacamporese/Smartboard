bool read_button(int num)
{
  for(int i = 0; i < ADDRESS_LENGTH; i++)
  {
    digitalWrite(ADDRESS_PINS[i], bitRead(num, i));
  }
  delay(1);

  if (!digitalRead(SIG_PIN))
  {
    last_input_time = millis();
    return true;
  }
  else
    return false;
}
