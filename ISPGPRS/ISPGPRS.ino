int d = 0;
#define BODY "{\"user_id\": \"example@gmail.com\",\"sensor_id\": \"5a6741cfa4408d3a9dde0ba0\",\"desc\" :\"GSM\",\"measure_value\": %d}\r\n"
#define MAX_SEND_TRIES 5
char toSND[255];
char resp[255];
boolean notSND = false;
int triedToSend = 0;
boolean sendSMS = false;
void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    startup();
    delay(1000);
    sendCMD("AT+CSCS=\"GSM\"\r\n",resp,1000);
    configBearer("internet");
//    setupSensor();
    randomSeed(analogRead(0)); //sensor simulated by random number
}
void loop()
{
//  d = getSensorReading();
  d = random(0,21); //random number to replace sensor reading
  sprintf(toSND, BODY, d);
  do
  {
    post(toSND);
    if(notSND)
    {
      Serial.println("ERROR - GPRS PACKET NOT SENT");
      triedToSend++;
    }
    while(triedToSend == MAX_SEND_TRIES)
    {
      Serial.println("ERROR - REQUESTS LIMIT EXCEEDED");
      delay(2000);
    }
  }while(notSND);
  recvSMS();
  if(sendSMS)
  {
    sprintf(toSND, "%d", d);
    sndSMS(toSND, "+48111222333");
    sendSMS = false;
  }
  delay(10000);
}
void startup()
{
  while(!strstr(resp,"OK")) //while no correct response keep trying
  {
    pinMode(12, OUTPUT); //boot sequence on 12 GPIO - HIGH for 2s then LOW
    digitalWrite(12,HIGH);
    delay(2000);
    digitalWrite(12,LOW);
    delay(2000);
    for(int i=0; i<5; i++)
    {
      sendCMD("AT\r\n",resp,1000); //send test message
    }
  }
}
