unsigned long make_time(int date[])
{   
  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = (date[YEAR_INDEX] - 1970) * (SECS_PER_DAY * 365);
  for (i = 0; i < (date[YEAR_INDEX] - 1970); i++) {
    if (LEAP_YEAR(i)) {
      seconds += SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < date[MONTH_INDEX]; i++) {
    if ((i == 2) && LEAP_YEAR((date[YEAR_INDEX] - 1970))) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * month_days[i-1];  //monthDay array starts from 0
    }
  }
  seconds += (date[DAY_INDEX]-1) * SECS_PER_DAY;
  seconds += date[HOUR_INDEX] * SECS_PER_HOUR;
  seconds += date[MINUTE_INDEX] * SECS_PER_MIN;
  return (unsigned long)seconds; 
}

void save_the_date(int address, int date[])
{
  lcd.clear();
  
  time_t seconds = make_time(date);
  EEPROM.updateLong(address * sizeof(long), seconds);

  lcd.setCursor(0, 0);
  if (address >= NUM_BUTTONS)
  {
    setTime(seconds);
    lcd.print("Time set!");
  }
  else
    lcd.print("Saved!");
}

void delete_entry(int address)
{
  EEPROM.updateLong(address * sizeof(long), 0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Deleted!");
}

void ask_for_time(int address)
{
  unsigned long seconds = read_date(address);
  int date[DATE_PRECISE];
  date[YEAR_INDEX] = year(seconds);
  date[MONTH_INDEX] = month(seconds);
  date[DAY_INDEX] = day(seconds);
  date[HOUR_INDEX] = hour(seconds);
  date[MINUTE_INDEX] = minute(seconds);
  
  int i;
  if (address >= NUM_BUTTONS)
  {
    i = DATE_PRECISE;  
  }
  else
  {
    i = DATE_SHORT;
    if (abs(date[YEAR_INDEX] - year()) > 1)
    {
      date[YEAR_INDEX] = year();
      date[MONTH_INDEX] = month();
      date[DAY_INDEX] = day();
    }
  }

  while (i > 0)
  {
    i--;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(date_parts[i] + ":");

    while (digitalRead(OK_PIN))
    {
      lcd.setCursor(0, 1);
      lcd.print((String)date[i]);

      if (!digitalRead(ADD_PIN))
      {
        date[i]++;
        while(!digitalRead(ADD_PIN)); 
      }

      if (!digitalRead(SUB_PIN))
      {
        date[i]--;
        while(!digitalRead(SUB_PIN));
        lcd.setCursor(0, 1);
        lcd.print("                ");
      }

      if (read_button(address))
      {
        while(read_button(address));
        lights_off();
        lcd.clear();
        return;
      }
    }
    while(!digitalRead(OK_PIN));
  }
  
  save_the_date(address, date);
  last_input_time = millis();
}

unsigned long read_date(int address)
{
  return EEPROM.readLong(address * sizeof(long));
}
