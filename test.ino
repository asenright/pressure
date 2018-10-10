#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ADAFRUIT_GFX.h>
#include <SPI.h>

// If using software SPI (the default case):
#define MOSI  41   //D1
#define CLK   43   //D0
#define OLED_DC    37
#define OLED_CS    35
#define OLED_RESET 39
#define MISO       45
#define SD_CS      47

#define SPEAKER 85

#define MILLIS_PER_SEC 1000

Adafruit_SSD1306 display(MOSI, CLK, OLED_DC, OLED_RESET, OLED_CS);

static unsigned int lastTicks;
 
static unsigned int secondsSinceGetup; //the system time since the user got up
static unsigned int maxSitTimeInSeconds;
static unsigned int minutes, seconds, milliseconds ;

void setup()   {                
//  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  lastTicks = 0;
  secondsSinceGetup = 0;
  maxSitTimeInSeconds = 10;
  minutes = 0; 
  seconds = 0;
  milliseconds = 0;
}

void loop()
{
  char runtime[20];
  int adcVal0 = analogRead(A0), adcVal1 = analogRead(A1);
  float voltage0 = adcVal0 * (5.0 / 1023.0), voltage1 = adcVal1 * (5.0/1023.0);
  
  milliseconds += (millis() - lastTicks);
  if (milliseconds >= MILLIS_PER_SEC) { //1000 milliseconds per sec
      milliseconds -= MILLIS_PER_SEC;
      seconds++;
      secondsSinceGetup++;
  }
  if (seconds >= 60) {
     seconds -= 60;
     minutes++;
  }

  if (voltage0 > 4.0 && voltage1 > 4.0) {
    secondsSinceGetup = 0;
  } 
  if (secondsSinceGetup >= maxSitTimeInSeconds) {
    tone(SPEAKER, 700, .1); 
  } else noTone(SPEAKER);
  sprintf(runtime,"Rt:%02d:%02d;GuT:%02d:%02d\nV0:%04f\nV1:%04f\n",minutes, seconds, (secondsSinceGetup / 60), secondsSinceGetup % 60, voltage0, voltage1);
  

  display.clearDisplay();
  display.setCursor(0,0);
  display.print(runtime);
  
  display.display();
  lastTicks = millis();
  delay(100);
}

