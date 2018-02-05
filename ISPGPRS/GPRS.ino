void startPost() //set URL and content type (JSON)
{
  sendCMD("AT+HTTPPARA=\"CID\",1\r\n",resp,1000);
  sendCMD("AT+HTTPPARA=\"URL\",\"http://alfa.smartstorm.io/api/v1/measure\"\r\n",resp,1000);
  sendCMD("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n",resp,1000);
}

void configBearer(char *uri) //configure Internet bearer (GPRS Internet)
{
  char apnSet[32];
  sendCMD("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n",resp,1000); //set connection type to GPRS
  sprintf(apnSet, "AT+SAPBR=3,1,\"APN\",\"%s\"\r\n", uri); //set Access Point Name to function argument)
  sendCMD(apnSet,resp,1000);
  sendCMD("AT+SAPBR=1,1\r\n",resp,1000); //enable GPRS
}

void connect() //start HTTP interaction
{
  sendCMD("AT+HTTPINIT\r\n",resp,1000);
}

void disconnect() //terminate HTTP interaction
{
  sendCMD("AT+HTTPTERM\r\n",resp,1000);
}

void post(char *body) //POST request
{
  char httpData[32];
  connect();
  startPost();
  unsigned int delayToDownload = 10000;
  sprintf(httpData, "AT+HTTPDATA=%d,%d\r\n", strlen(body), 10000); //AT+HTTPDATA=length of data in bytes, timeout in ms
  sendCMD(httpData,resp,1000);
  purgeSerial();
  sendCMD(body,resp,1000); //write request text
  sendCMD("AT+HTTPACTION=1\r\n",resp,1000); //http action (0: GET, 1:POST, 2:HEAD)
  if(!strstr(resp,"200"))
    notSND = true;
  else
    notSND = false;
  sendCMD("AT+HTTPREAD\r\n",resp,1000); //not actually reading anything
  purgeSerial();
  disconnect();
}
