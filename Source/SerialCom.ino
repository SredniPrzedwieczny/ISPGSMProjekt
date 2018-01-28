void purgeSerial()
{
  while (Serial1.available()) Serial1.read();
}

void sendCMD(char* command, char* resp, int time) //send command to SIM800H, wait for time ms then take response
{
  resp[0] = (char)0;
  if(strlen(command) != 0)
  {
    purgeSerial();
    Serial1.write(command);
    Serial.write(command);
  }
  delay(time);
  unsigned long t = millis();
  char c;
  unsigned long czas;
  while(!Serial1.available()) //wait until something on Serial1. If nothing for > 1s then quit.
  {
    czas = millis() - t;
    if(czas > 1000)
    {
      Serial.println("TIMEOUT");
      return;
    }
  }
  while(Serial1.available()) //print SIM800L response to resp
  {
    c = Serial1.read();
    sprintf(resp, "%s%c", resp, c);
    delay(5);
  }
  Serial.print(resp);
}
