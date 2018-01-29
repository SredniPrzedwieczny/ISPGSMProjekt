void sndSMS(char* text, char* num)
{
  char number[50];
  sprintf(number, "AT+CMGS=\"%s\"\r\n", num); //set phone number
  sprintf(text, "%s%c", text, (char)26); //^Z is terminator for SMS text
  sendCMD("AT+CMGF=1\r\n",resp,1000); //enter SMS text mode
  sendCMD(number,resp,1000);
  sendCMD(text,text,1000);
}

void recvSMS()
{
  char resp[255];
  sendCMD("AT+CMGF=1\r\n", resp, 1000); //enter SMS text mode
  sendCMD("AT+CMGR=1\r\n", resp, 1000); //read first message
  if(strstr(resp, "+48111222333")) //if from specified number, then send next data as SMS to that number
  {
    sendSMS=true;
  }
  sendCMD("AT+CMGD=1\r\n", resp, 1000); //delete message
}
