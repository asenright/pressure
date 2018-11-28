
#include <Adafruit_SSD1306.h>
#include <ADAFRUIT_GFX.h>

#define MOSI        51   //SPI MOSI; D1 on LCD screen
#define CLK         52   //SPI CLK; D0 on LCD Screen
#define OLED_DC    33 //OLED_DC
#define OLED_CS    31 //OLED Chip Select 
#define OLED_RESET 35 //OLED Reset


#define TIME_LIMIT 5    //how long between movements (should be around 30 minutes in real life
#define TIME_HOLD  5      // how long to have no pressure on butt
#define MIN_PRESSURE 0
#define SENSOR_COUNT 6
#define WARNING_LED 13

int sensor_address[6];


Adafruit_SSD1306 display(MOSI, CLK, OLED_DC, OLED_RESET, OLED_CS);
int Sensor[6];
int clk = TIME_LIMIT;
int radii[6];
int no_weight[6];
int all_weight[6];
int weight_difference[6];
int weight_mod[6];

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
  Serial.begin(9600);
  sensor_address[0] = A5;
  sensor_address[1] = A3;
  sensor_address[2] = A4;
  sensor_address[3] = A0;
  sensor_address[4] = A1;
  sensor_address[5] = A2;
  calibrate();
  pinMode(WARNING_LED, OUTPUT);

}
void calibrate() {
  int i;
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Remove weight from sensors for calibration...");
  display.display();
  delay(3000);

  for (i=0; i<SENSOR_COUNT;i++) {
    no_weight[i] = analogRead(sensor_address[i]);
  }

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Sit on sensor to complete calibration...");
  display.display();
  delay(3000);

  for (i=0; i<SENSOR_COUNT; i++) {
    all_weight[i] = analogRead(sensor_address[i]);
  }


  for (i=0; i<SENSOR_COUNT; i++) {
    weight_difference[i] = all_weight[i]-no_weight[i];
    weight_mod[i]=weight_difference[i]/5;
  }
  
}

void adjustSeat(){
  
  bool complete = false;
  int timer[6] = {0};
  boolean moved[6] = {false};
  int i;
  digitalWrite(WARNING_LED, HIGH);
  while(!complete){
    
    readSensor();
    draw_all_circs(radii, moved);
    complete = true;
    for(i=0; i<6;i++){
       if(radii[i] <= 1){
          timer[i]++;
       }
       if(timer[i] >= TIME_HOLD)
          moved[i] = true;
       if(moved[i]==false)
          complete = false;
     }
     delay(50);
   }
   digitalWrite(WARNING_LED, LOW);
}

void draw_all_circs(int radii[], boolean moved[]) {
    display.clearDisplay();
    delay(100);
    
    if (moved[0] == false)
      display.drawCircle(10, 13, radii[0], WHITE);
    else{
      display.setCursor(10,13);
      display.print("x");
    }

    if(moved[1] == false)
       display.drawCircle(50, 13, radii[1], WHITE);
    else {
      display.setCursor(50,13);
      display.print("x");
    }

    if(moved[2] == false)
       display.drawCircle(90, 13, radii[2], WHITE);
    else {
      display.setCursor(90,13);
      display.print("x");
    }

    if(moved[3] == false)
       display.drawCircle(10, 25, radii[3], WHITE);
    else {
      display.setCursor(10,25);
      display.print("x");
    }

    if(moved[4] == false)
       display.drawCircle(50, 25, radii[4], WHITE);
    else {
      display.setCursor(50,25);
      display.print("x");   
    }

     if(moved[5] == false)
       display.drawCircle(90, 25, radii[5], WHITE);
    else {
      display.setCursor(90,25);
      display.print("x");   
    }


    delay(100);
    display.display();
}

void readSensor(){
  int i;
  for (i=0; i<SENSOR_COUNT; i++) {
    Sensor[i] = analogRead(sensor_address[i]);
  }

  for (i=0; i<SENSOR_COUNT; i++) {
    radii[i] = convertReading(Sensor[i], i);
  }
 
  
}

int convertReading(int convert_me, int sensor_number){
  int value;
  value = convert_me / weight_mod[sensor_number];
  if (value>5)
    value = 5;
  return value;    
}


void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(clk);
  display.display();

  delay(500);
  clk--;
  if(clk <= 0){
     adjustSeat();
     clk = TIME_LIMIT;
  }
   
}
