#include <TinyGPSPlus.h>
/*
        GPS Print from NEO6M GPS Module, to be averaged and sent over Text when a SMS with a specific code is Received.
        Made by Vishesh Varma
*/
#define AVGSIZE 50
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;

// The TinyGPSPlus object
TinyGPSPlus gps;
double averageArrayX[AVGSIZE]={0};
double averageArrayY[AVGSIZE]={0};
double averageX=0;
double averageY=0;
bool startPrinting=false;
int iterator=0;
// The serial connection to the GPS device

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(35000);
    digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  Serial1.begin(9600);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
        sendToAverage();
    //   displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  if(startPrinting)
    Serial.print(averageX);
    Serial.print(F("/"));
    Serial.print(averageY);
}
void sendToAverage()
{
    if (gps.location.isValid())
    {
    averageArrayX[iterator]=gps.location.lat();
    averageArrayY[iterator]=gps.location.lng();
    iterator++;
    }
    if(iterator==AVGSIZE)
    {
        startPrinting=true;
        iterator=0;
        averageX=0;
        averageY=0;
        for(int j=0; j<AVGSIZE;j++){
            averageX+=averageArrayX[j];
            averageY+=averageArrayY[j];
        }
        averageX/=AVGSIZE;
        averageY/=AVGSIZE;
    }
}
void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}