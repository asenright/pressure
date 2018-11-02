#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ADAFRUIT_GFX.h>
#include <SPI.h>

#include "SD.h"

////SPI////////////////////////////////////////
#define MOSI        51   //SPI MOSI; D1 on LCD screen
#define CLK         52   //SPI CLK; D0 on LCD Screen
#define OLED_DC    33 //OLED_DC
#define OLED_CS    31 //OLED Chip Select 
#define OLED_RESET 35 //OLED Reset
#define MISO       50 //SPI MISO
#define SD_CS      53 //SD Chip Select
#define SD_CD       49  //Card Detect (needs 10k pullup resistor!!)

////Sensors///////////////////////////////////
//#define SensorEnable 26
#define Sensor1 A0
#define Sensor2 A3
#define Sensor3 A2
#define Sensor4 A1
#define Sensor5 A5
#define Sensor6 A4
/////Speaker/////////////////////////////////
//#define SPEAKER 79
/////Variables/////////////////////////////////
#define MILLIS_PER_SEC 1000

//////////////////Globals////////////////
Adafruit_SSD1306 display(MOSI, CLK, OLED_DC, OLED_RESET, OLED_CS);

static unsigned int lastTicks;
static unsigned int secondsSinceGetup; //the system time since the user got up
static unsigned int maxSitTimeInSeconds;
static unsigned int minutes, seconds, milliseconds ;
static String sdState = "In" ;
Sd2Card card;
SdVolume volume;
SdFile root;

//////////////////Main////////////////
void setup()   {         
  //if (Serial1 || Serial2 || Serial3) 
          Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  pinMode(SD_CD, INPUT_PULLUP); //Setup pin to read if a card is inserted
  lastTicks = 0;
  secondsSinceGetup = 0;
  maxSitTimeInSeconds = 10;
  minutes = 0; 
  seconds = 0;
  milliseconds = 0;
  //noTone(SPEAKER);
  display.clearDisplay();
  display.setCursor(0,0);
  //pinMode(SensorEnable, OUTPUT);
  //digitalWrite(SensorEnable, HIGH);
  if (!card.init(SPI_HALF_SPEED, SD_CS)) {
      display.print("!!!!!!!!!\nSD card\nFailed to init!");
      display.display();
      while (1);
  }  else {
           display.print("SD card OK");
      display.display();
      delay(3000);
  }
 
}

void loop()
{
  char runtime[64];
  int adcVal1 = analogRead(Sensor1), adcVal2 = analogRead(Sensor2);
  
  float voltage1 = adcVal1 * (5.0 / 1023.0), voltage2 = adcVal2 * (5.0/1023.0);

  //if (Serial1 || Serial2 || Serial3) {
      int   adcVal3 = analogRead(Sensor3), adcVal4 = analogRead(Sensor4),
            adcVal5 = analogRead(Sensor5), adcVal6 = analogRead(Sensor6);
      float voltage3 = adcVal3 * (5.0 / 1023.0), voltage4 = adcVal4 * (5.0/1023.0),
            voltage5 = adcVal5 * (5.0 / 1023.0), voltage6 = adcVal6 * (5.0/1023.0);
       char serialStr[50];
         
//  }
  
  milliseconds += (millis() - lastTicks);
  if (milliseconds >= MILLIS_PER_SEC) { //1000 milliseconds per sec
      milliseconds -= MILLIS_PER_SEC;
      seconds++;
      secondsSinceGetup++;

      if (digitalRead(SD_CD) == LOW) {
        sdState = "Not In";
      } else {
        sdState = "In";
      }
  }
  if (seconds >= 60) {
     seconds -= 60;
     minutes++;
  }

  if (voltage1 > 4.0 && voltage2 > 4.0) {
    secondsSinceGetup = 0;
  } 
  if (secondsSinceGetup >= maxSitTimeInSeconds) {
   // tone(SPEAKER, 700, .1); 
  } else {
    //noTone(SPEAKER);
  }

  switch (seconds % 3) {
      case (0): sprintf(runtime,"Rt:%02d:%02d;GuT:%02d:%02d\nV1:%02f\nV2:%02f\n",
                minutes, seconds, (secondsSinceGetup / 60), secondsSinceGetup % 60, 
                voltage1, voltage2);
                break;
      case (1):   sprintf(runtime,"Rt:%02d:%02d;GuT:%02d:%02d\nV3:%02f\nV4:%02f\n",
                minutes, seconds, (secondsSinceGetup / 60), secondsSinceGetup % 60, 
                voltage3, voltage4);
                break;
      case (2):   sprintf(runtime,"Rt:%02d:%02d;GuT:%02d:%02d\nV5:%02f\nV6:%02f\n",
                minutes, seconds, (secondsSinceGetup / 60), secondsSinceGetup % 60, 
                voltage5, voltage6);
                break;
  }

  
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(runtime);
  display.print("SD Card: ");
  display.print(sdState);
  display.display();
  lastTicks = millis();
  delay(100);
}

