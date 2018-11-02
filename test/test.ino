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
#define MISO       45 //SPI MISO
#define SD_CS      53 //SD Chip Select
#define SD_CD       49  //Card Detect (needs 10k pullup resistor!!)

////Sensors///////////////////////////////////
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
//  Serial.begin(9600);
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
  
  if (digitalRead(SD_CD) == LOW) {
       display.print("!!!!!!!!!\nSD Card\nNot Inserted!");
       display.display();
       while(1);
  }
  else if (!card.init(SPI_HALF_SPEED, SD_CS)) {

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
  char runtime[50] ;
  int adcVal1 = analogRead(Sensor1), adcVal2 = analogRead(Sensor2);
  float voltage0 = adcVal1 * (5.0 / 1023.0), voltage1 = adcVal2 * (5.0/1023.0);
  

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

  if (voltage0 > 4.0 && voltage1 > 4.0) {
    secondsSinceGetup = 0;
  } 
  if (secondsSinceGetup >= maxSitTimeInSeconds) {
   // tone(SPEAKER, 700, .1); 
  } else {
    //noTone(SPEAKER);
  }
  sprintf(runtime,"Rt:%02d:%02d;GuT:%02d:%02d\nV0:%04f\nV1:%04f\n",
                minutes, seconds, (secondsSinceGetup / 60), secondsSinceGetup % 60, 
                voltage0, voltage1);
  
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(runtime);
  display.print("SD Card: ");
  display.print(sdState);
  display.display();
  lastTicks = millis();
  delay(100);
}

