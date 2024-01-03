// Digital Watch TFT 480x320 
#include <UTFTGLUE.h>              //use GLUE class and constructor

UTFTGLUE myGLCD(0,A2,A1,A3,A4,A0); //all dummy args

const int x0 = 0;
const int y0 = 0;
const int width = 480;
const int height = 320;
const String startDate = "Wed-03/01/2024-22/40/00";
unsigned long previousMillis = 0;
const int interval = 1000;
String dateArr[3];

void extractDateAndTime(String currentDate, String dateArray[]) {
  // Find the positions of hyphens
  int firstHyphen = currentDate.indexOf('-');
  int secondHyphen = currentDate.indexOf('-', firstHyphen + 1);

  // Extract date and time
  dateArray[0] = currentDate.substring(0, firstHyphen);
  dateArray[1] = currentDate.substring(firstHyphen + 1, secondHyphen);
  dateArray[2] = currentDate.substring(secondHyphen + 1);
}


void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);

  extractDateAndTime(startDate, dateArr);

  myGLCD.setColor(255,255,255);
  myGLCD.fillRect(x0,y0,width,height);
}


void loop()
{
  int buf[478];
  int x, x2;
  int y, y2;
  int r;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    extractDateAndTime(startDate, dateArr);

    myGLCD.setColor(0, 0, 0);
    myGLCD.setBackColor(255, 255, 255);
    myGLCD.print(dateArr[0], CENTER, 93);
    myGLCD.print(dateArr[1], CENTER, 119);
    myGLCD.print(dateArr[2], CENTER, 152);

    Serial.println(dateArr[0]);
    Serial.println(dateArr[1]);
    Serial.println(dateArr[2]);
  }

}
