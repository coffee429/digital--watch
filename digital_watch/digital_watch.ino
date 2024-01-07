// Digital Watch TFT 480x320 
#include <UTFTGLUE.h>              //use GLUE class and constructor

UTFTGLUE myGLCD(0,A2,A1,A3,A4,A0); //all dummy args

const int maxWidth = 480;
const int maxHeight = 320;
const String startDate = "Wed-03/01/2024-22/40/00";
unsigned long previousMillis = 0;
const int interval = 1000;
const int xClock = 120;
const int yClock = 120;
int dateAndTime[7];
String dateInWeeks[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// Function to find the index of a string in an array
int findDayIndex(String date, String dateArray[]) {
  int size = sizeof(dateInWeeks) / sizeof(dateInWeeks[0]);`

  for (int i = 0; i < size; i++) {
    if (date.equals(dateArray[i])) {
      return i; // Return the index if found
    }
  }
  return -1; // Return -1 if not found
}


void extractDateTimeComponents(String input, int resultArray[]) {
  // Find the positions of hyphens
  int firstHyphen = input.indexOf('-');
  int secondHyphen = input.indexOf('-', firstHyphen + 1);

  resultArray[0] = findDayIndex(input.substring(0, firstHyphen), dateInWeeks);       // Day 
  
  String dayStr = input.substring(firstHyphen + 1, secondHyphen) ;
  int dayFirstSlash = dayStr.indexOf('/');
  int daySecondSlash = dayStr.indexOf('/', dayFirstSlash + 1);
  resultArray[1] = dayStr.substring(0, dayFirstSlash).toInt();
  resultArray[2] = dayStr.substring(dayFirstSlash + 1, daySecondSlash).toInt();
  resultArray[3] = dayStr.substring(daySecondSlash + 1).toInt();

  String timeStr = input.substring(secondHyphen + 1);
  int timeFirstSlash = timeStr.indexOf('/');
  int timeSecondSlash = timeStr.indexOf('/', timeFirstSlash + 1);
  resultArray[4] = timeStr.substring(0, timeFirstSlash).toInt();
  resultArray[5] = timeStr.substring(timeFirstSlash + 1, timeSecondSlash).toInt();
  resultArray[6] = timeStr.substring(timeSecondSlash + 1).toInt();
}

void updateDateTime(int dateTime[]) {
  int DD = dateTime[0];
  int dd = dateTime[1];
  int MM = dateTime[2];
  int yyyy = dateTime[3];
  int hh = dateTime[4];
  int mm = dateTime[5];
  int ss = dateTime[6];   
  ss ++;
    Serial.println(ss);

  if(ss == 60) {
    mm++;
    ss=0;
  }

  if (mm == 60) {
      mm = 0;
      hh++; // Increment hours when minutes reach 60
  }

  if (hh == 24) {
    hh = 0; // Reset hours to 0 when it reaches 24
    
    int daysInMonth;
    switch (MM) {
      case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        daysInMonth = 31;
        break;
      case 4: case 6: case 9: case 11:
        daysInMonth = 30;
        break;
      case 2:
        // Check for leap year
        if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) {
          daysInMonth = 29;
        } else {
          daysInMonth = 28;
        }
        break;
      default:
        daysInMonth = 0; // Invalid month
        break;
    }

    dd++;
    if (dd > daysInMonth) {
      dd = 1; // Reset day to 1
      MM++;   // Increment month
      if (MM > 12) {
        MM = 1; // Reset month to 1
        yyyy++; // Increment year
      }
    }

    DD++;
    if (DD > 6) {
      DD = 0; // Reset day in week to Sunday (0)
    }
  }


  dateTime[0] = DD;
  dateTime[1] = dd;
  dateTime[2] = MM;
  dateTime[3] = yyyy;
  dateTime[4] = hh;
  dateTime[5] = mm;
  dateTime[6] = ss;

}

String convertToTextFormat(int value) {
  return (value < 10) ? ("0" + String(value)) : String(value);
}

void drawClock() {
  int r1 = 110;
  int r2 = 100;
  int r3 = 90;

  setColor("RED");
  myGLCD.fillCircle(xClock, yClock, r1);
  setColor("WHITE");
  myGLCD.fillCircle(xClock, yClock, r2); 
  setColor("BLACK");
  myGLCD.fillCircle(xClock, yClock, r3);
}

void setBackgroundColor(String color) {
  if(color.equals("BLACK")) {
    myGLCD.setBackColor(0, 0, 0);
  } else if(color.equals("WHITE")) {
    myGLCD.setBackColor(255, 255, 255);
  }
}

void setColor(String color) {
  if(color.equals("BLACK")) {
    myGLCD.setColor(0, 0, 0);
  } else if(color.equals("WHITE")) {
    myGLCD.setColor(255, 255, 255);
  } else if(color.equals("RED")) {
    myGLCD.setColor(255, 0, 0);
  }
}


void showTime() {
  String hh = convertToTextFormat(dateAndTime[4]);
  String mm = convertToTextFormat(dateAndTime[5]);
  String ss = convertToTextFormat(dateAndTime[6]);

  setBackgroundColor("BLACK");
  setColor("WHITE");

  myGLCD.print( hh + ':' + mm, xClock - 30, yClock - 20);
  myGLCD.print( ss, xClock - 10, yClock + 30);
}

void showCalendar() {
  setBackgroundColor("WHITE");
  setColor("BLACK");

  int calendarW = maxWidth * 3/4 - 30;
  int calendarH = maxHeight/3;
  String date = dateInWeeks[dateAndTime[0]];
  String dd = convertToTextFormat(dateAndTime[1]);
  String mm = convertToTextFormat(dateAndTime[2]);
  String yyyy = convertToTextFormat(dateAndTime[3]);

  myGLCD.print(date, calendarW, calendarH);
  myGLCD.print(dd + "/" + mm, calendarW, calendarH + 30);
  myGLCD.print(yyyy, calendarW, calendarH + 60);
}

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);

  extractDateTimeComponents(startDate, dateAndTime);

  setColor("WHITE");
  myGLCD.fillRect(0, 0, maxWidth, maxHeight);

  setBackgroundColor("WHITE");
  setColor("BLACK");
  myGLCD.print("Hi boss", CENTER, maxHeight - 50);

  drawClock(); 
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateDateTime(dateAndTime);
    showTime();
    showCalendar();
  }

}
