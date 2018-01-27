#include <sim800cmd.h>
int d = 0;
#define BODY "{\"user_id\": \"klos639@gmail.com\",\"sensor_id\": \"5a6741cfa4408d3a9dde0ba0\",\"desc\" :\"GSM\",\"measure_value\": %d}\r\n"
char toSND[255];

Sim800Cmd sim800(fundebug);

void setup()
{
    while(!sim800.sim800init());
    Serial.begin(9600);
    Serial1.begin(19200);
    delay(1000);
    sendCMD("AT+CSCS=\"GSM\"\r\n",2,1000);
    configBearer("internet");
    randomSeed(analogRead(0));
}
void loop()
{
  d = random(0,21); //random number to replace sensor reading
  sprintf(toSND, BODY, d);
  post(toSND);
//  SMS
//-----------------------------------------------------
//  sprintf(toSND, "%d%c", d,(char)26);
//  sendCMD("AT+CMGF=1\r\n",1,1000);
//  sendCMD("AT+CMGS=\"+48asdasdasd\"\r\n",1,1000);
//  sendCMD(toSND,1,1000);
//-----------------------------------------------------
  delay(10000);
}
/
void post(char *body) //POST request
{
  char httpData[32];
  connect();
  startPost();
  unsigned int delayToDownload = 10000;
  sprintf(httpData, "AT+HTTPDATA=%d,%d\r\n", strlen(body), 10000); //AT+HTTPDATA=length of data in bytes, timeout in ms
  sendCMD(httpData,2,1000);
  purgeSerial();
  sendCMD(body,1,1000); //write request text
  sendCMD("AT+HTTPACTION=1\r\n",2,1000); //http action (0: GET, 1:POST, 2:HEAD)
  sendCMD("AT+HTTPREAD\r\n",2,1000); //not actually reading anything
  purgeSerial();
  disconnect();
}

void startPost() //set URL and content type (JSON)
{
  sendCMD("AT+HTTPPARA=\"CID\",1\r\n",2,1000);
  sendCMD("AT+HTTPPARA=\"URL\",\"http://alfa.smartstorm.io/api/v1/measure\"\r\n",2,1000);
  sendCMD("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n",2,1000);
}
void configBearer(char *uri) //configure Internet bearer (GPRS Internet)
{
  char apnSet[32];
  sendCMD("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n",2,1000); //set connection type to GPRS
  sprintf(apnSet, "AT+SAPBR=3,1,\"APN\",\"%s\"\r\n", uri); //set Access Point Name to function argument)
  sendCMD(apnSet,2,1000);
  sendCMD("AT+SAPBR=1,1\r\n",2,1000); //enable GPRS
}
void connect() //start HTTP interaction
{
  sendCMD("AT+HTTPINIT\r\n",2,1000);
}
void disconnect() //terminate HTTP interaction
{
  sendCMD("AT+HTTPTERM\r\n",2,1000);
}
void fundebug(void)
{
}
void purgeSerial()
{
  while (Serial.available()) Serial.read();
}
void sendCMD(char* command, int tries, int time) //send command to SIM800H, wait for time ms
{
  //TO DO: actually implement tries
  Serial1.write(command);
  Serial.write(command);
  delay(time);
  char c;
  while(Serial1.available())
  {
    c = Serial1.read();
    Serial.write(c);
  }
}
