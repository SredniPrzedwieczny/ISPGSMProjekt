unsigned int EchoPin = 2;           // connect Pin 2(Arduino digital io) to Echo at US-020
unsigned int TrigPin = 3;           // connect Pin 3(Arduino digital io) to Trig at US-020
void setupSensor()
{
  pinMode(EchoPin, INPUT);                    //Set EchoPin as input, to receive measure result from US-020
  pinMode(TrigPin, OUTPUT);                   //Set TrigPin as output, used to send high pusle to trig measurement (>10us)
}
int getSensorReading()
{
  int d;
  digitalWrite(TrigPin, HIGH);              //begin to send a high pulse, then US-020 begin to measure the distance
  delayMicroseconds(50);                    //set this high pulse width as 50us (>10us)
  digitalWrite(TrigPin, LOW);               //end this high pulse
  d = pulseIn(EchoPin, HIGH);
  d=(d*34/100)/2;
  return d;
}
