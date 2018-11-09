// Code written by Leon Hellmich
// Version: 2.1

#include <LiquidCrystal_I2C.h>
#include <Wire.h>



LiquidCrystal_I2C lcd(0x27, 16, 2);



#define CONV_FACTOR 0.00812 //CPM to uSV/h

// Variables

int geiger_input = 2;
long count = 0;
long countPerMinute = 0;
long timePrevious = 0;
long timePreviousMeassure = 0;
long time = 0;
long countPrevious = 0;
float radiationValue = 0.0;




//  --SETUP--
void setup(){

  // Geiger input
  pinMode(geiger_input, INPUT);
  digitalWrite(geiger_input,HIGH);
  
  // LCD power 
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  
  // INIT
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  // STARTUP DISPLAY
  delay(2000);
  lcd.print("geiger counter ");
  lcd.setCursor(0,1);
  lcd.print("Version: 2.0");
  delay(1000);
  lcd.clear();

  // LOADING 

  /* */


  // DEBUG
  Serial.println("CPM=");
  Serial.println(6*count);
  Serial.println(radiationValue); 


  // INTERRUPT
  attachInterrupt(0,countPulse,FALLING);

}

void loop(){

  // CALCULATING

  if (millis()-timePreviousMeassure > 10000){
    countPerMinute = 6*count;
    radiationValue = countPerMinute * CONV_FACTOR;
    timePreviousMeassure = millis();
    Serial.print("cpm = "); 
    Serial.print(countPerMinute,DEC);
    Serial.print(" - ");
    Serial.print("uSv/h = ");
    Serial.println(radiationValue,4);



  // LCD data updating service

  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("CPM=");
  lcd.setCursor(4,0);
  lcd.print(countPerMinute,DEC);
  lcd.setCursor(0,1);
  lcd.print(radiationValue);
  lcd.print("   uSv/h");

    


 
    // reset
    count = 0;

  }

}

void countPulse(){
  detachInterrupt(0);
  count++;
  //while(digitalRead(2)==0){}
  attachInterrupt(0,countPulse,FALLING);
}

