void switch_on()
{
  digitalWrite(OnOff_PIN, HIGH);
  delay(20);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setCursor(5, 0);
  // Print a message to the LCD.
  lcd.print("Hello!");
  on = true;
}

void switch_off()
{
  pinMode(RS_PIN, INPUT);
  pinMode(E_PIN, INPUT);
  pinMode(D4_PIN, INPUT);
  pinMode(D5_PIN, INPUT);
  pinMode(D6_PIN, INPUT);
  pinMode(D7_PIN, INPUT);
  delay(20);
  digitalWrite(OnOff_PIN, LOW);
  on = false;
}
